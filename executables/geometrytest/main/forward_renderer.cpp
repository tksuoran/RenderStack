#include "renderstack_toolkit/platform.hpp"
#include "main/forward_renderer.hpp"
#include "main/game.hpp"
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
using namespace renderstack::toolkit;
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;


forward_renderer::forward_renderer(
   shared_ptr<renderstack::graphics::renderer>  renderer,
   shared_ptr<programs>                         programs,
   shared_ptr<renderstack::graphics::buffer>    uniform_buffer
)
:  m_renderer(renderer)
,  m_programs(programs)
{
   if (m_programs->use_uniform_buffers())
   {
      m_mesh_render_uniform_buffer_range = std::make_shared<uniform_buffer_range>(
         m_programs->block,
         uniform_buffer
      );
   }

   m_mesh_render_states.depth.set_enabled(true);
   m_mesh_render_states.face_cull.set_enabled(true);
}


void forward_renderer::render_pass(
   shared_ptr<group> group,
   mat4 const &clip_from_world,
   mat4 const &view_from_world
)
{
   auto const &models = group->models();

   if (models.size() == 0)
      return;

   auto &r = *m_renderer;
   auto &t = r.track();
   auto p = m_programs->basic;

   t.execute(&m_mesh_render_states);

   r.set_program(p);
   vec4 material_parameters(4.0f, 0.0f, 0.0f, 1.0f);

   uniform_offsets &o = m_programs->uniform_offsets;

   {
      vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
      if (m_programs->use_uniform_buffers())
      {
         assert(m_mesh_render_uniform_buffer_range);
         r.set_uniform_buffer_range(m_programs->block->binding_point(), m_mesh_render_uniform_buffer_range);

         unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit(r);
         ::memcpy(&start[o.color], value_ptr(white), 4 * sizeof(float));
         m_mesh_render_uniform_buffer_range->end_edit(r);
      }
      else
      {
         gl::uniform_4fv(p->uniform_at(m_programs->uniform_keys.color), 1, value_ptr(white));
      }
   }

   for (auto i = models.cbegin(); i != models.cend(); ++i)
   {
      auto  model             = *i;
      mat4  world_from_model  = model->frame()->world_from_local().matrix();
      mat4  clip_from_model   = clip_from_world * world_from_model;
      mat4  view_from_model   = view_from_world * world_from_model;
      auto  geometry_mesh     = model->geometry_mesh();
      auto  vertex_stream     = geometry_mesh->vertex_stream();
      auto  mesh              = geometry_mesh->get_mesh();

      if (m_programs->use_uniform_buffers())
      {
         assert(m_mesh_render_uniform_buffer_range);
         r.set_uniform_buffer_range(m_programs->block->binding_point(), m_mesh_render_uniform_buffer_range);

         unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit(r);
         ::memcpy(&start[o.clip_from_model      ], value_ptr(clip_from_model),      16 * sizeof(float));
         ::memcpy(&start[o.view_from_model      ], value_ptr(view_from_model),      16 * sizeof(float));
         ::memcpy(&start[o.material_parameters  ], value_ptr(material_parameters),  4 * sizeof(float));
         m_mesh_render_uniform_buffer_range->end_edit(r);
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

         r.draw_elements_base_vertex(
            model->geometry_mesh()->vertex_stream(),
            begin_mode, count, index_type, index_pointer, base_vertex);
      }
   }

}
