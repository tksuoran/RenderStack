#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/Vertex_stream.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <stdexcept>

#define LOG_CATEGORY &log_graphics_vertex_buffer

namespace renderstack
{
namespace graphics
{

using namespace std;
using namespace gl;

vertex_buffer::vertex_buffer(
    size_t                       vertex_capacity,
    size_t                       vertex_stride,
    gl::buffer_usage_hint::value usage)
    : m_buffer(Buffer::Target::array_buffer, vertex_capacity, vertex_stride, usage)
{
}

vertex_buffer::~vertex_buffer()
{
}

size_t vertex_buffer::allocate_vertices(size_t vertex_count)
{
    return m_buffer.allocate(vertex_count);
}

void *vertex_buffer::map_vertices(size_t first_vertex, size_t vertex_count, gl::buffer_access_mask::value access)
{
    assert(context::current());
    assert(context::current()->mapped_vertex_buffer() == nullptr);

    context::current()->set_mapped_vertex_buffer(this);

    return m_buffer.map(first_vertex, vertex_count, access);
}

void vertex_buffer::unmap_vertices()
{
    assert(context::current());
    assert(context::current()->mapped_vertex_buffer() == this);

    context::current()->set_mapped_vertex_buffer(nullptr);
    m_buffer.unmap();
}

void vertex_buffer::flush_vertices(size_t first_vertex, size_t vertex_count)
{
    assert(context::current());
    assert(context::current()->mapped_vertex_buffer() == this);

    m_buffer.flush(first_vertex, vertex_count);
}

void vertex_buffer::flush_and_unmap_vertices(size_t vertex_count)
{
    assert(context::current());
    assert(context::current()->mapped_vertex_buffer() == this);

    context::current()->set_mapped_vertex_buffer(nullptr);
    m_buffer.flush_and_unmap(vertex_count);
}

size_t vertex_buffer::free_capacity_vertices() const
{
    return m_buffer.free_capacity();
}

} // namespace graphics
} // namespace renderstack
