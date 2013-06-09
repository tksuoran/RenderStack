#ifndef renderstack_graphics__buffer_hpp
#define renderstack_graphics__buffer_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_graphics/configuration.hpp"
#include <cstddef>
#include <map>
#include <deque>
#include <vector>
#include <memory>

namespace renderstack { namespace graphics {

namespace buffer_target
{
   enum value
   {
      array_buffer            = 0,  // 6.3   (bound VBO)
      pixel_pack_buffer       = 1,  // 6.16
      pixel_unpack_buffer     = 2,  // 6.16
      uniform_buffer          = 3,  // 6.25
      texture_buffer          = 4,  // TODO
      copy_read_buffer        = 5,  // 6.36
      copy_write_buffer       = 6,  // 6.36
      draw_indirect_buffer    = 7,  // TODO
      non_indexed_context_buffer_target_count = 8,
      element_array_buffer    = 8,
      all_buffer_target_count = 9,
   };

   gl::buffer_target::value gl_buffer_target(value rs_target);

   const char * const desc(value target);
}

class buffer : public ::std::enable_shared_from_this<buffer>
{
public:
   buffer(
      buffer_target::value          target,
      ::std::size_t                 capacity,
      ::std::size_t                 stride,
      gl::buffer_usage_hint::value  usage = gl::buffer_usage_hint::static_draw
   );
   ~buffer();

   void allocate_storage(class renderer &renderer);
   void set_debug_label(std::string const &value);
   std::string const &debug_label() const;

   ::std::size_t  stride         () const;
   ::std::size_t  capacity       () const;
   ::std::size_t  allocate       (::std::size_t count);
   void           *map           (class renderer &renderer, ::std::size_t first, ::std::size_t count, gl::buffer_access_mask::value access);
   void           unmap          (class renderer &renderer);
   void           flush          (class renderer &renderer, ::std::size_t first, ::std::size_t count);
   void           flush_and_unmap(class renderer &renderer, ::std::size_t count);
   ::std::size_t  free_capacity  () const;
   void           bind_range     (unsigned int binding_point, ::std::size_t offset, ::std::size_t size);
   buffer_target::value target() const { return m_target; }
#if defined(RENDERSTACK_GL_API_OPENGL)
   void           dump           () const;
#endif

friend class renderer;
friend class buffer;
friend class vertex_array;
friend class texture;
private:
   unsigned int                  gl_name() const { return m_gl_name; }

private:
   unsigned int                  m_gl_name;
   std::string                   m_debug_label;
   buffer_target::value          m_target;
   ::std::size_t                 m_stride;
   ::std::size_t                 m_capacity;
   ::std::size_t                 m_next_free;
   gl::buffer_usage_hint::value  m_usage;

   // Storage if not using MapBuffer
   ::std::vector<unsigned char>  m_data_copy;

   // Last MapBuffer
   void                          *m_mapped_ptr;
   ::std::size_t                 m_mapped_offset;
   ::std::size_t                 m_mapped_size;
   gl::buffer_access_mask::value m_mapped_access;

   void validate();
};

} }

#endif
