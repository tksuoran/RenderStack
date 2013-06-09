#ifndef quad_renderer__hpp
#define quad_renderer__hpp

#include "renderstack_toolkit/platform.hpp"
#include <memory>

namespace renderstack { namespace graphics {
   class renderer;
   class vertex_stream_mappings;
   class vertex_format;
   class vertex_stream;
   class buffer;
} }

class quad_renderer
{
public:
   quad_renderer(std::shared_ptr<renderstack::graphics::renderer> renderer);

public:
   void render_minus_one_to_one();
   void render_zero_to_one();

private:
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<renderstack::graphics::vertex_stream_mappings> m_mappings;
   std::shared_ptr<renderstack::graphics::vertex_format>          m_vertex_format;
   std::shared_ptr<renderstack::graphics::vertex_stream>          m_vertex_stream;
   std::shared_ptr<renderstack::graphics::buffer>                 m_vertex_buffer;
   std::shared_ptr<renderstack::graphics::buffer>                 m_index_buffer;
};

#endif
