#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <stdexcept>

namespace renderstack
{
namespace graphics
{

using namespace std;

Uniform_buffer_range::Uniform_buffer_range(Uniform_block *block, Buffer *uniform_buffer, size_t count)
    : m_uniform_buffer(uniform_buffer)
{
    assert(block != nullptr);
    assert(m_uniform_buffer != nullptr);
    m_byte_count = block->size_bytes() * count;
    m_first_byte = m_uniform_buffer->allocate(m_byte_count);
    m_in_edit    = false;
}

unsigned char *Uniform_buffer_range::begin_edit(Renderer &renderer)
{
    if (m_in_edit)
    {
        throw runtime_error("buffer was already in edit");
    }

    // We assume client is mapping
    if (!renderer.buffer_is_bound(Buffer::Target::uniform_buffer, m_uniform_buffer))
    {
        throw runtime_error("uniform buffer is not bound, cannot begin edit");
    }

    unsigned char *start = reinterpret_cast<unsigned char *>(
        m_uniform_buffer->map(renderer,
                              m_first_byte,
                              m_byte_count,
                              gl::buffer_access_mask::map_write_bit));
    m_in_edit = true;
    return start;
}

void Uniform_buffer_range::end_edit(Renderer &renderer)
{
    assert(m_uniform_buffer != nullptr);

    if (!m_in_edit)
    {
        throw runtime_error("buffer was not in edit");
    }

    m_uniform_buffer->unmap(renderer);
    m_in_edit = false;
}

void Uniform_buffer_range::flush(Renderer &renderer)
{
    assert(m_uniform_buffer != nullptr);
    m_uniform_buffer->flush(renderer, first_byte(), byte_count());
}

void Uniform_buffer_range::flush(Renderer &renderer, size_t bytes)
{
    assert(m_uniform_buffer != nullptr);
    m_uniform_buffer->flush(renderer, first_byte(), bytes);
}

} // namespace graphics
} // namespace renderstack
