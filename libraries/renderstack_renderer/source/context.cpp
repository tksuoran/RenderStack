#include "renderstack_toolkit/platform.hpp"
#include "renderstack_renderer/context.hpp"
#include "renderstack_renderer/renderer.hpp"

namespace renderstack { namespace renderer {

using namespace std;

shared_ptr<context> context::s_current_context;

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
/*static*/ void context::make_current(std::shared_ptr<context> context)
{
   s_current_context = context;
}
/*static*/ void context::deinitialize()
{
   s_current_context.reset();
}

std::shared_ptr<class renderer> context::renderer()
{
   if (!s_renderer)
      s_renderer = make_shared<class renderer>();

   return s_renderer;
}

} }
