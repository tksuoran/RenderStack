#include "renderstack_toolkit/platform.hpp"
#include "renderers/deferred_renderer.hpp"
#include "renderers/light_mesh.hpp"
#include "renderstack_geometry/shapes/cone.hpp"
#include "renderstack_geometry/shapes/triangle.hpp" // quad is currently here...
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
#include "renderstack_scene/viewport.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
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


deferred_renderer::deferred_renderer()
:  service("deferred_renderer")
,  m_renderer     (nullptr)
,  m_programs     (nullptr)
,  m_quad_renderer(nullptr)
,  m_gbuffer_fbo  (0)
,  m_linear_fbo   (0)
,  m_stencil_rbo  (0)
,  m_use_stencil  (true)
,  m_scale        (1)
{
}

/*virtual*/ deferred_renderer::~deferred_renderer()
{
}

void deferred_renderer::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<programs>                         programs_,
   shared_ptr<quad_renderer>                    quad_renderer_,
   std::shared_ptr<light_mesh>                  light_mesh_
)
{
   m_renderer = renderer_;
   m_programs = programs_;
   m_quad_renderer = quad_renderer_;
   m_light_mesh = light_mesh_;

   initialization_depends_on(m_renderer);
   initialization_depends_on(m_programs);
}

void deferred_renderer::initialize_service()
{
   assert(m_renderer);
   assert(m_programs);

   m_mesh_render_states.depth.set_enabled(true);
   m_mesh_render_states.face_cull.set_enabled(true);

   // Nothing to change in, use default render states:
   // m_show_rt_render_states

   m_light_stencil_render_states.depth.set_enabled   (true);
   m_light_stencil_render_states.depth.set_depth_mask(false);

   m_light_stencil_render_states.color_mask.set_red  (false);
   m_light_stencil_render_states.color_mask.set_green(false);
   m_light_stencil_render_states.color_mask.set_blue (false);
   m_light_stencil_render_states.color_mask.set_alpha(false);

   m_light_stencil_render_states.face_cull.set_enabled(false);

   m_light_stencil_render_states.stencil.set_enabled(true);
   m_light_stencil_render_states.stencil.back ().set_z_fail_op       (gl::stencil_op_enum::keep);
   m_light_stencil_render_states.stencil.back ().set_z_pass_op       (gl::stencil_op_enum::incr_wrap);
   m_light_stencil_render_states.stencil.back ().set_stencil_fail_op (gl::stencil_op_enum::keep);
   m_light_stencil_render_states.stencil.back ().set_function        (gl::stencil_function_enum::always);

   m_light_stencil_render_states.stencil.front().set_z_fail_op       (gl::stencil_op_enum::keep);
   m_light_stencil_render_states.stencil.front().set_z_pass_op       (gl::stencil_op_enum::decr_wrap);
   m_light_stencil_render_states.stencil.front().set_stencil_fail_op (gl::stencil_op_enum::keep);
   m_light_stencil_render_states.stencil.front().set_function        (gl::stencil_function_enum::always);

   m_light_stencil_render_states.stencil.set_separate(true);

   m_light_with_stencil_test_render_states.depth.set_enabled                   (false);
   m_light_with_stencil_test_render_states.depth.set_depth_mask                (false);
   m_light_with_stencil_test_render_states.face_cull.set_enabled               (true);
   m_light_with_stencil_test_render_states.face_cull.set_cull_face_mode        (gl::cull_face_mode::front);
   m_light_with_stencil_test_render_states.stencil.set_enabled                 (true);
   m_light_with_stencil_test_render_states.stencil.back ().set_function        (gl::stencil_function_enum::not_equal);
   m_light_with_stencil_test_render_states.stencil.back ().set_reference       (0);
   m_light_with_stencil_test_render_states.stencil.front().set_function        (gl::stencil_function_enum::not_equal);
   m_light_with_stencil_test_render_states.stencil.front().set_reference       (0);
   m_light_with_stencil_test_render_states.blend.set_enabled                   (true);
   m_light_with_stencil_test_render_states.blend.rgb().set_equation_mode       (gl::blend_equation_mode::func_add);
   m_light_with_stencil_test_render_states.blend.rgb().set_source_factor       (gl::blending_factor_src::one);
   m_light_with_stencil_test_render_states.blend.rgb().set_destination_factor  (gl::blending_factor_dest::one);
   m_light_with_stencil_test_render_states.stencil.back ().set_z_fail_op(gl::stencil_op_enum::replace);
   m_light_with_stencil_test_render_states.stencil.back ().set_z_pass_op(gl::stencil_op_enum::replace);
   m_light_with_stencil_test_render_states.stencil.front().set_z_fail_op(gl::stencil_op_enum::replace);
   m_light_with_stencil_test_render_states.stencil.front().set_z_pass_op(gl::stencil_op_enum::replace);

   m_light_render_states.depth.set_enabled                  (false);
   m_light_render_states.depth.set_depth_mask               (false);
   m_light_render_states.face_cull.set_enabled              (true);
   m_light_render_states.face_cull.set_cull_face_mode       (gl::cull_face_mode::front);
   m_light_render_states.blend.set_enabled                  (true);
   m_light_render_states.blend.rgb().set_equation_mode      (gl::blend_equation_mode::func_add);
   m_light_render_states.blend.rgb().set_source_factor      (gl::blending_factor_src::one);
   m_light_render_states.blend.rgb().set_destination_factor (gl::blending_factor_dest::one);

   if (renderstack::graphics::configuration::can_use.uniform_buffer_object)
   {
      auto &r = *m_renderer;

      size_t ubo_size = 0;

      m_ubr_sizes.camera   = 20;
      m_ubr_sizes.model    = 400;
      m_ubr_sizes.material = 100;
      m_ubr_sizes.lights   = 400;
      m_ubr_sizes.debug    = 100;

      ubo_size += m_programs->model_block   ->size_bytes() * m_ubr_sizes.model;
      ubo_size += m_programs->camera_block  ->size_bytes() * m_ubr_sizes.camera;
      ubo_size += m_programs->material_block->size_bytes() * m_ubr_sizes.material;
      ubo_size += m_programs->lights_block  ->size_bytes() * m_ubr_sizes.lights;
      ubo_size += m_programs->debug_block   ->size_bytes() * m_ubr_sizes.debug;

      m_uniform_buffer = make_shared<buffer>(
         renderstack::graphics::buffer_target::uniform_buffer,
         ubo_size,
         1
      );
      m_uniform_buffer->allocate_storage(r);

      m_model_ubr      = make_shared<uniform_buffer_range>(m_programs->model_block,    m_uniform_buffer, m_ubr_sizes.model);
      m_camera_ubr     = make_shared<uniform_buffer_range>(m_programs->camera_block,   m_uniform_buffer, m_ubr_sizes.camera);
      m_material_ubr   = make_shared<uniform_buffer_range>(m_programs->material_block, m_uniform_buffer, m_ubr_sizes.material);
      m_lights_ubr     = make_shared<uniform_buffer_range>(m_programs->lights_block,   m_uniform_buffer, m_ubr_sizes.lights);
   }
}

