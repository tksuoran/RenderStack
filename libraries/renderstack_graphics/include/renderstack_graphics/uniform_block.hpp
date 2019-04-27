#ifndef uniform_block_hpp_renderstack_graphics
#define uniform_block_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Program;
class Uniform;

class Uniform_block : public std::enable_shared_from_this<Uniform_block>
{
public:
    using Uniform_collection = std::vector<Uniform>;

    // Create uniforms to default block
    Uniform_block(const std::string &name);

    Uniform_block(unsigned int binding_point, const std::string &name);

    Uniform_block(unsigned int binding_point, const std::string &name, const std::string &block_name);

    ~Uniform_block() = default;

    void map_program(renderstack::graphics::Program &program) const;

    Uniform_collection const &uniforms() const;

    Uniform_collection &      uniforms();

    const std::string &name() const;

    const std::string &block_name() const;

    void set_name(const std::string &value);

    unsigned int binding_point() const;

    std::string source(int glsl_shader) const;

    size_t num_uniforms() const;

    size_t size_bytes() const;

    size_t offset() const;

    bool default_block() const;

public:
    void seal();

    const Uniform &add_float(const std::string &name);
    const Uniform &add_float(const std::string &name, unsigned int dimension);
    const Uniform &add_vec2(const std::string &name);
    const Uniform &add_vec2(const std::string &name, unsigned int dimension);
    const Uniform &add_vec3(const std::string &name);
    const Uniform &add_vec3(const std::string &name, unsigned int dimension);
    const Uniform &add_vec4(const std::string &name);
    const Uniform &add_vec4(const std::string &name, unsigned int dimension);
    const Uniform &add_mat4(const std::string &name);
    const Uniform &add_mat4(const std::string &name, unsigned int dimension);
    const Uniform &add_int(const std::string &name);
    const Uniform &add_int(const std::string &name, unsigned int dimension);
    // TODO add_ivecN(), add_uvecN()
    const Uniform &add_uint(const std::string &name);
    const Uniform &add_uint(const std::string &name, unsigned int dimension);

private:
    std::string  m_name;
    std::string  m_block_name;
    bool         m_default_block{false};
    unsigned int m_binding_point{0U};
    size_t       m_offset{0U};

    std::vector<Uniform> m_uniforms;
};

} // namespace graphics
} // namespace renderstack

#endif
