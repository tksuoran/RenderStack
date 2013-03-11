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
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/build_info.hpp"
#include "renderstack_renderer/context.hpp"
#include "renderstack_renderer/renderer.hpp"
#include <cassert>

#if defined(RENDERSTACK_USE_GLFW)
# include <GL/glfw3.h>
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

{
}

void game::connect(
   std::shared_ptr<application>  application,
   std::shared_ptr<menu>         menu,
   std::shared_ptr<programs>     programs,
   std::shared_ptr<textures>     textures
)
{
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

   m_update_time     = 0.0;
   m_simulation_time = 0.0;

   m_application->set_time(0.0);
}
void game::on_load()
{
   slog_trace("game::on_load()");

#if defined(USE_MESHES)
   {
      vector<std::shared_ptr<renderstack::geometry::geometry>> g_collection;

      //g_collection.push_back(make_shared<renderstack::geometry::shapes::disc>(1.0, 0.8, 32, 2));
      //g_collection.push_back(make_shared<renderstack::geometry::shapes::triangle>(0.8f / 0.57735027f));
      //g_collection.push_back(make_shared<renderstack::geometry::shapes::sphere>(1.0f, 12, 4));
      //g_collection.push_back(make_shared<renderstack::geometry::shapes::cuboctahedron>(1.0));
      //g_collection.push_back(make_shared<renderstack::geometry::shapes::dodecahedron>(1.0));
      //g_collection.push_back(make_shared<renderstack::geometry::shapes::icosahedron>(1.0));
      //g_collection.push_back(make_shared<renderstack::geometry::shapes::octahedron>(1.0));
      g_collection.push_back(make_shared<renderstack::geometry::shapes::tetrahedron>(1.0));

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
      buffer_info.set_vertex_buffer(
         make_shared<renderstack::graphics::vertex_buffer>(
            total_vertex_count,
            buffer_info.vertex_format()->stride(),
            gl::buffer_usage_hint::static_draw
         )
      );
      // Allocate a single IBO big enough to hold all indices
      buffer_info.set_index_buffer(
         make_shared<renderstack::graphics::index_buffer>(
            total_index_count,
            4,                                  // stride
            ::buffer_usage_hint::static_draw
         )
      );
      
      size_t count = g_collection.size();
      float x = -float(count - 1);

      int pos = 0;
      for (auto i = g_collection.begin(); i != g_collection.end(); ++i)
      {
         mat4 position;
         renderstack::create_translation(x, 0.0f, 0.0f, position);
         auto gm = make_shared<renderstack::mesh::geometry_mesh>(*i, format_info, buffer_info);
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

   if (renderstack::graphics::configuration::can_use.uniform_buffer_object)
   {
      size_t size = 0;

#if defined(USE_MESHES) || defined(USE_FONT)
      ++size;
#endif

#if defined(USE_MESHES)
      ++size;
#endif

#if defined(USE_FONT)
      ++size;
#endif

      if (size > 0)
      {
         m_uniform_buffer = make_shared<uniform_buffer>
            (m_programs->block->size() * size
         );
      }

#if defined(USE_MESHES)
      m_mesh_render_uniform_buffer_range = make_shared<uniform_buffer_range>(
         m_programs->block,
         m_uniform_buffer
      );
#endif

#if defined(USE_FONT)
      m_text_uniform_buffer_range = make_shared<uniform_buffer_range>(
         m_programs->block,
         m_uniform_buffer
      );
#endif
   }

   // Font
#if defined(RENDERSTACK_USE_FREETYPE) && defined(USE_FONT)
   auto p = m_programs->font;
   auto m = p->mappings();

   m_font = make_shared<font>("res/fonts/Ubuntu-R.ttf", 10);
   m_text_buffer = make_shared<text_buffer>(m_font, m);
#endif

   m_controls.home = vec3(0.0f, 1.7f, 10.0f);

   assert(m_application);

   reset();

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

   auto uc = renderstack::ui::context::current();
   auto r  = uc->gui_renderer();
   auto bs = r->button_style();
   auto cs = r->choice_style();
   auto ms = r->menulist_style();
   auto ps = r->colorpicker_style();
   auto ss = r->slider_style();

   float w = (float)m_application->width();
   float h = (float)m_application->height();
   rectangle size(w, h);

   m_root_layer = smart_ptr_builder::create_shared_ptr<area>(new layer(size));
   m_root_layer->set_name("m_root_layer");

   auto d = smart_ptr_builder::create_shared_ptr<area>(new menulist(ms, orientation::vertical));
   d->set_offset_free_size_relative(glm::vec2(  1.0f,  1.0f));
   d->set_offset_self_size_relative(glm::vec2( -1.0f, -1.0f));
   d->set_child_layout_style(area_layout_style::extend_horizontal);

   weak_ptr<action_sink> as = action_sink::shared_from_this();
   m_menu_button = smart_ptr_builder::create_shared_ptr<action_source, area>(new button("Back to Menu",  bs)); m_menu_button->set_sink(as);
   d->add(m_menu_button);

#if 0
   std::shared_ptr<renderstack::ui::choice> c = renderstack::toolkit::smart_ptr_builder::create_shared_ptr<
      renderstack::ui::action_source, 
      renderstack::ui::choice,
      renderstack::ui::area>(
         new renderstack::ui::choice(cs, bs, orientation::horizontal)
      );
   c->add_choice_item("Foo", true);

   c->add_choice_item("Bar");
   d->add(c);

   //auto p = new color_picker(ps);
   //d->add(p);

   auto s = smart_ptr_builder::create_shared_ptr<action_source, area>(new slider(ss, "Slider", 0.0f, 1.0f));
   d->add(s);
#endif

   m_root_layer->add(d);
#endif
}

void game::on_resize(int width, int height)
{
#if !defined(USE_GUI)
   (void)width;
   (void)height;
   slog_trace("game::on_resize() - USE_GUI not defined, skipping");
   return;
#else
   slog_trace("game::on_resize()");

   float w = (float)width;   // (float)m_window->width();
   float h = (float)height;  // (float)m_window->height();

   auto uc = renderstack::ui::context::current();
   auto r = uc->gui_renderer();
   r->on_resize(width, height);

   if (m_root_layer)
   {
      r->prepare();
      m_root_layer->set_layer_size(w, h);
      m_root_layer->update();
   }
#endif
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
#endif
}

void game::on_enter()
{
   slog_trace("game::on_enter()");

   assert(m_application);

   gl::disable(gl::enable_cap::blend);
   gl::blend_func(gl::blending_factor_src::one, gl::blending_factor_dest::one_minus_src_alpha);

   on_resize(m_application->width(), m_application->height());

   lock_mouse(true);
}
void game::on_exit()
{
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
void game::on_key_down(int key)
{
#if defined(RENDERSTACK_USE_GLFW)
   switch (key)
   {
   case GLFW_KEY_ESC:    toggle_mouse_lock(); break;
   case GLFW_KEY_SPACE:  m_controls.camera_controller.translate_y().set_more(true); break;
   case GLFW_KEY_LSHIFT: shift(true, true); break;
   case GLFW_KEY_RSHIFT: shift(false, true); break;
   case GLFW_KEY_F1: m_min_frame_dt = 1.0; m_max_frame_dt = 0.0; break;
   case 'W': m_controls.camera_controller.translate_z().set_less(true); break;
   case 'S': m_controls.camera_controller.translate_z().set_more(true); break;
   case 'D': m_controls.camera_controller.translate_x().set_more(true); break;
   case 'A': m_controls.camera_controller.translate_x().set_less(true); break;
   case 'B': m_controls.fov *= 1.1f; break;
   case 'N': m_controls.fov /= 1.1f; break;
   case 'M': reset(); break;
   }
#else
   (void)key;
#endif
}
void game::on_key_up(int key)
{
#if defined(RENDERSTACK_USE_GLFW)
   switch (key)
   {
   case GLFW_KEY_SPACE:  m_controls.camera_controller.translate_y().set_more(false); break;
   case GLFW_KEY_LSHIFT: shift(true, false); break;
   case GLFW_KEY_RSHIFT: shift(false, false); break;
   case 'W': m_controls.camera_controller.translate_z().set_less(false); break;
   case 'S': m_controls.camera_controller.translate_z().set_more(false); break;
   case 'D': m_controls.camera_controller.translate_x().set_more(false); break;
   case 'A': m_controls.camera_controller.translate_x().set_less(false); break;
   }
#else
   (void)key;
#endif
}
void game::on_mouse_moved(int x, int y)
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

   int x_delta = x - m_controls.mouse_x;
   int y_delta = y - m_controls.mouse_y;

   if (x_delta != 0)
   {
      float value = (float)(x_delta) / 8192.0f;
      m_controls.camera_controller.rotate_y().adjust(-value);
      m_controls.mouse_x = x;
   }

   if (y_delta != 0)
   {
      float value = (float)(y_delta) / 8192.0f;
      m_controls.camera_controller.rotate_x().adjust(-value);
      m_controls.mouse_y = y;
   }
}
void game::on_mouse_button(int button, int value)
{
   (void)button;
   (void)value;
}
void game::on_scroll(double x, double y)
{
   (void)x;
   (void)y;
}