void deferred_renderer::set_use_stencil(bool value)
{
   m_use_stencil = value;
}

void deferred_renderer::set_scale(int value)
{
   m_scale = value;
}

void deferred_renderer::resize(int width, int height)
{
   m_width = width / m_scale;
   m_height = height / m_scale;
   m_width_full = width;
   m_height_full = height;
   {
      if (m_gbuffer_fbo == 0)
         gl::gen_framebuffers(1, &m_gbuffer_fbo);

      gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, m_gbuffer_fbo);
      GLenum formats[] = {
         GL_RGBA16F,    // normal tangent
         GL_RGBA8,      // albedo
         GL_RGBA8       // material
      };

      for (int i = 0; i < 3; ++i)
      {
         m_gbuffer_rt[i].reset();

         m_gbuffer_rt[i] = make_shared<renderstack::graphics::texture>(
            renderstack::graphics::texture_target::texture_2d,
            formats[i],
            false,
            m_width,
            m_height,
            0
         );
         m_gbuffer_rt[i]->allocate_storage(*m_renderer);
         m_gbuffer_rt[i]->set_mag_filter(gl::texture_mag_filter::nearest);
         m_gbuffer_rt[i]->set_min_filter(gl::texture_min_filter::nearest);
         m_gbuffer_rt[i]->set_wrap(0, gl::texture_wrap_mode::clamp_to_edge);
         m_gbuffer_rt[i]->set_wrap(1, gl::texture_wrap_mode::clamp_to_edge);

         gl::framebuffer_texture_2d(
            GL_DRAW_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D,
            m_gbuffer_rt[i]->gl_name(),
            0
         );
         gl::framebuffer_texture_2d(
            GL_DRAW_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D,
            m_gbuffer_rt[i]->gl_name(),
            0
         );
      }

      GLenum depth_format = m_use_stencil ? GL_DEPTH32F_STENCIL8 : GL_DEPTH_COMPONENT32F;
      GLenum attachment_point = m_use_stencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;

      m_depth.reset();
      m_depth = make_shared<renderstack::graphics::texture>(
         renderstack::graphics::texture_target::texture_2d,
         depth_format,
         false,
         m_width,
         m_height,
         0
      );
      m_depth->set_mag_filter(gl::texture_mag_filter::nearest);
      m_depth->set_min_filter(gl::texture_min_filter::nearest);
      m_depth->set_wrap(0, gl::texture_wrap_mode::clamp_to_edge);
      m_depth->set_wrap(1, gl::texture_wrap_mode::clamp_to_edge);
      m_depth->allocate_storage(*m_renderer);

      gl::framebuffer_texture_2d(
         GL_DRAW_FRAMEBUFFER,
         attachment_point,
         GL_TEXTURE_2D,
         m_depth->gl_name(),
         0
      );

      GLenum a = gl::check_framebuffer_status(GL_FRAMEBUFFER);
      if (a != GL_FRAMEBUFFER_COMPLETE)
      {
         const char *status = gl::enum_string(a);
         throw runtime_error(status);
      }
   }

   {
      if (m_linear_fbo == 0)
         gl::gen_framebuffers(1, &m_linear_fbo);

      gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, m_linear_fbo);
      GLenum formats[] = { GL_RGBA16F };

      for (int i = 0; i < 1; ++i)
      {
         m_linear_rt[i].reset();

         m_linear_rt[i] = make_shared<renderstack::graphics::texture>(
            renderstack::graphics::texture_target::texture_2d,
            formats[i],
            false,
            m_width,
            m_height,
            0
         );
         m_linear_rt[i]->allocate_storage(*m_renderer);
         m_linear_rt[i]->set_mag_filter(gl::texture_mag_filter::nearest);
         m_linear_rt[i]->set_min_filter(gl::texture_min_filter::nearest);
         m_linear_rt[i]->set_wrap(0, gl::texture_wrap_mode::clamp_to_edge);
         m_linear_rt[i]->set_wrap(1, gl::texture_wrap_mode::clamp_to_edge);

         gl::framebuffer_texture_2d(
            GL_DRAW_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D,
            m_linear_rt[i]->gl_name(),
            0
         );
      }

      if (m_use_stencil)
      {
         if (m_stencil_rbo == 0)
            gl::gen_renderbuffers(1, &m_stencil_rbo);

         gl::bind_renderbuffer(GL_RENDERBUFFER, m_stencil_rbo);

         gl::renderbuffer_storage(
            GL_RENDERBUFFER,
            GL_DEPTH32F_STENCIL8,
            m_width,
            m_height
         );
         gl::framebuffer_renderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER,
            m_stencil_rbo
         );
      }

      GLenum a = gl::check_framebuffer_status(GL_FRAMEBUFFER);
      if (a != GL_FRAMEBUFFER_COMPLETE)
      {
         const char *status = gl::enum_string(a);
         throw runtime_error(status);
      }
   }

   gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void deferred_renderer::set_max_lights(int max_lights)
{
   m_max_lights = max_lights;
}

