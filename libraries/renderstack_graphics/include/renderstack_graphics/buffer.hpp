#ifndef renderstack_graphics__buffer_hpp
#define renderstack_graphics__buffer_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/gl.hpp"
#include <cstddef>
#include <map>
#include <deque>
#include <vector>

namespace renderstack { namespace graphics {

class buffer
{
public:
   buffer(
      gl::buffer_target::value      target,
      std::size_t                   capacity,
      std::size_t                   stride,
      gl::buffer_usage_hint::value  usage = gl::buffer_usage_hint::static_draw
   );
   ~buffer();

   void        bind           ();
   std::size_t stride         () const;
   std::size_t capacity       () const;
   std::size_t allocate       (std::size_t count);
   void        *map           (std::size_t first, std::size_t count, gl::buffer_access_mask::value access);
   void        unmap          ();
   void        flush          (std::size_t first, std::size_t count);
   void        flush_and_unmap(std::size_t count);
   std::size_t free_capacity  () const;
   void        bind_range     (unsigned int binding_point, std::size_t offset, std::size_t size);
   void        dump           () const;

private:
   gl::buffer_target::value   m_target;
   std::size_t                m_stride;
   std::size_t                m_capacity;
   std::size_t                m_next_free;
   unsigned int               m_buffer_object;

   // Storage if not using MapBuffer
   std::vector<unsigned char>    m_data_copy;

   // Last MapBuffer
   void                          *m_mapped_ptr;
   std::size_t                   m_mapped_offset;
   std::size_t                   m_mapped_size;
   gl::buffer_access_mask::value m_mapped_access;

   void validate();
};

} }

#endif
