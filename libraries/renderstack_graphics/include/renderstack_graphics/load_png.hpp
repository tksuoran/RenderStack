#ifndef load_png_hpp_renderstack_toolkit
#define load_png_hpp_renderstack_toolkit

#include "renderstack_toolkit/platform.hpp"
#include <memory>
#include <string>

namespace renderstack { namespace graphics {

class renderer;
class texture;

// Currently creates immutable texture without mipmaps
extern std::shared_ptr<renderstack::graphics::texture> load_png(
   renderstack::graphics::renderer &renderer,
   unsigned int texture_unit,
   std::string const &path
);

} }

#endif
