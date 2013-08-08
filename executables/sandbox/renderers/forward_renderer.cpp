#include "renderstack_toolkit/platform.hpp"
#include "renderers/forward_renderer.hpp"
#include "scene/group.hpp"
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


forward_renderer::forward_renderer()
:  service("forward_renderer")
,  m_renderer(nullptr)
,  m_programs(nullptr)
{
}

/*virtual*/ forward_renderer::~forward_renderer()
{
}

void forward_renderer::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<programs>                         programs_
)
{
   m_renderer = renderer_;
   m_programs = programs_;

   initialization_depends_on(renderer_);
   initialization_depends_on(programs_);
}

void forward_renderer::initialize_service()
{
   assert(m_renderer);
   assert(m_programs);

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

   {
      vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
      if (m_programs->use_uniform_buffers())
      {
         assert(m_programs);
         assert(m_programs->material_ubr);

         r.set_uniform_buffer_range(m_programs->material_block->binding_point(), m_programs->material_ubr);

         unsigned char *start = m_programs->material_ubr->begin_edit(r);
         ::memcpy(&start[m_programs->material_block_access.color], value_ptr(white), 4 * sizeof(float));
         m_programs->material_ubr->end_edit(r);
      }
      else
      {
         gl::uniform_4fv(
            p->uniform_at(m_programs->material_block_access.color),
            1, value_ptr(white));
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

      model->frame()->update_hierarchical_no_cache(); // TODO

      if (m_programs->use_uniform_buffers())
      {
         assert(m_programs);
         assert(m_programs->model_ubr);

         unsigned char *start          = m_programs->begin_edit_uniforms();
         unsigned char *model_start    = &start[m_programs->model_ubr->first_byte()];
         unsigned char *material_start = &start[m_programs->material_ubr->first_byte()];
         ::memcpy(&model_start[m_programs->model_block_access.clip_from_model], value_ptr(clip_from_model), 16 * sizeof(float));
         ::memcpy(&model_start[m_programs->model_block_access.view_from_model], value_ptr(view_from_model), 16 * sizeof(float));
         ::memcpy(&material_start[m_programs->material_block_access.material_parameters], value_ptr(material_parameters), 4 * sizeof(float));
         m_programs->model_ubr->flush(r);
         m_programs->material_ubr->flush(r);
         m_programs->end_edit_uniforms();
      }
      else
      {
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->model_block_access.clip_from_model), 1, GL_FALSE, value_ptr(clip_from_model));
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->model_block_access.view_from_model), 1, GL_FALSE, value_ptr(view_from_model));
         gl::uniform_4fv(p->uniform_at(m_programs->material_block_access.material_parameters),  1, value_ptr(material_parameters));
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
