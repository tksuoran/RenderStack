#include "renderstack_toolkit/platform.hpp"
#include "main/game.hpp"
#include "main/application.hpp"
#include "main/textures.hpp"
#include "main/menu.hpp"
#include "main/programs.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/button.hpp"
#include "renderstack_ui/choice.hpp"
#include "renderstack_ui/color_picker.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/menulist.hpp"
#include "renderstack_ui/push_button.hpp"
#include "renderstack_ui/slider.hpp"
#include "renderstack_geometry/sphere.hpp"
#include "renderstack_geometry/disc.hpp"
#include "renderstack_geometry/triangle.hpp"
#include "renderstack_geometry/cone.hpp"
#include "renderstack_geometry/cube.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/build_info.hpp"
#include "renderstack_graphics/renderer.hpp"
#include <cassert>

#if defined(RENDERSTACK_USE_GLFW)
# include <GLFW/glfw3.h>
#endif
#if defined(RENDERSTACK_USE_GLWT)
# include <GLWT/glwt.h>
#endif

#define LOG_CATEGORY &log_game

#define USE_MESHES      1
#define USE_FONT        1
#define USE_GUI         1

using namespace std;
using namespace gl;
using namespace glm;
using namespace renderstack::graphics;
using namespace renderstack::toolkit;
using namespace renderstack::mesh;
using namespace renderstack::ui;

game::game()
:  m_application              ()
,  m_menu                     (nullptr)
,  m_programs                 (nullptr)
,  m_textures                 (nullptr)
,  m_font                     (nullptr)
,  m_text_buffer              (nullptr)
,  m_root_layer               (nullptr)
,  m_menu_button              (nullptr)
,  m_text_uniform_buffer_range(nullptr)
,  m_frame_dt                 (0.0)
,  m_screen_active            (false)
,  m_mouse_down               (false)
{
}

