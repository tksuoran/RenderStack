#ifndef renderstack_ui__area_hpp
#define renderstack_ui__area_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/enable_shared_from_this.hpp"
#include "renderstack_ui/style.hpp"
#include "renderstack_ui/rectangle.hpp"
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <memory>
#include <glm/glm.hpp>

namespace renderstack { namespace ui {

struct ui_context
{
   glm::ivec2  mouse;
   bool        mouse_buttons[3]; // 0 left 1 right 2 middle
};

namespace area_order
{
   enum value
   {
      self_first,     //  Draw self before children
      post_self,      //  Draw self last, after children
      separate        //  Separate DrawSelf() call
   };
}

namespace area_layout_style
{
   enum value
   {
      normal,
      extend_horizontal,
      extend_vertical
   };
}

namespace area_layout_order
{
   enum value
   {
      increasing,
      decreasing,
      none
   };
}

class area : public renderstack::toolkit::enable_shared_from_this<area>
{
public:
   area();
   area(std::shared_ptr<class style> style);
   area(std::shared_ptr<class style> style, area_layout_order::value layout_x_order, area_layout_order::value layout_y_order);
   virtual ~area();

   typedef std::vector<std::shared_ptr<area> >  area_collection;

public:
   std::string const             &name() const { return m_name; }
   void                          set_name(std::string const &value){ m_name = value; }
   bool                          hidden() const { return m_hidden; }
   void                          set_hidden(bool value){ m_hidden = value; }
   glm::vec2 const               &offset_pixels() const { return m_offset_pixels; }
   void                          set_offset_pixels(glm::vec2 const &value){ m_offset_pixels = value; }
   glm::vec2 const               &offset_self_size_relative() const { return m_offset_self_size_relative; }
   void                          set_offset_self_size_relative(glm::vec2 const &value){ m_offset_self_size_relative = value; }
   glm::vec2 const               &offset_free_size_relative() const { return m_offset_free_size_relative; }
   void                          set_offset_free_size_relative(glm::vec2 const &value){ m_offset_free_size_relative = value; }
   glm::vec2 const               &fill_base_pixels() const { return m_fill_base_pixels; }
   void                          set_fill_base_pixels(glm::vec2 const &value){ m_fill_base_pixels = value; }
   glm::vec2 const               &fill_free_size_relative() const { return m_fill_free_size_relative; }
   void                          fill_free_size_relative(glm::vec2 const &value){ m_fill_free_size_relative = value; }
   glm::vec2 const               &size() const { return m_size; }
   glm::vec2                     &size(){ return m_size; }
   void                          set_size(glm::vec2 const &value){ m_size = value; }
   rectangle const               &rect() const { return m_rect; }
   rectangle const               &in_rect() const { return m_in_rect; }
   area_collection const         &children() const { return m_children; }
   bool                          clip_to_reference() const { return m_clip_to_reference; }
   void                          set_clip_to_reference(bool value){ m_clip_to_reference = value; }
   std::weak_ptr<area>           link() const { return m_link; }
   void                          set_link(std::weak_ptr<area> value){ m_link = value; }
   std::weak_ptr<area>           parent() const { return m_parent; }
   void                          set_parent(std::weak_ptr<area> value){ m_parent = value; }
   void                          reset_parent(){ m_parent.reset(); }
   area_order::value             draw_ordering() const { return m_draw_ordering; }
   void                          set_draw_ordering(area_order::value value){ m_draw_ordering = value; }
   area_order::value             event_ordering() const { return m_event_ordering; }
   void                          set_event_ordering(area_order::value value){ m_event_ordering = value; }
   area_layout_order::value      layout_x_order() const { return m_layout_x_order; }
   area_layout_order::value      layout_y_order() const { return m_layout_y_order; }
   area_layout_style::value      layout_style() const { return m_layout_style; }
   void                          set_layout_style(area_layout_style::value value){ m_layout_style = value; }
   std::shared_ptr<class style>  style() const { return m_style; }
   void                          set_style(std::shared_ptr<class style> value){ m_style = value; }

protected:
   rectangle                     &rect(){ return m_rect; }
   void                          set_rect(rectangle const &value){ m_rect = value; }
   void                          update_in_rect(){ m_in_rect = rect().shrink(style()->padding()); }

public:
   virtual std::shared_ptr<class area> add(std::shared_ptr<class area> area);
   std::shared_ptr<class area>         remove(std::shared_ptr<class area> a);
   std::shared_ptr<class area>         get_hit(glm::vec2 hit_position);

   virtual void                draw_self(ui_context &context);
   void                        draw(ui_context &context);

   //  Layout
   virtual void                begin_size(glm::vec2 const &free_size_reference);
   virtual void                call_size(std::shared_ptr<area> area);
   virtual void                end_size();
   // Do not make this virtual.
   // Derived classes should override begin_size() instead
   glm::vec2 const             &do_size(glm::vec2 const &free_size_reference);

   virtual void                begin_place(rectangle const &reference, glm::vec2 const &container_grow_direction);
   virtual void                call_place(std::shared_ptr<class area> area);
   virtual void                end_place();
   // Do not make this virtual.
   // Derived classes should override begin_place() instead
   glm::vec2 const             &do_place(rectangle const &reference_location, glm::vec2 const &grow_direction);

private:
   std::string                     m_name;
   std::shared_ptr<class style>    m_style;
   bool                            m_hidden;
   glm::vec2                       m_offset_pixels;
   glm::vec2                       m_offset_self_size_relative;
   glm::vec2                       m_offset_free_size_relative;
   glm::vec2                       m_fill_base_pixels;
   glm::vec2                       m_fill_free_size_relative;
   std::weak_ptr<class area>       m_parent;
   area_collection                 m_children;
   area_order::value               m_draw_ordering;
   area_order::value               m_event_ordering;
   area_layout_style::value        m_layout_style;
   area_layout_order::value        m_layout_x_order;
   area_layout_order::value        m_layout_y_order;
   std::weak_ptr<class area>       m_link;
   glm::vec2                       m_size;
   rectangle                       m_rect;
   rectangle                       m_in_rect;
   bool                            m_clip_to_reference;

   bool                            m_in_draw;
   area_collection                 m_add_list;
   area_collection                 m_remove_list;
};

} }

#endif
