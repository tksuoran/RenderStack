#define _CRT_SECURE_NO_WARNINGS

#include "renderstack_ui/area.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include <glm/gtx/string_cast.hpp>

// #define LOG_LAYOUT

namespace renderstack { namespace ui {

using namespace std;
using namespace glm;

area::area(shared_ptr<class gui_renderer> renderer)
:  m_renderer                 (renderer)
,  m_style                    (renderer->default_style())
,  m_hidden                   (false)
,  m_offset_pixels            (glm::vec2(0.0f, 0.0f))
,  m_offset_self_size_relative(glm::vec2(0.0f, 0.0f))
,  m_offset_free_size_relative(glm::vec2(0.0f, 0.0f))
,  m_fill_base_pixels         (glm::vec2(0.0f, 0.0f))
,  m_fill_free_size_relative  (glm::vec2(0.0f, 0.0f))
,  m_parent                   ()
,  m_draw_ordering            (area_order::self_first)
,  m_event_ordering           (area_order::post_self)
,  m_layout_style             (area_layout_style::normal)
,  m_layout_x_order           (area_layout_order::none)
,  m_layout_y_order           (area_layout_order::none)
,  m_link                     ()
,  m_size                     (glm::vec2(0.0f, 0.0f))
,  m_rect                     (0.0f, 0.0f)
,  m_in_rect                  (0.0f, 0.0f)
,  m_clip_to_reference        (true)
,  m_in_draw                  (false)
{
}
area::area(shared_ptr<class gui_renderer> renderer, shared_ptr<class style> style)
:  m_renderer                 (renderer)
,  m_style                    (style)
,  m_hidden                   (false)
,  m_offset_pixels            (glm::vec2(0.0f, 0.0f))
,  m_offset_self_size_relative(glm::vec2(0.0f, 0.0f))
,  m_offset_free_size_relative(glm::vec2(0.0f, 0.0f))
,  m_fill_base_pixels         (glm::vec2(0.0f, 0.0f))
,  m_fill_free_size_relative  (glm::vec2(0.0f, 0.0f))
,  m_parent                   ()
,  m_draw_ordering            (area_order::self_first)
,  m_event_ordering           (area_order::post_self)
,  m_layout_style             (area_layout_style::normal)
,  m_layout_x_order           (area_layout_order::none)
,  m_layout_y_order           (area_layout_order::none)
,  m_link                     ()
,  m_size                     (glm::vec2(0.0f, 0.0f))
,  m_rect                     (0.0f, 0.0f)
,  m_in_rect                  (0.0f, 0.0f)
,  m_clip_to_reference        (true)
,  m_in_draw                  (false)
{
}

area::area(
   shared_ptr<class gui_renderer>   renderer,
   shared_ptr<class style>          style,
   area_layout_order::value         layout_x_order, 
   area_layout_order::value         layout_y_order
)
:  m_renderer                 (renderer)
,  m_style                    (style)
,  m_hidden                   (false)
,  m_offset_pixels            (glm::vec2(0.0f, 0.0f))
,  m_offset_self_size_relative(glm::vec2(0.0f, 0.0f))
,  m_offset_free_size_relative(glm::vec2(0.0f, 0.0f))
,  m_fill_base_pixels         (glm::vec2(0.0f, 0.0f))
,  m_fill_free_size_relative  (glm::vec2(0.0f, 0.0f))
,  m_parent                   ()
,  m_draw_ordering            (area_order::self_first)
,  m_event_ordering           (area_order::separate)
,  m_layout_style             (area_layout_style::normal)
,  m_layout_x_order           (layout_x_order)
,  m_layout_y_order           (layout_y_order)
,  m_link                     ()
,  m_size                     (glm::vec2(0.0f, 0.0f))
,  m_rect                     (0.0f, 0.0f)
,  m_in_rect                  (0.0f, 0.0f)
,  m_clip_to_reference        (true)
,  m_in_draw                  (false)
{
}

area::~area()
{
}

shared_ptr<class area> area::add(shared_ptr<class area> area)
{
   if (m_in_draw)
      m_add_list.push_back(area);
   else
   {
      m_children.push_back(area);
      area->set_parent(shared_from_this());
   }
   return area;
}
shared_ptr<class area> area::remove(shared_ptr<class area> a)
{
   if (m_in_draw)
      m_remove_list.push_back(a);
   else
   {
      m_children.erase(
         std::remove(m_children.begin(), m_children.end(), a), 
         m_children.end()
         );
      a->reset_parent();
   }
   return a;
}
shared_ptr<class area> area::get_hit(glm::vec2 hit_position)
{
   if (m_hidden)
      return nullptr;

   if (m_event_ordering == area_order::self_first)
      if (m_rect.hit(hit_position))
         return shared_from_this();

   for (auto child = m_children.cbegin(); child != m_children.cend(); ++child)
   {
      auto hit = (*child)->get_hit(hit_position);
      if (hit != nullptr)
         return hit;
   }

   if (m_event_ordering == area_order::post_self)
   {
      if (m_rect.hit(hit_position))
         return shared_from_this();
   }

   return nullptr;
}

void area::draw_self(ui_context &context)
{
   (void)context;
}
void area::draw(ui_context &context)
{
   if (m_hidden)
      return;

   m_in_draw = true;
   if (m_draw_ordering == area_order::self_first)
      draw_self(context);

   for (auto child = m_children.cbegin(); child != m_children.cend(); ++child)
      (*child)->draw(context);

   if (m_draw_ordering == area_order::post_self)
      draw_self(context);

   m_in_draw = false;

   for (auto i = m_add_list.cbegin(); i != m_add_list.cend(); ++i)
      add(*i);

   for (auto i = m_remove_list.cbegin(); i != m_remove_list.cend(); ++i)
      remove(*i);

   m_add_list.clear();
   m_remove_list.clear();
}

//  Layout
#if 1
void area::begin_size(glm::vec2 const &free_size_reference)
{
#if defined(LOG_LAYOUT)
   log() << name() << " area::begin_size() reference = " << to_string(free_size_reference) << '\n';
   log().flush();
#endif

   m_size = m_fill_base_pixels + free_size_reference * m_fill_free_size_relative;
   m_size = glm::round(m_size);

#if defined(LOG_LAYOUT)
   log() << name() << " size = " << to_string(size()) << '\n';
   log().flush();
#endif
}
void area::call_size(shared_ptr<area> area)
{
#if defined(LOG_LAYOUT)
   log() << name() << " area::call_size(" << area->name() << ")\n";
   log().flush();
#endif

   area->do_size(m_size);
}
void area::end_size()
{
#if defined(LOG_LAYOUT)
   log() << name() << " area::end_size() size = " << to_string(size()) << '\n';
   log().flush();
#endif
}
// Do not make this virtual.
// Derived classes should override BeginSize() instead
glm::vec2 const &area::do_size(glm::vec2 const &free_size_reference)
{
#if defined(LOG_LAYOUT)
   log() << m_name << " area::do_size() reference = " << to_string(free_size_reference) << '\n';
   log().flush();
#endif

   begin_size(free_size_reference);
   for (auto child = m_children.cbegin(); child != m_children.cend(); ++child)
      call_size(*child);

   end_size();

#if defined(LOG_LAYOUT)
   log() << "  " << name() << " size = " << to_string(size()) << '\n';
   log().flush();
#endif

   return m_size;
}
void area::begin_place(rectangle const &reference, glm::vec2 const &container_grow_direction)
{
   (void)container_grow_direction; // TODO
#if defined(LOG_LAYOUT)
   log() << name() << " area::begin_place() reference = " << to_string(reference.min()) << ".." << to_string(reference.max()) << '\n';
   log() << "  " << name() << " size = " + to_string(size()) << '\n';
   log().flush();
#endif

   switch (m_layout_style)
   {
   case area_layout_style::normal:
      break;
   case area_layout_style::extend_horizontal:
      m_size.x = reference.size().x;
      break;
   case area_layout_style::extend_vertical:
      m_size.y = reference.size().y;
      break;
   }

#if defined(LOG_LAYOUT)
   log() << "  " << name() << " size = " + to_string(size()) << " after layout style\n";
   log().flush();
#endif

   m_rect.min() = 
      reference.min() + 
      offset_pixels() + 
      reference.size() * offset_free_size_relative() + 
      size() * offset_self_size_relative();

   m_rect.max() = m_rect.min() + size();

   m_rect.min() = glm::round(m_rect.min());
   m_rect.max() = glm::round(m_rect.max());

   if (m_clip_to_reference)
      m_rect.clip_to(reference);

   m_in_rect = m_rect.shrink(m_style->padding());
}
void area::call_place(shared_ptr<class area> area)
{
#if defined(LOG_LAYOUT)
   log() << name() << " area::call_place(" + area->name() << ")\n";
   log().flush();
#endif

   area->do_place(m_rect, glm::vec2(1.0f, 1.0f));
}
void area::end_place()
{
}
// Do not make this virtual.
// Derived classes should override BeginPlace() instead
glm::vec2 const &area::do_place(rectangle const &reference_location, glm::vec2 const &grow_direction)
{
#if defined(LOG_LAYOUT)
   log() << name() << " area::do_place() reference size = " << to_string(reference_location.size()) << '\n';
   log() << "  " << name() << " size = " + to_string(size()) << '\n';
   log().flush();
#endif

   begin_place(reference_location, grow_direction);
   for (auto child = m_children.cbegin(); child != m_children.cend(); ++child)
      call_place(*child);
      //string name = area.Name != null ? area.Name : "";

   end_place();
   return m_size;
}

#endif // layout #if 1

} }

