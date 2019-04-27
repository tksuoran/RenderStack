#ifndef samplers_hpp_renderstack_graphics
#define samplers_hpp_renderstack_graphics

#include "renderstack_graphics/uniform.hpp"
#include <unordered_map>
#include <string>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Sampler;

class Samplers
{
public:
    using Uniform_collection = std::unordered_map<std::string, Uniform>;

    Uniform_collection const &sampler_uniforms();

    const Uniform *sampler(const std::string &key) const;

    void seal();

    Uniform &add(const std::string &name, gl::active_uniform_type::value type, Sampler *sampler);

    std::string str() const;

private:
    Uniform_collection        m_samplers;
    std::vector<unsigned int> m_textures;
};

} // namespace graphics
} // namespace renderstack

#endif
