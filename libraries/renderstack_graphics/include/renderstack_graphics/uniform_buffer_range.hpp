#ifndef uniform_buffer_range_hpp_renderstack_graphics
#define uniform_buffer_range_hpp_renderstack_graphics

#include "renderstack_graphics/buffer.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <memory>

namespace renderstack
{
namespace graphics
{

class uniform_block;
class uniform_buffer;

class uniform_buffer_range
{
public:
    uniform_buffer_range(
        std::shared_ptr<class uniform_block> block,
        std::shared_ptr<class buffer>        uniform_buffer,
        std::size_t                          count);
    ~uniform_buffer_range();

    unsigned char *begin_edit(class renderer &renderer);
    void           end_edit(class renderer &renderer);
    std::size_t    first_byte()
    {
        return m_first_byte;
    }
    std::size_t byte_count()
    {
        return m_byte_count;
    }
    void flush(class renderer &renderer);
    void flush(class renderer &renderer, std::size_t bytes);

    std::weak_ptr<class buffer> uniform_buffer()
    {
        return m_uniform_buffer;
    }

private:
    std::weak_ptr<class uniform_block> m_uniform_block;
    std::weak_ptr<class buffer>        m_uniform_buffer;
    std::size_t                        m_first_byte;
    std::size_t                        m_byte_count;
    bool                               m_in_edit;
};

} // namespace graphics
} // namespace renderstack

#endif
