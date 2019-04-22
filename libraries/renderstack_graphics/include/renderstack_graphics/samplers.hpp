#ifndef samplers_hpp_renderstack_graphics
#define samplers_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace graphics
{

class sampler;
class uniform;

class samplers
{
public:
    std::vector<std::shared_ptr<uniform>> const &sampler_uniforms();

    std::shared_ptr<uniform> sampler(std::string const &key) const;
    void                     seal();
    std::shared_ptr<uniform> add(std::string const &name, gl::active_uniform_type::value type, std::shared_ptr<class sampler> sampler);
    std::string              str() const;

private:
    std::map<std::string, std::shared_ptr<uniform>> m_sampler_dictionary;
    std::vector<std::shared_ptr<uniform>>           m_samplers;
    std::vector<unsigned int>                       m_textures;
};

} // namespace graphics
} // namespace renderstack

#endif
