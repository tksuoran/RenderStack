#include "renderstack_toolkit/platform.hpp"
#include "renderers/debug_renderer.hpp"
#include "renderers/forward_renderer.hpp"
#include "renderstack_geometry/shapes/cone.hpp"
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
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/light.hpp"
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

using namespace renderstack::toolkit;
using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace renderstack::scene;
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;


forward_renderer::forward_renderer()
:  service("forward_renderer")
,  m_renderer(nullptr)
,  m_programs(nullptr)
,  m_max_lights(1)
{
}

/*virtual*/ forward_renderer::~forward_renderer()
{
}

void forward_renderer::connect(
   shared_ptr<renderer>       renderer_,
   shared_ptr<debug_renderer> debug_renderer_,
   shared_ptr<programs>       programs_
)
{
   m_renderer = renderer_;
   m_debug_renderer = debug_renderer_;
   m_programs = programs_;

   initialization_depends_on(renderer_);
   initialization_depends_on(programs_);
}

void forward_renderer::initialize_service()
{
   assert(m_renderer);
   assert(m_programs);

   m_first_pass_render_states.depth.set_enabled(true);
   m_other_pass_render_states.depth.set_function(gl::depth_function::less);
   m_first_pass_render_states.face_cull.set_enabled(true);
   m_first_pass_render_states.blend.set_enabled(false);

   m_other_pass_render_states.depth.set_enabled(true);
   m_other_pass_render_states.depth.set_function(gl::depth_function::l_equal);
   m_other_pass_render_states.face_cull.set_enabled(true);
   m_other_pass_render_states.blend.set_enabled(true);
   m_other_pass_render_states.blend.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
   m_other_pass_render_states.blend.rgb().set_source_factor(gl::blending_factor_src::one);
   m_other_pass_render_states.blend.rgb().set_destination_factor(gl::blending_factor_dest::one);   
   m_other_pass_render_states.blend.alpha().set_equation_mode(gl::blend_equation_mode::func_add);
   m_other_pass_render_states.blend.alpha().set_source_factor(gl::blending_factor_src::one);
   m_other_pass_render_states.blend.alpha().set_destination_factor(gl::blending_factor_dest::one);   
}

void forward_renderer::print_matrix(mat4 const &m, std::string const &desc)
{
   m_debug_renderer->printf(
      "Matrix %s:\n"
      "\t% 6.4f  % 6.4f  % 6.4f  % 6.4f\n"
      "\t% 6.4f  % 6.4f  % 6.4f  % 6.4f\n"
      "\t% 6.4f  % 6.4f  % 6.4f  % 6.4f\n"
      "\t% 6.4f  % 6.4f  % 6.4f  % 6.4f\n",
      desc.c_str(),
      m[0][0], m[1][0], m[2][0], m[3][0],
      m[0][1], m[1][1], m[2][1], m[3][1],
      m[0][2], m[1][2], m[2][2], m[3][2],
      m[0][3], m[1][3], m[2][3], m[3][3]
   );
}

void forward_renderer::set_max_lights(int max_lights)
{
   m_max_lights = max_lights;
}

