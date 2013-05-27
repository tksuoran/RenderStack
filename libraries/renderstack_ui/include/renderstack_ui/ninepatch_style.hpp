#ifndef renderstack_ui__ninepatch_style_hpp
#define renderstack_ui__ninepatch_style_hpp

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace renderstack { namespace graphics {

class program;
class renderer;
class texture;

} }

namespace renderstack { namespace ui {

class ninepatch_style
{
public:
   ninepatch_style(
      renderstack::graphics::renderer &renderer,
      std::string const &path,
      std::shared_ptr<renderstack::graphics::program> background_program,
      int texture_unit
   );

   glm::vec2 const   &border_pixels    () const;
   glm::vec2 const   &border_uv        () const;
   void              set_texture_unit  (unsigned int value){ m_texture_unit = value; }
   unsigned int      texture_unit      () const { return m_texture_unit; }

   std::shared_ptr<renderstack::graphics::texture> texture() const;
   std::shared_ptr<renderstack::graphics::program> program() const;

private:
   std::shared_ptr<renderstack::graphics::texture> m_texture;
   std::shared_ptr<renderstack::graphics::program> m_program;
   unsigned int                                    m_texture_unit;
   glm::vec2                                       m_border_uv;
   glm::vec2                                       m_border_pixels;
};

} }

#endif
