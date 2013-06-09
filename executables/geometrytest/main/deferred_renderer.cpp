#include "renderstack_toolkit/platform.hpp"
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


deferred_renderer::deferred_renderer(
   std::shared_ptr<renderstack::graphics::renderer>   renderer,
   std::shared_ptr<programs>                          programs,
   std::shared_ptr<renderstack::graphics::buffer>     uniform_buffer
)
:  m_renderer     (renderer)
,  m_programs     (programs)
,  m_fbo          (0)
,  m_quad_renderer(nullptr)
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

   m_light_render_states.depth.set_enabled(false);
   m_light_render_states.face_cull.set_enabled(false);
   m_light_render_states.blend.set_enabled(true);
   m_light_render_states.blend.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
   m_light_render_states.blend.rgb().set_source_factor(gl::blending_factor_src::one);
   m_light_render_states.blend.rgb().set_destination_factor(gl::blending_factor_dest::one);

   // Nothing to change in, use default render states:
   // m_show_rt_render_states

   m_quad_renderer = make_shared<quad_renderer>(m_renderer);
}

void deferred_renderer::bind_fbo()
{
   gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

   GLenum draw_buffers[] =
   {
      GL_COLOR_ATTACHMENT0,
      GL_COLOR_ATTACHMENT1,
      GL_COLOR_ATTACHMENT2,
      GL_COLOR_ATTACHMENT3
   };
   gl::draw_buffers(4, draw_buffers);
}

void deferred_renderer::bind_default_framebuffer()
{
   gl::bind_framebuffer(gl::framebuffer_target::draw_framebuffer, 0);

#if defined(RENDERSTACK_GL_API_OPENGL)
   gl::draw_buffer(GL_BACK);
#endif

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   GLenum draw_buffer = GL_BACK;
   gl::draw_buffers(1, &draw_buffer);
#endif
}

