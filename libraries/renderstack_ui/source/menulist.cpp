#include "renderstack_ui/context.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/menulist.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_toolkit/math_util.hpp"

namespace renderstack { namespace ui {

void menulist::update()
{
   if (m_ninepatch.size() != rect().size())
      m_ninepatch.place(renderer(), 0.0f, 0.0f, rect().size().x, rect().size().y);
}

void menulist::begin_place(rectangle const &reference, glm::vec2 const &grow_direction)
{
   dock::begin_place(reference, grow_direction);
   glm::mat4 a;
   create_translation(rect().min(), a);
   glm::mat4 const &o = renderer()->ortho();
   m_background_frame = o * a;
}
void menulist::draw_self(ui_context &context)
{
   update();

   auto r = renderer();

   gl::enable(gl::enable_cap::blend);

   r->set_program(style()->ninepatch_style()->program());
   r->set_texture(style()->ninepatch_style()->texture_unit(), style()->ninepatch_style()->texture());
   r->begin_edit();
   r->set_transform(m_background_frame);
   if (rect().hit(context.mouse))
   {
      r->set_color_scale(glm::vec4( 1.00f,  1.00f, 1.0f, 0.125f));
      r->set_color_add  (glm::vec4(-0.25f, -0.25f, 0.1f, 0.0f));
   }
   else
   {
      r->set_color_scale(glm::vec4(1.0f, 1.0f, 1.0f, 0.125f));
      r->set_color_add  (glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
   }
   r->end_edit();
   m_ninepatch.render(r);

   gl::disable(gl::enable_cap::blend);
}

} }
