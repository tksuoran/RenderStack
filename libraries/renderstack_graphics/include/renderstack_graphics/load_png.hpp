#ifndef load_png_hpp_renderstack_toolkit
#define load_png_hpp_renderstack_toolkit

#include "renderstack_toolkit/platform.hpp"
#include <memory>
#include <string>

namespace renderstack
{
namespace graphics
{

class Renderer;
class Texture;

// Currently creates immutable texture without mipmaps
bool load_png(renderstack::graphics::Texture  &texture,
              renderstack::graphics::Renderer &renderer,
              unsigned int                     texture_unit,
              const std::string &              path);

} // namespace graphics
} // namespace renderstack

#endif
