#include "renderstack_toolkit/service.hpp"
#include "renderstack_toolkit/platform.hpp"

using namespace std;

namespace renderstack
{
namespace toolkit
{

service::service(const std::string &name)
    : m_name(name), m_initialized(false), m_is_registered(false)
{
}

bool service::initialized() const
{
    return m_initialized;
}

void service::initialize()
{
    initialize_service();
    m_initialized = true;
}

bool service::ready() const
{
    return m_dependencies.size() == 0;
}

void service::remove_dependencies(set<shared_ptr<service>> const &remove_set)
{
    for (auto i = remove_set.cbegin(); i != remove_set.cend(); ++i)
        m_dependencies.erase(*i);
}

} // namespace toolkit
} // namespace renderstack
