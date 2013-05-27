#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include <cassert>
#include <stdexcept>

#define LOG_CATEGORY &log_graphics_index_buffer

namespace renderstack { namespace graphics {

using namespace std;
using namespace gl;


index_buffer::index_buffer(
   size_t                        index_capacity,
   size_t                        index_stride,
   gl::buffer_usage_hint::value  usage
)
:  m_buffer(buffer_target::element_array_buffer, index_capacity, index_stride, usage)
{
}
index_buffer::~index_buffer()
{
}
size_t index_buffer::allocate_indices(size_t index_count)
{
   return m_buffer.allocate(index_count);
}
void index_buffer::flush_indices(size_t first_index, size_t index_count)
{
   assert(context::current());
   assert(context::current()->mapped_index_buffer() == this);

   m_buffer.flush(first_index, index_count);
   context::current()->set_mapped_index_buffer(nullptr);
}
void *index_buffer::map_indices(size_t first_index, size_t index_count, gl::buffer_access_mask::value access)
{
   assert(context::current());
   assert(context::current()->mapped_index_buffer() == nullptr);

   context::current()->set_mapped_index_buffer(this);

   return m_buffer.map(first_index, index_count, access);
}
void index_buffer::flush_and_unmap_indices(size_t index_count)
{
   assert(context::current()->mapped_index_buffer() == this);

   m_buffer.flush_and_unmap(index_count);
   context::current()->set_mapped_index_buffer(nullptr);
}
void index_buffer::unmap_indices()
{
   assert(context::current());
   assert(context::current()->mapped_index_buffer() == this);

   m_buffer.unmap();

   context::current()->set_mapped_index_buffer(nullptr);
}
size_t index_buffer::free_capacity_indices() const
{
   return m_buffer.free_capacity();
}

} }
