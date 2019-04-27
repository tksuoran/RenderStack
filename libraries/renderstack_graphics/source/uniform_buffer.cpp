#include "renderstack_graphics/uniform_buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <stdexcept>

#define LOG_CATEGORY &log_ubo_pool

namespace renderstack
{
namespace graphics
{

using namespace std;
using namespace gl;

uniform_buffer::uniform_buffer(size_t byte_capacity, gl::buffer_usage_hint::value usage)
    : m_buffer(Buffer::Target::uniform_buffer, byte_capacity, 1, usage)
{
}

uniform_buffer::~uniform_buffer()
{
}

size_t uniform_buffer::allocate_bytes(size_t byte_count)
{
    return m_buffer.allocate(byte_count);
}

void uniform_buffer::flush_bytes(size_t first_byte, size_t byte_count)
{
    assert(context::current());
    assert(context::current()->mapped_uniform_buffer() == this);

    if (context::current()->mapped_uniform_buffer() != this)
    {
        throw runtime_error("ubo mapped to something else");
    }

    m_buffer.flush(first_byte, byte_count);
}

void *uniform_buffer::map_bytes(size_t first_byte, size_t byte_count, gl::buffer_access_mask::value access)
{
    assert(context::current());
    assert(context::current()->mapped_uniform_buffer() == nullptr);

    if (context::current()->mapped_uniform_buffer())
    {
        throw runtime_error("ubo already mapped");
    }

    context::current()->set_mapped_uniform_buffer(this);
    return m_buffer.map(first_byte, byte_count, access);
}

void uniform_buffer::flush_and_unmap_bytes(size_t byte_count)
{
    assert(context::current());
    assert(context::current()->mapped_uniform_buffer() == this);

    if (context::current()->mapped_uniform_buffer() != this)
    {
        throw runtime_error("ubo mapped to something else");
    }

    context::current()->set_mapped_uniform_buffer(nullptr);
    m_buffer.flush_and_unmap(byte_count);
}

void uniform_buffer::unmap_bytes()
{
    assert(context::current());
    assert(context::current()->mapped_uniform_buffer() == this);

    if (context::current()->mapped_uniform_buffer() != this)
    {
        throw runtime_error("ubo mapped to something else");
    }

    context::current()->set_mapped_uniform_buffer(nullptr);
    m_buffer.unmap();
}

size_t uniform_buffer::free_capacity_bytes() const
{
    return m_buffer.free_capacity();
}

} // namespace graphics
} // namespace renderstack
