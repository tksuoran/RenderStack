#ifndef service_hpp_renderstack_toolkit
#define service_hpp_renderstack_toolkit

#include "renderstack_toolkit/log.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <exception>
#include <memory>
#include <set>
#include <string>

namespace renderstack
{
namespace toolkit
{

class service
{
private:
    service(const service &other);

protected:
    service(std::string const &name);
    virtual ~service();

public:
    void initialization_depends_on(std::shared_ptr<service> a)
    {
        if (a)
        {
            if (!a->is_registered())
            {
                log_write(&log_services,
                          true,
                          LOG_ERROR,
                          "Service %s dependency %s has not been registered as a service",
                          name().c_str(),
                          a->name().c_str());
                throw std::runtime_error("Dependency has not been registered");
            }
            m_dependencies.insert(a);
        }
    }

    virtual void initialize_service() = 0;

    std::string const &name() const
    {
        return m_name;
    }
    bool initialized() const;
    bool is_registered() const
    {
        return m_is_registered;
    }
    void register_as_service()
    {
        m_is_registered = true;
    }
    void unregister()
    {
        m_is_registered = false;
    }
    void                                      initialize();
    bool                                      ready() const;
    void                                      remove_dependencies(std::set<std::shared_ptr<service>> const &remove_set);
    std::set<std::shared_ptr<service>> const &dependencies()
    {
        return m_dependencies;
    }

private:
    std::string                        m_name;
    bool                               m_initialized;
    bool                               m_is_registered;
    std::set<std::shared_ptr<service>> m_dependencies;
};

} // namespace toolkit
} // namespace renderstack

#endif
