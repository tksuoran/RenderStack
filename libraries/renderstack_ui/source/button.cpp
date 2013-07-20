#include "renderstack_ui/button.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/log.hpp"

#define LOG_CATEGORY &log_button

namespace renderstack { namespace ui {

using namespace std;

button::button(
   shared_ptr<class gui_renderer> renderer,
   string const                   &label,
   shared_ptr<class style>        style
)
:  area           (renderer, style)
,  m_text_buffer  (renderer, style->font(), style->program()->mappings())
,  m_ninepatch    (renderer, style->ninepatch_style())
,  m_dirty        (true)
,  m_trigger      (false)
{
   set_name(label);
   set_label(label);
}

string const &button::label() const
{
   return m_label;
}
void button::set_label(string const &value)
{
   if (value != m_label)
   {
      m_dirty = true;
      m_label = value;
   }
}
glm::mat4 &button::text_frame(){ return m_text_frame; }
glm::mat4 &button::background_frame(){ return m_background_frame; }
class ninepatch &button::ninepatch(){ return m_ninepatch; }
bool button::trigger() const { return m_trigger; }
void button::set_trigger(bool value){ m_trigger = value; }
class text_buffer &button::text_buffer(){ return m_text_buffer; }

void button::update_size()
{
   slog_trace("button::update_size()");

   if (m_dirty)
   {
      if (style()->font())
      {
         m_text_buffer.begin_print();
         m_text_buffer.print(m_label, 0.0f, 0.0f);
         m_text_buffer.end_print();
         m_bounds = m_text_buffer.bounding_box();

         set_fill_base_pixels(
            m_bounds.max() + 2.0f * style()->padding()
         );
      }
      else
         set_fill_base_pixels(glm::vec2(30.0f, 10.0f));

      m_ninepatch.place(renderer(), 0.0f, 0.0f, fill_base_pixels().x, fill_base_pixels().y);
      m_dirty = false;
   }
}
void button::update_place()
{
   slog_trace("button::update_place()");
   if (size().x != m_bounds.max().x + 2.0f * style()->padding().x)
      m_ninepatch.place(renderer(), 0.0f, 0.0f, size().x, size().y);
}
void button::begin_size(glm::vec2 const &free_size_reference)
{
   update_size();
   area::begin_size(free_size_reference);
}
void button::begin_place(rectangle const &reference, glm::vec2 const &grow_direction)
{
   area::begin_place(reference, grow_direction);
   update_place();
   glm::mat4 a, b;
   create_translation(rect().min() + style()->padding(), a);
   create_translation(rect().min(), b);

   glm::mat4 const &o = renderer()->ortho();
   m_text_frame       = o * a;
   m_background_frame = o * b;
}
void button::draw_self(ui_context &context)
{
   slog_trace("button::draw_self()");

   auto gr = renderer();

   //r->push();

   gr->set_program(style()->ninepatch_style()->program());

   auto &r = *gr->renderer();
   auto t = style()->ninepatch_style()->texture();
   r.set_texture(style()->ninepatch_style()->texture_unit(), t);
   //t->apply(rr, style()->ninepatch_style()->texture_unit());

   gr->begin_edit();
   gr->set_transform(m_background_frame);
   gr->set_color_scale(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
   if (rect().hit(context.mouse))
   {
      if (context.mouse_buttons[0])
      {
         gr->set_color_add(glm::vec4(0.2f, 0.35f, 0.45f, 0.0f));
         m_trigger = true;
      }
      else
      {
         if (trigger())
         {
            auto s = sink().lock();
            if (s)
               s->action(action_source::shared_from_this());
            set_trigger(false);
         }
         gr->set_color_add(glm::vec4(0.1f, 0.2f, 0.35f, 0.0f));
      }
   }
   else
   {
      set_trigger(false);
      gr->set_color_add(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
   }

   gr->end_edit();
   m_ninepatch.render(gr);

   /*  Then draw text  */ 
   if (style()->font())
   {
      gr->blend_alpha();
      gr->begin_edit();
      gr->set_program(style()->program());
      auto t = style()->font()->texture();
      gr->set_texture(style()->texture_unit(), t);
      t->apply(r, style()->texture_unit());

      gr->set_color_add  (glm::vec4(0.00f, 0.00f, 0.00f, 0.0f));
      gr->set_color_scale(glm::vec4(0.72f, 0.72f, 0.72f, 2.0f));
      gr->set_transform(m_text_frame);
      gr->end_edit();
      m_text_buffer.render();
      gr->blend_disable();
   }
   //r->pop();
}

} }
