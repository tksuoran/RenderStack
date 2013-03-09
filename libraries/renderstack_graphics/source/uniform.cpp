#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/uniform.hpp"
#include <exception>

namespace renderstack { namespace graphics {

using namespace std;

uniform::uniform(std::string const &name, GLint index, size_t count, gl::active_uniform_type::value type, size_t offset)
:   m_name      (name)
,   m_count     (count)
,   m_type      (type)
,   m_index     (index)
,   m_block     (-1)
,   m_offset    (offset)
,   m_texture_unit_index(0)
//,   m_sampler   (nullptr)
,   m_is_array  (false)
{
}

uniform::uniform(std::string const &name, GLint index, size_t count, gl::active_uniform_type::value type)
:   m_name      (name)
,   m_count     (count)
,   m_type      (type)
,   m_index     (index)
,   m_block     (-1)
,   m_offset    (0)
,   m_texture_unit_index(0)
//,   m_sampler   (nullptr)
,   m_is_array  (false)
{
}

bool uniform::is_array() const
{
   return m_is_array;
}

void uniform::set_is_array(bool value)
{
   m_is_array = value;
}

int uniform::index() const
{
   return m_index;
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

std::string const &uniform::name() const
{ 
   return m_name; 
}

GLint uniform::block() const 
{ 
   return m_block; 
}

size_t uniform::offset() const 
{ 
   return m_offset; 
}

shared_ptr<class sampler> uniform::sampler() const 
{ 
   return m_sampler; 
}

void uniform::set_sampler(shared_ptr<class sampler> value)
{
   m_sampler = value;
}

void uniform::program_set_index(GLint index)
{
   m_index = index;
}

} }
