#include "renderstack_toolkit/platform.hpp"
#include "main/id_renderer.hpp"
#include "renderer/model.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>

using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;


id_renderer::id_renderer(
   std::shared_ptr<renderstack::graphics::renderer>   renderer,
   std::shared_ptr<programs>                          programs,
   std::shared_ptr<renderstack::graphics::buffer>     uniform_buffer
)
:  m_renderer(renderer)
,  m_programs(programs)
{
   if (m_programs->use_uniform_buffers())
   {
      m_id_render_uniform_buffer_range = std::make_shared<uniform_buffer_range>(
         m_programs->block,
         uniform_buffer
      );
   }

   m_id_render_states.depth.set_enabled(true);
   m_id_render_states.face_cull.set_enabled(true);
}


void id_renderer::clear()
{
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void id_renderer::render_pass(
   std::vector<std::shared_ptr<class model>> const &models,
   mat4 const &clip_from_world
)
{
   if (models.size() == 0)
      return;

   auto &r = *m_renderer;
   auto &t = r.track();
   auto p = m_programs->id;

   t.execute(&m_id_render_states);

   r.set_program(p);
   
   uniform_offsets &o = m_programs->uniform_offsets;

   {
      vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
      if (m_programs->use_uniform_buffers())
      {
         assert(m_id_render_uniform_buffer_range);
         r.set_uniform_buffer_range(m_programs->block->binding_point(), m_id_render_uniform_buffer_range);

         unsigned char *start = m_id_render_uniform_buffer_range->begin_edit(r);
         ::memcpy(&start[o.color], value_ptr(white), 4 * sizeof(float));
         m_id_render_uniform_buffer_range->end_edit(r);
      }
      else
      {
         gl::uniform_4fv(p->uniform_at(m_programs->uniform_keys.color), 1, value_ptr(white));
      }
   }

   uint32_t id_offset = 0;
   for (auto i = models.cbegin(); i != models.cend(); ++i)
   {
      auto model              = *i;
      mat4 world_from_model   = model->frame()->world_from_local().matrix();
      mat4 clip_from_model    = clip_from_world * world_from_model;
      auto geometry_mesh      = model->geometry_mesh();
      auto vertex_stream      = geometry_mesh->vertex_stream();
      auto mesh               = geometry_mesh->get_mesh();

      gl::begin_mode::value         begin_mode     = gl::begin_mode::triangles;
      index_range const             &index_range   = geometry_mesh->fill_indices();
      GLsizei                       count          = static_cast<GLsizei>(index_range.index_count);
      gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_int;
      GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>((index_range.first_index + mesh->first_index()) * mesh->index_buffer()->stride());
      GLint                         base_vertex    = configuration::can_use.draw_elements_base_vertex
         ? static_cast<GLint>(mesh->first_vertex())
         : 0;

      uint32_t power_of_two   = next_power_of_two(count);
      uint32_t mask           = power_of_two - 1;
      uint32_t current_bits   = id_offset & mask;
      if (current_bits != 0)
      {
         uint add = power_of_two - current_bits;
         id_offset += add;
      }

      vec3 id_offset_vec3 = vec3_from_uint(id_offset);

      if (m_programs->use_uniform_buffers())
      {
         assert(m_id_render_uniform_buffer_range);
         r.set_uniform_buffer_range(m_programs->block->binding_point(), m_id_render_uniform_buffer_range);

         unsigned char *start = m_id_render_uniform_buffer_range->begin_edit(r);
         ::memcpy(&start[o.clip_from_model], value_ptr(clip_from_model),  16 * sizeof(float));
         ::memcpy(&start[o.id_offset      ], value_ptr(id_offset_vec3),    3 * sizeof(float));
         m_id_render_uniform_buffer_range->end_edit(r);
      }
      else
      {
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.clip_from_model), 1, GL_FALSE, value_ptr(clip_from_model));
         gl::uniform_3fv(p->uniform_at(m_programs->uniform_keys.id_offset), 1, value_ptr(id_offset_vec3));
      }

      r.draw_elements_base_vertex(
         model->geometry_mesh()->vertex_stream(),
         begin_mode, count, index_type, index_pointer, base_vertex);
   }

}

