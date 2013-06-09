#ifndef renderstack_ui__slider_hpp
#define renderstack_ui__slider_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/area.hpp"
#include "renderstack_ui/action.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace ui {

class slider
:  public area
,  public action_source
{
public:
   std::string const &label() const;
   float current_display_value() const;
   float slider_relative_value() const;
   float nonlinear_value() const;
   float relative_value() const;
   float min() const;
   float max() const;
   bool  trigger() const { return m_trigger; }

   void set_value(std::string const &value);
   void set_current_display_value(float value);
   void set_slider_relative_value(float value);
   void set_nonlinear_value(float value);
   void set_relative_value(float value);
   void set_min(float value);
   void set_max(float value);
   void set_trigger(bool value){ m_trigger = value; }

   slider(
      std::shared_ptr<class gui_renderer> renderer,
      std::shared_ptr<class style>        style,
      std::string const                   &label, 
      float                               min, 
      float                               max
   );
   virtual ~slider(){}

   void begin_size(glm::vec2 const &free_size_reference);
   void update_size();
   void begin_place(rectangle const &reference, glm::vec2 const &grow_direction);
   void update_place();            

   void draw_self(ui_context &context);

private:
   glm::mat4         m_text_frame;
   glm::mat4         m_background_frame;
   class text_buffer m_text_buffer;
   class ninepatch   m_ninepatch;
   rectangle         m_bounds;
   std::string       m_label;
   float             m_min_value;
   float             m_max_value;
   bool              m_label_dirty;
   bool              m_value_dirty;
   bool              m_nonlinear;
   bool              m_trigger;
   float             m_current_relative_value;
};

} }

#endif

