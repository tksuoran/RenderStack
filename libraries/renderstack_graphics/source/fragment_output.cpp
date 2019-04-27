#include "renderstack_graphics/fragment_output.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace graphics
{

Fragment_output::Fragment_output(const std::string &             name,
                                 gl::fragment_output_type::value type,
                                 unsigned int                    location)
    : m_name(name), m_type(type), m_location(location)
{
}

const std::string &Fragment_output::name() const
{
    return m_name;
}

gl::fragment_output_type::value Fragment_output::type() const
{
    return m_type;
}

unsigned int Fragment_output::location() const
{
    return m_location;
}

} // namespace graphics
} // namespace renderstack