void deferred_renderer::bind_gbuffer_fbo()
{
   gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, m_gbuffer_fbo);

   GLenum draw_buffers[] =
   {
      GL_COLOR_ATTACHMENT0,
      GL_COLOR_ATTACHMENT1,
      GL_COLOR_ATTACHMENT2
   };
   gl::draw_buffers(3, draw_buffers);
}

void deferred_renderer::fbo_clear()
{
   GLfloat albedo_clear          [4] = { 0.5f, 0.5f, 0.5f, 0.0f };
   GLfloat normal_tangent_clear  [4];
   GLfloat material_clear        [4];
   GLfloat one = 1.0f;

   vec3 N = vec3(0.0f, 0.0f, -1.0f);
   vec3 T = vec3(0.0f, 1.0f,  0.0f); 

   cartesian_to_spherical(N, normal_tangent_clear[0], normal_tangent_clear[1]);
   cartesian_to_spherical(T, normal_tangent_clear[2], normal_tangent_clear[3]);

   auto &r = *m_renderer;
   r.reset_texture(0, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.reset_texture(1, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.reset_texture(2, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.reset_texture(3, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.reset_texture(4, renderstack::graphics::texture_target::texture_2d, nullptr);

   //gl::clear_buffer_fv(GL_COLOR, 0, &emission_clear      [0]);
   GLenum a = gl::check_framebuffer_status(GL_FRAMEBUFFER);
   if (a != GL_FRAMEBUFFER_COMPLETE)
      throw runtime_error("FBO is not complete");

   gl::clear_buffer_fv(GL_COLOR, 0, &normal_tangent_clear[0]);
   gl::clear_buffer_fv(GL_COLOR, 1, &albedo_clear        [0]);
   gl::clear_buffer_fv(GL_COLOR, 2, &material_clear      [0]);
   gl::clear_buffer_fv(GL_DEPTH, 0, &one);
}

void deferred_renderer::geometry_pass(
   shared_ptr<vector<shared_ptr<material> > > materials,
   shared_ptr<vector<shared_ptr<model> > > models,
   shared_ptr<renderstack::scene::camera> camera
)
{
   bind_gbuffer_fbo();

   gl::viewport(0, 0, m_width, m_height);

   fbo_clear();

   if (models->size() == 0)
      return;

   auto &r = *m_renderer;
   auto &t = r.track();
   auto p = m_programs->gbuffer;
   t.reset();
   t.execute(&m_mesh_render_states);
   r.set_program(p);

   r.set_buffer(renderstack::graphics::buffer_target::uniform_buffer, m_uniform_buffer);
   void *start0 = m_uniform_buffer->map(
      r, 
      0, 
      m_uniform_buffer->capacity(), 
      static_cast<gl::buffer_access_mask::value>(
         gl::buffer_access_mask::map_write_bit | 
         gl::buffer_access_mask::map_flush_explicit_bit |
         gl::buffer_access_mask::map_invalidate_buffer_bit
      )
   );

   ubr_ptr start;
   start.model    = static_cast<unsigned char*>(start0) + m_model_ubr   ->first_byte();
   start.camera   = static_cast<unsigned char*>(start0) + m_camera_ubr  ->first_byte();
   start.material = static_cast<unsigned char*>(start0) + m_material_ubr->first_byte();
   start.lights   = static_cast<unsigned char*>(start0) + m_lights_ubr  ->first_byte();
   start.debug    = nullptr;
   ubr_pos offsets;

   // Material
   int material_index = 0;
   for (auto i = materials->cbegin(); i != materials->cend(); ++i)
   {
      auto material = *i;
      vec4  color = material->color();
      float r = material->roughness();
      float p = material->isotropy();

      ::memcpy(start.material + offsets.material + m_programs->material_block_access.color    , value_ptr(color), 4 * sizeof(float));
      ::memcpy(start.material + offsets.material + m_programs->material_block_access.roughness, &r, sizeof(float));
      ::memcpy(start.material + offsets.material + m_programs->material_block_access.isotropy , &p, sizeof(float));
      offsets.material += m_programs->material_block->size_bytes();
      ++material_index;
   }
   assert(material_index < m_ubr_sizes.material);
   m_material_ubr->flush(r, offsets.material);

   // Camera
   mat4 const &world_from_view = camera->frame()->world_from_local().matrix();
   mat4 const &clip_from_world = camera->clip_from_world().matrix();
   mat4 const &view_from_world = camera->frame()->world_from_local().inverse_matrix();
   //float exposure = 0.1f;
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.world_from_view, value_ptr(world_from_view), 16 * sizeof(float));
   offsets.camera += m_programs->camera_block->size_bytes();
   m_camera_ubr->flush(r, offsets.camera);

   // Models
   int model_index = 0;
   for (auto j = models->cbegin(); j != models->cend(); ++j)
   {
      auto model = *j;

      model->frame()->update_hierarchical_no_cache(); // TODO

      mat4 world_from_model   = model->frame()->world_from_local().matrix();
      mat4 clip_from_model    = clip_from_world * world_from_model;
      mat4 view_from_model    = view_from_world * world_from_model;

      ::memcpy(start.model + offsets.model + m_programs->model_block_access.clip_from_model,  value_ptr(clip_from_model), 16 * sizeof(float));
      ::memcpy(start.model + offsets.model + m_programs->model_block_access.view_from_model,  value_ptr(view_from_model), 16 * sizeof(float));
      ::memcpy(start.model + offsets.model + m_programs->model_block_access.world_from_model, value_ptr(world_from_model), 16 * sizeof(float));
      offsets.model += m_programs->model_block->size_bytes();
      ++model_index;
   }
   assert(model_index < m_ubr_sizes.model);
   m_model_ubr->flush(r, offsets.model);

   m_uniform_buffer->unmap(r);

   m_uniform_buffer->bind_range(
      m_programs->camera_block->binding_point(),
      m_camera_ubr->first_byte(),
      m_programs->camera_block->size_bytes()
   );
   m_uniform_buffer->bind_range(
      m_programs->material_block->binding_point(),
      m_material_ubr->first_byte(),
      m_programs->material_block->size_bytes()
   );

   model_index = 0;
   for (auto i = models->cbegin(); i != models->cend(); ++i)
   {
      auto model           = *i;
      auto geometry_mesh   = model->geometry_mesh();
      auto vertex_stream   = geometry_mesh->vertex_stream();
      auto mesh            = geometry_mesh->get_mesh();
      auto material        = model->material();
      size_t material_index = material->index();

      gl::begin_mode::value         begin_mode     = gl::begin_mode::triangles;
      index_range const             &index_range   = geometry_mesh->fill_indices();
      GLsizei                       count          = static_cast<GLsizei>(index_range.index_count);
      gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_int;
      GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>((index_range.first_index + mesh->first_index()) * mesh->index_buffer()->stride());
      GLint                         base_vertex    = configuration::can_use.draw_elements_base_vertex
         ? static_cast<GLint>(mesh->first_vertex())
         : 0;

      m_uniform_buffer->bind_range(
         m_programs->model_block->binding_point(),
         m_model_ubr->first_byte() + (model_index * m_programs->model_block->size_bytes()),
         m_programs->model_block->size_bytes()
      );

      m_uniform_buffer->bind_range(
         m_programs->material_block->binding_point(),
         m_material_ubr->first_byte() + (material_index * m_programs->material_block->size_bytes()),
         m_programs->material_block->size_bytes()
      );

      r.draw_elements_base_vertex(
         model->geometry_mesh()->vertex_stream(),
         begin_mode,
         count,
         index_type,
         index_pointer,
         base_vertex
      );

      ++model_index;
   }
}

void deferred_renderer::bind_linear_fbo()
{
   GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
   gl::draw_buffers(1, draw_buffers);

   gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, m_linear_fbo);
   if (m_use_stencil)
   {
      gl::bind_framebuffer(GL_READ_FRAMEBUFFER, m_gbuffer_fbo);
      gl::blit_framebuffer(
         0, 0, m_width, m_height,
         0, 0, m_width, m_height,
         GL_DEPTH_BUFFER_BIT,
         GL_NEAREST
      );
   }

   GLenum a = gl::check_framebuffer_status(GL_FRAMEBUFFER);
   if (a != GL_FRAMEBUFFER_COMPLETE)
      throw runtime_error("FBO is not complete");
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

void deferred_renderer::light_pass(
   shared_ptr<vector<shared_ptr<light> > > lights,
   shared_ptr<camera> camera
)
{
   bind_linear_fbo();

   gl::viewport(0, 0, m_width, m_height);

   gl::clear_color(0.0f, 0.0f, 0.0f, 1.0f);
   gl::clear_stencil(0);
   if (m_use_stencil)
      gl::clear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   else
      gl::clear(GL_COLOR_BUFFER_BIT);

   auto &r = *m_renderer;
   auto &t = r.track();

   // Don't bind emission texture for now
   r.set_texture(0, m_gbuffer_rt[0]); // normal tangent
   r.set_texture(1, m_gbuffer_rt[1]); // albedo
   r.set_texture(2, m_gbuffer_rt[2]); // material
   r.reset_texture(3, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.set_texture(4, m_depth);
   r.set_program(m_programs->light_spot);

   r.set_buffer(renderstack::graphics::buffer_target::uniform_buffer, m_uniform_buffer);
   void *start0 = m_uniform_buffer->map(
      r, 
      0, 
      m_uniform_buffer->capacity(), 
      static_cast<gl::buffer_access_mask::value>(
         gl::buffer_access_mask::map_write_bit | 
         gl::buffer_access_mask::map_flush_explicit_bit |
         gl::buffer_access_mask::map_invalidate_buffer_bit
      )
   );

   // viewport (in camera constant buffer)
   vec4 vp;
   vp.x = static_cast<float>(0);
   vp.y = static_cast<float>(0);
   vp.z = static_cast<float>(m_width);
   vp.w = static_cast<float>(m_height);

   ubr_ptr start;
   start.model    = static_cast<unsigned char*>(start0) + m_model_ubr   ->first_byte();
   start.camera   = static_cast<unsigned char*>(start0) + m_camera_ubr  ->first_byte();
   start.material = static_cast<unsigned char*>(start0) + m_material_ubr->first_byte();
   start.lights   = static_cast<unsigned char*>(start0) + m_lights_ubr  ->first_byte();
   start.debug    = nullptr;
   ubr_pos offsets;

   // Camera
   mat4 const &world_from_clip = camera->clip_from_world().inverse_matrix();
   mat4 const &world_from_view = camera->frame()->world_from_local().matrix();
   mat4 const &clip_from_world = camera->clip_from_world().matrix();
   mat4 const &view_from_world = camera->frame()->world_from_local().inverse_matrix();
   glm::vec3 view_position_in_world = vec3(world_from_view * vec4(0.0f, 0.0f, 0.0f, 1.0f));
   float exposure = 1.0f;
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.world_from_view, value_ptr(world_from_view), 16 * sizeof(float));
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.world_from_clip, value_ptr(world_from_clip), 16 * sizeof(float));
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.viewport, value_ptr(vp), 4 * sizeof(float));
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.exposure, &exposure, sizeof(float));
   offsets.camera += m_programs->camera_block->size_bytes();
   m_camera_ubr->flush(r, offsets.camera);

   int light_index = 0;
   for (auto i = lights->cbegin(); i != lights->cend(); ++i)
   {
      auto l = *i;

      l->frame()->update_hierarchical_no_cache(); // TODO

      mat4 scale              = m_light_mesh->get_light_transform(l);
      mat4 world_from_light   = l->frame()->world_from_local().matrix() * scale;
      mat4 view_from_light    = view_from_world * world_from_light;
      mat4 clip_from_light;

      switch (l->type())
      {
      case light_type::spot:
         clip_from_light = clip_from_world * world_from_light;
         break;

      case light_type::directional:
         clip_from_light = mat4(1.0f);
         break;

      default:
         assert(0);
         break;
      }

      glm::vec3 position   = vec3(l->frame()->world_from_local().matrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f));
      glm::vec3 direction  = vec3(l->frame()->world_from_local().matrix() * vec4(0.0f, 0.0f, 1.0f, 0.0f));
      glm::vec3 radiance   = l->intensity() * l->color();

      direction = normalize(direction);

      // Somewhat unneeded for other than point lights
      float spot_angle  = l->spot_angle() * 0.5f;
      float spot_cutoff = std::cos(spot_angle);

      ::memcpy(start.model + offsets.model + m_programs->model_block_access.clip_from_model,  value_ptr(clip_from_light),  16 * sizeof(float));
      ::memcpy(start.model + offsets.model + m_programs->model_block_access.world_from_model, value_ptr(world_from_light), 16 * sizeof(float));
      ::memcpy(start.model + offsets.model + m_programs->model_block_access.view_from_model,  value_ptr(view_from_light),  16 * sizeof(float));

      ::memcpy(start.lights + offsets.lights + m_programs->lights_block_access.position,     value_ptr(position),    3 * sizeof(float));
      ::memcpy(start.lights + offsets.lights + m_programs->lights_block_access.direction,    value_ptr(direction),   3 * sizeof(float));
      ::memcpy(start.lights + offsets.lights + m_programs->lights_block_access.radiance,     value_ptr(radiance),    3 * sizeof(float));
      ::memcpy(start.lights + offsets.lights + m_programs->lights_block_access.spot_cutoff,  &spot_cutoff,           1 * sizeof(float));

      offsets.model += m_programs->model_block->size_bytes();
      offsets.lights += m_programs->lights_block->size_bytes();

      ++light_index;
   }
   assert(light_index < m_ubr_sizes.model);
   m_model_ubr->flush(r, offsets.model);
   assert(light_index < m_ubr_sizes.lights);
   m_lights_ubr->flush(r, offsets.lights);

   m_uniform_buffer->unmap(r);

   m_uniform_buffer->bind_range(
      m_programs->camera_block->binding_point(),
      m_camera_ubr->first_byte(),
      m_programs->camera_block->size_bytes()
   );

   light_index = 0;
   if (!m_use_stencil)
      t.execute(&m_light_render_states);
   for (auto i = lights->cbegin(); i != lights->cend(); ++i)
   {
      auto l = *i;

      if (light_index == m_max_lights)
         break;

      auto geometry_mesh   = m_light_mesh->get_light_mesh(l);
      auto vertex_stream   = geometry_mesh->vertex_stream();
      auto mesh            = geometry_mesh->get_mesh();

      m_uniform_buffer->bind_range(
         m_programs->lights_block->binding_point(),
         m_lights_ubr->first_byte() + light_index * m_programs->lights_block->size_bytes(),
         m_programs->lights_block->size_bytes()
      );

      m_uniform_buffer->bind_range(
         m_programs->model_block->binding_point(),
         m_model_ubr->first_byte() + (light_index * m_programs->model_block->size_bytes()),
         m_programs->model_block->size_bytes()
      );

      gl::begin_mode::value         begin_mode     = gl::begin_mode::triangles;
      index_range const             &index_range   = geometry_mesh->fill_indices();
      GLsizei                       count          = static_cast<GLsizei>(index_range.index_count);
      gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_int;
      GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>((index_range.first_index + mesh->first_index()) * mesh->index_buffer()->stride());
      GLint                         base_vertex    = configuration::can_use.draw_elements_base_vertex
         ? static_cast<GLint>(mesh->first_vertex())
         : 0;

      assert(index_range.index_count > 0);

      bool used_stencil = false;
      if (m_use_stencil && (l->type() == light_type::spot))
      {
         bool view_in_light = m_light_mesh->point_in_light(view_position_in_world, l);
         if (!view_in_light)
         {
            t.execute(&m_light_stencil_render_states);
            r.set_program(m_programs->stencil);

            r.draw_elements_base_vertex(
               geometry_mesh->vertex_stream(),
               begin_mode, count, index_type, index_pointer, base_vertex);
            used_stencil = true;
         }
      }

      switch (l->type())
      {
      case light_type::spot:
         if (m_use_stencil)
            if (used_stencil)
               t.execute(&m_light_with_stencil_test_render_states);
            else
               t.execute(&m_light_render_states);
         r.set_program(m_programs->light_spot);
         break;

      case light_type::directional:
         if (m_use_stencil)
            t.execute(&m_light_render_states);
         r.set_program(m_programs->light_directional);
         break;

      default:
         assert(0);
      }

      r.draw_elements_base_vertex(
         geometry_mesh->vertex_stream(),
         begin_mode, count, index_type, index_pointer, base_vertex);

      ++light_index;
   }


   bind_default_framebuffer();

   t.execute(&m_camera_render_states);
   r.set_texture(0, m_linear_rt[0]);
   r.reset_texture(1, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.reset_texture(2, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.reset_texture(3, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.reset_texture(4, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.set_program(m_programs->camera);

   gl::viewport(0, 0, m_width_full, m_height_full);
   gl::enable(GL_FRAMEBUFFER_SRGB);
   m_quad_renderer->render_minus_one_to_one();
   gl::disable(GL_FRAMEBUFFER_SRGB);
}

void deferred_renderer::show_rt()
{
#if 0
   bind_default_framebuffer();

   auto &r = *m_renderer;
   auto &t = r.track();
   t.reset();
   m_show_rt_render_states.face_cull.set_enabled(false);
   t.execute(&m_show_rt_render_states);
   for (int i = 0; i < 4; ++i)
   {
      shared_ptr<renderstack::graphics::program> p = (i == 2)
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

      if (p->use_uniform_buffers())
      {
         assert(m_programs->model_ubr);
         assert(m_programs->debug_ubr);

         unsigned char *start = m_programs->begin_edit_uniforms();
         ::memcpy(&start[m_programs->model_ubr->first_byte() + m_programs->model_block_access.clip_from_model], value_ptr(transform), 16 * sizeof(float));
         ::memcpy(&start[m_programs->debug_ubr->first_byte() + m_programs->debug_block_access.show_rt_transform], value_ptr(identity), 16 * sizeof(float));
         m_programs->model_ubr->flush(r);
         m_programs->debug_ubr->flush(r);
         m_programs->end_edit_uniforms();
      }
      else
      {
         gl::uniform_matrix_4fv(
            p->uniform_at(m_programs->model_block_access.clip_from_model),
            1, GL_FALSE, value_ptr(transform));
         gl::uniform_matrix_4fv(
            p->uniform_at(m_programs->debug_block_access.show_rt_transform),
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
}

