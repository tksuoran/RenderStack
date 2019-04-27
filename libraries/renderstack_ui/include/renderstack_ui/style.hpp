#ifndef style_hpp_renderstack_ui
#define style_hpp_renderstack_ui

#include "renderstack_graphics/program.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace renderstack
{

namespace graphics
{
class Program;
}

namespace ui
{

class Style
{
public:
    Style(glm::vec2 padding, glm::vec2 inner_padding)
        : padding(padding)
        , inner_padding(inner_padding)
    {
    }

    Style(glm::vec2                      padding,
          glm::vec2                      inner_padding,
          Font                           *font,
          Ninepatch_style                *ninepatch_style,
          renderstack::graphics::Program *program)
        : padding(padding)
        , inner_padding(inner_padding)
        , font(font)
        , ninepatch_style(ninepatch_style)
        , program(program)
    {
    }

    Style(const Style &) = delete;
    Style &operator=(const Style &) = delete;

    glm::vec2                      padding{0.0f};
    glm::vec2                      inner_padding{0.0f};
    Font                           *font{nullptr};
    Ninepatch_style                *ninepatch_style{nullptr};
    renderstack::graphics::Program *program{nullptr};
    unsigned int                   texture_unit{0U};
};

} // namespace ui
} // namespace renderstack

#endif
