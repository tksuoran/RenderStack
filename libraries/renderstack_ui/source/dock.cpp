#define _CRT_SECURE_NO_WARNINGS

#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/log.hpp"
#include <glm/gtx/string_cast.hpp>

// #define LOG_LAYOUT

namespace renderstack { namespace ui {

using namespace std;
using namespace glm;


dock::dock(
   shared_ptr<class gui_renderer>   renderer,
   shared_ptr<class style>          style,
   orientation::value               orientation,
   area_layout_style::value         child_layout_style)
:  area(
      renderer,
      style,
      ((orientation == orientation::horizontal) ? area_layout_order::increasing : area_layout_order::none),
      ((orientation == orientation::vertical  ) ? area_layout_order::increasing : area_layout_order::none)
   )
,  m_orientation        (orientation)
,  m_cursor_start       (0.0f, 0.0f)
,  m_cursor_end         (0.0f, 0.0f)
,  m_grow_direction     (0.0f, 0.0f)
,  m_child_layout_style (child_layout_style)
{
   switch (layout_x_order())
   {
   case area_layout_order::increasing:
      m_grow_direction.x = 1.0f;
      break;
   case area_layout_order::decreasing:
      m_grow_direction.x = -1.0f;
      break;
   case area_layout_order::none:
      m_grow_direction.x = 0.0f;
      break;
   }

   switch (layout_y_order())
   {
   case area_layout_order::increasing:
      m_grow_direction.y = 1.0f;
      break;
   case area_layout_order::decreasing:
      m_grow_direction.y = -1.0f;
      break;
   case area_layout_order::none:
      m_grow_direction.y = 0.0f;
      break;
   }

   set_name("dock");
}
void dock::begin_size(glm::vec2 const &free_size_reference)
{
   (void)free_size_reference;
   set_size(style()->padding());
}
void dock::call_size(shared_ptr<class area> area)
{
#if defined(LOG_LAYOUT)
   log() << name() << " dock::call_size(" << area->name() << ")\n";
   log().flush();
#endif

   glm::vec2 sub_size = area->do_size(size());

#if defined(LOG_LAYOUT)
   log() << "  " << name() << " dock::call_size(" << area->name() << ") sub_size = " << to_string(sub_size) << '\n';
   log().flush();
#endif

   // size[axis_sum]   += sub_size[axis_sum] + style->padding[axis_sum];
   // size[axis_max]    = MAX( size[axis_max], sub_size[axis_max] );
   switch (m_orientation)
   {
   case orientation::horizontal:
      size().x += sub_size.x + style()->inner_padding().x;
      size().y = std::max(size().y, sub_size.y);
      break;
   case orientation::vertical:
      size().y += sub_size.y + style()->inner_padding().y;
      size().x = std::max(size().x, sub_size.x);
      break;
   }

#if defined(LOG_LAYOUT)
   log() << "  " << name() << " size = " << to_string(size()) << '\n';
   log().flush();
#endif
}
shared_ptr<class area> dock::add(shared_ptr<class area> area)
{
   if (!area)
      return nullptr;

   area->set_layout_style(m_child_layout_style);

   return area::add(area);
}
void dock::end_size()
{
#if defined(LOG_LAYOUT)
   log() << name() << " dock::end_size() size = " << to_string(size()) << '\n';
   log().flush();
#endif

   switch (m_orientation)
   {
   case orientation::horizontal:
      size().x -= m_grow_direction.x * style()->inner_padding().x;
      size().x += m_grow_direction.x * style()->padding().x;
      size().y += /*m_grow_direction.y **/ 2.0f * style()->padding().y;
      break;
   case orientation::vertical:
      size().y -= m_grow_direction.y * style()->inner_padding().y;
      size().y += m_grow_direction.y * style()->padding().y;
      size().x += /*m_grow_direction.x **/ 2.0f * style()->padding().x;
      break;
   }

#if defined(LOG_LAYOUT)
   log() << "  " << name() << " dock::end_size() size = " << to_string(size()) << '\n';
   log().flush();
#endif
}
void dock::begin_place(rectangle const &reference, glm::vec2 const &container_grow_direction)
{
   (void)container_grow_direction; // TODO
#if defined(LOG_LAYOUT)
   log() << name() << " dock::begin_place() reference in size = " << to_string(reference.size()) << '\n';
   log().flush();
#endif

   // rect.min = ref.min  + offset_pixels + ref.getSize() * offset_free_size_relative + size * offset_self_size_relative;
   // rect.max = rect.min + size;
   // 
   // if( isEnabled(Area::USE_CLIP_TO_REFERENCE) ){
   //     rect.intersect( ref );
   // }
   // 
   // in_rect = rect.shrink( style->padding );
   // 
   // cursor_start = rect.min + style->padding;
   // cursor_end   = rect.max - style->padding;

   rect().set_min( 
      reference.min() + 
      offset_pixels() + 
      (reference.size() * offset_free_size_relative()) + 
      (size() * offset_self_size_relative())
   );

   rect().set_size(size());

   if (clip_to_reference())
      rect().clip_to(reference);

   rect().min() = glm::round(rect().min());
   rect().max() = glm::round(rect().max());

   update_in_rect(); // inRect = rect.Shrink(Style.Padding);

   switch (layout_x_order())
   {
   case area_layout_order::increasing:
   case area_layout_order::none:
      m_cursor_start.x = rect().min().x + style()->padding().x;
      m_cursor_end.x   = rect().max().x - style()->padding().x;
      break;
   case area_layout_order::decreasing:
      m_cursor_start.x = rect().max().x - style()->padding().x;
      m_cursor_end.x   = rect().min().x + style()->padding().x;
      break;
   }

   switch (layout_y_order())
   {
   case area_layout_order::increasing:
   case area_layout_order::none:
      m_cursor_start.y = rect().min().y + style()->padding().y;
      m_cursor_end.y   = rect().max().y - style()->padding().y;
      break;
   case area_layout_order::decreasing:
      m_cursor_start.y = rect().max().y - style()->padding().y;
      m_cursor_end.y   = rect().min().y + style()->padding().y;
      break;
   }

   rectangle reference_out(m_cursor_start, m_cursor_end);

#if defined(LOG_LAYOUT)
   log() << "  " << name() << " dock::begin_place() reference out size = " << to_string(reference_out.size()) << " cursor_start = " << to_string(m_cursor_start) << '\n';
   log().flush();
#endif
}
void dock::call_place(shared_ptr<class area> area)
{
   rectangle reference(m_cursor_start, m_cursor_end);

#if defined(LOG_LAYOUT)
   log() << name() << " dock::call_place(" << area->name() << ")\n";
   log() << "  reference size = " << to_string(reference.size()) << " cursor_start = " << to_string(m_cursor_start) << '\n';
   log().flush();
#endif

   glm::vec2 sub_size = area->do_place(reference, m_grow_direction);

   // Vector2 sub_size = a->doPlace( Rect(cursor_start,cursor_end) );
   // cursor_start[axis_sum] += sub_size[axis_sum] + style->padding[axis_sum];

   switch (m_orientation)
   {
   case orientation::horizontal:   m_cursor_start.x += m_grow_direction.x * (sub_size.x + style()->inner_padding().x); break;
   case orientation::vertical:     m_cursor_start.y += m_grow_direction.y * (sub_size.y + style()->inner_padding().y); break;
   }

#if defined(LOG_LAYOUT)
   log() << "  cursor_start = " << to_string(m_cursor_start) << " sub_size was " + to_string(sub_size) << '\n';
   log().flush();
#endif
}

} }