void game::connect(
   shared_ptr<renderstack::graphics::renderer>   renderer,
   shared_ptr<renderstack::ui::gui_renderer>     gui_renderer,
   std::shared_ptr<application>  application,
   std::shared_ptr<menu>         menu,
   std::shared_ptr<programs>     programs,
   std::shared_ptr<textures>     textures
)
{
   m_renderer     = renderer;
   m_gui_renderer = gui_renderer;
   m_application  = application;
   m_menu         = menu;
   m_programs     = programs;
   m_textures     = textures;
}
void game::disconnect()
{
   slog_trace("game::disconnect()");

   m_application.reset();
   m_menu.reset();
   m_programs.reset();
   m_textures.reset();
}
void game::reset()
{
   slog_trace("game::reset()");

   m_controls.reset();

   m_frame_dt        = 0.0;
   m_update_time     = m_application->time();
   m_simulation_time = m_update_time;
}
void game::on_load()
{
   assert(m_application);

   slog_trace("game::on_load()");

   if (m_programs->use_uniform_buffers())
   {
      size_t size = 0;

#if defined(USE_MESHES) || defined(USE_FONT)
      ++size;
#endif

#if defined(USE_MESHES)
      ++size; // forward renderer
      ++size; // deferred renderer
      ++size; // id renderer
      ++size;
#endif

#if defined(USE_FONT)
      ++size;
#endif

      if (size > 0)
      {
         // TODO should I use size here as stride?
         m_uniform_buffer = make_shared<buffer>(
            renderstack::graphics::buffer_target::uniform_buffer,
            m_programs->block->size() * size,
            1
         );
         m_uniform_buffer->allocate_storage(*m_renderer);
      }

#if defined(USE_FONT)
      m_text_uniform_buffer_range = make_shared<uniform_buffer_range>(
         m_programs->block,
         m_uniform_buffer
      );
#endif
   }

   m_deferred_renderer = make_shared<deferred_renderer>(m_renderer, m_programs, m_uniform_buffer);
   m_forward_renderer = make_shared<forward_renderer>(m_renderer, m_programs, m_uniform_buffer);
   m_id_renderer = make_shared<id_renderer>(m_renderer, m_programs, m_uniform_buffer);

#if defined(USE_MESHES)
   {
      vector<std::shared_ptr<renderstack::geometry::geometry>> g_collection;

      //g_collection.push_back(make_shared<renderstack::geometry::shapes::sphere>(1.0f, 12, 4));
#if 1
      g_collection.push_back(make_shared<renderstack::geometry::shapes::disc>(1.0, 0.8, 32, 2));
      g_collection.push_back(make_shared<renderstack::geometry::shapes::triangle>(0.8f / 0.57735027f));
      g_collection.push_back(make_shared<renderstack::geometry::shapes::sphere>(1.0f, 12 * 4, 4 * 6));
      g_collection.push_back(make_shared<renderstack::geometry::shapes::cuboctahedron>(1.0));
      g_collection.push_back(make_shared<renderstack::geometry::shapes::dodecahedron>(1.0));
      g_collection.push_back(make_shared<renderstack::geometry::shapes::icosahedron>(1.0));
      g_collection.push_back(make_shared<renderstack::geometry::shapes::octahedron>(1.0));
      g_collection.push_back(make_shared<renderstack::geometry::shapes::tetrahedron>(1.0));
#endif

#if 0
      {
         auto body = make_shared<cube>(8, 12, 4);
         //auto head = make_shared<cube>(8,  8, 8);
         //auto hand = make_shared<cube>(4, 12, 4);
         //auto foot = make_shared<cube>(4, 12, 4);
         g_collection.push_back(body->mesh());
      }
#endif

      // Count how big VBO and IBO is needed,
      // and which vertex attributes are needed.
      // Edges need to be built here in order for buffer size
      // computations to be able to include line indices.
      renderstack::geometry::geometry::mesh_info total_info;
      renderstack::geometry::geometry::mesh_info info;
      geometry_mesh_format_info format_info;
      geometry_mesh_buffer_info buffer_info;
      format_info.set_want_fill_triangles(true);
      format_info.set_want_edge_lines(true);
      format_info.set_want_position(true);
      format_info.set_want_normal(true);
      format_info.set_want_color(true);
      format_info.set_want_id(true);
      format_info.set_normal_style(normal_style::corner_normals);
      format_info.set_mappings(m_programs->mappings);
      for (auto i = g_collection.begin(); i != g_collection.end(); ++i)
      {
         (*i)->build_edges();
         (*i)->info(info);
         total_info += info;
         geometry_mesh::prepare_vertex_format(*i, format_info, buffer_info);
      }

      size_t total_vertex_count = 0;
      size_t total_index_count = 0;
      total_vertex_count += total_info.vertex_count_corners;
      if (format_info.want_centroid_points())
         total_vertex_count += total_info.vertex_count_centroids;
      
      if (format_info.want_fill_triangles())
         total_index_count += total_info.index_count_fill_triangles;

      if (format_info.want_edge_lines())
         total_index_count += total_info.index_count_edge_lines;

      if (format_info.want_corner_points())
         total_index_count += total_info.index_count_corner_points;

      if (format_info.want_centroid_points())
         total_index_count += total_info.index_count_centroid_points;

      // Allocate a single VBO big enough to hold all vertices
      auto vbo = make_shared<renderstack::graphics::buffer>(
         renderstack::graphics::buffer_target::array_buffer,
         total_vertex_count,
         buffer_info.vertex_format()->stride(),
         gl::buffer_usage_hint::static_draw
      );
      vbo->allocate_storage(*m_renderer);
      buffer_info.set_vertex_buffer(vbo);

      // Allocate a single IBO big enough to hold all indices
      auto ibo = make_shared<renderstack::graphics::buffer>(
         renderstack::graphics::buffer_target::element_array_buffer,
         total_index_count,
         4,                                  // stride
         gl::buffer_usage_hint::static_draw
      );
      ibo->allocate_storage(*m_renderer);

      buffer_info.set_index_buffer(ibo);
      
      size_t count = g_collection.size();
      float x = -float(count - 1);

      int pos = 0;
      for (auto i = g_collection.begin(); i != g_collection.end(); ++i)
      {
         mat4 position;
         renderstack::create_translation(x, 0.0f, 0.0f, position);
         auto gm = make_shared<renderstack::mesh::geometry_mesh>(*m_renderer, *i, format_info, buffer_info);
         auto m = make_shared<model>();
         m->set_geometry_mesh(gm);
         m->frame()->parent_from_local().set(position);
         m->frame()->update_hierarchical_no_cache();

         m_models.push_back(m);

         ++pos;
         x += 2.0f;
      }
   }
#endif

   // Font
#if defined(RENDERSTACK_USE_FREETYPE) && defined(USE_FONT)
   auto p = m_programs->font;
   auto m = p->mappings();

   m_font = make_shared<font>(*m_renderer, "res/fonts/Ubuntu-R.ttf", 10);
   m_text_buffer = make_shared<text_buffer>(m_gui_renderer, m_font, m);
#endif

   m_controls.home = vec3(0.0f, 1.7f, 10.0f);

   reset();

#if defined(USE_FONT)
   m_font_render_states.blend.set_enabled(true);
   m_font_render_states.blend.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
   m_font_render_states.blend.rgb().set_source_factor(gl::blending_factor_src::one);
   m_font_render_states.blend.rgb().set_destination_factor(gl::blending_factor_dest::one_minus_src_alpha);
#endif

#if defined(USE_GUI)
   setup_gui();
#endif
}

