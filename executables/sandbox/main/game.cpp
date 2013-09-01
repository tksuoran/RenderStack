#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_geometry/shapes/sphere.hpp"
#include "renderstack_geometry/shapes/disc.hpp"
#include "renderstack_geometry/shapes/triangle.hpp"
#include "renderstack_geometry/shapes/cone.hpp"
#include "renderstack_geometry/shapes/cube.hpp"
#include "renderstack_geometry/operation/clone.hpp"
#include "renderstack_geometry/operation/catmull_clark.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/build_info.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/button.hpp"
#include "renderstack_ui/choice.hpp"
#include "renderstack_ui/color_picker.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/menulist.hpp"
#include "renderstack_ui/push_button.hpp"
#include "renderstack_ui/slider.hpp"
#include "main/game.hpp"
#include "main/application.hpp"
#include "main/textures.hpp"
#include "main/menu.hpp"
#include "main/programs.hpp"
#include "main/log.hpp"
#include "parsers/xml_polyhedron.hpp"
#include "scene/group.hpp"
#include <cassert>

#if defined(RENDERSTACK_USE_GLFW)
# include <GLFW/glfw3.h>
#endif
#if defined(RENDERSTACK_USE_GLWT)
# include <GLWT/glwt.h>
#endif

// #define SHARED_BUFFERS 1

#define LOG_CATEGORY &log_game

#define USE_MESHES      1
#define USE_FONT        1
#define USE_GUI         1

using namespace renderstack::graphics;
using namespace renderstack::toolkit;
using namespace renderstack::mesh;
using namespace renderstack::ui;
using namespace std;
using namespace gl;
using namespace glm;

game::game()
:  service("game")

/* services */
,  m_renderer           (nullptr)
,  m_gui_renderer       (nullptr)
,  m_programs           (nullptr)
,  m_textures           (nullptr)
,  m_debug_renderer     (nullptr)
,  m_forward_renderer   (nullptr)
,  m_deferred_renderer  (nullptr)
,  m_id_renderer        (nullptr)
,  m_menu               (nullptr)
,  m_application        (nullptr)
,  m_shader_monitor     (nullptr)

/* self owned parts */
,  m_models             (nullptr)
,  m_manipulator_frame  (nullptr)
,  m_manipulator        (nullptr)
,  m_root_layer         (nullptr)
,  m_menu_button        (nullptr)
,  m_slider             (nullptr)

,  m_update_time        (0.0)
,  m_frame_dt           (0.0)
,  m_min_frame_dt       (0.0)
,  m_max_frame_dt       (0.0)
,  m_simulation_time    (0.0)
,  m_screen_active      (false)
,  m_mouse_down         (false)

,  m_camera             (nullptr)
{
}

/*virtual*/ game::~game()
{
}

void game::connect(
   shared_ptr<renderstack::graphics::renderer>        renderer,
   shared_ptr<renderstack::graphics::shader_monitor>  shader_monitor,
   shared_ptr<renderstack::ui::gui_renderer>          gui_renderer,
   shared_ptr<programs>                               programs_,
   shared_ptr<textures>                               textures_,
   shared_ptr<debug_renderer>                         debug_renderer_,
   shared_ptr<forward_renderer>                       forward_renderer_,
   shared_ptr<deferred_renderer>                      deferred_renderer_,
   shared_ptr<id_renderer>                            id_renderer_,
   shared_ptr<menu>                                   menu_,
   shared_ptr<application>                            application_
)
{
   m_renderer           = renderer;
   m_shader_monitor     = shader_monitor;
   m_gui_renderer       = gui_renderer;
   m_application        = application_;
   m_menu               = menu_;
   m_programs           = programs_;
   m_textures           = textures_;
   m_deferred_renderer  = deferred_renderer_;
   m_forward_renderer   = forward_renderer_;
   m_id_renderer        = id_renderer_;
   m_debug_renderer     = debug_renderer_;

   initialization_depends_on(renderer);
   initialization_depends_on(gui_renderer);
   initialization_depends_on(programs_);
   initialization_depends_on(textures_);
}

void game::disconnect()
{
   slog_trace("game::disconnect()");

   m_application.reset();
   m_menu.reset();
   m_programs.reset();
   m_textures.reset();
}

void game::reset_build_info()
{
   m_format_info = geometry_mesh_format_info();
   m_buffer_info = geometry_mesh_buffer_info();
}

shared_ptr<model> game::make_model(
   shared_ptr<renderstack::scene::frame> parent,
   shared_ptr<renderstack::geometry::geometry> g,
   vec3 position
)
{
   geometry_mesh::prepare_vertex_format(g, m_format_info, m_buffer_info);
   mat4 transform;
   create_translation(position, transform);

   auto gm = make_shared<renderstack::mesh::geometry_mesh>(*m_renderer, g, m_format_info, m_buffer_info);
   auto m = make_shared<model>();
   m->set_geometry_mesh(gm);
   m->frame()->set_parent(parent);
   m->frame()->parent_from_local().set(transform);
   m->frame()->update_hierarchical_no_cache();

   return m;
}

