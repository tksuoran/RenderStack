#ifndef renderstack_graphics__index_buffer_hpp
#define renderstack_graphics__index_buffer_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/buffer.hpp"
#include <map>
#include <deque>
#include <cstddef>
#include <memory>

namespace renderstack { namespace graphics {

class index_buffer
{
public:
   index_buffer(
      std::size_t                   index_capacity,
      std::size_t                   index_stride,
      gl::buffer_usage_hint::value  usage = gl::buffer_usage_hint::static_draw
   );
   ~index_buffer();

   void        bind                    ();
   std::size_t index_stride            () const { return m_buffer.stride(); }
   std::size_t index_capacity          () const { return m_buffer.capacity(); }
   std::size_t allocate_indices        (std::size_t index_count);
   void        flush_indices           (std::size_t first_index, std::size_t index_count);
   void        *map_indices            (std::size_t first_index, std::size_t index_count, gl::buffer_access_mask::value access);
   void        flush_and_unmap_indices (std::size_t index_count);
   void        unmap_indices           ();
   std::size_t free_capacity_indices   () const;

private:
   buffer m_buffer;
};

} }

#endif
