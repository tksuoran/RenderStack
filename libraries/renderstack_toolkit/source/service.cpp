#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"

using namespace std;

namespace renderstack { namespace toolkit {

service::service(string const &name)
:  m_name(name)
,  m_initialized(false)
{
}

service::~service()
{
}

bool service::initialized() const { return m_initialized; }

void service::initialize()
{
   initialize_service();
   m_initialized = true;
}

bool service::ready() const
{
   return m_dependencies.size() == 0;
}

void service::remove_dependencies(set<shared_ptr<service> > const &remove_set)
{
   for (auto i = remove_set.cbegin(); i != remove_set.cend(); ++i)
      m_dependencies.erase(*i);
}

} }
