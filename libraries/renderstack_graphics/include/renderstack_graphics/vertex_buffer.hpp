#ifndef renderstack_graphics__vertex_buffer_hpp
#define renderstack_graphics__vertex_buffer_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/buffer.hpp"
#include <cstddef>
#include <map>
#include <deque>
#include <memory>

namespace renderstack { namespace graphics {

class buffer;
class vertex_buffer;


class vertex_buffer
{
public:
   vertex_buffer(
      std::size_t                   vertex_capacity,
      std::size_t                   vertex_stride,
      gl::buffer_usage_hint::value  usage = gl::buffer_usage_hint::static_draw
   );
   ~vertex_buffer();

   void        bind                    ();
   std::size_t vertex_stride           () const { return m_buffer.stride(); }
   std::size_t vertex_capacity         () const { return m_buffer.capacity(); }
   std::size_t allocate_vertices       (std::size_t vertex_count);
   void        *map_vertices           (std::size_t first_vertex, std::size_t vertex_count, gl::buffer_access_mask::value access);
   void        unmap_vertices          ();
   void        flush_vertices          (std::size_t first_vertex, std::size_t vertex_count);
   void        flush_and_unmap_vertices(std::size_t vertex_count);
   std::size_t free_capacity_vertices  () const;

private:
   buffer  m_buffer;
};

} }

#endif
