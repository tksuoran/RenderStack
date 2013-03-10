#include "renderstack_toolkit/platform.hpp"
#include "main/game.hpp"
#include "main/application.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer.hpp"
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

   // Test all conditions; can_use.uniform_buffer_object can be forced to false
   bool use_uniform_buffers = 
      renderstack::graphics::configuration::can_use.uniform_buffer_object &&
      (m_programs->glsl_version() >= 140) &&
      (renderstack::graphics::configuration::shader_model_version >= 4);

   float w = (float)m_application->width();
   float h = (float)m_application->height();

   gl::viewport(0, 0, (GLsizei)w, (GLsizei)h);
   mat4 ortho = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
   gl::scissor(0, 0, (GLsizei)w, (GLsizei)h);

   auto p = m_programs->font;
   p->use();

   glm::vec4 white(1.0f, 1.0f, 1.0f, 0.66f); // gamma in 4th component
   if (use_uniform_buffers)
   {
      if (m_text_uniform_buffer_range)
      {
         uniform_offsets &o = m_programs->uniform_offsets;
         m_text_uniform_buffer_range->bind(m_programs->block->binding_point());
         unsigned char *start = m_text_uniform_buffer_range->begin_edit();
         ::memcpy(&start[o.model_to_clip],   value_ptr(ortho), 16 * sizeof(float));
         ::memcpy(&start[o.color],           value_ptr(white), 4 * sizeof(float));
         m_text_uniform_buffer_range->end_edit();
      }
   }
   else
   {
      int model_to_clip_ui = p->uniform_at(m_programs->uniform_keys.model_to_clip);
      int color_ui         = p->uniform_at(m_programs->uniform_keys.color);

      gl::uniform_matrix_4fv(model_to_clip_ui, 1, GL_FALSE, value_ptr(ortho));
      gl::uniform_4fv(color_ui, 1, value_ptr(white));
   }

   m_text_buffer->begin_print();
#if 1
   for (size_t i = 0; i < m_debug_lines.size(); ++i)
      m_text_buffer->print(m_debug_lines[i], 0.0f, h - (i + 1) * m_font->line_height());

   if (m_controls.mouse_locked)
      m_text_buffer->print("Mouse locked", 0.0f, 0.0f);
#endif

   int chars_printed = m_text_buffer->end_print();

   if (chars_printed > 0)
   {
      p->use();

      gl::depth_mask    (0);
      gl::disable       (gl::enable_cap::cull_face);
      gl::disable       (gl::enable_cap::depth_test);
      gl::disable       (gl::enable_cap::stencil_test);
      gl::disable       (gl::enable_cap::scissor_test);
      gl::enable        (gl::enable_cap::blend);
      gl::blend_equation(gl::blend_equation_mode::func_add);
      gl::blend_func    (gl::blending_factor_src::one, gl::blending_factor_dest::one_minus_src_alpha);

      int texture_ui = p->uniform("font_texture")->index();
      gl::uniform_1i(texture_ui, 0);

      m_text_buffer->render();

      gl::depth_mask(1);
      gl::enable  (gl::enable_cap::cull_face);
      gl::enable  (gl::enable_cap::depth_test);
      gl::disable (gl::enable_cap::blend);
   }

}

