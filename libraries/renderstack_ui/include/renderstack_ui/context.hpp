#ifndef renderstack_ui__context_hpp
#define renderstack_ui__context_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include "renderstack_ui/style.hpp"
#include "renderstack_ui/ninepatch.hpp"

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace renderstack { namespace ui {

class gui_renderer;

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

   context_text_buffer  &text_buffer();
   context_style        &style();
   context_ninepatch    &ninepatch();

   std::shared_ptr<class gui_renderer>                   gui_renderer();
   std::shared_ptr<renderstack::graphics::index_buffer>  get_2d_index_buffer();
   std::shared_ptr<renderstack::graphics::vertex_buffer> get_2d_vertex_buffer();

private:
   context_text_buffer  m_text_buffer;
   context_style        m_style;
   context_ninepatch    m_ninepatch;

   std::shared_ptr<class gui_renderer>                   s_gui_renderer;
   std::shared_ptr<renderstack::graphics::index_buffer>  s_2d_index_buffer;
   std::shared_ptr<renderstack::graphics::vertex_buffer> s_2d_vertex_buffer;

private:
   static std::shared_ptr<context> s_current_context;
};

} }

#endif
