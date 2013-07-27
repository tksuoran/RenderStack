#ifndef textures_hpp
#define textures_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/texture.hpp"
#include <string>
#include <memory>

class textures : public renderstack::toolkit::service
{
public:
   textures();
   /*virtual*/ ~textures();
   void connect(std::shared_ptr<renderstack::graphics::renderer> renderer);
   /*virtual*/ void initialize_service();

   std::shared_ptr<renderstack::graphics::texture> load(
      unsigned int texture_unit,
      std::string const &path
   );

   std::shared_ptr<renderstack::graphics::texture> background_texture;

private:
   std::shared_ptr<renderstack::graphics::renderer> m_renderer;
};

#endif