void game::setup_gui()
{
#if !defined(USE_GUI)
   slog_trace("game::setup_gui() - USE_GUI not defined, skipping");
   return;
#else
   slog_trace("game::setup_gui()");
   assert(m_application);

   auto gr = m_gui_renderer;
   auto bs = gr->button_style();
   auto cs = gr->choice_style();
   auto ms = gr->menulist_style();
   auto ps = gr->colorpicker_style();
   auto ss = gr->slider_style();

   float w = (float)m_application->width();
   float h = (float)m_application->height();
   rectangle size(w, h);

   m_root_layer = smart_ptr_builder::create_shared_ptr<area>(new layer(gr, size));
   m_root_layer->set_name("m_root_layer");

   auto d = smart_ptr_builder::create_shared_ptr<area>(new menulist(gr, ms, orientation::vertical));
   d->set_offset_free_size_relative(glm::vec2(  1.0f,  1.0f));
   d->set_offset_self_size_relative(glm::vec2( -1.0f, -1.0f));
   d->set_child_layout_style(area_layout_style::extend_horizontal);

   weak_ptr<action_sink> as = action_sink::shared_from_this();
   m_menu_button = smart_ptr_builder::create_shared_ptr<action_source, area>(
      new button(gr, "Back to Menu", bs)
   );
   m_menu_button->set_sink(as);
   d->add(m_menu_button);

#if 1
   std::shared_ptr<renderstack::ui::choice> c = renderstack::toolkit::smart_ptr_builder::create_shared_ptr<
      renderstack::ui::action_source, 
      renderstack::ui::choice,
      renderstack::ui::area>(
         new renderstack::ui::choice(gr, cs, bs, orientation::horizontal)
      );
   c->add_choice_item("Foo", true);

   c->add_choice_item("Bar");
   d->add(c);

   //auto p = new color_picker(ps);
   //d->add(p);

   m_slider = smart_ptr_builder::create_shared_ptr<action_source, area>(
      new slider(gr, ss, "Slider", 0.0f, 80.0f));
   d->add(m_slider);
#endif

   m_root_layer->add(d);
#endif
}

void game::on_resize(int width, int height)
{
   if (!m_screen_active)
      return;

#if !defined(USE_GUI)
   (void)width;
   (void)height;
   slog_trace("game::on_resize() - USE_GUI not defined, skipping");
   return;
#else
   slog_trace("game::on_resize()");

   float w = (float)width;   // (float)m_window->width();
   float h = (float)height;  // (float)m_window->height();

   auto gr = m_gui_renderer;
   gr->on_resize(width, height);

   if (m_root_layer)
   {
      gr->prepare();
      m_root_layer->set_layer_size(w, h);
      m_root_layer->update();
   }
#endif

   m_deferred_renderer->resize(width, height);
}

void game::action(weak_ptr<action_source> source)
{
#if !defined(USE_GUI)
   slog_trace("game::action() - USE_GUI not defined, skipping");
   return;
#else
   slog_trace("game::action()");

   auto s = source.lock();
   if (s == m_menu_button)
   {
      if (m_menu)
         m_application->set_screen(m_menu);
      else
         throw runtime_error("m_menu does not exist");
   }

   /*if (s == m_slider)
   {
      m_slider->relative_value();
   }*/
#endif
}

