#ifndef renderstack_ui__action_hpp
#define renderstack_ui__action_hpp

#include "renderstack_toolkit/enable_shared_from_this.hpp"
#include <memory>

namespace renderstack { namespace ui {

class action_sink;

class action_source : public renderstack::toolkit::enable_shared_from_this<class action_source>
{
private:
   std::weak_ptr<class action_sink> m_action_sink;

protected:
   action_source() {}
   action_source(std::weak_ptr<class action_sink> sink)
   :  m_action_sink(sink)
   {
   }

public:
   std::weak_ptr<class action_sink> sink() const { return m_action_sink; }
   void set_sink(std::weak_ptr<class action_sink> value){ m_action_sink = value; }
};

class action_sink : public renderstack::toolkit::enable_shared_from_this<class action_sink>
{
public:
   virtual ~action_sink(){}

   virtual void action(std::weak_ptr<class action_source> source)
   {
      (void)source;
   }
};

} }

#endif
