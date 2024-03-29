#include "renderstack_graphics/vertex_array.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/log.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <algorithm>
#include <cassert>
#include <stdexcept>

#define LOG_CATEGORY &log_vertex_stream

namespace renderstack
{
namespace graphics
{

using namespace std;

Vertex_array::Vertex_array(bool default_)
{
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (configuration::can_use.vertex_array_object && !default_)
    {
        gl::gen_vertex_arrays(1, &m_gl_name);
    }
#endif

    int count = std::min(RS_ATTRIBUTE_COUNT, configuration::max_vertex_attribs);

    for (int i = 0; i < count; ++i)
    {
        m_requested.attrib[i].enabled = false;
        m_effective.attrib[i].enabled = false;
    }
}
Vertex_array::~Vertex_array()
{
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (configuration::can_use.vertex_array_object &&
        (m_gl_name != 0))
    {
        gl::delete_vertex_arrays(1, &m_gl_name);
    }
#endif
}

void Vertex_array::set_bound(bool value)
{
    //log_warn("set_bound(value = %s) gl_name = %d", (value ? "true" : "false"), m_gl_name);
    m_bound = value;
}

void Vertex_array::clear_enabled_attribs()
{
    int count = std::min(RS_ATTRIBUTE_COUNT, configuration::max_vertex_attribs);

    for (int i = 0; i < count; ++i)
    {
        m_requested.attrib[i].enabled = false;
        m_requested.attrib[i].divisor = 0;
    }
}
void Vertex_array::apply_attrib_enables()
{
    int count = std::min(RS_ATTRIBUTE_COUNT, configuration::max_vertex_attribs);

    for (int i = 0; i < count; ++i)
    {
        if (m_effective.attrib[i].enabled != m_requested.attrib[i].enabled)
        {
            if (m_requested.attrib[i].enabled)
            {
                gl::enable_vertex_attrib_array(i);
            }
            else
            {
                gl::disable_vertex_attrib_array(i);
            }

            m_effective.attrib[i].enabled = m_requested.attrib[i].enabled;
        }
        if (m_effective.attrib[i].divisor != m_requested.attrib[i].divisor)
        {
            gl::vertex_attrib_divisor(i, m_requested.attrib[i].divisor);
            m_effective.attrib[i].divisor = m_requested.attrib[i].divisor;
        }
    }
}

Buffer *Vertex_array::set_index_buffer(Buffer *buffer)
{
    Buffer *old = m_effective.element_array_buffer_binding;

    if (!m_bound)
    {
        throw runtime_error("vertex array is not currently bound, cannot bind index buffer");
    }

    m_requested.element_array_buffer_binding = buffer;
    if (m_effective.element_array_buffer_binding != m_requested.element_array_buffer_binding)
    {
        if (m_requested.element_array_buffer_binding)
        {
            gl::bind_buffer(gl::buffer_target::element_array_buffer,
                            buffer->gl_name());
        }

        m_effective.element_array_buffer_binding = m_requested.element_array_buffer_binding;
    }

    return old;
}

Buffer *Vertex_array::index_buffer() const
{
    return m_effective.element_array_buffer_binding;
}

void Vertex_array::enable_attrib(
    uint32_t                              index,
    uint32_t                              dimension,
    gl::vertex_attrib_pointer_type::value shader_type,
    gl::vertex_attrib_pointer_type::value data_type,
    uint32_t                              stride,
    uint32_t                              offset,
    bool                                  normalized,
    unsigned int                          divisor)
{
    unsigned int count = std::min(RS_ATTRIBUTE_COUNT, configuration::max_vertex_attribs);

    if (index >= count)
    {
        throw runtime_error("vertex attrib index out of supported range");
    }

    m_requested.attrib[index].enabled = true;
    m_requested.attrib[index].divisor = divisor;

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    switch (shader_type)
    {
        case gl::vertex_attrib_pointer_type::byte:
        case gl::vertex_attrib_pointer_type::unsigned_byte:
        case gl::vertex_attrib_pointer_type::short_:
        case gl::vertex_attrib_pointer_type::unsigned_short:
        case gl::vertex_attrib_pointer_type::int_:
        case gl::vertex_attrib_pointer_type::unsigned_int:
            gl::vertex_attrib_i_pointer(
                index,
                static_cast<GLint>(dimension),
                data_type,
                static_cast<GLsizei>(stride),
                reinterpret_cast<char *>(offset));
            break;

        case gl::vertex_attrib_pointer_type::float_:
        case gl::vertex_attrib_pointer_type::double_:
        case gl::vertex_attrib_pointer_type::half_float:
        case gl::vertex_attrib_pointer_type::fixed:
        case gl::vertex_attrib_pointer_type::unsigned_int_2101010_rev:
        case gl::vertex_attrib_pointer_type::int_2101010_rev:
            gl::vertex_attrib_pointer(
                index,
                static_cast<GLint>(dimension),
                data_type,
                normalized,
                static_cast<GLsizei>(stride),
                reinterpret_cast<char *>(offset));
            break;

        default:
            throw runtime_error("Bad vertex attrib pointer type");
            break;
    }
#else
    gl::vertex_attrib_pointer(
        mapping->dst_index(),
        static_cast<GLint>(attribute->dimension()),
        attribute->data_type(),
        attribute->normalized(),
        static_cast<GLsizei>(binding.stride()),
        reinterpret_cast<char *>(base_vertex * binding.stride() + attribute->offset()));
#endif
}

} // namespace graphics
} // namespace renderstack
