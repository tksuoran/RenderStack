#include "renderstack_toolkit/platform.hpp"
#include "renderers/light_debug_renderer.hpp"
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


light_debug_renderer::light_debug_renderer()
:  service("light_debug_renderer")
,  m_renderer  (nullptr)
,  m_programs  (nullptr)
,  m_max_lights(1)
{
}

/*virtual*/ light_debug_renderer::~light_debug_renderer()
{
}

void light_debug_renderer::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<programs>                         programs_
)
{
   m_renderer = renderer_;
   m_programs = programs_;

   initialization_depends_on(m_renderer);
   initialization_depends_on(m_programs);
}

void light_debug_renderer::initialize_service()
{
   assert(m_renderer);
   assert(m_programs);

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


void light_debug_renderer::update_light_model(shared_ptr<light> l)
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
         float apothem  = length * std::tan(alpha * 0.5f);
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

void light_debug_renderer::set_max_lights(int max_lights)
{
   m_max_lights = max_lights;
}

void light_debug_renderer::light_pass(
   shared_ptr<vector<shared_ptr<light> > > lights,
   shared_ptr<camera> camera
)
{
   auto &r = *m_renderer;
   auto &t = r.track();
   auto p = m_programs->debug_light;

   t.execute(&m_debug_light_render_states);
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

   // Camera
   mat4 const &world_from_clip = camera->clip_from_world().inverse_matrix();
   mat4 const &world_from_view = camera->frame()->world_from_local().matrix();
   mat4 const &clip_from_world = camera->clip_from_world().matrix();
   mat4 const &view_from_world = camera->frame()->world_from_local().inverse_matrix();
   //float exposure = 0.1f;
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.world_from_view, value_ptr(world_from_view), 16 * sizeof(float));
   ::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.world_from_clip, value_ptr(world_from_clip), 16 * sizeof(float));
   //::memcpy(start.camera + offsets.camera + m_programs->camera_block_access.viewport, value_ptr(vp), 4 * sizeof(float));
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

      gl::begin_mode::value         begin_mode     = gl::begin_mode::lines;
      index_range const             &index_range   = geometry_mesh->edge_line_indices();
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
}

