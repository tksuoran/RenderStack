#ifndef layer_hpp_renderstack_ui
#define layer_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/area.hpp"

namespace renderstack { namespace ui {

class layer : public area
{
private:
   rectangle m_fixed_size;

public:
   layer(
      std::shared_ptr<class gui_renderer> renderer,
      rectangle const &fixed_size
   )
   :  area        (renderer)
   ,  m_fixed_size(fixed_size)
   {
      reset_parent();
      set_draw_ordering(area_order::post_self);
      set_event_ordering(area_order::separate);

      update();
   }
   virtual ~layer(){}

   void set_layer_size(float width, float height)
   {
      m_fixed_size = rectangle(width, height);
   }

   void update()
   {
      set_rect(m_fixed_size);
      set_size(rect().size());
      place();
   }

   void place()
   {
      for (auto child = children().cbegin(); child != children().cend(); ++child)
         (*child)->do_size(size());

      for (auto child = children().cbegin(); child != children().cend(); ++child)
         (*child)->do_place(rect(), glm::vec2(1.0f, 1.0f));
   }
};

} }

#endif
