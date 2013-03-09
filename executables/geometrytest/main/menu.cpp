//#define DEBUG_FONT 1

#include "renderstack_toolkit/platform.hpp"
#include "main/application.hpp"
#include "main/menu.hpp"
#include "main/game.hpp"
#include "main/programs.hpp"
#include "main/textures.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_renderer/context.hpp"
#include "renderstack_renderer/renderer.hpp"
#include "renderstack_ui/button.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/menulist.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define LOG_CATEGORY &log_menu

using namespace gl;
using namespace glm;
using namespace std;
using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace renderstack::ui;
using namespace renderstack::toolkit;

//#define DEBUG_FONT
#define RENDER_TEXT
#define RENDER_BACKGROUND
#define RENDER_GUI

menu::menu()
{
}

menu::~menu()
{
}

void menu::connect(
   shared_ptr<application> application,
   shared_ptr<class game>  game,
   shared_ptr<programs>    programs,
   shared_ptr<textures>    textures
)
{
   m_application = application;
   m_game = game;
   m_programs = programs;
   m_textures = textures;
}
void menu::disconnect()
{
   slog_trace("menu::disconnect()");

   m_application.reset();
   m_game.reset();
   m_programs.reset();
   m_textures.reset();
}
void menu::action(weak_ptr<action_source> source)
{
   auto s = source.lock();
   if (s == m_map_editor)
   {
      assert(m_game);
      m_application->set_screen(m_game);
   }

   if (s == m_quit)
      m_application->close();
}
void menu::on_load()
{
   slog_trace("menu::on_load()");

   if (renderstack::graphics::configuration::can_use.uniform_buffer_object)
   {
      m_uniform_buffer = make_shared<uniform_buffer>(m_programs->block->size());
      m_uniform_buffer_range = make_shared<uniform_buffer_range>(
         m_programs->block,
         m_uniform_buffer
      );
   }

   m_vertex_format = make_shared<renderstack::graphics::vertex_format>();
   m_vertex_format->make_attribute(
      static_cast<vertex_attribute_usage::value>(
         vertex_attribute_usage::position | vertex_attribute_usage::tex_coord
      ),
      gl::vertex_attrib_pointer_type::float_,
      gl::vertex_attrib_pointer_type::float_,
      0, 
      4,
      false
   );

   m_vertex_stream = make_shared<vertex_stream>();

#if defined(RENDER_TEXT) || defined(DEBUG_FONT)
   m_font = make_shared<font>("res/fonts/Ubuntu-R.ttf", 48, 4.0f);
   m_text_buffer = make_shared<text_buffer>(m_font);
#endif

   auto gc = renderstack::graphics::context::current();
   auto uc = renderstack::ui::context::current();

#if defined(RENDER_BACKGROUND)
   gc->global_vertex_stream_mappings()->bind_attributes(
      *m_vertex_stream.get(),
      *m_vertex_format.get()
   );
   m_vertex_stream->use();

   m_mesh = make_shared<renderstack::mesh::mesh>();

   m_mesh->allocate_vertex_buffer(uc->get_2d_vertex_buffer(), 4);
   m_mesh->allocate_index_buffer(uc->get_2d_index_buffer(), 6);

   m_mesh->vertex_buffer()->bind();
   m_mesh->index_buffer()->bind();

   /*  Write indices for one quad  */
   {
      unsigned short *start = static_cast<unsigned short *>(
         m_mesh->index_buffer()->map_indices(
         m_mesh->first_index(), 
            m_mesh->index_count(), 
            (gl::buffer_access_mask::value)
            (
               gl::buffer_access_mask::map_write_bit | 
               gl::buffer_access_mask::map_invalidate_range_bit
            )
         )
      );
      unsigned short *ptr = start;
      *ptr++ = 0;
      *ptr++ = 2;
      *ptr++ = 3;
      *ptr++ = 0;
      *ptr++ = 1;
      *ptr++ = 2;
      m_mesh->index_buffer()->unmap_indices();
   }
#endif

#if defined(RENDER_GUI)
   auto r = uc->gui_renderer();
   auto bs = r->button_style();
   auto ms = r->menulist_style();

   float w = (float)m_application->width();
   float h = (float)m_application->height();
   rectangle size(w, h);

   m_root_layer = smart_ptr_builder::create_shared_ptr<area>(new layer(size));
   m_root_layer->set_name("m_root_layer");

   auto d = smart_ptr_builder::create_shared_ptr<area>(new menulist(ms, orientation::vertical));
   d->set_offset_free_size_relative(glm::vec2( 0.50f,  0.25f));
   d->set_offset_self_size_relative(glm::vec2(-0.50f, -0.50f));
   d->set_child_layout_style(area_layout_style::extend_horizontal);

   weak_ptr<action_sink> as = action_sink::shared_from_this();
   m_map_editor = smart_ptr_builder::create_shared_ptr<action_source, area>(new button("Map Editor", bs));
   m_quit       = smart_ptr_builder::create_shared_ptr<action_source, area>(new button("Quit",       bs));

   m_map_editor->set_sink(as);
   m_quit->set_sink(as);

   d->add(m_quit);      
   d->add(m_map_editor);

   m_root_layer->add(d);
#endif
}
void menu::on_resize(int width, int height)
{
   slog_trace("menu::on_resize()");

   // Test all conditions; can_use.uniform_buffer_object can be forced to false
   bool use_uniform_buffers = 
      renderstack::graphics::configuration::can_use.uniform_buffer_object &&
      (m_programs->glsl_version() >= 140) &&
      (renderstack::graphics::configuration::shader_model_version >= 4);

   float w = (float)width;   // (float)m_window->width();
   float h = (float)height;  // (float)m_window->height();

#if defined(RENDER_GUI)
   auto uc = renderstack::ui::context::current();
   auto r = uc->gui_renderer();
   r->prepare();
   r->on_resize(width, height);

   m_root_layer->set_layer_size(w, h);
   m_root_layer->update();
#endif

   //  viewport
   gl::viewport(0, 0, (GLsizei)w, (GLsizei)h);

   mat4        ortho = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
   glm::vec4   white(1.0f, 1.0f, 1.0f, 1.0f);

   if (use_uniform_buffers)
   {
      uniform_offsets &o = m_programs->uniform_offsets;
      m_uniform_buffer_range->bind(m_programs->block->binding_point());
      unsigned char *start = m_uniform_buffer_range->begin_edit();

      ::memcpy(&start[o.model_to_clip],   value_ptr(ortho), 16 * sizeof(float));
      ::memcpy(&start[o.color],           value_ptr(white),  4 * sizeof(float));
      m_uniform_buffer_range->end_edit();

   }
   else
   {

#if defined(RENDER_BACKGROUND)
      {
         auto p = m_programs->textured_gui;
         p->use();
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.model_to_clip), 1, GL_FALSE, value_ptr(ortho));
         gl::uniform_4fv       (p->uniform_at(m_programs->uniform_keys.color        ), 1, value_ptr(white));
      }
