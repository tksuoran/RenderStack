#ifndef text_buffer_hpp_renderstack_ui
#define text_buffer_hpp_renderstack_ui

#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/rectangle.hpp"
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace ui
{

class Gui_renderer;

class Text_buffer
{
public:
    Text_buffer(Gui_renderer &renderer, Font &font, unsigned int max_chars = 3000);

    ~Text_buffer() = default;

    Rectangle bounding_box() const
    {
        return m_bounding_box;
    }

    float line_height() const
    {
        return m_font.line_height();
    }

    void begin_print();

    size_t end_print();

    void print(const std::string &text, int x, int y);

    void measure(const std::string &text);

    void print_center(const std::string &text, float x, float y);

    void render();

    Font &font();

private:
    Gui_renderer            &m_renderer;
    Font                    &m_font;
    unsigned int            m_max_chars{0};
    Rectangle               m_bounding_box;
    renderstack::mesh::mesh m_mesh;
    float *                 m_vertex_ptr{nullptr};
    size_t                  m_chars_printed;
};

} // namespace ui
} // namespace renderstack

#endif
