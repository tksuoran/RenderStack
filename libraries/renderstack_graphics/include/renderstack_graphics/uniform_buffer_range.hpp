#ifndef uniform_buffer_range_hpp_renderstack_graphics
#define uniform_buffer_range_hpp_renderstack_graphics

#include "renderstack_graphics/buffer.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <memory>

namespace renderstack
{
namespace graphics
{

class Uniform_block;
class Uniform_buffer;

class Uniform_buffer_range
{
public:
    Uniform_buffer_range(Uniform_block *block, Buffer *uniform_buffer, size_t count);

    ~Uniform_buffer_range() = default;

    unsigned char *begin_edit(Renderer &renderer);

    void end_edit(Renderer &renderer);

    size_t first_byte() const
    {
        return m_first_byte;
    }

    size_t byte_count() const
    {
        return m_byte_count;
    }

    void flush(Renderer &renderer);

    void flush(Renderer &renderer, size_t bytes);

    Buffer *uniform_buffer()
    {
        return m_uniform_buffer;
    }

private:
    Uniform_block *m_uniform_block{nullptr};
    Buffer        *m_uniform_buffer{nullptr};
    size_t        m_first_byte{0U};
    size_t        m_byte_count{0U};
    bool          m_in_edit{false};
};

} // namespace graphics
} // namespace renderstack

#endif
