#include "renderstack_toolkit/platform.hpp"
#include "main/game.hpp"
#include "main/application.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/slider.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>

#define LOG_CATEGORY &log_game

#define EXTRA_DEBUG 1

using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace renderstack::ui;
using namespace renderstack::toolkit;
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;

void game::render_ui()
{
   slog_trace("game::render_ui()");

   assert(m_programs);
   assert(m_text_buffer);

   float w = (float)m_application->width();
   float h = (float)m_application->height();

   m_text_buffer->begin_print();
   for (size_t i = 0; i < m_debug_lines.size(); ++i)
      m_text_buffer->print(m_debug_lines[i], 0.0f, h - (i + 1) * m_font->line_height());

   if (m_controls.mouse_locked)
      m_text_buffer->print("Mouse locked", 0.0f, 0.0f);

   int chars_printed = m_text_buffer->end_print();

   if (chars_printed > 0)
   {
      auto r = m_gui_renderer;

      auto p = m_programs->font;
      r->set_program(p);

      gl::viewport(0, 0, (GLsizei)w, (GLsizei)h);
      mat4 ortho = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
      gl::scissor(0, 0, (GLsizei)w, (GLsizei)h);

      glm::vec4 white(1.0f, 1.0f, 1.0f, 0.66f); // gamma in 4th component
      if (use_uniform_buffers())
      {
         if (m_text_uniform_buffer_range)
         {
            uniform_offsets &o = m_programs->uniform_offsets;
            r->renderer().set_uniform_buffer_range(
               m_programs->block->binding_point(),
               m_text_uniform_buffer_range);
            unsigned char *start = m_text_uniform_buffer_range->begin_edit(r->renderer());
            ::memcpy(&start[o.clip_from_model], value_ptr(ortho), 16 * sizeof(float));
            ::memcpy(&start[o.color],           value_ptr(white), 4 * sizeof(float));
            m_text_uniform_buffer_range->end_edit(r->renderer());
         }
      }
      else
      {
         int model_to_clip_ui = p->uniform_at(m_programs->uniform_keys.clip_from_model);
         int color_ui         = p->uniform_at(m_programs->uniform_keys.color);

         gl::uniform_matrix_4fv(model_to_clip_ui, 1, GL_FALSE, value_ptr(ortho));
         gl::uniform_4fv(color_ui, 1, value_ptr(white));
      }

      m_renderer->track.execute(&m_font_render_states);

      int texture_ui = p->uniform("font_texture")->index();
      gl::uniform_1i(texture_ui, 0);

      auto t = m_text_buffer->font()->texture();
      t->set_min_filter(texture_min_filter::nearest);
      t->set_mag_filter(texture_mag_filter::nearest);
      (void)m_renderer->set_texture(0, t);
      t->apply(*m_renderer, 0);

      m_text_buffer->render();
   }

}

void game::render(shared_ptr<renderstack::mesh::geometry_mesh> geometry_mesh)
{
   auto vertex_stream   = geometry_mesh->vertex_stream();
   auto mesh            = geometry_mesh->get_mesh();

   gl::begin_mode::value         begin_mode     = gl::begin_mode::triangles;
   index_range const             &index_range   = geometry_mesh->fill_indices();
   GLsizei                       count          = static_cast<GLsizei>(index_range.index_count);
   gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_int;
   GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>((index_range.first_index + mesh->first_index()) * mesh->index_buffer()->stride());
   GLint                         base_vertex    = configuration::can_use.draw_elements_base_vertex
      ? static_cast<GLint>(mesh->first_vertex())
      : 0;

   m_renderer->draw_elements_base_vertex(
      vertex_stream,
      begin_mode, count, index_type, index_pointer, base_vertex);
}