void game::on_enter()
{
   slog_trace("game::on_enter()");

   assert(m_application);

   m_screen_active = true;
   on_resize(m_application->width(), m_application->height());

   lock_mouse(true);
}
void game::on_exit()
{
   gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
   m_screen_active = false;
   m_application->capture_mouse(false);
}
void controls::reset()
{
   slog_trace("controls::reset()");

   camera_controller.clear();
   camera_controller.set_elevation(0.0f);
   camera_controller.set_heading(0.0f);
   camera_controller.set_position(home);
   fov   =    1.0f / 1.5f;
   near_ =    0.01f;
   far_  = 1000.0f;
   mouse_locked = true;
}
void game::lock_mouse(bool lock)
{
   m_controls.mouse_locked = lock;
   if (m_controls.mouse_locked)
   {
      int mouse_x = m_application->width() / 2;
      int mouse_y = m_application->height() / 2;
      //::glfwSetMousePos(mouse_x, mouse_y);
      //::glfwDisable(GLFW_MOUSE_CURSOR);
      //::glfwGetMousePos(&m_controls.mouse_x, &m_controls.mouse_y);

      m_application->set_mouse_pos(mouse_x, mouse_y);
      //m_application->set_input_mode(GLFW_CURSOR_MODE, GLFW_CURSOR_CAPTURED);
      m_application->capture_mouse(true);
      m_application->get_mouse_pos(m_controls.mouse_x, m_controls.mouse_y);

      //int dx = m_controls.mouse_x - mouse_x;
      //int dy = m_controls.mouse_y - mouse_y;

      //  Enable mouse tracking
      m_controls.ignore_mouse = true;
   }
   else
   {
      //m_application->set_input_mode(GLFW_CURSOR_MODE, GLFW_CURSOR_NORMAL);
      m_application->capture_mouse(false);
   }
}
void game::toggle_mouse_lock()
{
   assert(m_application);

   lock_mouse(!m_controls.mouse_locked);
}
void game::shift(bool left, bool value)
{
   if (left)
      m_controls.left_shift = value;
   else
      m_controls.right_shift = value;

   bool any_shift_pressed = m_controls.left_shift || m_controls.right_shift;
   m_controls.camera_controller.translate_y().set_less(any_shift_pressed);
}
void game::on_key(int key, int scancode, int action, int mods)
{
   bool pressed = action != 0;

   (void)mods;
   (void)scancode;

#if defined(RENDERSTACK_USE_GLFW)
   if (pressed)
   {
      switch (key)
      {
      case GLFW_KEY_ESCAPE:   toggle_mouse_lock(); break;
      case GLFW_KEY_F1:       m_min_frame_dt = 1.0; m_max_frame_dt = 0.0; break;
      case GLFW_KEY_B:        m_controls.fov *= 1.1f; break;
      case GLFW_KEY_N:        m_controls.fov /= 1.1f; break;
      case GLFW_KEY_M:        reset(); break;
      }
   }

   switch (key)
   {
   case GLFW_KEY_SPACE:  m_controls.camera_controller.translate_y().set_more(pressed); break;
   case GLFW_KEY_LEFT_SHIFT: shift(true, pressed); break;
   case GLFW_KEY_RIGHT_SHIFT: shift(false, pressed); break;
   case GLFW_KEY_W: m_controls.camera_controller.translate_z().set_less(pressed); break;
   case GLFW_KEY_S: m_controls.camera_controller.translate_z().set_more(pressed); break;
   case GLFW_KEY_D: m_controls.camera_controller.translate_x().set_more(pressed); break;
   case GLFW_KEY_A: m_controls.camera_controller.translate_x().set_less(pressed); break;
   }
#endif
#if defined(RENDERSTACK_USE_GLWT)
   if (pressed)
   {
      switch (key)
      {
      case GLWT_KEY_ESCAPE:   toggle_mouse_lock(); break;
      case GLWT_KEY_F1:       m_min_frame_dt = 1.0; m_max_frame_dt = 0.0; break;
      case GLWT_KEY_B:        m_controls.fov *= 1.1f; break;
      case GLWT_KEY_N:        m_controls.fov /= 1.1f; break;
      case GLWT_KEY_M:        reset(); break;
      }
   }

   switch (key)
   {
   case GLWT_KEY_SPACE:    m_controls.camera_controller.translate_y().set_more(pressed); break;
   case GLWT_KEY_LSHIFT:   shift(true, pressed); break;
   case GLWT_KEY_RSHIFT:   shift(false, pressed); break;
   case GLWT_KEY_W:        m_controls.camera_controller.translate_z().set_less(pressed); break;
   case GLWT_KEY_S:        m_controls.camera_controller.translate_z().set_more(pressed); break;
   case GLWT_KEY_D:        m_controls.camera_controller.translate_x().set_more(pressed); break;
   case GLWT_KEY_A:        m_controls.camera_controller.translate_x().set_less(pressed); break;
   }
#endif
}
void game::on_mouse_moved(double x, double y)
{
   if (m_controls.mouse_locked == false)
   {
      m_controls.mouse_x = x;
      m_controls.mouse_y = y;
      return;
   }
   if (m_controls.ignore_mouse == true)
   {
      m_controls.mouse_x = x;
      m_controls.mouse_y = y;
      m_controls.ignore_mouse = false;
      return;
   }

   if (!m_mouse_down)
   {
      m_controls.mouse_x = x;
      m_controls.mouse_y = y;
      return;
   }

   double x_delta = m_controls.mouse_x - x;
   double y_delta = m_controls.mouse_y - y;

   if (x_delta != 0)
   {
      float value = static_cast<float>(x_delta / 8192.0);
      m_controls.camera_controller.rotate_y().adjust(-value);
      m_controls.mouse_x = x;
   }

   if (y_delta != 0)
   {
      float value = static_cast<float>(y_delta / 8192.0);
      m_controls.camera_controller.rotate_x().adjust(-value);
      m_controls.mouse_y = y;
   }
}
void game::on_mouse_button(int button, int action, int mods)
{
   m_mouse_down = !!action;

   if (m_mouse_down)
   {

   }

   (void)button;
   (void)mods;
}
void game::on_scroll(double x, double y)
{
   (void)x;
   (void)y;
}
