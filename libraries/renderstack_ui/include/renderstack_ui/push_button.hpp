#ifndef renderstack_ui__push_button_hpp
#define renderstack_ui__push_button_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/button.hpp"
#include <memory>

namespace renderstack { namespace ui {

class push_button : public button
{
private:
   bool m_pressed;

public:
   bool pressed() const { return m_pressed; }
   void set_pressed(bool value){ m_pressed = value; }

public:
   push_button(
      std::shared_ptr<class gui_renderer> renderer,
      std::string const &label,
      std::shared_ptr<class style> style
   );
   virtual ~push_button(){}

   void draw_self(ui_context &context);
};

} }

#endif