void game::render_meshes()
{
   slog_trace("game::render_meshes()");

   if (m_models.size() == 0)
      return;

   auto r = m_renderer;

   gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
   {
      GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
      gl::draw_buffers(4, draw_buffers);
   }
   fbo_clear();

   {
      r->track.execute(&m_mesh_render_states);

      auto p = m_programs->gbuffer;

      r->set_program(p);

      // Temp assignments
      // x = specular exponent
      // y = 
      // z = 
      // w = solidity
      vec4 material_parameters(1.0f, 0.0f, 0.0f, 1.0f);
      if (use_uniform_buffers())
      {
         assert(m_mesh_render_uniform_buffer_range);
         r->set_uniform_buffer_range(
            m_programs->block->binding_point(),
            m_mesh_render_uniform_buffer_range);
      }
      uniform_offsets &o = m_programs->uniform_offsets;

      for (auto i = m_models.cbegin(); i != m_models.cend(); ++i)
      {
         auto  model             = *i;
         mat4  world_from_model  = model->frame()->world_from_local().matrix();
         mat4  clip_from_model   = m_controls.clip_from_world * world_from_model;
         mat4  view_from_model   = m_controls.camera_controller.local_from_parent() * world_from_model;
         auto  geometry_mesh     = model->geometry_mesh();
         auto  vertex_stream     = geometry_mesh->vertex_stream();
         auto  mesh              = geometry_mesh->get_mesh();

         material_parameters.x = m_slider->current_display_value();

         if (use_uniform_buffers())
         {
            unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit(*r);
            ::memcpy(&start[o.clip_from_model      ], value_ptr(clip_from_model),      16 * sizeof(float));
            ::memcpy(&start[o.view_from_model      ], value_ptr(view_from_model),      16 * sizeof(float));
            ::memcpy(&start[o.material_parameters  ], value_ptr(material_parameters),  4 * sizeof(float));
            m_mesh_render_uniform_buffer_range->end_edit(*r);
         }
         else
         {
            gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.clip_from_model), 1, GL_FALSE, value_ptr(clip_from_model));
            gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.view_from_model), 1, GL_FALSE, value_ptr(view_from_model));
            gl::uniform_4fv(p->uniform_at(m_programs->uniform_keys.material_parameters),  1, value_ptr(material_parameters));
         }

         {
            gl::begin_mode::value         begin_mode     = gl::begin_mode::triangles;
            index_range const             &index_range   = geometry_mesh->fill_indices();
            GLsizei                       count          = static_cast<GLsizei>(index_range.index_count);
            gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_int;
            GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>((index_range.first_index + mesh->first_index()) * mesh->index_buffer()->stride());
            GLint                         base_vertex    = configuration::can_use.draw_elements_base_vertex
               ? static_cast<GLint>(mesh->first_vertex())
               : 0;

            r->draw_elements_base_vertex(
               model->geometry_mesh()->vertex_stream(),
               begin_mode, count, index_type, index_pointer, base_vertex);
         }
      }
   }

#if 1
   {
      r->track.execute(&m_light_render_states);

      // Temp draw directly to screen
      gl::bind_framebuffer(gl::framebuffer_target::draw_framebuffer, 0);
#if defined(RENDERSTACK_GL_API_OPENGL)
      gl::draw_buffer(GL_BACK);
#endif

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
      GLenum draw_buffer = GL_BACK;
      gl::draw_buffers(1, &draw_buffer);
#endif
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      //GLenum color0 = GL_COLOR_ATTACHMENT0;
      //gl::draw_buffers(1, &color0);

      auto p = m_programs->light;
      r->set_program(p);
      // Don't bind emission texture for now
      r->reset_texture(0, renderstack::graphics::texture_target::texture_2d, nullptr);
      r->set_texture(1, m_rt[1], nullptr);
      r->set_texture(2, m_rt[2], nullptr);
      r->set_texture(3, m_rt[3], nullptr);

      mat4 identity = mat4(1.0f);
      mat4 const &world_from_view = m_controls.camera_controller.parent_from_local();

      if (use_uniform_buffers())
      {
         if (m_mesh_render_uniform_buffer_range)
         {
            r->set_uniform_buffer_range(
               m_programs->block->binding_point(),
               m_mesh_render_uniform_buffer_range);

            uniform_offsets &o = m_programs->uniform_offsets;
            unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit(*r);
            ::memcpy(&start[o.clip_from_model], value_ptr(identity), 16 * sizeof(float));
            ::memcpy(&start[o.world_from_view], value_ptr(world_from_view), 16 * sizeof(float));
            m_mesh_render_uniform_buffer_range->end_edit(*r);
         }
      }
      else
      {
         gl::uniform_matrix_4fv(
            p->uniform_at(m_programs->uniform_keys.clip_from_model),
            1, GL_FALSE, value_ptr(identity));
         gl::uniform_matrix_4fv(
            p->uniform_at(m_programs->uniform_keys.world_from_view),
            1, GL_FALSE, value_ptr(world_from_view));
      }

      m_quad_renderer->render_minus_one_to_one();

      //int iw = m_application->width();
      //int ih = m_application->height();

      //gl::bind_framebuffer(gl::framebuffer_target::read_framebuffer, m_fbo);
      //gl::bind_framebuffer(gl::framebuffer_target::draw_framebuffer, 0);

      /*
      gl::blit_framebuffer(
         0, 0, iw, ih,
         0, 0, iw, ih,
         GL_COLOR_BUFFER_BIT,
         GL_NEAREST);*/
   }
