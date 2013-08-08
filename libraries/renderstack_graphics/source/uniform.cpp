#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include <exception>
#include <cassert>

namespace renderstack { namespace graphics {

using namespace std;

uniform::uniform(string const &name, std::shared_ptr<uniform_block> block, size_t count, gl::active_uniform_type::value type)
:  m_name               (name)
,  m_count              (count)
,  m_type               (type)
,  m_precision          (precision::highp)
,  m_is_array           (false)
,  m_location           (-2)
,  m_index_in_block     (block->num_uniforms())
,  m_block_index        (-1)
,  m_offset_in_block    (block->offset())
,  m_texture_unit_index (0)
,  m_sampler            (nullptr)
,  m_block              (block)
{
}

uniform::uniform(string const &name, int location, size_t count, gl::active_uniform_type::value type)
:  m_name               (name)
,  m_count              (count)
,  m_type               (type)
,  m_is_array           (false)
,  m_location           (location)
,  m_precision          (precision::highp)
,  m_index_in_block     (0)
,  m_block_index        (-1)
,  m_offset_in_block    (0)
,  m_texture_unit_index (0)
,  m_sampler            (nullptr)
,  m_block              (nullptr)
{
}

// If uniform is in default block, access is index in block
// If uniform is in non-default block, access is offset to uniform block / buffer range
size_t uniform::access() const
{
   assert(m_block);
   return m_block->default_block() ? m_index_in_block : m_offset_in_block;
}

bool uniform::is_array() const
{
   return m_is_array;
}

void uniform::set_is_array(bool value)
{
   m_is_array = value;
}

int uniform::location() const
{
   return m_location;
}

int uniform::index_in_block() const
{
   return m_index_in_block;
}

GLint uniform::texture_unit_index() const
{ 
   return m_texture_unit_index;
}

void uniform::set_texture_unit_index(GLint value)
{
   m_texture_unit_index = value;
}

size_t uniform::count() const
{ 
   return m_count; 
}

gl::active_uniform_type::value uniform::type() const
{ 
   return m_type;
}

precision::value uniform::precision_qualifier() const
{
   return m_precision;
}

void uniform::set_precision_qualifier(precision::value value)
{
   m_precision = value;
}

string const &uniform::name() const
{ 
   return m_name; 
}

GLint uniform::block_index() const 
{ 
   return m_block_index; 
}

size_t uniform::offset_in_block() const 
{ 
   return m_offset_in_block; 
}

shared_ptr<class sampler> uniform::sampler() const 
{ 
   return m_sampler; 
}

void uniform::set_sampler(shared_ptr<class sampler> value)
{
   m_sampler = value;
}


} }
