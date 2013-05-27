#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/popup.hpp"

namespace renderstack { namespace ui {

using namespace std;

popup::popup(
   shared_ptr<class gui_renderer> renderer,
   shared_ptr<class area> closed,
   shared_ptr<class area> open
) 
:  dock     (renderer, renderer->null_padding_style(), orientation::horizontal)
,  m_closed (closed)
,  m_open   (open)
,  m_is_open(false)
{
   add(closed);
}

void popup::toggle()
{
   set(!m_is_open);
}

void popup::set(bool open)
{
   if (open)
   {
      if (!m_is_open)
      {
         m_is_open = true;
         if (m_closed)
            remove(m_closed);

         if (m_open)
            add(m_open);

         //if (parent().lock() != nullptr)
            // window_manager->update();

      }
   }
   else
   {
      if (m_is_open)
      {
         m_is_open = false;
         if (m_open)
            remove(m_open);

         if (m_closed)
            add(m_closed);

         // if (parent().lock() != nullptr)
            //window_manager->update();
      }
   }
}

void popup::set_open(shared_ptr<class area> value)
{
   if (m_open != value)
   {
      toggle(); 
      m_open = value; 
      toggle(); 
   } 
}

void popup::set_closed(shared_ptr<class area> value)
{
   if (m_closed != value)
   {
      toggle(); 
      m_closed = value; 
      toggle(); 
   } 
} 

} }
