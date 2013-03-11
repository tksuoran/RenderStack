#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include <cstdint>
#include <cassert>
#include <cstring>
#include <stdexcept>

#define LOG_CATEGORY &graphics_buffer

namespace renderstack { namespace graphics {

using namespace gl;
using namespace std;

#define GUARD_BYTES 32
#define DEBUG_BUFFER 1

buffer::buffer(
   gl::buffer_target::value      target, 
   size_t                        capacity, 
   size_t                        stride,
   gl::buffer_usage_hint::value  usage
)
:  m_target       (target)
,  m_stride       (stride)
,  m_capacity     (capacity)
,  m_next_free    (0)
,  m_buffer_object(0)
{
   //  vertices
   gl::gen_buffers(1, &m_buffer_object);
   bind_buffer(m_target, m_buffer_object);
   buffer_data(m_target, static_cast<GLintptr>(capacity * stride), nullptr, usage);

   if (!configuration::can_use.map_buffer_range)
   {
      size_t new_size = GUARD_BYTES + (capacity * stride) + GUARD_BYTES;
      try
      {
         m_data_copy.resize(new_size);
      }
      catch(...)
      {
         // Most likely we run out of memory :(
         throw runtime_error("memory error in buffer::buffer()");
      }
      ::memset(&m_data_copy[0], 0xcd, new_size);
   }

   log_trace(
      "buffer::buffer(target = %s, capacity = %u, stride = %u, usage = %s)",
      gl::enum_string(target),
      static_cast<unsigned int>(capacity),
      static_cast<unsigned int>(stride),
      gl::enum_string(usage)
   );
}

void buffer::validate()
{
   if (!configuration::can_use.map_buffer_range)
   {
      for (int i = 0; i < GUARD_BYTES; ++i)
      {
         if (m_data_copy[i] != 0xcd)
            throw runtime_error("buffer memory corruption");

         if (m_data_copy[(m_capacity * m_stride) + GUARD_BYTES + i] != 0xcd)
            throw runtime_error("buffer memory corruption");
      }
   }
}

buffer::~buffer()
{
   delete_buffers(1, &m_buffer_object);

   validate();
}

void buffer::bind()
{
   // GLint vao = 0;
   // gl::get_integer_v(GL_VERTEX_ARRAY_BINDING, &vao);

   bind_buffer(m_target, m_buffer_object);
}

size_t buffer::allocate(size_t count)
{
   size_t first = m_next_free;
   m_next_free += count;
   if (m_next_free > m_capacity)
      throw runtime_error("out of buffer capacity");

   return first;
}

void *buffer::map(size_t first, size_t count, gl::buffer_access_mask::value access)
{
   slog_trace("buffer::map(target = %s first = %u count = %u access = 0x%x)",
      gl::enum_string(m_target),
      static_cast<unsigned int>(first),
      static_cast<unsigned int>(count),
      access);

   // Note, this is in multiples of stride
   if (first + count > m_capacity)
      throw runtime_error("first + count > m_capacity");

   m_mapped_offset = static_cast<GLsizeiptr>(first * m_stride);
   m_mapped_size   = static_cast<GLsizeiptr>(count * m_stride);
   m_mapped_access = access;

#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (configuration::can_use.map_buffer_range)
   {
      m_mapped_ptr = gl::map_buffer_range(
         m_target,
         m_mapped_offset,
         m_mapped_size,
         access
      );
   } 
   else 
#endif
   {
      // Access data copy
      uint8_t *start = &m_data_copy[GUARD_BYTES];
     uint8_t *ptr    = &start[m_mapped_offset];
      m_mapped_ptr   = reinterpret_cast<void*>(ptr);

#if defined(_DEBUG)
      validate();

# if 0
      bool invalidate_range   = (access & gl::buffer_access_mask::map_invalidate_range_bit ) == gl::buffer_access_mask::map_invalidate_range_bit;
      bool invalidate_buffer  = (access & gl::buffer_access_mask::map_invalidate_buffer_bit) == gl::buffer_access_mask::map_invalidate_buffer_bit;

      if (invalidate_range)
         ::memset(ptr, 0xff, count * m_stride);
      if (invalidate_buffer)
         ::memset(start, 0xff, m_capacity * m_stride);
# endif
#endif
   }

   log_trace(":m_mapped_offset = %u m_mapped_size = %u m_mapped_ptr = %p)", 
      static_cast<unsigned int>(m_mapped_offset),
      static_cast<unsigned int>(m_mapped_size),
      m_mapped_ptr);

   return m_mapped_ptr;
}

void buffer::unmap()
{
   slog_trace(
      "buffer::unmap(target = %s offset = %u size = %u ptr = %p)",
      gl::enum_string(m_target),
      static_cast<unsigned int>(m_mapped_offset),
      static_cast<unsigned int>(m_mapped_size),
      m_mapped_ptr);
   set_text_color(C_GREY);

#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (configuration::can_use.map_buffer_range)
   {
      GLboolean res = gl::unmap_buffer(m_target);
      assert(res == GL_TRUE);
   }
   else
#endif
   {
      bool flush_explicit = (m_mapped_access & gl::buffer_access_mask::map_flush_explicit_bit) == gl::buffer_access_mask::map_flush_explicit_bit;

#if defined(_DEBUG)
      validate();
#endif

      // If flush explicit is not selected, unmap will flush full mapped range
      if (!flush_explicit)
      {
         uint8_t *start = &m_data_copy[GUARD_BYTES];
         uint8_t *ptr   = &start[m_mapped_offset];

         if (m_mapped_ptr != reinterpret_cast<void*>(ptr))
            throw runtime_error("m_mapped_ptr != reinterpret_cast<void*>(ptr)");

         gl::buffer_sub_data(
            m_target,
            m_mapped_offset,
            m_mapped_size,
            m_mapped_ptr
         );
      }
   }
   m_mapped_offset = size_t(~0);
   m_mapped_size   = 0;
   m_mapped_access = gl::buffer_access_mask::value(0);
}

void buffer::flush(size_t first, size_t count)
{
   // unmap will do flush
   size_t offset  = first * m_stride;
   size_t size    = count * m_stride;

#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (configuration::can_use.map_buffer_range)
   {
      slog_trace(
         "buffer::flush(target = %s first = %u count = %u offset = %u size = %u) m_mapped_ptr = %p",
         gl::enum_string(m_target),
         static_cast<unsigned int>(first),
         static_cast<unsigned int>(count),
         static_cast<unsigned int>(offset),
         static_cast<unsigned int>(size),
         m_mapped_ptr);
                        
      gl::flush_mapped_buffer_range(
         m_target,
         static_cast<GLintptr>(offset),
         static_cast<GLsizeiptr>(size)
      );
   }
   else
#endif
   { 
      uint8_t *start = &m_data_copy[GUARD_BYTES];
      uint8_t *ptr   = &start[m_mapped_offset + offset];

      slog_trace("buffer::flush(target = %s first = %u count = %u offset = %u size = %u ptr = %p)",
         gl::enum_string(m_target),
         static_cast<unsigned int>(first),
         static_cast<unsigned int>(count),
         static_cast<unsigned int>(offset),
         static_cast<unsigned int>(size),
         ptr);

      validate();

      if (size > m_mapped_size)
         throw runtime_error("size > m_mapped_size");

      gl::buffer_sub_data(
         m_target,
         m_mapped_offset + offset,
         size,
         reinterpret_cast<const GLvoid*>(ptr)
      );
   }
}

void buffer::dump() const
{
   bool     allocated   = false;
   bool     unmap       = false;
   size_t   count       = ((m_capacity * m_stride) / 4);
   GLint    mapped      = 0;
   uint32_t *data       = nullptr;

   gl::get_buffer_parameter_iv(m_target, GL_BUFFER_MAPPED, &mapped);

   if (mapped == GL_FALSE)
   {
      uint32_t *data = reinterpret_cast<uint32_t *>(
         gl::map_buffer_range(
            m_target,
            0,
            m_capacity * m_stride,
            gl::buffer_access_mask::map_read_bit
         )
      );
      unmap = data != nullptr;
   }

   if (data == nullptr)
   {
      // This happens if we already had buffer mapped
      data = new uint32_t[count + 1];

      gl::get_buffer_sub_data(m_target, 0, count * 4, data);

      allocated = true;
   }

   for (size_t i = 0; i < count; ++i)
   {
      if (i % 16 == 0)
         printf("%08x: ", static_cast<unsigned int>(i));

      printf("%08x ", data[i]);

      if (i % 16 == 15)
         printf("\n");
   }
   printf("\n");

   if (unmap)
   {
      gl::unmap_buffer(m_target);
   }

   if (allocated)
   {
      delete[] data;
   }
}

void buffer::flush_and_unmap(size_t count)
{
   bool flush_explicit = (m_mapped_access & gl::buffer_access_mask::map_flush_explicit_bit) == gl::buffer_access_mask::map_flush_explicit_bit;

#if defined(_DEBUG)
   validate();
#endif

   slog_trace("flush_and_unmap(count = %u)",
      static_cast<unsigned int>(count));

   //  If explicit is not selected, unmap will do full flush
   //  so we do manual flush only if explicit is selected
   if (flush_explicit)
      flush(0, count);

   unmap();
}

size_t buffer::free_capacity() const
{
   return m_capacity - m_next_free;
}

void buffer::bind_range(unsigned int binding_point, size_t offset, size_t size)
{
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (configuration::can_use.bind_buffer_range)
   {
      gl::bind_buffer_range(
         m_target, 
         binding_point, 
         m_buffer_object, 
         static_cast<GLsizeiptr>(offset), 
         static_cast<GLsizeiptr>(size)
      );
   }
   else
#endif
   {
      (void)binding_point;
      (void)offset;
      (void)size;
      throw runtime_error("bind_range() not supported");
   }
}

std::size_t buffer::stride() const
{
   return m_stride;
}
std::size_t buffer::capacity() const
{
   return m_capacity;
}

} }