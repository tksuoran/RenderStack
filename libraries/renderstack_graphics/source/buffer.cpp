#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/log.hpp"
#include <cstdint>
#include <cassert>
#include <cstring>
#include <stdexcept>

#define LOG_CATEGORY &log_buffer

namespace renderstack { namespace graphics {

using namespace renderstack::toolkit;
using namespace gl;
using namespace std;

#define GUARD_BYTES 32
#define DEBUG_BUFFER 1

namespace buffer_target
{

int count_in_context()
{
   // TODO This is a bit messed up
   if (configuration::use_vertex_array_object)
      return value::non_indexed_context_buffer_target_count;

   return value::all_buffer_target_count;
}

gl::buffer_target::value gl_buffer_target(value rs_target)
{
   switch (rs_target)
   {
   case array_buffer        : return gl::buffer_target::array_buffer          ;
   case element_array_buffer: return gl::buffer_target::element_array_buffer  ;
   case pixel_pack_buffer   : return gl::buffer_target::pixel_pack_buffer     ;
   case pixel_unpack_buffer : return gl::buffer_target::pixel_unpack_buffer   ;
   case copy_read_buffer    : return gl::buffer_target::copy_read_buffer      ;
   case copy_write_buffer   : return gl::buffer_target::copy_write_buffer     ;

   case uniform_buffer      :
      if (!configuration::can_use.uniform_buffer_object)
         throw runtime_error("uniform buffer not supported");
      return gl::buffer_target::uniform_buffer;

   case texture_buffer      :
      if (!configuration::can_use.texture_buffer_object)
         throw runtime_error("texture buffer not supported");
      return gl::buffer_target::texture_buffer;

   case draw_indirect_buffer:
      // TODO can sue
      return gl::buffer_target::draw_indirect_buffer;

   default:
      throw runtime_error("invalid buffer target");
   }
}

const char * const desc(value target)
{
   switch (target)
   {
   case array_buffer        : return "array_buffer";
   case element_array_buffer: return "element_array_buffer";
   case pixel_pack_buffer   : return "pixel_pack_buffer";
   case pixel_unpack_buffer : return "pixel_unpack_buffer";
   case copy_read_buffer    : return "copy_read_buffer";
   case copy_write_buffer   : return "copy_write_buffer";
   case uniform_buffer      : return "uniform_buffer";
   case texture_buffer      : return "texture_buffer";
   case draw_indirect_buffer: return "draw_indirect_buffer";

   default:
      throw runtime_error("invalid buffer target");
   }
}

}

buffer::buffer(
   buffer_target::value          target, 
   size_t                        capacity, 
   size_t                        stride,
   gl::buffer_usage_hint::value  usage
)
:  m_gl_name   (0)
,  m_target    (target)
,  m_stride    (stride)
,  m_capacity  (capacity)
,  m_next_free (0)
,  m_usage     (usage)
{
   log_trace(
      "buffer::buffer(target = %s, capacity = %u, stride = %u, usage = %s) name = %u",
      buffer_target::desc(target),
      static_cast<unsigned int>(capacity),
      static_cast<unsigned int>(stride),
      gl::enum_string(usage),
      m_gl_name
   );
}

void buffer::allocate_storage(class renderer &renderer)
{
   shared_ptr<class buffer> old;

   gl::gen_buffers(1, &m_gl_name);

   if (m_target == buffer_target::element_array_buffer)
   {
      auto va = renderer.vertex_array();
      assert(va);
      old = va->set_index_buffer(shared_from_this());
   }
   else
   {
      old = renderer.set_buffer(m_target, shared_from_this());
   }

   log_trace(
      "buffer::allocate_storage() target = %s, name = %u",
      buffer_target::desc(m_target),
      m_gl_name
   );

   buffer_data(
      buffer_target::gl_buffer_target(m_target),
      static_cast<GLintptr>(m_capacity * m_stride),
      nullptr,
      m_usage
   );

   if (!configuration::can_use.map_buffer_range)
   {
      size_t new_size = GUARD_BYTES + (m_capacity * m_stride) + GUARD_BYTES;

      try
      {
         m_data_copy.resize(new_size);
      }
      catch (...)
      {
         // Most likely we run out of memory :(
         throw runtime_error("memory error in buffer::buffer()");
      }

      ::memset(&m_data_copy[0], 0xcd, new_size);
   }

   if (renderstack::graphics::configuration::use_vertex_array_object)
   {
      if (m_target == buffer_target::element_array_buffer)
      {
         auto va = renderer.vertex_array();
         (void)va->set_index_buffer(old);
      }
      else
      {
         (void)renderer.set_buffer(m_target, old);
      }
   }
}

void buffer::set_debug_label(string const &value)
{
   m_debug_label = value;
}

string const &buffer::debug_label() const
{
   return m_debug_label;
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
   delete_buffers(1, &m_gl_name);

   validate();
}

size_t buffer::allocate(size_t count)
{
   size_t first = m_next_free;
   m_next_free += count;
   if (m_next_free > m_capacity)
      throw runtime_error("out of buffer capacity");

   return first;
}

void *buffer::map(class renderer &renderer, size_t first, size_t count, gl::buffer_access_mask::value access)
{
   (void)renderer; // avoid unused warnings in release builds
   assert(count > 0);

   slog_trace("buffer::map(target = %s first = %u count = %u access = 0x%x) name = %u",
      buffer_target::desc(m_target),
      static_cast<unsigned int>(first),
      static_cast<unsigned int>(count),
      access,
      m_gl_name);

   if (!m_gl_name)
      throw runtime_error("storage not allocated");

   // Note, this is in multiples of stride
   if (first + count > m_capacity)
      throw runtime_error("first + count > m_capacity");

   assert(m_gl_name);
   assert(renderer.map_buffer(m_target, shared_from_this()));

   m_mapped_offset = static_cast<GLsizeiptr>(first * m_stride);
   m_mapped_size   = static_cast<GLsizeiptr>(count * m_stride);
   m_mapped_access = access;

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   if (configuration::can_use.map_buffer_range)
   {
      m_mapped_ptr = gl::map_buffer_range(
         buffer_target::gl_buffer_target(m_target),
         m_mapped_offset,
         m_mapped_size,
         access
      );
      assert(m_mapped_ptr);
   } 
   else 
#endif
   {
      // Access data copy
      uint8_t *start = &m_data_copy[GUARD_BYTES];
      uint8_t *ptr   = &start[m_mapped_offset];
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

void buffer::unmap(class renderer &renderer)
{
   (void)renderer; // avoid unused warnings in release builds
   slog_trace(
      "buffer::unmap(target = %s offset = %u size = %u ptr = %p) name = %u",
      buffer_target::desc(m_target),
      static_cast<unsigned int>(m_mapped_offset),
      static_cast<unsigned int>(m_mapped_size),
      m_mapped_ptr,
      m_gl_name);
   set_text_color(C_GREY);

   assert(m_gl_name);
   assert(renderer.unmap_buffer(m_target, shared_from_this()));

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   if (configuration::can_use.map_buffer_range)
   {
      GLboolean res = gl::unmap_buffer(buffer_target::gl_buffer_target(m_target));
      (void)res;
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
            buffer_target::gl_buffer_target(m_target),
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

void buffer::flush(class renderer &renderer, size_t first, size_t count)
{
   // unmap will do flush
   size_t offset  = first * m_stride;
   size_t size    = count * m_stride;

   bool flush_explicit = (m_mapped_access & gl::buffer_access_mask::map_flush_explicit_bit) == gl::buffer_access_mask::map_flush_explicit_bit;
   assert(flush_explicit);

   if (first + count > m_capacity)
      throw runtime_error("first + count > m_capacity");

   assert(m_gl_name);
   assert(renderer.buffer_is_mapped(m_target, shared_from_this()));
   (void)renderer;

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   if (configuration::can_use.map_buffer_range)
   {
      slog_trace(
         "buffer::flush(target = %s first = %u count = %u offset = %u size = %u) m_mapped_ptr = %p name = %u",
         buffer_target::desc(m_target),
         static_cast<unsigned int>(first),
         static_cast<unsigned int>(count),
         static_cast<unsigned int>(offset),
         static_cast<unsigned int>(size),
         m_mapped_ptr,
         m_gl_name);
                        
      gl::flush_mapped_buffer_range(
         buffer_target::gl_buffer_target(m_target),
         static_cast<GLintptr>(offset),
         static_cast<GLsizeiptr>(size)
      );
   }
   else
#endif
   { 
      uint8_t *start = &m_data_copy[GUARD_BYTES];
      uint8_t *ptr   = &start[m_mapped_offset + offset];

      slog_trace("buffer::flush(target = %s first = %u count = %u offset = %u size = %u ptr = %p) name = %u",
         buffer_target::desc(m_target),
         static_cast<unsigned int>(first),
         static_cast<unsigned int>(count),
         static_cast<unsigned int>(offset),
         static_cast<unsigned int>(size),
         ptr,
         m_gl_name);

      validate();

      if (size > m_mapped_size)
         throw runtime_error("size > m_mapped_size");

      gl::buffer_sub_data(
         buffer_target::gl_buffer_target(m_target),
         m_mapped_offset + offset,
         size,
         reinterpret_cast<const GLvoid*>(ptr)
      );
   }
}
#if defined(RENDERSTACK_GL_API_OPENGL)
void buffer::dump() const
{
   bool     allocated   = false;
   bool     unmap       = false;
   size_t   count       = ((m_capacity * m_stride) / 4);
   GLint    mapped      = 0;
   uint32_t *data       = nullptr;

   gl::get_buffer_parameter_iv(buffer_target::gl_buffer_target(m_target), GL_BUFFER_MAPPED, &mapped);

   if (mapped == GL_FALSE)
   {
      data = reinterpret_cast<uint32_t *>(
         gl::map_buffer_range(
            buffer_target::gl_buffer_target(m_target),
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

      gl::get_buffer_sub_data(buffer_target::gl_buffer_target(m_target), 0, count * 4, data);

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
      gl::unmap_buffer(buffer_target::gl_buffer_target(m_target));

   if (allocated)
      delete[] data;
}
#endif
void buffer::flush_and_unmap(class renderer &renderer, size_t count)
{
   bool flush_explicit = (m_mapped_access & gl::buffer_access_mask::map_flush_explicit_bit) == gl::buffer_access_mask::map_flush_explicit_bit;

   assert(m_gl_name);
   assert(renderer.buffer_is_mapped(m_target, shared_from_this()));

#if defined(_DEBUG)
   validate();
#endif

   slog_trace("flush_and_unmap(count = %u) name = %u",
      static_cast<unsigned int>(count), m_gl_name);

   //  If explicit is not selected, unmap will do full flush
   //  so we do manual flush only if explicit is selected
   if (flush_explicit)
      flush(renderer, 0, count);

   unmap(renderer);
}

size_t buffer::free_capacity() const
{
   return m_capacity - m_next_free;
}

size_t buffer::stride() const
{
   return m_stride;
}
size_t buffer::capacity() const
{
   return m_capacity;
}

void buffer::bind_range(unsigned int binding_point, size_t first, size_t size)
{
   gl::bind_buffer_range(
      buffer_target::gl_buffer_target(m_target),
      binding_point,
      m_gl_name,
      first * m_stride,
      size * m_stride
   );
}
   
} }