void game::initialize_service()
{
   assert(m_renderer);
   assert(m_gui_renderer);
   assert(m_programs);
   assert(m_textures);

   slog_trace("game::on_load()");


   m_models = make_shared<group>();

   m_camera = make_shared<renderstack::scene::camera>();
   m_camera->projection().set_fov_y(1.0f / 1.5f);
   m_camera->projection().set_near(0.01f);
   m_camera->projection().set_far(1000.0f);

   m_controls.camera_controller.set_frame(m_camera->frame());
   m_controls.home = vec3(0.0f, 1.7f, 10.0f);

#if defined(USE_MESHES)
   {
      vector<shared_ptr<renderstack::geometry::geometry>> g_collection;

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

      g_collection.push_back(make_shared<renderstack::geometry::shapes::dodecahedron>(2.0));
      auto g0 = make_shared<renderstack::geometry::shapes::dodecahedron>(2.0);
      g0->build_edges();

      auto o1 = make_shared<renderstack::geometry::operation::catmull_clark>(g0);
      auto g1 = o1->destination();

      auto o2 = make_shared<renderstack::geometry::operation::catmull_clark>(g1);
      auto g2 = o2->destination();

      g2->compute_polygon_normals();
      g2->compute_point_normals("point_normals");
      g_collection.push_back(g2);

      auto xml = make_shared<xml_polyhedron>("res/polyhedra/127.xml");
      g2->compute_polygon_normals();
      g_collection.push_back(xml);

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
#if SHARED_BUFFERS
      renderstack::geometry::geometry::mesh_info total_info;
#endif
      renderstack::geometry::geometry::mesh_info info;
      reset_build_info();
      m_format_info.set_want_fill_triangles(true);
      m_format_info.set_want_edge_lines(true);
      m_format_info.set_want_position(true);
      m_format_info.set_want_normal(true);
      m_format_info.set_want_tangent(true);
      m_format_info.set_want_color(true);
      m_format_info.set_want_id(true);
      m_format_info.set_normal_style(normal_style::corner_normals);
      m_format_info.set_mappings(m_programs->mappings);
      for (auto i = g_collection.begin(); i != g_collection.end(); ++i)
      {
         (*i)->build_edges();
         (*i)->info(info);
#if SHARED_BUFFERS
         total_info += info;
#endif
         geometry_mesh::prepare_vertex_format(*i, m_format_info, m_buffer_info);
      }

#if SHARED_BUFFERS
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
      m_buffer_info.set_vertex_buffer(vbo);

      // Allocate a single IBO big enough to hold all indices
      auto ibo = make_shared<renderstack::graphics::buffer>(
         renderstack::graphics::buffer_target::element_array_buffer,
         total_index_count,
         4,                                  // stride
         gl::buffer_usage_hint::static_draw
      );
      ibo->allocate_storage(*m_renderer);

      m_buffer_info.set_index_buffer(ibo);
#endif
      
      size_t count = g_collection.size();
      float x = -float(count - 1);

      int pos = 0;
      for (auto i = g_collection.begin(); i != g_collection.end(); ++i)
      {
         auto m = make_model(nullptr, *i, vec3(x, 0.0f, 0.0f));
         m_models->add(m);

         ++pos;
         x += 2.0f;
      }

# if 1
      {
         reset_build_info();
         m_format_info.set_want_fill_triangles(true);
         m_format_info.set_want_position(true);
         m_format_info.set_want_normal(true);
         m_format_info.set_want_tangent(true);
         m_format_info.set_want_color(true);
         m_format_info.set_want_id(true);
         m_format_info.set_normal_style(normal_style::corner_normals);
         m_format_info.set_mappings(m_programs->mappings);
         m_manipulator_frame = make_shared<renderstack::scene::frame>();
         m_format_info.set_constant_color(vec4(1.0f, 0.0f, 0.0f, 1.0f));
         mat4 x_to_y;
         mat4 x_to_z;
         
         auto x_tip_g  = make_shared<renderstack::geometry::shapes::cone      >(0.75f, 1.00f, 0.15f, true, 18, 2);
         auto x_tail_g = make_shared<renderstack::geometry::shapes::cylinder  >(0.00f, 0.75f, 0.03f, true, false, 12, 2);
         auto x_tip_m  = make_model(m_manipulator_frame, x_tip_g);
         auto x_tail_m = make_model(m_manipulator_frame, x_tail_g);
         m_manipulator = make_shared<group>();
         m_manipulator->add(x_tip_m);
         m_manipulator->add(x_tail_m);
      }
# endif
   }
#endif

   reset();

#if defined(USE_GUI)
   setup_gui();
#endif
}

void game::reset()
{
   slog_trace("game::reset()");

   m_controls.reset();

   m_frame_dt        = 0.0;
   m_update_time     = m_application->time();
   m_simulation_time = m_update_time;
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
   d->set_offset_free_size_relative(vec2(  1.0f,  1.0f));
   d->set_offset_self_size_relative(vec2( -1.0f, -1.0f));
   d->set_child_layout_style(area_layout_style::extend_horizontal);

   weak_ptr<action_sink> as = action_sink::shared_from_this();
   m_menu_button = smart_ptr_builder::create_shared_ptr<action_source, area>(
      new button(gr, "Back to Menu", bs)
   );
   m_menu_button->set_sink(as);
   d->add(m_menu_button);

#if 1
   shared_ptr<renderstack::ui::choice> c = renderstack::toolkit::smart_ptr_builder::create_shared_ptr<
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

   m_viewport.set_width(width);
   m_viewport.set_height(height);

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
   mouse_locked = true;
}
void game::lock_mouse(bool lock)
{
   m_controls.mouse_locked = lock;

#if 0
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
#endif
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
      case GLFW_KEY_B:        /* m_controls.fov *= 1.1f; TODO */ break;
      case GLFW_KEY_N:        /* m_controls.fov /= 1.1f; TODO */ break;
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
      case GLWT_KEY_B:        /* m_controls.fov *= 1.1f; TODO */ break;
      case GLWT_KEY_N:        /* m_controls.fov /= 1.1f; TODO */ break;
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