#endif

#if (defined(RENDER_TEXT) || defined(RENDER_GUI))
      {
         auto p = m_programs->font;
         p->use();
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.model_to_clip), 1, GL_FALSE, value_ptr(ortho));
         gl::uniform_4fv       (p->uniform_at(m_programs->uniform_keys.color        ), 1, value_ptr(white));
      }
#endif
   }

#if defined(RENDER_BACKGROUND)
   {
      /*  Write corner vertices for one quad  */
      m_vertex_stream->use();
      m_mesh->vertex_buffer()->bind();
      float *ptr = (float*)m_mesh->vertex_buffer()->map_vertices(
         m_mesh->first_vertex(),
         m_mesh->vertex_count(),
         (gl::buffer_access_mask::value)
         (
            gl::buffer_access_mask::map_write_bit | 
            gl::buffer_access_mask::map_invalidate_range_bit
         )
      );

#if defined(DEBUG_FONT)
      float max_x = static_cast<float>(m_font->texture_width());
      float max_y = static_cast<float>(m_font->texture_height());
#else
      float max_x = w;
      float max_y = h;
#endif

      *ptr++ =    0.0f; *ptr++ =  0.0f; *ptr++ = 0.0f; *ptr++ = 0.0f;
      *ptr++ =   max_x; *ptr++ =  0.0f; *ptr++ = 1.0f; *ptr++ = 0.0f;
      *ptr++ =   max_x; *ptr++ = max_y; *ptr++ = 1.0f; *ptr++ = 1.0f;
      *ptr++ =    0.0f; *ptr++ = max_y; *ptr++ = 0.0f; *ptr++ = 1.0f;

      m_mesh->vertex_buffer()->unmap_vertices();
   }
#endif

#if defined(RENDER_TEXT)
   std::string title = "Hello, World!";
   //std::string title = "The quick brown fox jumps over the lazy dog. 1234567890";
   //std::string title = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG. 1234567890";
   //std::string title = "Volumetric Elements";
   float x  = std::floor(w / 2.0f);
   float y  = std::ceil(3.0f * h / 4.0f);

   m_text_buffer->begin_print();
   m_text_buffer->print_center(title, x, y);
   m_text_buffer->end_print();
