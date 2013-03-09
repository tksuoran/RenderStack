#ifndef renderstack_renderer__context_hpp
#define renderstack_renderer__context_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_renderer/renderer.hpp"
#include <memory>
#include <vector>
#include <string>
#include <map>

namespace renderstack { namespace renderer {

class context
{
public:
   context();
   ~context();

public:
   static std::shared_ptr<context> current();
   static void make_current(std::shared_ptr<context> context);
   static void deinitialize();

   std::shared_ptr<class renderer> renderer();

private:
   static std::shared_ptr<context> s_current_context;

   std::shared_ptr<class renderer> s_renderer;

};

} }

#endif
