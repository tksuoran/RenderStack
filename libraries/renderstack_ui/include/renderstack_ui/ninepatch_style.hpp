#ifndef renderstack_ui__ninepatch_style_hpp
#define renderstack_ui__ninepatch_style_hpp

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <string>

namespace renderstack { namespace ui {

class ninepatch_style
{
private:
   unsigned int m_texture;
   glm::vec2    m_border_uv;
   glm::vec2    m_border_pixels;

public:
   ninepatch_style(std::string const &path);

   unsigned int    texture       () const;
   glm::vec2 const &border_pixels() const;
   glm::vec2 const &border_uv    () const;
};

} }

#endif