#endif
}
void menu::update()
{
   slog_trace("menu::update()");

   m_programs->update_fixed_step();
   render();
}
void menu::on_key_down(int key)
{
   (void)key;
}
void menu::on_key_up(int key)
{
   (void)key;
}
void menu::on_mouse_moved(int x, int y)
{
   (void)x;
   (void)y;
}
void menu::on_mouse_button(int button, int value)
{
   (void)button;
   (void)value;
}
void menu::on_scroll(double x, double y)
{
   (void)x;
   (void)y;
}
void menu::on_enter()
{
   slog_trace("menu::on_enter()");

   gl::depth_mask(0);
   gl::disable(gl::enable_cap::cull_face);
   gl::disable(gl::enable_cap::depth_test);

   //gl::blend_equation(gl::blend_equation_mode::func_add);
   gl::blend_func(gl::blending_factor_src::one, gl::blending_factor_dest::one_minus_src_alpha);

   gl::clear_color(0.3f, 0.1f, 0.2f, 0.0f);

   gl::disable(gl::enable_cap::scissor_test);

   on_resize(m_application->width(), m_application->height());
}
void menu::on_exit()
{
   slog_trace("menu::on_exit()");

   gl::depth_mask(1);
   gl::enable(gl::enable_cap::cull_face);
   gl::enable(gl::enable_cap::depth_test);
}
void menu::render()
{
   slog_trace("menu::render()");

   gl::clear_color(0.5f, 0.0f, 0.0f, 1.0f);
   gl::clear(clear_buffer_mask::color_buffer_bit | clear_buffer_mask::depth_buffer_bit);

   assert(m_application);
   assert(m_programs);

   //  background
#if defined(RENDER_BACKGROUND) || defined(DEBUG_FONT)
   if (m_textures)
   {
      auto p = m_programs->textured_gui;
      p->use();

      active_texture(gl::texture_unit::texture0 + 1);

#if defined(DEBUG_FONT)
      bind_texture(texture_target::texture_2d, m_font->texture());
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_min_filter, texture_min_filter::nearest);
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_mag_filter, texture_mag_filter::nearest);
#else
      bind_texture(texture_target::texture_2d, m_textures->background_texture_object);
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_min_filter, texture_min_filter::linear);
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_mag_filter, texture_mag_filter::linear);
#endif
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_wrap_r,     gl::texture_wrap_mode::clamp_to_edge);
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_wrap_s,     gl::texture_wrap_mode::clamp_to_edge);
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_wrap_t,     gl::texture_wrap_mode::clamp_to_edge);

      //  1.) Bind vertex stream (vertex array object)
      assert(m_vertex_stream);

      m_vertex_stream->use();

      //  2.) Bind VBO. This makes sure VAO has the right VBO
      assert(m_mesh);

      m_mesh->vertex_buffer()->bind();
      m_mesh->index_buffer()->bind();

      //  3.) Issue draw call
      m_vertex_stream->draw_elements_base_vertex(
         gl::begin_mode::triangles,
         6,
         gl::draw_elements_type::unsigned_short,
         0,
         m_mesh->first_vertex()
      );
   }
#endif

#if defined(RENDER_TEXT)
   {
      auto p = m_programs->font;

      p->use();

      assert(m_font);
      assert(m_text_buffer);

      enable(gl::enable_cap::blend);
      active_texture(GL_TEXTURE0);
      bind_texture(texture_target::texture_2d, m_font->texture());
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_min_filter, texture_min_filter::nearest);
      tex_parameter_i(texture_target::texture_2d, texture_parameter_name::texture_mag_filter, texture_mag_filter::nearest);
      m_text_buffer->render();
      disable(gl::enable_cap::blend);
   }
#endif

#if defined(RENDER_GUI)
   {
      ui_context c;
      int iw = m_application->width();
      int ih = m_application->height();

      m_application->get_mouse_pos(c.mouse.x, c.mouse.y);
      c.mouse.y = ih - 1 - c.mouse.y;
# if defined(RENDERSTACK_USE_GLFW)
      c.mouse_buttons[0] = (m_application->get_mouse_button(0) == GLFW_PRESS);
      c.mouse_buttons[1] = (m_application->get_mouse_button(1) == GLFW_PRESS);
      c.mouse_buttons[2] = (m_application->get_mouse_button(2) == GLFW_PRESS);
# endif
      auto uc = renderstack::ui::context::current();
      auto r = uc->gui_renderer();
      r->prepare();
      r->on_resize(iw, ih);
      m_root_layer->draw(c);
   }
#endif

   m_application->swap_buffers();
}
