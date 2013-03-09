#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/context.hpp"

namespace renderstack { namespace ui {

using namespace std;

/*static*/ shared_ptr<context> context::s_current_context;

context::context()
{
}
context::~context()
{
}
/*static*/ std::shared_ptr<context> context::current()
{
   return s_current_context;
}
/*static*/ void context::deinitialize()
{
   s_current_context.reset();
}
/*static*/ void context::make_current(shared_ptr<context> context)
{
   s_current_context = context;
}

shared_ptr<class gui_renderer> context::gui_renderer()
{
   if (!s_gui_renderer)
   {
      s_gui_renderer = std::make_shared<class gui_renderer>();
   }
   return s_gui_renderer;
}
context_text_buffer &context::text_buffer()
{
   return m_text_buffer;
}
context_style &context::style()
{
   return m_style;
}
context_ninepatch &context::ninepatch()
{
   return m_ninepatch;
}

shared_ptr<renderstack::graphics::index_buffer> context::get_2d_index_buffer()
{
   if (!s_2d_index_buffer)
      s_2d_index_buffer = make_shared<renderstack::graphics::index_buffer>(
         512 * 1024,
         sizeof(unsigned short)
      );
   return s_2d_index_buffer;
}

shared_ptr<renderstack::graphics::vertex_buffer> context::get_2d_vertex_buffer()
{
   if (!s_2d_vertex_buffer)
      s_2d_vertex_buffer = make_shared<renderstack::graphics::vertex_buffer>(
         128 * 1024,
         4 * sizeof(float)
      );

   return s_2d_vertex_buffer;
}

} }
