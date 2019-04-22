#ifndef vertex_stream_mapping_hpp_renderstack_graphics
#define vertex_stream_mapping_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"

#include <string>

namespace renderstack
{
namespace graphics
{

class fragment_output
{
public:
    fragment_output(std::string const &name, gl::fragment_output_type::value type, unsigned int location);

    std::string const &             name() const;
    gl::fragment_output_type::value type() const;
    unsigned int                    location() const;

private:
    std::string                     m_name;
    gl::fragment_output_type::value m_type;
    unsigned int                    m_location;
};

} // namespace graphics
} // namespace renderstack

#endif
