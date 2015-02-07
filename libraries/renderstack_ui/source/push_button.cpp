#include "renderstack_ui/context.hpp"
#include "renderstack_ui/push_button.hpp"
#include "renderstack_ui/ninepatch_style.hpp"

namespace renderstack { namespace ui {

using namespace std;

push_button::push_button(
   shared_ptr<class gui_renderer> renderer,
   string const &label,
   shared_ptr<class style> style
)
:  button(renderer, label, style)
{
   set_name(label);
}

void push_button::draw_self(ui_context &context)
{
   auto gr = renderer();

   //  First draw ninepatch
   gr->set_program(style()->ninepatch_style()->program());
   gr->set_texture(style()->ninepatch_style()->texture_unit(), style()->ninepatch_style()->texture());
   gr->set_transform(background_frame());
   gr->set_color_scale(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

   if (rect().hit(context.mouse))
   {
      if (context.mouse_buttons[0])
      {
         if (m_pressed)
            gr->set_color_add(glm::vec4(0.3f, 0.3f, 0.7f, 0.0f));
         else
            gr->set_color_add(glm::vec4(0.6f, 0.6f, 0.8f, 0.0f));

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
            gr->set_color_add(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
         else
            gr->set_color_add(glm::vec4(0.72f, 0.72f, 0.72f, 0.0f));
      }
   }
   else
   {
      set_trigger(false);
      if (m_pressed)
         gr->set_color_add(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
      else
         gr->set_color_add(glm::vec4(0.72f, 0.72f, 0.72f, 0.0f));
   }

   ninepatch().render(gr);

   /*  Then draw text  */ 
   if (style()->font())
   {
      gr->blend_alpha();
      gr->set_program(style()->program());
      gr->set_texture(style()->texture_unit(), style()->font()->texture());

      // TODO Font shader does not currently have color scale
      //gr->set_color_add(glm::vec4(0.00f, 0.00f, 0.00f, 0.0f));

      if (m_pressed)
         gr->set_color_scale(glm::vec4(1.0f, 1.0f, 1.0f, 2.0f));
      else
         gr->set_color_scale(glm::vec4(0.0f, 0.0f, 0.0f, 2.0f));

      gr->set_transform(text_frame());
      text_buffer().render();
      gr->blend_disable();
   }
}

} }
