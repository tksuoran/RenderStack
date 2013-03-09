#ifndef renderstack_graphics__uniform_buffer_hpp
#define renderstack_graphics__uniform_buffer_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/buffer.hpp"
#include <cstddef>
#include <map>
#include <deque>

namespace renderstack { namespace graphics {


class uniform_buffer
{
public:
   uniform_buffer(size_t byte_capacity, gl::buffer_usage_hint::value usage = gl::buffer_usage_hint::static_draw);
   ~uniform_buffer();

   void        bind                 ();
   void        bind_range           (unsigned int binding_point, std::size_t offset, std::size_t size);
   std::size_t byte_capacity        () const { return m_buffer.capacity(); }
   std::size_t allocate_bytes       (std::size_t byte_count);
   void        flush_bytes          (std::size_t first_byte, std::size_t byte_count);
   void        *map_bytes           (std::size_t first_byte, std::size_t byte_count, gl::buffer_access_mask::value access);
   void        flush_and_unmap_bytes(std::size_t byte_count);
   void        unmap_bytes          ();
   std::size_t free_capacity_bytes  () const;

private:
   buffer m_buffer;
};

} }

#endif
