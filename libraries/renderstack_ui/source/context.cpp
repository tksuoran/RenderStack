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
context_style &context::style()
{
   return m_style;
}

} }
