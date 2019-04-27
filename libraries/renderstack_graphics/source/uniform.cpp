#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <exception>

namespace renderstack
{
namespace graphics
{

using namespace std;

Uniform::Uniform(const std::string &name, Uniform_block *block, size_t count, gl::active_uniform_type::value type)
    : m_name(name)
    , m_count(count)
    , m_type(type)
    , m_block(block)
    , m_index_in_block(block ? block->num_uniforms() : 0)
    , m_offset_in_block(block ? block->offset() : 0)
{
    assert(block != nullptr);
}

Uniform::Uniform(const std::string &name, int location, size_t count, gl::active_uniform_type::value type)
    : m_name(name)
    , m_count(count)
    , m_type(type)
    , m_location(location)
{
}

Uniform::Uniform(const std::string &name, Sampler *sampler, int texture_unit, gl::active_uniform_type::value type)
    : m_name(name)
    , m_count(1)
    , m_type(type)
    , m_texture_unit_index(texture_unit)
    , m_sampler(sampler)
    , m_location(-1)
{
}

// If uniform is in default block, access is index in block
// If uniform is in non-default block, access is offset to Uniform block / buffer range
size_t Uniform::access() const
{
    assert(m_block);
    return m_block->default_block() ? m_index_in_block : m_offset_in_block;
}

bool Uniform::is_array() const
{
    return m_is_array;
}

void Uniform::set_is_array(bool value)
{
    m_is_array = value;
}

int Uniform::location() const
{
    return m_location;
}

int Uniform::index_in_block() const
{
    return m_index_in_block;
}

GLint Uniform::texture_unit_index() const
{
    return m_texture_unit_index;
}

void Uniform::set_texture_unit_index(GLint value)
{
    m_texture_unit_index = value;
}

size_t Uniform::count() const
{
    return m_count;
}

gl::active_uniform_type::value Uniform::type() const
{
    return m_type;
}

precision::value Uniform::precision_qualifier() const
{
    return m_precision;
}

void Uniform::set_precision_qualifier(precision::value value)
{
    m_precision = value;
}

const std::string &Uniform::name() const
{
    return m_name;
}

GLint Uniform::block_index() const
{
    return m_block_index;
}

size_t Uniform::offset_in_block() const
{
    return m_offset_in_block;
}

Sampler *Uniform::sampler() const
{
    return m_sampler;
}

void Uniform::set_sampler(Sampler *value)
{
    m_sampler = value;
}

} // namespace graphics
} // namespace renderstack
