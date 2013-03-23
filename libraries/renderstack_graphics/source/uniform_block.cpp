#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp" // #define USE_EMBEDDED
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/configuration.hpp"
#include <stdexcept>
#include <sstream>

namespace renderstack { namespace graphics {

using namespace std;


static char const * const gl_uniform_type_name(gl::active_uniform_type::value type)
{
   switch (type)
   {
   case gl::active_uniform_type::int_:         return "int  ";
   case gl::active_uniform_type::unsigned_int: return "uint ";
   case gl::active_uniform_type::float_:       return "float";
   case gl::active_uniform_type::float_vec2:   return "vec2 ";
   case gl::active_uniform_type::float_vec3:   return "vec3 ";
   case gl::active_uniform_type::float_vec4:   return "vec4 ";
   case gl::active_uniform_type::float_mat4:   return "mat4 ";
   default: throw std::runtime_error("unknown uniform type");
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

std::string const &uniform_block::name() const
{ 
   return m_name; 
}

std::string const &uniform_block::block_name() const
{ 
   return m_block_name; 
}

void uniform_block::set_name(string const &value)
{
   m_name = value;
}

uniform_block::uniform_block(string const &name, string const &block_name, unsigned int binding_point)
:  m_name(name)
,  m_block_name(block_name)
,  m_binding_point(binding_point)
,  m_offset(0)
{
#if 0
   if (m_binding_point > configuration::max_uniform_buffer_bindings)
      throw std::runtime_error("uniform_block binding point too high");
#endif
}

unsigned int uniform_block::binding_point() const
{
   return m_binding_point; 
}

string uniform_block::source(int glsl_version) const
{
   stringstream ss;

   bool use_uniform_buffers = 
      renderstack::graphics::configuration::can_use.uniform_buffer_object && 
      (glsl_version >= 140) &&
      (renderstack::graphics::configuration::shader_model_version >= 4);

   if (use_uniform_buffers)
   {
      ss << "layout(std140) uniform " << m_block_name << "\n{\n";
      for (uniform_collection::const_iterator i = uniforms().cbegin(); i != uniforms().cend(); ++i)
      {
         auto uniform = *i;
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
         auto uniform = *i;
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

size_t uniform_block::size() const 
{ 
   return m_offset;
}

shared_ptr<uniform> uniform_block::add_float(string const &name)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      1,
      gl::active_uniform_type::float_, 
      uniform_offset
   );
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_float(string const &name, unsigned int dimension)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += dimension * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      dimension,
      gl::active_uniform_type::float_, 
      uniform_offset
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec2(string const &name)
{
   while ((m_offset % (2 * 4)) != 0) ++m_offset; // align by 2 * 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += 2 * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      1,
      gl::active_uniform_type::float_vec2, 
      uniform_offset
   );
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec2(string const &name, unsigned int dimension)
{
   while ((m_offset % (2 * 4)) != 0) ++m_offset; // align by 2 * 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += dimension * 2 * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      dimension,
      gl::active_uniform_type::float_vec2, 
      uniform_offset
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec3(string const &name)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += 4 * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      1,
      gl::active_uniform_type::float_vec3, 
      uniform_offset
   );
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec3(string const &name, unsigned int dimension)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += dimension * 4 * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      dimension,
      gl::active_uniform_type::float_vec3, 
      uniform_offset
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec4(string const &name)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += 4 * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      1,
      gl::active_uniform_type::float_vec4, 
      uniform_offset
   );
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_vec4(string const &name, unsigned int dimension)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += dimension * 4 * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      dimension,
      gl::active_uniform_type::float_vec4, 
      uniform_offset
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_mat4(string const &name)
{
   while((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += 16 * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      1,
      gl::active_uniform_type::float_mat4, 
      uniform_offset
   );
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_mat4(string const &name, unsigned int dimension)
{
   while ((m_offset % (4 * 4)) != 0) ++m_offset; // align by 4 * 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += dimension * 16 * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      dimension,
      gl::active_uniform_type::float_vec4, 
      uniform_offset
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_int(string const &name)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      1,
      gl::active_uniform_type::int_, 
      uniform_offset
   );
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_int(string const &name, unsigned int dimension)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += dimension * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      dimension,
      gl::active_uniform_type::int_, 
      uniform_offset
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_uint(string const &name)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      1,
      gl::active_uniform_type::unsigned_int, 
      uniform_offset
   );
   uniforms().push_back(uniform);
   return uniform;
}

shared_ptr<uniform> uniform_block::add_uint(string const &name, unsigned int dimension)
{
   while ((m_offset % 4) != 0) ++m_offset; // align by 4 bytes
   size_t uniform_offset = m_offset;
   m_offset += dimension * 4;
   auto uniform = make_shared<class uniform>(
      name,
      static_cast<GLint>(uniforms().size()),
      dimension,
      gl::active_uniform_type::unsigned_int, 
      uniform_offset
   );
   uniform->set_is_array(true);
   uniforms().push_back(uniform);
   return uniform;
}

} }
