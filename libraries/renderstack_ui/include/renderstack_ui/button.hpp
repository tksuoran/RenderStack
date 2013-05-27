#ifndef renderstack_ui__button_hpp
#define renderstack_ui__button_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/area.hpp"
#include "renderstack_ui/action.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace ui {

class button : public area, public action_source
{
public:
   button(
      std::shared_ptr<class gui_renderer> renderer,
      std::string const &label,
      std::shared_ptr<class style> style);
   virtual ~button(){}

   std::string const &label() const;
   void set_label    (std::string const &value);
   void update_size  ();
   void update_place ();
   void begin_size   (glm::vec2 const &free_size_reference);
   void begin_place  (rectangle const &reference, glm::vec2 const &grow_direction);
   void draw_self    (ui_context &context);

protected:
   glm::mat4         &text_frame();
   glm::mat4         &background_frame();
   class ninepatch   &ninepatch();
   bool              trigger() const;
   void              set_trigger(bool value);
   class text_buffer &text_buffer();

private:
   glm::mat4         m_text_frame;
   glm::mat4         m_background_frame;
   class text_buffer m_text_buffer;
   class ninepatch   m_ninepatch;
   rectangle         m_bounds;
   std::string       m_label;
   bool              m_dirty;
   bool              m_trigger;
};

} }

#endif
