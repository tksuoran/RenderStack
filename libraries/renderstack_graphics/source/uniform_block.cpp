#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform.hpp"
#include <cassert>
#include <stdexcept>
#include <sstream>

namespace renderstack { namespace graphics {

using namespace std;


static char const * const gl_uniform_type_name(gl::active_uniform_type::value type)
{
   switch (type)
   {
   case gl::active_uniform_type::int_:                return "int   ";
   case gl::active_uniform_type::int_vec2:            return "ivec2 ";
   case gl::active_uniform_type::int_vec3:            return "ivec3 ";
   case gl::active_uniform_type::int_vec4:            return "ivec4 ";
   case gl::active_uniform_type::unsigned_int:        return "uint  ";
   case gl::active_uniform_type::unsigned_int_vec2:   return "uvec2 ";
   case gl::active_uniform_type::unsigned_int_vec3:   return "uvec3 ";
   case gl::active_uniform_type::unsigned_int_vec4:   return "uvec4 ";
   case gl::active_uniform_type::float_:              return "float ";
   case gl::active_uniform_type::float_vec2:          return "vec2  ";
   case gl::active_uniform_type::float_vec3:          return "vec3  ";
   case gl::active_uniform_type::float_vec4:          return "vec4  ";
   case gl::active_uniform_type::float_mat4:          return "mat4  ";
   default:
      throw runtime_error("unknown uniform type");
   }
}

uniform_block::uniform_collection const &uniform_block::uniforms() const
{
   return m_uniforms;
}

uniform_block::uniform_collection &uniform_block::uniforms()
{
   return m_uniforms;
}

string const &uniform_block::name() const
{ 
   return m_name; 
}

string const &uniform_block::block_name() const
{ 
   return m_block_name; 
}

void uniform_block::set_name(string const &value)
{
   m_name = value;
}

uniform_block::uniform_block(string const &name)
:  m_name         (name)
,  m_block_name   (name + "_block")
,  m_default_block(true)
,  m_binding_point(0)
,  m_offset       (0)
{
}

uniform_block::uniform_block(unsigned int binding_point, string const &name)
:  m_name         (name)
,  m_block_name   (name + "_block")
,  m_default_block(false)
,  m_binding_point(binding_point)
,  m_offset       (0)
{
   assert((int)binding_point < configuration::max_uniform_buffer_bindings);
}

uniform_block::uniform_block(unsigned int binding_point, string const &name, string const &block_name)
:  m_name         (name)
,  m_block_name   (block_name)
,  m_default_block(false)
,  m_binding_point(binding_point)
,  m_offset       (0)
{
   assert((int)binding_point < configuration::max_uniform_buffer_bindings);
}

unsigned int uniform_block::binding_point() const
{
   return m_binding_point; 
}

string uniform_block::source(int glsl_version) const
{
   stringstream ss;

   bool use_uniform_buffer = !m_default_block && 
      renderstack::graphics::configuration::can_use.uniform_buffer_object && 
      (glsl_version >= 140) &&
      (renderstack::graphics::configuration::shader_model_version >= 4);

   if (use_uniform_buffer)
   {
      ss << "layout(std140) uniform " << m_block_name << "\n{\n";
      for (uniform_collection::const_iterator i = uniforms().cbegin(); i != uniforms().cend(); ++i)
      {
         auto &uniform = *i;
         string const &name = uniform->name();
         ss << "    ";
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
         ss << precision::desc(uniform->precision_qualifier()) << " ";
#endif
         ss << gl_uniform_type_name(uniform->type()) << " " << name;
         if (uniform->is_array())
            ss << "[" << uniform->count() << "]";

         ss << ";\n";
      }

      ss << "} " << name() << ";\n\n";

   }
   else
   {

      for (uniform_collection::const_iterator i = uniforms().cbegin(); i != uniforms().cend(); ++i)
      {
         auto &uniform = *i;
         string const &name = uniform->name();
         ss << "uniform " << gl_uniform_type_name(uniform->type()) << " " << m_name << "_" << name;
         if (uniform->is_array())
            ss << "[" << uniform->count() << "]";

         ss << ";\n";
      }
   }

   return ss.str();
}

void uniform_block::seal()
{
   while ((m_offset % configuration::uniform_buffer_offset_alignment) != 0)
      ++m_offset;
}

size_t uniform_block::num_uniforms() const 
{ 
   return m_uniforms.size();
}

size_t uniform_block::size_bytes() const 
{ 
   return m_offset;
}

size_t uniform_block::offset() const 
{ 
   return m_offset;
}

bool uniform_block::default_block() const
{
   return m_default_block;
}

void uniform_block::map_program(shared_ptr<renderstack::graphics::program> p) const
{
   for (uniform_collection::const_iterator i = uniforms().cbegin(); i != uniforms().cend(); ++i)
   {
      stringstream ss;
      auto u = *i;
      ss << m_name << "_" << u->name();
      p->map_uniform(u->index_in_block(), ss.str());
   }
}

shared_ptr<uniform> uniform_block::add_float(string const &name)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      1,
      gl::active_uniform_type::float_
   );
   uniforms().push_back(uniform);
   m_offset += 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_float(string const &name, unsigned int dimension)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      dimension,
      gl::active_uniform_type::float_
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   m_offset += dimension * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec2(string const &name)
{
   while ((m_offset % (2 * 4)) != 0) ++m_offset; // align by 2 * 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      1,
      gl::active_uniform_type::float_vec2
   );
   uniforms().push_back(uniform);
   m_offset += 2 * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec2(string const &name, unsigned int dimension)
{
   while ((m_offset % (2 * 4)) != 0) ++m_offset; // align by 2 * 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      dimension,
      gl::active_uniform_type::float_vec2
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   m_offset += dimension * 2 * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec3(string const &name)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      1,
      gl::active_uniform_type::float_vec3
   );
   uniforms().push_back(uniform);
   m_offset += 3 * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec3(string const &name, unsigned int dimension)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      dimension,
      gl::active_uniform_type::float_vec3
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   m_offset += dimension * 4 * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec4(string const &name)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      1,
      gl::active_uniform_type::float_vec4
   );
   uniforms().push_back(uniform);
   m_offset += 4 * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec4(string const &name, unsigned int dimension)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      dimension,
      gl::active_uniform_type::float_vec4
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   m_offset += dimension * 4 * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_mat4(string const &name)
{
   while((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      1,
      gl::active_uniform_type::float_mat4
   );
   uniforms().push_back(uniform);
   m_offset += 16 * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_mat4(string const &name, unsigned int dimension)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      dimension,
      gl::active_uniform_type::float_vec4
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   m_offset += dimension * 16 * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_int(string const &name)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      1,
      gl::active_uniform_type::int_
   );
   uniforms().push_back(uniform);
   m_offset += 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_int(string const &name, unsigned int dimension)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      dimension,
      gl::active_uniform_type::int_
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   m_offset += dimension * 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_uint(string const &name)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      1,
      gl::active_uniform_type::unsigned_int
   );
   uniforms().push_back(uniform);
   m_offset += 4;
   return uniform;
}

shared_ptr<uniform> uniform_block::add_uint(string const &name, unsigned int dimension)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   auto uniform = make_shared<class uniform>(
      name,
      shared_from_this(),
      dimension,
      gl::active_uniform_type::unsigned_int
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   m_offset += dimension * 4;
   return uniform;
}

} }
