#ifndef vertex_array_hpp_renderstack_graphics
#define vertex_array_hpp_renderstack_graphics

#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/log.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Buffer;
class index_buffer;

#define RS_ATTRIBUTE_COUNT 32 // TODO Get rid of this kind of defines?

struct Vertex_array_state
{
    struct Attrib
    {
        bool     enabled = false;
        uint32_t divisor = 0;
    };
    std::array<Attrib, RS_ATTRIBUTE_COUNT> attrib;
    Buffer                                 *element_array_buffer_binding{nullptr};
};

class Vertex_array
{
public:
    Vertex_array(bool default_ = false);

    ~Vertex_array();

    void use() const;

    void clear_enabled_attribs();

    void enable_attrib(uint32_t                              index,
                       uint32_t                              dimension,
                       gl::vertex_attrib_pointer_type::value shader_type,
                       gl::vertex_attrib_pointer_type::value data_type,
                       uint32_t                              stride,
                       uint32_t                              offset,
                       bool                                  normalized,
                       uint32_t                              divisor);

    void apply_attrib_enables();

    void trash()
    {
        for (unsigned int i = 0; i < RS_ATTRIBUTE_COUNT; ++i)
        {
            m_effective.attrib[i].enabled = false;
            m_effective.attrib[i].divisor = 0;
            gl::disable_vertex_attrib_array(i);
#if !defined(__APPLE__)
            gl::vertex_attrib_divisor(i, 0);
#endif
        }
        m_effective.element_array_buffer_binding = nullptr;
        gl::bind_buffer(gl::buffer_target::element_array_buffer, 0);
    }

    // returns previous index buffer
    Buffer *set_index_buffer(Buffer *buffer);

    Buffer *index_buffer() const;

    friend class Renderer;

private:
    unsigned int gl_name()
    {
        return m_gl_name;
    }

    void set_bound(bool value);

private:
    GLuint             m_gl_name{0U};
    Vertex_array_state m_requested;
    Vertex_array_state m_effective;
    bool               m_bound{false}; // Is currently bound?
};

} // namespace graphics
} // namespace renderstack

#endif