void forward_renderer::render_pass(
   shared_ptr<vector<shared_ptr<model> > > models,
   shared_ptr<vector<shared_ptr<light> > > lights,
   shared_ptr<camera> camera
)
{
   assert(models);
   assert(lights);
   assert(camera);

   if (models->size() == 0)
      return;

   auto &r = *m_renderer;
   auto &t = r.track();
   //auto p = m_programs->basic;
   auto p = m_programs->anisotropic;

   r.set_program(p);

   // Material
   vec4  color(1.0f, 1.0f, 1.0f, 1.0f);
   float roughness = 0.10f;
   float isotropy = 0.02f;

   // Camera
   mat4 const &world_from_view = camera->frame()->world_from_local().matrix();
   float exposure = 0.1f;

   if (m_programs->use_uniform_buffers())
   {
      unsigned char *start = m_programs->begin_edit_uniforms();
      ::memcpy(&start[m_programs->material_ubr->first_byte() + m_programs->material_block_access.color      ], value_ptr(color), 4 * sizeof(float));
      ::memcpy(&start[m_programs->material_ubr->first_byte() + m_programs->material_block_access.roughness  ], &roughness,       sizeof(float));
      ::memcpy(&start[m_programs->material_ubr->first_byte() + m_programs->material_block_access.isotropy   ], &isotropy,        sizeof(float));
      m_programs->material_ubr->flush(r);

      ::memcpy(&start[m_programs->lights_ubr->first_byte() + m_programs->lights_block_access.exposure], &exposure, sizeof(float));
      m_programs->lights_ubr->flush(r);

      ::memcpy(&start[m_programs->camera_ubr->first_byte() + m_programs->camera_block_access.world_from_view], value_ptr(world_from_view), 16 * sizeof(float));
      m_programs->camera_ubr->flush(r);

      m_programs->end_edit_uniforms();
   }
   else
   {
      gl::uniform_4fv(p->uniform_at(m_programs->material_block_access.color), 1, value_ptr(color));
      gl::uniform_1f(p->uniform_at(m_programs->material_block_access.roughness), roughness);
      gl::uniform_1f(p->uniform_at(m_programs->material_block_access.roughness), isotropy);

      gl::uniform_1f(p->uniform_at(m_programs->lights_block_access.exposure),    exposure);

      gl::uniform_matrix_4fv(p->uniform_at(m_programs->camera_block_access.world_from_view), 1, GL_FALSE, value_ptr(world_from_view));
   }

   mat4 const &clip_from_world = camera->clip_from_world().matrix();
   mat4 const &view_from_world = camera->frame()->world_from_local().inverse_matrix();

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   bool first_pass = true;
   int light_index = 0;
   for (auto i = lights->cbegin(); i != lights->cend(); ++i)
   {
      if (first_pass)
         t.execute(&m_first_pass_render_states);

      // Light
      assert(lights);
      auto l = *i;

      ++light_index;
      if (light_index > m_max_lights)
         break;

      glm::vec3 position   = vec3(l->frame()->world_from_local().matrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f));
      glm::vec3 direction  = vec3(l->frame()->world_from_local().matrix() * vec4(0.0f, 0.0f, 1.0f, 0.0f));
      glm::vec3 radiance   = l->intensity() * l->color();

      direction = normalize(direction);

      if (m_programs->use_uniform_buffers())
      {
         unsigned char *start = m_programs->begin_edit_uniforms();
         ::memcpy(&start[m_programs->lights_ubr->first_byte() + m_programs->lights_block_access.position  ], value_ptr(position),  3 * sizeof(float));
         ::memcpy(&start[m_programs->lights_ubr->first_byte() + m_programs->lights_block_access.direction ], value_ptr(direction), 3 * sizeof(float));
         ::memcpy(&start[m_programs->lights_ubr->first_byte() + m_programs->lights_block_access.radiance  ], value_ptr(radiance),  3 * sizeof(float));
         m_programs->lights_ubr->flush(r);
         m_programs->end_edit_uniforms();
      }
      else
      {
         gl::uniform_3fv(p->uniform_at(m_programs->lights_block_access.position),   1, value_ptr(position));
         gl::uniform_3fv(p->uniform_at(m_programs->lights_block_access.direction),  1, value_ptr(direction));
         gl::uniform_3fv(p->uniform_at(m_programs->lights_block_access.radiance),   1, value_ptr(radiance));
      }

      for (auto j = models->cbegin(); j != models->cend(); ++j)
      {
         auto model              = *j;
         mat4 world_from_model   = model->frame()->world_from_local().matrix();
         mat4 clip_from_model    = clip_from_world * world_from_model;
         mat4 view_from_model    = view_from_world * world_from_model;
         auto geometry_mesh      = model->geometry_mesh();
         auto vertex_stream      = geometry_mesh->vertex_stream();
         auto mesh               = geometry_mesh->get_mesh();

         model->frame()->update_hierarchical_no_cache(); // TODO

         if (m_programs->use_uniform_buffers())
         {
            assert(m_programs);
            assert(m_programs->model_ubr);

            unsigned char *start       = m_programs->begin_edit_uniforms();
            unsigned char *model_start = &start[m_programs->model_ubr->first_byte()];
            ::memcpy(&model_start[m_programs->model_block_access.clip_from_model],  value_ptr(clip_from_model), 16 * sizeof(float));
            ::memcpy(&model_start[m_programs->model_block_access.view_from_model],  value_ptr(view_from_model), 16 * sizeof(float));
            ::memcpy(&model_start[m_programs->model_block_access.world_from_model], value_ptr(world_from_model), 16 * sizeof(float));
            m_programs->model_ubr->flush(r);
            m_programs->end_edit_uniforms();
         }
         else
         {
            gl::uniform_matrix_4fv(p->uniform_at(m_programs->model_block_access.clip_from_model), 1, GL_FALSE, value_ptr(clip_from_model));
            gl::uniform_matrix_4fv(p->uniform_at(m_programs->model_block_access.view_from_model), 1, GL_FALSE, value_ptr(view_from_model));
            gl::uniform_matrix_4fv(p->uniform_at(m_programs->model_block_access.world_from_model), 1, GL_FALSE, value_ptr(world_from_model));
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

      if (first_pass)
      {
         t.execute(&m_other_pass_render_states);
         first_pass = false;
      }
   }
}
