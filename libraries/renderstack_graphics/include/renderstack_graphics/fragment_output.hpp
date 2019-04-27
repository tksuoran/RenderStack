#ifndef vertex_stream_mapping_hpp_renderstack_graphics
#define vertex_stream_mapping_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"

#include <string>

namespace renderstack
{
namespace graphics
{

class Fragment_output
{
public:
    Fragment_output(const std::string &name, gl::fragment_output_type::value type, unsigned int location);

    const std::string &name() const;

    gl::fragment_output_type::value type() const;

    unsigned int location() const;

private:
    std::string                     m_name;
    gl::fragment_output_type::value m_type;
    unsigned int                    m_location;
};

} // namespace graphics
} // namespace renderstack

#endif
