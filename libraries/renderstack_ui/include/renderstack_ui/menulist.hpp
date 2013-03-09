#ifndef renderstack_ui__menulist_hpp
#define renderstack_ui__menulist_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace renderstack { namespace ui {

class menulist : public dock
{
public:
   menulist(
      std::shared_ptr<class style>  style, 
      orientation::value            orientation, 
      area_layout_style::value      child_layout_style = area_layout_style::normal
   )
   :   dock        (style, orientation, child_layout_style)
   ,   m_ninepatch (style->ninepatch_style())
   {
   }
   virtual ~menulist(){}

   void update();
   void begin_place(rectangle const &reference, glm::vec2 const &grow_direction);
   void draw_self(ui_context &context);

private:
   glm::mat4       m_background_frame;
   class ninepatch m_ninepatch;
};

} }

#endif
