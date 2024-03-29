#if 0
#    ifndef renderstack_ui__context_hpp
#        define renderstack_ui__context_hpp

#        include "renderstack_toolkit/platform.hpp"
#        include "renderstack_ui/ninepatch.hpp"
#        include "renderstack_ui/style.hpp"
#        include "renderstack_ui/text_buffer.hpp"

#        include <map>
#        include <memory>
#        include <string>
#        include <vector>

namespace renderstack { namespace ui {

class Gui_renderer;

class context
{
private:

public:
   context();
   ~context();

public:
   static std::shared_ptr<context> current();
   static void make_current(std::shared_ptr<context> context);
   static void deinitialize();

   context_style        &style();
   context_ninepatch    &ninepatch();

   std::shared_ptr<gui_renderer> gui_renderer();

private:
   context_style        m_style;

   std::shared_ptr<Gui_renderer> s_gui_renderer;

private:
   static std::shared_ptr<context> s_current_context;
};

} }

#    endif

#endif
