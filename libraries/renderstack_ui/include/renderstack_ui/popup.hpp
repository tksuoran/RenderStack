#ifndef renderstack_ui__popup_hpp
#define renderstack_ui__popup_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/dock.hpp"

namespace renderstack { namespace ui {

class popup : public dock
{
private:
   std::shared_ptr<class area> m_closed;
   std::shared_ptr<class area> m_open;
   bool                        m_is_open;

public:
   popup(
      std::shared_ptr<class gui_renderer> renderer,
      std::shared_ptr<class area> closed,
      std::shared_ptr<class area> open);

public:
   std::shared_ptr<class area> current     () const { return (m_is_open ? m_open : m_closed); }
   std::shared_ptr<class area> closed      () const { return m_closed; }
   void                        set_closed  (std::shared_ptr<class area> value);
   std::shared_ptr<class area> open        () const { return m_open; }
   void                        set_open    (std::shared_ptr<class area> value);
   bool                        is_open     () const { return m_is_open; }

   void toggle();

   //  Set(true) to open
   //  Set(false) to close
   void set(bool open);
};

} }

#endif
