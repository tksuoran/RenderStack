#include "renderstack_graphics/fragment_output.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace graphics
{

using namespace std;

fragment_output::fragment_output(
    string const &                  name,
    gl::fragment_output_type::value type,
    unsigned int                    location)
    : m_name(name), m_type(type), m_location(location)
{
}

string const &fragment_output::name() const
{
    return m_name;
}

gl::fragment_output_type::value fragment_output::type() const
{
    return m_type;
}

unsigned int fragment_output::location() const
{
    return m_location;
}

} // namespace graphics
} // namespace renderstack
