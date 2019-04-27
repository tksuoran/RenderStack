#include "renderstack_ui/text_buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/log.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <vector>

#define LOG_CATEGORY &log_text_buffer

namespace renderstack
{
namespace ui
{

using namespace renderstack::graphics;
using namespace gl;
using namespace std;

Text_buffer::Text_buffer(Gui_renderer &renderer, Font &font, unsigned int max_chars)
    : m_renderer(renderer)
    , m_font(font)
    , m_max_chars(max_chars)
{
    if (m_mesh.index_count() > numeric_limits<unsigned int>::max())
    {
        throw runtime_error("font::prepare_gl_resources: no code path for index types other than unsigned int");
    }

    // TODO Support colored text
    m_mesh.allocate_vertex_buffer(m_renderer.vertex_buffer(), 4 * m_max_chars);
    m_mesh.allocate_index_buffer(m_renderer.index_buffer(), 6 * m_max_chars);

    log_trace("preparing index buffer\n");

    auto &r = m_renderer.renderer();

    m_renderer.set_index_buffer();
    unsigned short *start = static_cast<unsigned short *>(
        m_mesh.index_buffer()->map(
            r,
            m_mesh.first_index(),
            m_mesh.index_count(),
            (gl::buffer_access_mask::value)(
                gl::buffer_access_mask::map_write_bit |
                gl::buffer_access_mask::map_invalidate_range_bit)));
    unsigned short *ptr = start;
    unsigned short  vertex_index =
        configuration::can_use.draw_elements_base_vertex
            ? 0
            : static_cast<unsigned short>(m_mesh.first_vertex());
    for (unsigned short i = 0; i < m_max_chars; ++i)
    {
        *ptr++ = vertex_index;
        *ptr++ = vertex_index + 1;
        *ptr++ = vertex_index + 2;
        *ptr++ = vertex_index;
        *ptr++ = vertex_index + 2;
        *ptr++ = vertex_index + 3;
        vertex_index += 4;
    }
    m_mesh.index_buffer()->unmap(r);
}

Font &Text_buffer::font()
{
    return m_font;
}

void Text_buffer::render()
{
    slog_trace("Text_buffer::render() m_chars_printed = %u",
               static_cast<unsigned int>(m_chars_printed));

    if (m_chars_printed > 0)
    {
        gl::begin_mode::value         begin_mode    = gl::begin_mode::triangles;
        GLsizei                       count         = static_cast<GLsizei>(m_chars_printed * 6); // 2 triangles = 6 indices per char
        gl::draw_elements_type::value index_type    = gl::draw_elements_type::unsigned_short;
        GLvoid *                      index_pointer = reinterpret_cast<GLvoid *>(m_mesh.first_index() * m_mesh.index_buffer()->stride());
        GLint                         base_vertex   = configuration::can_use.draw_elements_base_vertex
                                ? static_cast<GLint>(m_mesh.first_vertex())
                                : 0;

        m_renderer.draw_elements_base_vertex(begin_mode, count, index_type, index_pointer, base_vertex);
    }
}

void Text_buffer::begin_print()
{
    slog_trace("Text_buffer::begin_print()");

    //  We want to write directly to the vertex buffer but
    //  we don't yet know how many char / vertices will be
    //  written - use explicit flushing.
    m_renderer.set_vertex_buffer();

    auto &r = m_renderer.renderer();

    m_vertex_ptr = static_cast<float *>(
        m_mesh.vertex_buffer()->map(
            r,
            m_mesh.first_vertex(),
            m_mesh.vertex_count(),
            (gl::buffer_access_mask::value)(
                gl::buffer_access_mask::map_write_bit |
                gl::buffer_access_mask::map_invalidate_range_bit |
                gl::buffer_access_mask::map_flush_explicit_bit)));

    log_trace("m_vertex_ptr = %p\n", m_vertex_ptr);

    m_chars_printed = 0;
    m_bounding_box.reset_for_grow();
}

size_t Text_buffer::end_print()
{
    slog_trace("Text_buffer::end_print() m_chars_printed = %u",
               static_cast<unsigned int>(m_chars_printed));

    auto &r = m_renderer.renderer();

    m_vertex_ptr = nullptr;
    m_mesh.vertex_buffer()->flush_and_unmap(r, 4 * m_chars_printed);
    return m_chars_printed;
}

void Text_buffer::print(const std::string &text, int x, int y)
{
    if (m_chars_printed == m_max_chars)
    {
        return;
    }

    int a = 0;
    m_chars_printed += m_font.print(text, m_bounding_box, m_vertex_ptr,
                                    static_cast<float>(x), static_cast<float>(y),
                                    m_max_chars - m_chars_printed);
    ++a;
}

void Text_buffer::measure(const std::string &text)
{
    m_bounding_box.reset_for_grow();
    if (text.size() == 0)
    {
        return;
    }

    m_font.measure(text, m_bounding_box);
}

void Text_buffer::print_center(const std::string &text, float x, float y)
{
    measure(text);
    glm::vec2 p(x, y);
    p -= m_bounding_box.half_size();
    print(text, static_cast<int>(std::ceil(p.x)), static_cast<int>(std::ceil(p.y)));
}

} // namespace ui
} // namespace renderstack
