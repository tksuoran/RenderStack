#include "renderstack_toolkit/platform.hpp"
#include "main/debug_renderer.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/vertex_format.hpp"
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
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;


debug_renderer::debug_renderer(
   std::shared_ptr<renderstack::graphics::renderer>   renderer,
   std::shared_ptr<programs>                          programs,
   std::shared_ptr<renderstack::graphics::buffer>     uniform_buffer
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

   auto vf = m_vertex_format = make_shared<vertex_format>();
   vf->make_attribute(
      vertex_attribute_usage::position,
      gl::vertex_attrib_pointer_type::float_,
      gl::vertex_attrib_pointer_type::float_,
      0,
      3
   );
   vf->make_attribute(
      vertex_attribute_usage::color,
      gl::vertex_attrib_pointer_type::float_,
      gl::vertex_attrib_pointer_type::float_,
      0,
      4
   );

   m_capacity_lines = 1000;

   m_vertex_buffer = make_shared<buffer>(
      renderstack::graphics::buffer_target::array_buffer,
      m_capacity_lines * 2,
      vf->stride()
   );

   m_index_buffer = make_shared<buffer>(
      renderstack::graphics::buffer_target::array_buffer,
      m_capacity_lines * 2,
      vf->stride()
   );

}

void debug_renderer::set_camera(
   glm::mat4 const &clip_from_world,
   glm::mat4 const &view_from_world
)
{
   m_clip_from_world = clip_from_world;
   m_view_from_world = view_from_world;
}

void debug_renderer::set_model(glm::mat4 const &world_from_model)
{
   m_world_from_model = world_from_model;
}

void debug_renderer::set_color(glm::vec4 color)
{
   m_color = color;
}

void debug_renderer::add_line(glm::vec3 start, glm::vec3 end)
{
   (void)start;
   (void)end;
}

void debug_renderer::add_box(glm::vec3 center, glm::vec3 half_axes)
{
   (void)center;
   (void)half_axes;
}

void debug_renderer::add_sphere(glm::vec3 center, float radius)
{
   (void)center;
   (void)radius;
}

