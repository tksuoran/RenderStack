#ifndef service_hpp_renderstack_toolkit
#define service_hpp_renderstack_toolkit

#include "renderstack_toolkit/platform.hpp"
#include <set>
#include <memory>
#include <string>

namespace renderstack { namespace toolkit {

class service
{
private:
   service(const service &other);

protected:
   service(std::string const &name);
   virtual ~service();

public:

#if 0
   template<typename ... T>
   void initialization_depends_on(std::shared_ptr<service> first, std::shared_ptr<T> ... rest)
   {
      if (first)
         m_dependencies.insert(first);

      initialization_depends_on(rest);
   }
#endif

   void initialization_depends_on(std::shared_ptr<service>a)
   {
      if (a)
         m_dependencies.insert(a);
   }

   virtual void initialize_service() = 0;

   std::string const &name() const { return m_name; }
   bool initialized() const;
   void initialize();
   bool ready() const;
   void remove_dependencies(std::set<std::shared_ptr<service> > const &remove_set);

private:             
   std::string                         m_name;
   bool                                m_initialized;
   std::set<std::shared_ptr<service> > m_dependencies;

};

} }

#endif

