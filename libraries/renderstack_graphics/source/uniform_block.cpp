#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <sstream>
#include <stdexcept>

namespace renderstack
{
namespace graphics
{

using namespace std;

static char const *const gl_uniform_type_name(gl::active_uniform_type::value type)
{
    switch (type)
    {
        case gl::active_uniform_type::int_: return "int   ";
        case gl::active_uniform_type::int_vec2: return "ivec2 ";
        case gl::active_uniform_type::int_vec3: return "ivec3 ";
        case gl::active_uniform_type::int_vec4: return "ivec4 ";
        case gl::active_uniform_type::unsigned_int: return "uint  ";
        case gl::active_uniform_type::unsigned_int_vec2: return "uvec2 ";
        case gl::active_uniform_type::unsigned_int_vec3: return "uvec3 ";
        case gl::active_uniform_type::unsigned_int_vec4: return "uvec4 ";
        case gl::active_uniform_type::float_: return "float ";
        case gl::active_uniform_type::float_vec2: return "vec2  ";
        case gl::active_uniform_type::float_vec3: return "vec3  ";
        case gl::active_uniform_type::float_vec4: return "vec4  ";
        case gl::active_uniform_type::float_mat4: return "mat4  ";
        default:
            throw runtime_error("unknown uniform type");
    }
}

Uniform_block::Uniform_collection const &Uniform_block::uniforms() const
{
    return m_uniforms;
}

Uniform_block::Uniform_collection &Uniform_block::uniforms()
{
    return m_uniforms;
}

const std::string &Uniform_block::name() const
{
    return m_name;
}

const std::string &Uniform_block::block_name() const
{
    return m_block_name;
}

void Uniform_block::set_name(const std::string &value)
{
    m_name = value;
}

Uniform_block::Uniform_block(const std::string &name)
    : m_name(name)
    , m_block_name(name + "_block")
    , m_default_block(true)
{
}

Uniform_block::Uniform_block(unsigned int binding_point, const std::string &name)
    : m_name(name)
    , m_block_name(name + "_block")
    , m_binding_point(binding_point)
{
    assert((int)binding_point < configuration::max_uniform_buffer_bindings);
}

Uniform_block::Uniform_block(unsigned int binding_point, const std::string &name, const std::string &block_name)
    : m_name(name)
    , m_block_name(block_name)
    , m_binding_point(binding_point)
{
    assert((int)binding_point < configuration::max_uniform_buffer_bindings);
}

unsigned int Uniform_block::binding_point() const
{
    return m_binding_point;
}

string Uniform_block::source(int glsl_version) const
{
    stringstream ss;

    bool use_uniform_buffer = !m_default_block &&
                              renderstack::graphics::configuration::can_use.uniform_buffer_object &&
                              (glsl_version >= 140) &&
                              (renderstack::graphics::configuration::shader_model_version >= 4);

    if (use_uniform_buffer)
    {
        ss << "layout(std140) uniform " << m_block_name << "\n{\n";
        for (auto &uniform : m_uniforms)
        {
            const std::string &name = uniform.name();
            ss << "    ";
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
            ss << precision::desc(uniform->precision_qualifier()) << " ";
#endif
            ss << gl_uniform_type_name(uniform.type()) << " " << name;
            if (uniform.is_array())
            {
                ss << "[" << uniform.count() << "]";
            }

            ss << ";\n";
        }

        ss << "} " << name() << ";\n\n";
    }
    else
    {

        for (auto &uniform : m_uniforms)
        {
            const std::string &name = uniform.name();
            ss << "uniform " << gl_uniform_type_name(uniform.type()) << " " << m_name << "_" << name;
            if (uniform.is_array())
            {
                ss << "[" << uniform.count() << "]";
            }

            ss << ";\n";
        }
    }

    return ss.str();
}

void Uniform_block::seal()
{
    while ((m_offset % configuration::uniform_buffer_offset_alignment) != 0)
    {
        ++m_offset;
    }
}

size_t Uniform_block::num_uniforms() const
{
    return m_uniforms.size();
}

size_t Uniform_block::size_bytes() const
{
    return m_offset;
}

size_t Uniform_block::offset() const
{
    return m_offset;
}

bool Uniform_block::default_block() const
{
    return m_default_block;
}

void Uniform_block::map_program(renderstack::graphics::Program &p) const
{
    for (auto &uniform : m_uniforms)
    {
        stringstream ss;
        ss << m_name << "_" << uniform.name();
        p.map_uniform(uniform.index_in_block(), ss.str());
    }
}

const Uniform &Uniform_block::add_float(const std::string &name)
{
    while ((m_offset % 4) != 0)
    {
        ++m_offset; // align by 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, 1, gl::active_uniform_type::float_);
    m_offset += 4;
    return uniform;
}

const Uniform &Uniform_block::add_float(const std::string &name, unsigned int dimension)
{
    while ((m_offset % 4) != 0)
    {
        ++m_offset; // align by 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, dimension, gl::active_uniform_type::float_);
    uniform.set_is_array(true);
    m_offset += dimension * 4;
    return uniform;
}

const Uniform &Uniform_block::add_vec2(const std::string &name)
{
    while ((m_offset % (2 * 4)) != 0)
    {
        ++m_offset; // align by 2 * 4 bytes
    }

    auto &uniform = m_uniforms.emplace_back(name, this, 1, gl::active_uniform_type::float_vec2);
    m_offset += 2 * 4;
    return uniform;
}

const Uniform &Uniform_block::add_vec2(const std::string &name, unsigned int dimension)
{
    while ((m_offset % (2 * 4)) != 0)
    {
        ++m_offset; // align by 2 * 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, dimension, gl::active_uniform_type::float_vec2);
    uniform.set_is_array(true);
    m_offset += dimension * 2 * 4;
    return uniform;
}

const Uniform &Uniform_block::add_vec3(const std::string &name)
{
    while ((m_offset % (4 * 4)) != 0)
    {
        ++m_offset; // align by 4 * 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, 1, gl::active_uniform_type::float_vec3);
    m_offset += 3 * 4;
    return uniform;
}

const Uniform &Uniform_block::add_vec3(const std::string &name, unsigned int dimension)
{
    while ((m_offset % (4 * 4)) != 0)
    {
        ++m_offset; // align by 4 * 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, dimension,
                                        gl::active_uniform_type::float_vec3);
    uniform.set_is_array(true);
    m_offset += dimension * 4 * 4;
    return uniform;
}

const Uniform &Uniform_block::add_vec4(const std::string &name)
{
    while ((m_offset % (4 * 4)) != 0)
    {
        ++m_offset; // align by 4 * 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, 1, gl::active_uniform_type::float_vec4);
    m_offset += 4 * 4;
    return uniform;
}

const Uniform &Uniform_block::add_vec4(const std::string &name, unsigned int dimension)
{
    while ((m_offset % (4 * 4)) != 0)
    {
        ++m_offset; // align by 4 * 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, dimension, gl::active_uniform_type::float_vec4);
    uniform.set_is_array(true);
    m_offset += dimension * 4 * 4;
    return uniform;
}

const Uniform &Uniform_block::add_mat4(const std::string &name)
{
    while ((m_offset % (4 * 4)) != 0)
    {
        ++m_offset; // align by 4 * 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, 1, gl::active_uniform_type::float_mat4);
    m_offset += 16 * 4;
    return uniform;
}

const Uniform &Uniform_block::add_mat4(const std::string &name, unsigned int dimension)
{
    while ((m_offset % (4 * 4)) != 0)
    {
        ++m_offset; // align by 4 * 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, dimension, gl::active_uniform_type::float_vec4);
    uniform.set_is_array(true);
    m_offset += dimension * 16 * 4;
    return uniform;
}

const Uniform &Uniform_block::add_int(const std::string &name)
{
    while ((m_offset % 4) != 0)
    {
        ++m_offset; // align by 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, 1, gl::active_uniform_type::int_);
    m_offset += 4;
    return uniform;
}

const Uniform &Uniform_block::add_int(const std::string &name, unsigned int dimension)
{
    while ((m_offset % 4) != 0)
    {
        ++m_offset; // align by 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, dimension, gl::active_uniform_type::int_);
    uniform.set_is_array(true);
    m_offset += dimension * 4;
    return uniform;
}

const Uniform &Uniform_block::add_uint(const std::string &name)
{
    while ((m_offset % 4) != 0)
    {
        ++m_offset; // align by 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, 1, gl::active_uniform_type::unsigned_int);
    m_offset += 4;
    return uniform;
}

const Uniform &Uniform_block::add_uint(const std::string &name, unsigned int dimension)
{
    while ((m_offset % 4) != 0)
    {
        ++m_offset; // align by 4 bytes
    }
    auto &uniform = m_uniforms.emplace_back(name, this, dimension, gl::active_uniform_type::unsigned_int);
    uniform.set_is_array(true);
    m_offset += dimension * 4;
    return uniform;
}

} // namespace graphics
} // namespace renderstack
