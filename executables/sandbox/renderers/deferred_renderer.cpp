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
,  m_quad_renderer(nullptr)
,  m_gbuffer_fbo  (0)
,  m_linear_fbo   (0)
,  m_stencil_rbo  (0)
{
}

/*virtual*/ deferred_renderer::~deferred_renderer()
{
}

void deferred_renderer::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<class programs>                   programs_,
   shared_ptr<quad_renderer>                    quad_renderer_,
   std::shared_ptr<class light_mesh>            light_mesh_
)
{
   base_connect(renderer_, programs_, light_mesh_);
   m_quad_renderer = quad_renderer_;

   initialization_depends_on(renderer_);
   initialization_depends_on(programs_);
}

void deferred_renderer::initialize_service()
{
   base_initialize_service();

   m_gbuffer_render_states.depth.set_enabled(true);
   m_gbuffer_render_states.face_cull.set_enabled(true);

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
}

void deferred_renderer::resize(int width_, int height_)
{
   base_resize(width_, height_);
   {
      if (m_gbuffer_fbo == 0)
         gl::gen_framebuffers(1, &m_gbuffer_fbo);

      gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, m_gbuffer_fbo);
      GLenum formats[] = {
         GL_RGBA16F,    // normal tangent
         GL_RGBA8,      // albedo
         GL_RGBA16F     // material
         //GL_RGBA8       // material
      };

      for (int i = 0; i < 3; ++i)
      {
         m_gbuffer_rt[i].reset();

         m_gbuffer_rt[i] = make_shared<renderstack::graphics::texture>(
            renderstack::graphics::texture_target::texture_2d,
            formats[i],
            false,
            width(),
            height(),
            0
         );
         m_gbuffer_rt[i]->allocate_storage(renderer());
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

      GLenum depth_format = use_stencil() ? GL_DEPTH32F_STENCIL8 : GL_DEPTH_COMPONENT32F;
      GLenum attachment_point = use_stencil() ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;

      m_depth.reset();
      m_depth = make_shared<renderstack::graphics::texture>(
         renderstack::graphics::texture_target::texture_2d,
         depth_format,
         false,
         width(),
         height(),
         0
      );
      m_depth->set_mag_filter(gl::texture_mag_filter::nearest);
      m_depth->set_min_filter(gl::texture_min_filter::nearest);
      m_depth->set_wrap(0, gl::texture_wrap_mode::clamp_to_edge);
      m_depth->set_wrap(1, gl::texture_wrap_mode::clamp_to_edge);
      m_depth->allocate_storage(renderer());

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
            width(),
            height(),
            0
         );
         m_linear_rt[i]->allocate_storage(renderer());
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

      if (use_stencil())
      {
         if (m_stencil_rbo == 0)
            gl::gen_renderbuffers(1, &m_stencil_rbo);

         gl::bind_renderbuffer(GL_RENDERBUFFER, m_stencil_rbo);

         gl::renderbuffer_storage(
            GL_RENDERBUFFER,
            GL_DEPTH32F_STENCIL8,
            width(),
            height()
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

   auto &r = renderer();
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
   vector<shared_ptr<material> > const &materials,
   vector<shared_ptr<model> > const &models,
   shared_ptr<renderstack::scene::camera> camera
)
{
   bind_gbuffer_fbo();

   gl::viewport(0, 0, width(), height());

   fbo_clear();

   if (models.size() == 0)
      return;

   update_camera(camera);
   update_models(models, camera);
   update_materials(materials);

   auto &r = renderer();
   auto &t = r.track();
   auto p = programs()->gbuffer;
   t.reset();
   t.execute(&m_gbuffer_render_states);
   r.set_program(p);

   bind_camera();

   size_t model_index = 0;
   for (auto i = models.cbegin(); i != models.cend(); ++i)
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

      bind_model(model_index);
      bind_material(material_index);

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
   if (use_stencil())
   {
      gl::bind_framebuffer(GL_READ_FRAMEBUFFER, m_gbuffer_fbo);
      gl::blit_framebuffer(
         0, 0, width(), height(),
         0, 0, width(), height(),
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
   vector<shared_ptr<light> > const &lights,
   shared_ptr<class camera> camera
)
{
   bind_linear_fbo();

   gl::viewport(0, 0, width(), height());
   gl::clear_color(0.0f, 0.0f, 0.0f, 1.0f);
   gl::clear_stencil(0);
   if (use_stencil())
      gl::clear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   else
      gl::clear(GL_COLOR_BUFFER_BIT);

   update_lights_models(lights, camera);
   update_lights(lights, camera);

   mat4 const &world_from_view = camera->frame()->world_from_local().matrix();
   vec3 view_position_in_world = vec3(world_from_view * vec4(0.0f, 0.0f, 0.0f, 1.0f));

   auto &r = renderer();
   auto &t = r.track();

   // Don't bind emission texture for now
   r.set_texture(0, m_gbuffer_rt[0]); // normal tangent
   r.set_texture(1, m_gbuffer_rt[1]); // albedo
   r.set_texture(2, m_gbuffer_rt[2]); // material
   r.reset_texture(3, renderstack::graphics::texture_target::texture_2d, nullptr);
   r.set_texture(4, m_depth);
   r.set_program(programs()->light_spot);

   bind_camera();

   size_t light_index = 0;
   if (!use_stencil())
      t.execute(&m_light_render_states);

   for (auto i = lights.cbegin(); i != lights.cend(); ++i)
   {
      auto l = *i;

      if (light_index == max_lights())
         break;

      auto geometry_mesh   = light_mesh()->get_light_mesh(l);
      auto vertex_stream   = geometry_mesh->vertex_stream();
      auto mesh            = geometry_mesh->get_mesh();

      bind_light_model(light_index);
      bind_light(light_index);

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
      if (use_stencil() && (l->type() == light_type::spot))
      {
         bool view_in_light = light_mesh()->point_in_light(view_position_in_world, l);
         if (!view_in_light)
         {
            t.execute(&m_light_stencil_render_states);
            r.set_program(programs()->stencil);

            r.draw_elements_base_vertex(
               geometry_mesh->vertex_stream(),
               begin_mode, count, index_type, index_pointer, base_vertex);
            used_stencil = true;
         }
      }

      switch (l->type())
      {
      case light_type::spot:
         if (use_stencil())
            if (used_stencil)
               t.execute(&m_light_with_stencil_test_render_states);
            else
               t.execute(&m_light_render_states);
         r.set_program(programs()->light_spot);
         break;

      case light_type::directional:
         if (use_stencil())
            t.execute(&m_light_render_states);
         r.set_program(programs()->light_directional);
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
   r.set_program(programs()->camera);

   gl::viewport(0, 0, width_full(), height_full());
   //gl::enable(GL_FRAMEBUFFER_SRGB);
   m_quad_renderer->render_minus_one_to_one();
   //gl::disable(GL_FRAMEBUFFER_SRGB);
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

