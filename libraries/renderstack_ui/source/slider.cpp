#include "renderstack_ui/context.hpp"
#include "renderstack_ui/slider.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include <sstream>

namespace renderstack { namespace ui {

using namespace std;
using namespace glm;

string const &slider::label() const
{
   return m_label;
}
float slider::current_display_value() const
{
   return m_min_value + m_current_relative_value * (m_max_value - m_min_value);
}
float slider::slider_relative_value() const
{
   if (m_nonlinear)
      return nonlinear_value();

   return relative_value();
}
float slider::nonlinear_value() const
{
   return (float)std::pow(relative_value(), 1.0f / 6.0f);
}
float slider::relative_value() const
{
   return m_current_relative_value;
}
float slider::min() const
{
   return m_min_value;
}
float slider::max() const
{
   return m_max_value;
}

void slider::set_value(string const &value)
{
   m_label = value;
}
void slider::set_current_display_value(float value)
{
   m_current_relative_value = (value - m_min_value) / (m_max_value - m_min_value);
}
void slider::set_slider_relative_value(float value)
{
   if (m_nonlinear)
      set_nonlinear_value(value);
   else
      set_relative_value(value);
}
void slider::set_nonlinear_value(float value)
{
   set_relative_value(std::pow(value, 6.0f));
}
void slider::set_relative_value(float value)
{
   if (value != m_current_relative_value) {
      m_current_relative_value = value;
      m_value_dirty = true;
   }
}
void slider::set_min(float value)
{
   m_min_value = value;
}
void slider::set_max(float value)
{
   m_max_value = value;
}

slider::slider(
   shared_ptr<class gui_renderer>   renderer,
   shared_ptr<class style>          style,
   string const                     &label, 
   float                            min, 
   float                            max
)
:  area           (renderer, style)
,  m_text_buffer  (renderer, style->font(), style->program()->mappings())
,  m_ninepatch    (renderer, style->ninepatch_style())
,  m_label        (label)
,  m_min_value    (min)
,  m_max_value    (max)
,  m_label_dirty  (true)
,  m_value_dirty  (true)
,  m_nonlinear    (true)
{
   set_name(label);
   set_slider_relative_value(0.5f);
}

void slider::begin_size(vec2 const &free_size_reference)
{
   update_size();
   area::begin_size(free_size_reference);
}
void slider::update_size()
{
   if (m_label_dirty)
   {
      if (style()->font())
      {
         m_text_buffer.measure(m_label + ": 180.99");
         m_bounds = m_text_buffer.bounding_box();

         set_fill_base_pixels(m_bounds.max() + 2.0f * style()->padding());
      }
      else
      {
         set_fill_base_pixels(vec2(30.0f, 10.0f));
      }

      m_ninepatch.place(renderer(), 0.0f, 0.0f, fill_base_pixels().x, fill_base_pixels().y);
      m_label_dirty = false;
      m_value_dirty = true;
   }
}

void slider::begin_place(rectangle const &reference, vec2 const &grow_direction)
{
   area::begin_place(reference, grow_direction);
   update_place();
   mat4 a, b;
   create_translation(rect().min() + style()->padding(), a);
   create_translation(rect().min(), b);
   mat4 const &o = renderer()->ortho();
   m_text_frame       = o * a;
   m_background_frame = o * b;
}
void slider::update_place()
{
   if (size().x != m_bounds.max().x + 2.0f * style()->padding().x)
      m_ninepatch.place(renderer(), 0.0f, 0.0f, size().x, size().y);
}
void slider::draw_self(ui_context &context)
{
   if (m_value_dirty && style()->font())
   {
      stringstream ss;
      ss << m_label << ": " << current_display_value();
      m_text_buffer.begin_print();
      m_text_buffer.print(ss.str(), 0.0f, 0.0f);
      m_text_buffer.end_print();
   }

   auto r = renderer();

   //r->push();

   r->set_program(style()->ninepatch_style()->program());
   r->set_texture(style()->ninepatch_style()->texture_unit(), style()->ninepatch_style()->texture());
   r->begin_edit();
   r->set_transform(m_background_frame);
   r->set_color_scale(vec4(1.0f, 1.0f, 1.0f, 1.0f));

   if (rect().hit(context.mouse))
   {
      if (context.mouse_buttons[0])
      {
         float x = context.mouse.x - rect().min().x;
         float relative_value = x / (rect().size().x - 1.0f);
         set_relative_value(relative_value);
         r->set_color_add(vec4(0.3f, 0.4f, 0.5f, 0.0f));
         set_trigger(true);
      }
      else
      {
         if (trigger())
         {
            /*if (action_sink() != nullptr)
            {
            action_sink()->action(this);
            }*/
            set_trigger(false);
         }
         r->set_color_add(vec4(0.2f, 0.3f, 0.4f, 0.0f));
      }
   }
   else
   {
      set_trigger(false);
      r->set_color_add(vec4(0.1f, 0.2f, 0.3f, 0.0f));
   }

   float t = relative_value();
   float pixel_x = rect().min().x + t * (rect().size().x - 1.0f);

   r->set_t(pixel_x);

   r->end_edit();
   m_ninepatch.render(r);

   /*  Then draw text  */ 
   if (style()->font())
   {
      r->blend_alpha();
      r->begin_edit();
      r->set_program(style()->program());
      r->set_texture(style()->texture_unit(), style()->font()->texture());
      r->set_color_add  (vec4(0.00f, 0.00f, 0.00f, 0.0f));
      r->set_color_scale(vec4(0.72f, 0.72f, 0.72f, 2.0f));
      r->set_transform(m_text_frame);
      r->end_edit();
      m_text_buffer.render();
      r->blend_disable();
   }
   //r->pop();
}

} }
