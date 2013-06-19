#ifndef debug_renderer_hpp
#define debug_renderer_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "main/programs.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace renderstack
{
   namespace graphics
   {
      class uniform_buffer;
      class uniform_buffer_range;
   }
}


class debug_renderer
{
public:
   debug_renderer(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<programs>                          programs,
      std::shared_ptr<renderstack::graphics::buffer>     uniform_buffer
   );

   void set_camera(glm::mat4 const &clip_from_world, glm::mat4 const &view_from_world);
   void set_model(glm::mat4 const &world_from_model);
   void set_color(glm::vec4 color);

   void add_line(glm::vec3 start, glm::vec3 end);
   void add_box(glm::vec3 center, glm::vec3 half_axes);
   void add_sphere(glm::vec3 center, float radius);

private:
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<programs>                                      m_programs;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_mesh_render_uniform_buffer_range;
   renderstack::graphics::render_states                           m_mesh_render_states;

   std::shared_ptr<renderstack::graphics::vertex_format>          m_vertex_format;
   std::shared_ptr<renderstack::graphics::buffer>                 m_vertex_buffer;
   std::shared_ptr<renderstack::graphics::buffer>                 m_index_buffer;
   std::shared_ptr<renderstack::graphics::vertex_stream>          m_vertex_stream;

   int         m_capacity_lines;
   glm::mat4   m_clip_from_world;
   glm::mat4   m_view_from_world;
   glm::mat4   m_world_from_model;
   glm::vec4   m_color;

};


#endif
