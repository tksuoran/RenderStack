#ifndef debug_renderer_hpp_renderers
#define debug_renderer_hpp_renderers

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "main/programs.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <cstdint>

namespace renderstack
{
   namespace graphics
   {
      class uniform_buffer;
      class uniform_buffer_range;
   }
}

struct draw
{
   glm::mat4   world_from_model;
   int         first;
   int         count;
};

class debug_renderer : public renderstack::toolkit::service
{
public:
   debug_renderer();
   /*virtual*/ ~debug_renderer();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<programs>                          programs
   );

   void initialize_service();

   void begin_edit();
   void end_edit();
   void render();

   void set_camera(glm::mat4 const &clip_from_world, glm::mat4 const &view_from_world);
   void set_model(glm::mat4 const &world_from_model);
   void set_color(glm::vec4 color);

   void add_line(glm::vec3 start, glm::vec3 end);
   void add_box(glm::vec3 min_, glm::vec3 max_);

private:
   std::uint16_t add_point(glm::vec3 p);

private:
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<programs>                                      m_programs;

   renderstack::graphics::render_states                           m_render_states;

   std::shared_ptr<renderstack::graphics::vertex_format>          m_vertex_format;
   std::shared_ptr<renderstack::graphics::vertex_stream_mappings> m_mappings;
   std::shared_ptr<renderstack::graphics::buffer>                 m_vertex_buffer;
   std::shared_ptr<renderstack::graphics::buffer>                 m_index_buffer;
   std::shared_ptr<renderstack::graphics::vertex_stream>          m_vertex_stream;

   bool              m_in_edit;
   float             *m_vertex_ptr_start;
   std::uint16_t     *m_index_ptr_start;
   float             *m_vertex_ptr;
   std::uint16_t     *m_index_ptr;
   int               m_capacity_lines;
   std::uint16_t     m_vertex_offset;
   int               m_index_offset;

   glm::mat4         m_clip_from_world;
   glm::mat4         m_view_from_world;
   glm::vec4         m_color;

   draw              m_current_draw;
   std::vector<draw> m_draws;
};


#endif
