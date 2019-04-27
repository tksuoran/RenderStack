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

class Program;

class Fragment_outputs : public std::enable_shared_from_this<Fragment_outputs>
{
public:
    void clear();
    void add(const std::string &name, gl::fragment_output_type::value type, unsigned int location);

    void bind(Program &program);

    std::string source(int glsl_version) const;

private:
    std::vector<std::shared_ptr<Fragment_output>> m_outputs;
};

} // namespace graphics
} // namespace renderstack

#endif
