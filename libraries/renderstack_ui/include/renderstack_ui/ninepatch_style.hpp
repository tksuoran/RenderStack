#ifndef renderstack_ui__ninepatch_style_hpp
#define renderstack_ui__ninepatch_style_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/program.hpp"
#include <glm/glm.hpp>
#include <string>

namespace renderstack { namespace ui {

class ninepatch_style
{
public:
   ninepatch_style(
      std::string const &path,
      std::shared_ptr<renderstack::graphics::program> background_program,
      int texture_unit
   );

   unsigned int    texture       () const;
   glm::vec2 const &border_pixels() const;
   glm::vec2 const &border_uv    () const;
   std::shared_ptr<renderstack::graphics::program> program         () const { return m_program; }
   void                                            set_texture_unit(unsigned int value){ m_texture_unit = value; }
   unsigned int                                    texture_unit    () const { return m_texture_unit; }

private:
   unsigned int m_texture;
   glm::vec2    m_border_uv;
   glm::vec2    m_border_pixels;
   std::shared_ptr<renderstack::graphics::program> m_program;
   unsigned int                                    m_texture_unit;
};

} }

#endif