void deferred_renderer::resize(int width, int height)
{
   if (m_fbo == 0)
      gl::gen_framebuffers(1, &m_fbo);

   gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
   GLenum formats[] = { GL_RGBA8, GL_RGBA32F, GL_RGBA32F, GL_RGBA32F };
   for (int i = 0; i < 4; ++i)
   {
      m_rt[i].reset();

      m_rt[i] = make_shared<renderstack::graphics::texture>(
         renderstack::graphics::texture_target::texture_2d,
         formats[i],
         false,
         width,
         height,
         0
      );
      m_rt[i]->allocate_storage(*m_renderer);
      m_rt[i]->set_mag_filter(gl::texture_mag_filter::nearest);
      m_rt[i]->set_min_filter(gl::texture_min_filter::nearest);

      gl::framebuffer_texture_2d(
         GL_DRAW_FRAMEBUFFER,
         GL_COLOR_ATTACHMENT0 + i,
         GL_TEXTURE_2D,
         m_rt[i]->gl_name(),
         0);
   }

   m_depth.reset();
   m_depth = make_shared<renderstack::graphics::texture>(
      renderstack::graphics::texture_target::texture_2d,
      GL_DEPTH_COMPONENT32F,
      false,
      width,
      height,
      0
   );
   m_depth->allocate_storage(*m_renderer);
   gl::framebuffer_texture_2d(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth->gl_name(), 0);
   gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void deferred_renderer::fbo_clear()
{
   GLfloat emission_clear        [4] = { 0.2f, 0.1f, 0.1f, 1.0f };
   GLfloat albedo_clear          [4] = { 0.1f, 0.2f, 0.1f, 0.0f };
   GLfloat normal_tangent_clear  [4] = { 0.0f, 0.0f, 0.0f, 0.0f };
   GLfloat material_clear        [4] = { 0.0f, 0.0f, 0.0f, 0.0f };
   GLfloat one = 1.0f;

   vec3 N = vec3(0.0f, 0.0f, -1.0f);
   vec3 T = vec3(0.0f, 1.0f, 0.0f); 
   vec2 sN = cartesian_to_spherical(N);
   vec2 sT = cartesian_to_spherical(T);

   normal_tangent_clear[0] = sN.x;
   normal_tangent_clear[1] = sN.y;
   normal_tangent_clear[2] = sT.x;
   normal_tangent_clear[3] = sT.y;

   gl::clear_buffer_fv(GL_COLOR, 0, &emission_clear      [0]);
   gl::clear_buffer_fv(GL_COLOR, 1, &albedo_clear        [0]);
   gl::clear_buffer_fv(GL_COLOR, 2, &normal_tangent_clear[0]);
   gl::clear_buffer_fv(GL_COLOR, 3, &material_clear      [0]);
   gl::clear_buffer_fv(GL_DEPTH, 0, &one);
}

void deferred_renderer::geometry_pass(
   std::vector<std::shared_ptr<class model>> const &models,
   mat4 const &clip_from_world,
   mat4 const &view_from_world
)
{
   bind_fbo();

   fbo_clear();

   if (models.size() == 0)
      return;

   auto &r = *m_renderer;
   auto &t = r.track();
   auto p = m_programs->gbuffer;
   t.reset();
   t.execute(&m_mesh_render_states);
   r.set_program(p);
   vec4 material_parameters(4.0f, 0.0f, 0.0f, 1.0f);

   uniform_offsets &o = m_programs->uniform_offsets;

   for (auto i = models.cbegin(); i != models.cend(); ++i)
   {
      auto model              = *i;
      mat4 world_from_model   = model->frame()->world_from_local().matrix();
      mat4 clip_from_model    = clip_from_world * world_from_model;
      mat4 view_from_model    = view_from_world * world_from_model;
      auto geometry_mesh      = model->geometry_mesh();
      auto vertex_stream      = geometry_mesh->vertex_stream();
      auto mesh               = geometry_mesh->get_mesh();

      if (m_programs->use_uniform_buffers())
      {
         assert(m_mesh_render_uniform_buffer_range);
         r.set_uniform_buffer_range(
            m_programs->block->binding_point(),
            m_mesh_render_uniform_buffer_range);

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

void deferred_renderer::light_pass(mat4 const &world_from_view)
{
   // Temp draw directly to screen
   bind_default_framebuffer();
   //bind_fbo();

   glClearColor(0.0f, 0.0f, 0.15f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   auto &r = *m_renderer;
   auto &t = r.track();
   auto p = m_programs->light;

   // Don't bind emission texture for now
   //t.reset();
   t.execute(&m_light_render_states);
   r.reset_texture(0, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.set_texture(1, m_rt[1]);
   r.set_texture(2, m_rt[2]);
   r.set_texture(3, m_rt[3]);
   r.set_program(p);

   mat4 identity = mat4(1.0f);

   if (m_programs->use_uniform_buffers())
   {
      assert(m_mesh_render_uniform_buffer_range);
      r.set_uniform_buffer_range(
         m_programs->block->binding_point(),
         m_mesh_render_uniform_buffer_range);

      uniform_offsets &o = m_programs->uniform_offsets;
      unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit(r);
      ::memcpy(&start[o.clip_from_model], value_ptr(identity),        16 * sizeof(float));
      ::memcpy(&start[o.world_from_view], value_ptr(world_from_view), 16 * sizeof(float));
      m_mesh_render_uniform_buffer_range->end_edit(r);
   }
   else
   {
      gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.clip_from_model), 1, GL_FALSE, value_ptr(identity));
      gl::uniform_matrix_4fv(p->uniform_at(m_programs->uniform_keys.world_from_view), 1, GL_FALSE, value_ptr(world_from_view));
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

void deferred_renderer::show_rt()
{
   bind_default_framebuffer();

   auto &r = *m_renderer;
   auto &t = r.track();
   t.reset();
   m_show_rt_render_states.face_cull.set_enabled(false);
   t.execute(&m_show_rt_render_states);
   for (int i = 0; i < 4; ++i)
   {
      std::shared_ptr<renderstack::graphics::program> p = (i == 2)
            ? m_programs->show_rt_spherical
            : m_programs->show_rt;
      r.set_program(p);

      r.set_texture(0, m_rt[i]);

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

      if (m_programs->use_uniform_buffers())
      {
         assert(m_mesh_render_uniform_buffer_range);
         r.set_uniform_buffer_range(
            m_programs->block->binding_point(),
            m_mesh_render_uniform_buffer_range);

         uniform_offsets &o = m_programs->uniform_offsets;
         unsigned char *start = m_mesh_render_uniform_buffer_range->begin_edit(r);
         ::memcpy(&start[o.clip_from_model],    value_ptr(transform),   16 * sizeof(float));
         ::memcpy(&start[o.show_rt_transform],  value_ptr(identity),    16 * sizeof(float));
         m_mesh_render_uniform_buffer_range->end_edit(r);
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
}

