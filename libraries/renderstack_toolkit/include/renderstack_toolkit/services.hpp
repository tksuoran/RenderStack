#ifndef services_hpp_renderstack_toolkit
#define services_hpp_renderstack_toolkit

#include "renderstack_toolkit/log.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <memory>
#include <stdexcept>

namespace renderstack
{
namespace toolkit
{

class services
{
public:
    services();

    void add(std::shared_ptr<renderstack::toolkit::service> s);
    void cleanup_services();
    void initialize_services();

private:
    std::set<std::shared_ptr<renderstack::toolkit::service>> m_services;
};

} // namespace toolkit
} // namespace renderstack

#endif
