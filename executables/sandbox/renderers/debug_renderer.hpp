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
#include <deque>

namespace renderstack { namespace graphics {
   class uniform_buffer;
   class uniform_buffer_range;
} }
namespace renderstack { namespace scene {
   class camera;
   class viewport;
} }
namespace renderstack { namespace ui {
   class font;
   class gui_renderer;
   class text_buffer;
} }

struct draw
{
   glm::mat4   clip_from_model;
   int         first;
   int         count;
};

struct print_at
{
   int x;
   int y;
   std::string text;
};

class debug_renderer : public renderstack::toolkit::service
{
public:
   debug_renderer();
   /*virtual*/ ~debug_renderer();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer> renderer,
      std::shared_ptr<renderstack::ui::gui_renderer> gui_renderer,
      std::shared_ptr<programs> programs
   );

   void initialize_service();

   void begin_edit();
   void end_edit();
   void render();

   void clear_text_lines();
   void record_frame_duration(float frame_duration);
   void add_frame_duration_graph(renderstack::scene::viewport const &vp);

   void printf(const char *format, ...);
   void printf(int x, int y, const char *format, ...);
   void render_text_lines(renderstack::scene::viewport const &vp);

   void set_camera(std::shared_ptr<renderstack::scene::camera> camera);
   void set_model(glm::mat4 const &world_from_model);
   void set_color(glm::vec4 color);

   void add_line(glm::vec3 start, glm::vec3 end);
   void add_box(glm::vec3 min_, glm::vec3 max_);

private:
   std::uint16_t add_point(glm::vec3 p);

private:
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<renderstack::ui::gui_renderer>                 m_gui_renderer;
   std::shared_ptr<programs>                                      m_programs;

   std::shared_ptr<renderstack::scene::camera>                    m_camera;

   // Self owned
   std::shared_ptr<renderstack::graphics::vertex_format>          m_vertex_format;
   std::shared_ptr<renderstack::graphics::vertex_stream_mappings> m_mappings;
   std::shared_ptr<renderstack::graphics::buffer>                 m_vertex_buffer;
   std::shared_ptr<renderstack::graphics::buffer>                 m_index_buffer;
   std::shared_ptr<renderstack::graphics::vertex_stream>          m_vertex_stream;

   // Debug text lines
   std::vector<std::string>                        m_debug_lines;
   std::vector<print_at>                           m_debug_print_ats;

#if defined(RENDERSTACK_USE_FREETYPE)
   std::shared_ptr<renderstack::ui::font>          m_font;
   std::shared_ptr<renderstack::ui::text_buffer>   m_text_buffer;
   renderstack::graphics::render_states            m_font_render_states;
#endif

   renderstack::graphics::render_states            m_render_states;
   
   bool                 m_in_edit;
   float                *m_vertex_ptr_start;
   std::uint16_t        *m_index_ptr_start;
   float                *m_vertex_ptr;
   std::uint16_t        *m_index_ptr;
   int                  m_capacity_lines;
   std::uint16_t        m_vertex_offset;
   int                  m_index_offset;

   glm::vec4            m_color;

   draw                 m_current_draw;
   std::vector<draw>    m_draws;
   std::deque<float>    m_frame_durations; // TODO use ringbuffer, implement on top of vector<>
   std::size_t          m_frame_duration_graph_size;
};


#endif