void game::render_meshes()
{
   slog_trace("game::render_meshes()");

   if (m_models.size() == 0)
      return;

   // Test all conditions; can_use.uniform_buffer_object can be forced to false
   bool use_uniform_buffers = 
      renderstack::graphics::configuration::can_use.uniform_buffer_object &&
      (m_programs->glsl_version() >= 140) &&
      (renderstack::graphics::configuration::shader_model_version >= 4);

   //gl::enable(gl::enable_cap::cull_face);
   gl::disable(gl::enable_cap::cull_face);

   auto p = m_programs->basic;
   p->use();

   for (auto i = m_models.cbegin(); i != m_models.cend(); ++i)
   {
      auto model           = *i;
      mat4 model_transform = model->frame()->world_from_local().matrix();
      mat4 mvp             = m_controls.view_projection * model_transform;
      auto geometry_mesh   = model->geometry_mesh();
      auto vertex_stream   = geometry_mesh->vertex_stream();
      auto mesh            = geometry_mesh->get_mesh();

      glm::vec4 fill_color(1.0f, 1.0f, 1.0f, 1.0f);
      if (use_uniform_buffers)
      {
         if (m_mesh_render_uniform_buffer_range)
         {
            m_mesh_render_uniform_buffer_range->bind(m_programs->block->binding_point());
            uniform_offsets &o = m_programs->uniform_offsets;
            unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit();
            ::memcpy(&start[o.model_to_clip],   value_ptr(mvp), 16 * sizeof(float));
            ::memcpy(&start[o.color],           value_ptr(fill_color), 4 * sizeof(float));
            m_mesh_render_uniform_buffer_range->end_edit();
         }
      }
      else
      {
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.model_to_clip), 1, GL_FALSE, value_ptr(mvp));
         gl::uniform_4fv(p->uniform_at(m_programs->uniform_keys.color), 1, value_ptr(fill_color));
      }

      //gl::polygon_offset(1.0f, 1.0f);
      //gl::enable(gl::enable_cap::polygon_offset_fill);

      gl::begin_mode::value         begin_mode     = gl::begin_mode::triangles;
      index_range const             &index_range   = geometry_mesh->fill_indices();
      GLsizei                       count          = static_cast<GLsizei>(index_range.index_count);
      gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_int;
      GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>(index_range.first_index + mesh->first_index() * sizeof(unsigned int));
      GLint                         base_vertex    = static_cast<GLint>(mesh->first_vertex());

      if (vertex_stream->use())
      {
         if (!configuration::can_use.draw_elements_base_vertex)
            throw std::runtime_error("not yet implemented");
         gl::draw_elements_base_vertex(begin_mode, count, index_type, index_pointer, base_vertex);
      }
      else
      {
         mesh->vertex_buffer()->bind();
         mesh->index_buffer()->bind();
         vertex_stream->setup_attribute_pointers(base_vertex);
         gl::draw_elements(begin_mode, count, index_type, index_pointer);
      }

      //gl::disable(gl::enable_cap::polygon_offset_fill);

#if 0
      glm::vec4 line_color(0.5f, 0.5f, 0.5f, 1.0f);
      if (use_uniform_buffers)
      {
         if (m_mesh_render_uniform_buffer_range)
         {
            unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit();
            uniform_offsets &o = m_programs->uniform_offsets;
            ::memcpy(&start[o.color], value_ptr(line_color), 4 * sizeof(float));
            m_mesh_render_uniform_buffer_range->end_edit();
         }
      }
      else
      {
         gl::uniform_4fv(p->uniform_at(m_programs->uniform_keys.color), 1, value_ptr(line_color));
      }

      mesh->vertex_stream().use();
      mesh->get_mesh()->vertex_buffer()->bind();
      mesh->get_mesh()->index_buffer()->bind();
      mesh->vertex_stream().setup_attribute_pointers();
      mesh->render(gl::begin_mode::lines, mesh->edge_line_indices(), normal_style::corner_normals);
#endif
   }
}
void game::render()
{
   slog_trace("game::render()");

   int iw = m_application->width();
   int ih = m_application->height();
   //float w = (float)iw;
   //float h = (float)ih;

   gl::depth_mask(1);
   gl::enable(gl::enable_cap::cull_face);
   gl::enable(gl::enable_cap::depth_test);
   gl::disable(gl::enable_cap::scissor_test);

   gl::disable(gl::enable_cap::blend);
   //gl::blend_equation(gl::blend_equation_mode::func_add);
   gl::blend_func(gl::blending_factor_src::one, gl::blending_factor_dest::one_minus_src_alpha);

   gl::viewport(0, 0, iw, ih);
   gl::clear_color(0.0f, 0.2f, 0.2f, 0.0f);
   gl::clear(clear_buffer_mask::color_buffer_bit | clear_buffer_mask::depth_buffer_bit);

   render_meshes();

#if 0
   if (m_text_buffer)
      render_ui();
#endif

#if 1
   if ((m_controls.mouse_locked == false) && m_root_layer)
   {
      ui_context c;
      m_application->get_mouse_pos(c.mouse.x, c.mouse.y);
      c.mouse.y = ih - 1 - c.mouse.y;
      c.mouse_buttons[0] = (m_application->get_mouse_button(0) != 0);
      c.mouse_buttons[1] = (m_application->get_mouse_button(1) != 0);
      c.mouse_buttons[2] = (m_application->get_mouse_button(2) != 0);
      auto uc = renderstack::ui::context::current();
      auto r = uc->gui_renderer();
      r->prepare();
      r->on_resize(iw, ih);
      
      m_root_layer->draw(c);
   }
#endif

   m_application->swap_buffers();
}

