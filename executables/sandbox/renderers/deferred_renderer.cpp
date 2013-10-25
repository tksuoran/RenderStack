#include "renderstack_toolkit/platform.hpp"
#include "renderers/deferred_renderer.hpp"
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
#include "renderstack_scene/viewport.hpp"
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


deferred_renderer::deferred_renderer()
:  service("deferred_renderer")
,  m_renderer     (nullptr)
,  m_programs     (nullptr)
,  m_quad_renderer(nullptr)
,  m_fbo          (0)
{
}

/*virtual*/ deferred_renderer::~deferred_renderer()
{
}

void deferred_renderer::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<programs>                         programs_,
   shared_ptr<quad_renderer>                    quad_renderer_
)
{
   m_renderer = renderer_;
   m_programs = programs_;
   m_quad_renderer = quad_renderer_;

   initialization_depends_on(m_renderer);
   initialization_depends_on(m_programs);
}

void deferred_renderer::initialize_service()
{
   assert(m_renderer);
   assert(m_programs);

   m_mesh_render_states.depth.set_enabled(true);
   m_mesh_render_states.face_cull.set_enabled(true);

   m_light_render_states.depth.set_enabled(false);
   m_light_render_states.face_cull.set_enabled(true);
   m_light_render_states.face_cull.set_cull_face_mode(gl::cull_face_mode::front);
   m_light_render_states.blend.set_enabled(true);
   //m_light_render_states.blend.set_enabled(false);
   m_light_render_states.blend.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
   m_light_render_states.blend.rgb().set_source_factor(gl::blending_factor_src::one);
   m_light_render_states.blend.rgb().set_destination_factor(gl::blending_factor_dest::one);

   m_debug_light_render_states.depth.set_enabled(true);
   m_debug_light_render_states.depth.set_function(gl::depth_function::l_equal);
   m_debug_light_render_states.face_cull.set_enabled(true);
   m_debug_light_render_states.blend.set_enabled(true);
   m_debug_light_render_states.blend.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
   m_debug_light_render_states.blend.rgb().set_source_factor(gl::blending_factor_src::src_alpha);
   m_debug_light_render_states.blend.rgb().set_destination_factor(gl::blending_factor_dest::one);   
   m_debug_light_render_states.blend.alpha().set_equation_mode(gl::blend_equation_mode::func_add);
   m_debug_light_render_states.blend.alpha().set_source_factor(gl::blending_factor_src::one);
   m_debug_light_render_states.blend.alpha().set_destination_factor(gl::blending_factor_dest::one);   

   // Nothing to change in, use default render states:
   // m_show_rt_render_states

   if (renderstack::graphics::configuration::can_use.uniform_buffer_object)
   {
      auto &r = *m_renderer;

      size_t ubo_size = 0;

      m_ubr_sizes.camera   = 100;
      m_ubr_sizes.model    = 100;
      m_ubr_sizes.material = 100;
      m_ubr_sizes.lights   = 100;
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
   //GLenum formats[] = { GL_RGBA8, GL_RGBA8, GL_RGBA16_SNORM, GL_RGBA8 };
   GLenum formats[] = { GL_RGBA8, GL_RGBA8, GL_RGBA16F, GL_RGBA8 };
   //GLenum formats[] = { GL_RGBA8, GL_RGBA8, GL_RGBA16_SNORM, GL_RGBA8 };
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
      m_rt[i]->set_wrap(0, gl::texture_wrap_mode::clamp_to_edge);
      m_rt[i]->set_wrap(1, gl::texture_wrap_mode::clamp_to_edge);

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
   m_depth->set_mag_filter(gl::texture_mag_filter::nearest);
   m_depth->set_min_filter(gl::texture_min_filter::nearest);
   m_depth->set_wrap(0, gl::texture_wrap_mode::clamp_to_edge);
   m_depth->set_wrap(1, gl::texture_wrap_mode::clamp_to_edge);
   m_depth->allocate_storage(*m_renderer);
   gl::framebuffer_texture_2d(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth->gl_name(), 0);
   gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
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
   {
      throw runtime_error("FBO is not complete");
   }
   gl::clear_buffer_fv(GL_COLOR, 1, &albedo_clear        [0]);
   gl::clear_buffer_fv(GL_COLOR, 2, &normal_tangent_clear[0]);
   gl::clear_buffer_fv(GL_COLOR, 3, &material_clear      [0]);
   gl::clear_buffer_fv(GL_DEPTH, 0, &one);
}

void deferred_renderer::set_max_lights(int max_lights)
{
   m_max_lights = max_lights;
}

void deferred_renderer::geometry_pass(
   shared_ptr<vector<shared_ptr<model> > > models,
   shared_ptr<renderstack::scene::camera> camera
)
{
   bind_fbo();

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
   vec4  color(1.0f, 1.0f, 1.0f, 1.0f);
   float roughness = 0.10f;
   float isotropy = 0.02f;
   ::memcpy(start.material + offsets.material + m_programs->material_block_access.color    , value_ptr(color), 4 * sizeof(float));
   ::memcpy(start.material + offsets.material + m_programs->material_block_access.roughness, &roughness,       sizeof(float));
   ::memcpy(start.material + offsets.material + m_programs->material_block_access.isotropy , &isotropy,        sizeof(float));
   offsets.material += m_programs->material_block->size_bytes();
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

void deferred_renderer::update_light_model(shared_ptr<light> l)
{
   switch (l->type())
   {
   case light_type::directional:
      assert(0);
      break;
   case light_type::point:
      assert(0);
      break;

   case light_type::spot:
      {
         //           Side:                     Bottom:
         //             .                    ______________
         //            /|\                  /       |    / \
         //           / | \                /   apothem  /   \
         //          /  +--+ alpha        /         | radius \
         //         /   |   \            /          | /       \
         //        /    |    \          /           |/         \
         //       /     |     \         \                      /
         //      /      |      \         \                    /
         //     /     length    \         \                  /
         //    /        |        \         \                /
         //   /         |         \         \______________/
         //  +----------+----------+

         int   n        = 16;
         float alpha    = l->spot_angle(); 
         float length   = l->range();
         float apothem  = length * std::sin(alpha);
         float radius   = apothem / std::cos(glm::pi<float>() / static_cast<float>(n));

         shared_ptr<renderstack::geometry::geometry> g = make_shared<renderstack::geometry::shapes::conical_frustum>(
            0.0,        // min x
            length,     // max x
            0.0,        // bottom radius
            radius,     // top radius
            false,      // use bottom
            true,       // use top (end)
            n,          // slice count
            0           // stack division
         );

         g->transform(mat4_rotate_xz_cw);
         g->build_edges();

         renderstack::mesh::geometry_mesh_format_info format_info;
         renderstack::mesh::geometry_mesh_buffer_info buffer_info;

         renderstack::geometry::geometry::mesh_info info;

         format_info.set_want_fill_triangles(true);
         format_info.set_want_edge_lines(true);
         format_info.set_want_position(true);
         format_info.set_vertex_attribute_mappings(m_programs->attribute_mappings);

         geometry_mesh::prepare_vertex_format(g, format_info, buffer_info);

         auto &r = *m_renderer;

         auto gm = make_shared<renderstack::mesh::geometry_mesh>(r, g, format_info, buffer_info);

         m_light_meshes[l] = gm;
      }
      break;

   default:
      assert(0);
   }
}

void deferred_renderer::light_pass(
   shared_ptr<vector<shared_ptr<light> > > lights,
   shared_ptr<camera> camera,
   renderstack::scene::viewport const &viewport
)
{
   // Temp draw directly to screen
   bind_default_framebuffer();
   //bind_fbo();

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   // glEnable(GL_FRAMEBUFFER_SRGB);

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
   r.set_texture(4, m_depth);
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

   // viewport (in camera constant buffer)
   vec4 vp;
   vp.x = static_cast<float>(viewport.x());
   vp.y = static_cast<float>(viewport.y());
   vp.z = static_cast<float>(viewport.width());
   vp.w = static_cast<float>(viewport.height());

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
   //float exposure = 0.1f;
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.world_from_view, value_ptr(world_from_view), 16 * sizeof(float));
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.world_from_clip, value_ptr(world_from_clip), 16 * sizeof(float));
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.viewport, value_ptr(vp), 4 * sizeof(float));
   offsets.camera += m_programs->camera_block->size_bytes();
   m_camera_ubr->flush(r, offsets.camera);

   int light_index = 0;
   for (auto i = lights->cbegin(); i != lights->cend(); ++i)
   {
      auto l = *i;

      l->frame()->update_hierarchical_no_cache(); // TODO

      mat4 world_from_light   = l->frame()->world_from_local().matrix();
      mat4 clip_from_light    = clip_from_world * world_from_light;
      mat4 view_from_light    = view_from_world * world_from_light;

      glm::vec3 position   = vec3(l->frame()->world_from_local().matrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f));
      glm::vec3 direction  = vec3(l->frame()->world_from_local().matrix() * vec4(0.0f, 0.0f, 1.0f, 0.0f));
      glm::vec3 radiance   = l->intensity() * l->color();

      direction = normalize(direction);

      ::memcpy(start.model + offsets.model + m_programs->model_block_access.clip_from_model,  value_ptr(clip_from_light), 16 * sizeof(float));
      ::memcpy(start.model + offsets.model + m_programs->model_block_access.world_from_model, value_ptr(world_from_light), 16 * sizeof(float));
      ::memcpy(start.model + offsets.model + m_programs->model_block_access.view_from_model,  value_ptr(view_from_light), 16 * sizeof(float));

      ::memcpy(start.lights + offsets.lights + m_programs->lights_block_access.position , value_ptr(position),  3 * sizeof(float));
      ::memcpy(start.lights + offsets.lights + m_programs->lights_block_access.direction, value_ptr(direction), 3 * sizeof(float));
      ::memcpy(start.lights + offsets.lights + m_programs->lights_block_access.radiance , value_ptr(radiance),  3 * sizeof(float));

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
   for (auto i = lights->cbegin(); i != lights->cend(); ++i)
   {
      auto l = *i;

      if (light_index == m_max_lights)
         break;

      assert(l->type() == light_type::spot);

      if (m_light_meshes.find(l) == m_light_meshes.end())
         update_light_model(l);

      auto geometry_mesh   = m_light_meshes[l];
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

      r.draw_elements_base_vertex(
         geometry_mesh->vertex_stream(),
         begin_mode, count, index_type, index_pointer, base_vertex);

      ++light_index;
   }
   //glDisable(GL_FRAMEBUFFER_SRGB);

   // m_quad_renderer->render_minus_one_to_one();

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

