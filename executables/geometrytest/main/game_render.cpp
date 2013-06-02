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
            ::memcpy(&start[o.model_to_clip],   value_ptr(ortho), 16 * sizeof(float));
            ::memcpy(&start[o.color],           value_ptr(white), 4 * sizeof(float));
            m_text_uniform_buffer_range->end_edit(r->renderer());
         }
      }
      else
      {
         int model_to_clip_ui = p->uniform_at(m_programs->uniform_keys.model_to_clip);
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

void game::render_meshes()
{
   slog_trace("game::render_meshes()");

   if (m_models.size() == 0)
      return;

   auto r = m_renderer;

   r->track.execute(&m_mesh_render_states);

   auto p = m_programs->basic;

   // r->trash(); ?
   r->set_program(p);

   for (auto i = m_models.cbegin(); i != m_models.cend(); ++i)
   {
      auto model           = *i;
      mat4 model_transform = model->frame()->world_from_local().matrix();
      mat4 mvp             = m_controls.view_projection * model_transform;
      auto geometry_mesh   = model->geometry_mesh();
      auto vertex_stream   = geometry_mesh->vertex_stream();
      auto mesh            = geometry_mesh->get_mesh();

      glm::vec4 fill_color(1.0f, 1.0f, 1.0f, 1.0f);
      if (use_uniform_buffers())
      {
         if (m_mesh_render_uniform_buffer_range)
         {
            r->set_uniform_buffer_range(
               m_programs->block->binding_point(),
               m_mesh_render_uniform_buffer_range);

            uniform_offsets &o = m_programs->uniform_offsets;
            unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit(*r);
            ::memcpy(&start[o.model_to_clip],   value_ptr(mvp), 16 * sizeof(float));
            ::memcpy(&start[o.color],           value_ptr(fill_color), 4 * sizeof(float));
            m_mesh_render_uniform_buffer_range->end_edit(*r);
         }
      }
      else
      {
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.model_to_clip), 1, GL_FALSE, value_ptr(mvp));
         gl::uniform_4fv(p->uniform_at(m_programs->uniform_keys.color), 1, value_ptr(fill_color));
      }

      gl::polygon_offset(1.0f, 1.0f);
      gl::enable(gl::enable_cap::polygon_offset_fill);
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
      gl::disable(gl::enable_cap::polygon_offset_fill);

#if 1
      glm::vec4 line_color(0.5f, 0.5f, 0.5f, 1.0f);
      if (use_uniform_buffers())
      {
         if (m_mesh_render_uniform_buffer_range)
         {
            unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit(*r);
            uniform_offsets &o = m_programs->uniform_offsets;
            ::memcpy(&start[o.color], value_ptr(line_color), 4 * sizeof(float));
            m_mesh_render_uniform_buffer_range->end_edit(*r);
         }
      }
      else
      {
         gl::uniform_4fv(p->uniform_at(m_programs->uniform_keys.color), 1, value_ptr(line_color));
      }

      {
         gl::begin_mode::value         begin_mode     = gl::begin_mode::lines;
         index_range const             &index_range   = geometry_mesh->edge_line_indices();
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
#endif
   }
}
void game::render()
{
   slog_trace("game::render()");

   int iw = m_application->width();
   int ih = m_application->height();

   gl::disable(gl::enable_cap::scissor_test); // TODO render state for this

   gl::viewport(0, 0, iw, ih);
   gl::clear_color(0.0f, 0.2f, 0.2f, 0.0f);
   gl::clear(clear_buffer_mask::color_buffer_bit | clear_buffer_mask::depth_buffer_bit);

   render_meshes();

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

