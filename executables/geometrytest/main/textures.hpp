#ifndef textures_hpp
#define textures_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/texture.hpp"
#include <string>
#include <memory>

class textures
{
public:
   textures();

public:
   void on_load(renderstack::graphics::renderer &renderer);

   std::shared_ptr<renderstack::graphics::texture> load(
      renderstack::graphics::renderer &renderer,
      unsigned int texture_unit,
      std::string const &path);

   std::shared_ptr<renderstack::graphics::texture> background_texture;
};

#endif
