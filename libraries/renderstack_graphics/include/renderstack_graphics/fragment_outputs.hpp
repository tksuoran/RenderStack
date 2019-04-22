#ifndef fragment_outputs_hpp_renderstack_graphics
#define fragment_outputs_hpp_renderstack_graphics

#include "renderstack_graphics/fragment_output.hpp"
#include "renderstack_toolkit/platform.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace graphics
{

class program;

class fragment_outputs : public std::enable_shared_from_this<fragment_outputs>
{
public:
    void clear();
    void add(std::string const &name, gl::fragment_output_type::value type, unsigned int location);

    void bind(program &program);

    std::string source(int glsl_version) const;

private:
    std::vector<std::shared_ptr<fragment_output>> m_outputs;
};

} // namespace graphics
} // namespace renderstack

#endif
