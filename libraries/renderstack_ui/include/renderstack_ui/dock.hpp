#ifndef dock_hpp_renderstack_ui
#define dock_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/area.hpp"
#include <limits>
#include <memory>

namespace renderstack { namespace ui {

   namespace orientation
   {
      enum value
      {
         horizontal,
         vertical
      };
   }

class dock : public area
{
public:
   dock(
      std::shared_ptr<class gui_renderer> renderer,
      std::shared_ptr<class style>        style, 
      orientation::value                  orientation, 
      area_layout_style::value            child_layout_style = area_layout_style::normal
   );
   virtual ~dock()
   {
   }

   orientation::value orientation() const { return m_orientation; }
   void set_orientation(orientation::value value){ m_orientation = value; }
   area_layout_style::value child_layout_style() const { return m_child_layout_style; }
   void set_child_layout_style(area_layout_style::value value){ m_child_layout_style = value; }

   void begin_size(glm::vec2 const &free_size_reference);
   void call_size(std::shared_ptr<class area> area);
   std::shared_ptr<class area> add(std::shared_ptr<class area> area);
   void end_size();
   void begin_place(rectangle const &reference, glm::vec2 const &container_grow_direction);
   void call_place(std::shared_ptr<class area> area);

private:
   orientation::value       m_orientation;
   glm::vec2                m_cursor_start;
   glm::vec2                m_cursor_end;
   glm::vec2                m_grow_direction;
   area_layout_style::value m_child_layout_style;
};

} }

#endif