#endif

   // Debug visualization
#if 1
   m_show_rt_render_states.face_cull.set_enabled(false);
   r->track.execute(&m_show_rt_render_states);
   for (int i = 0; i < 4; ++i)
   {
      r->set_texture(0, m_rt[i], nullptr);

      std::shared_ptr<renderstack::graphics::program> p = (i == 2)
            ? m_programs->show_rt_spherical
            : m_programs->show_rt;
      r->set_program(p);

      mat4 identity = mat4(1.0f);
      //mat4 const &ortho = m_gui_renderer->ortho();
      mat4 scale;
      mat4 to_bottom_left;
      mat4 offset;
      create_translation(-1.0, -1.0, 0.0f, to_bottom_left);
      create_scale(0.33f, scale);
      create_translation(0.33f * i, 0.0, 0.0f, offset);

      mat4 transform = identity;

      transform = scale * transform;
      transform = to_bottom_left * transform;
      transform = offset * transform;

      if (use_uniform_buffers())
      {
         assert(m_mesh_render_uniform_buffer_range);
         r->set_uniform_buffer_range(
            m_programs->block->binding_point(),
            m_mesh_render_uniform_buffer_range);

         uniform_offsets &o = m_programs->uniform_offsets;
         unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit(*r);
         ::memcpy(&start[o.clip_from_model],    value_ptr(transform),   16 * sizeof(float));
         ::memcpy(&start[o.show_rt_transform],  value_ptr(identity),    16 * sizeof(float));
         m_mesh_render_uniform_buffer_range->end_edit(*r);
      }
      else
      {
         gl::uniform_matrix_4fv(
            p->uniform_at(m_programs->uniform_keys.clip_from_model),
            1, GL_FALSE, value_ptr(transform));
         gl::uniform_matrix_4fv(
            p->uniform_at(m_programs->uniform_keys.show_rt_transform),
            1, GL_FALSE, value_ptr(identity));
      }

      m_quad_renderer->render_zero_to_one();

      //int iw = m_application->width();
      //int ih = m_application->height();

      //gl::bind_framebuffer(gl::framebuffer_target::read_framebuffer, m_fbo);
      //gl::bind_framebuffer(gl::framebuffer_target::draw_framebuffer, 0);

      /*
      gl::blit_framebuffer(
         0, 0, iw, ih,
         0, 0, iw, ih,
         GL_COLOR_BUFFER_BIT,
         GL_NEAREST);*/
   }
#endif

#if 0 // just blit one rt to screen
   int iw = m_application->width();
   int ih = m_application->height();

   gl::bind_framebuffer(gl::framebuffer_target::read_framebuffer, m_fbo);
   gl::read_buffer(GL_COLOR_ATTACHMENT1);

   gl::bind_framebuffer(gl::framebuffer_target::draw_framebuffer, 0);

   glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   gl::blit_framebuffer(
      0, 0, iw, ih,
      0, 0, iw / 6, ih / 6,
      GL_COLOR_BUFFER_BIT,
      GL_NEAREST);
#endif

}
void game::render()
{
   slog_trace("game::render()");

   int iw = m_application->width();
   int ih = m_application->height();

   gl::disable(gl::enable_cap::scissor_test); // TODO render state for this

   gl::viewport(0, 0, iw, ih);

   render_meshes();

   {
      gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
      gl::bind_framebuffer(GL_READ_FRAMEBUFFER, 0);

#if defined(RENDERSTACK_GL_API_OPENGL)
      gl::draw_buffer(GL_BACK);
#endif

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
      GLenum draw_buffer = GL_BACK;
      gl::draw_buffers(1, &draw_buffer);
#endif
   }

   if (m_text_buffer)
      render_ui();

   if ((m_controls.mouse_locked == false) && m_root_layer)
   {
      ui_context c;
      double x;
      double y;
      m_application->get_mouse_pos(x, y);
      c.mouse.x = static_cast<float>(x);
      c.mouse.y = static_cast<float>(ih - 1 - y);
      c.mouse_buttons[0] = (m_application->get_mouse_button(0) != 0);
      c.mouse_buttons[1] = (m_application->get_mouse_button(1) != 0);
      c.mouse_buttons[2] = (m_application->get_mouse_button(2) != 0);
      auto r = m_gui_renderer;
      r->prepare();
      r->on_resize(iw, ih);
      
      m_root_layer->draw(c);
   }

   m_application->swap_buffers();
}

