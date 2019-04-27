#ifndef ninepatch_style_hpp_renderstack_ui
#define ninepatch_style_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace renderstack
{
namespace graphics
{

class Program;
class Renderer;
class Texture;

} // namespace graphics
} // namespace renderstack

namespace renderstack
{
namespace ui
{

class Ninepatch_style
{
public:
    Ninepatch_style(renderstack::graphics::Renderer &renderer,
                    const std::string               &path,
                    renderstack::graphics::Program  *background_program,
                    int                             texture_unit);

    std::unique_ptr<renderstack::graphics::Texture> texture;
    renderstack::graphics::Program                  *program{nullptr};
    unsigned int                                    texture_unit{0U};
    glm::vec2                                       border_uv;
    glm::vec2                                       border_pixels;
};

} // namespace ui
} // namespace renderstack

#endif
