#include "renderstack_ui/context.hpp"
#include "renderstack_ui/push_button.hpp"
#include "renderstack_ui/ninepatch_style.hpp"

namespace renderstack { namespace ui {

using namespace std;

push_button::push_button(string const &label, shared_ptr<class style> style)
:  button(label, style)
{
   set_name(label);
}

void push_button::draw_self(ui_context &context)
{
   auto uc = context::current();
   auto r = uc->gui_renderer();
   // Renderer.Push();

   //  First draw ninepatch
   r->set_program(style()->background_program());
   r->set_texture(style()->background_texture_unit(), style()->ninepatch_style()->texture());
   r->begin_edit();
   r->set_transform(background_frame());
   r->set_color_scale(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

   if (rect().hit(context.mouse))
   {
      if (context.mouse_buttons[0])
      {
         if (m_pressed)
            r->set_color_add(glm::vec4(0.3f, 0.3f, 0.7f, 0.0f));
         else
            r->set_color_add(glm::vec4(0.6f, 0.6f, 0.8f, 0.0f));

         set_trigger(true);
      }
      else
      {
         if (trigger()) 
         {
            auto s = sink().lock();
            if (s)
               s->action(action_source::shared_from_this());
            else
               m_pressed = !m_pressed;

            set_trigger(false);
         }

         if (m_pressed)
            r->set_color_add(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
         else
            r->set_color_add(glm::vec4(0.72f, 0.72f, 0.72f, 0.0f));
      }
   }
   else
   {
      set_trigger(false);
      if (m_pressed)
         r->set_color_add(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
      else
         r->set_color_add(glm::vec4(0.72f, 0.72f, 0.72f, 0.0f));
   }

   r->end_edit();
   ninepatch().render();

   /*  Then draw text  */ 
   if (style()->font())
   {
      gl::enable(gl::enable_cap::blend);
      r->begin_edit();
      r->set_program(style()->foreground_program());
      r->set_texture(style()->foreground_texture_unit(), style()->font()->texture());
      r->set_color_add(glm::vec4(0.00f, 0.00f, 0.00f, 0.0f));

      if (m_pressed)
         r->set_color_scale(glm::vec4(1.0f, 1.0f, 1.0f, 2.0f));
      else
         r->set_color_scale(glm::vec4(0.0f, 0.0f, 0.0f, 2.0f));

      r->set_transform(text_frame());
      r->end_edit();
      text_buffer().render();
      gl::disable(gl::enable_cap::blend);
   }
}

} }
