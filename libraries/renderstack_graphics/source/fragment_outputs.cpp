#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/fragment_outputs.hpp"
#include "renderstack_graphics/log.hpp"
#include <sstream>
#include <stdexcept>

#define LOG_CATEGORY &log_fragment_outputs

namespace renderstack { namespace graphics {

using namespace std;
using namespace renderstack::toolkit;

static char const * const gl_fragment_output_type_name(gl::fragment_output_type::value type)
{
   switch (type)
   {
   case gl::active_uniform_type::int_:                return "int    ";
   case gl::active_uniform_type::int_vec2:            return "ivec2  ";
   case gl::active_uniform_type::int_vec3:            return "ivec3  ";
   case gl::active_uniform_type::int_vec4:            return "ivec4  ";
   case gl::active_uniform_type::unsigned_int:        return "uint   ";
   case gl::active_uniform_type::unsigned_int_vec2:   return "uvec2  ";
   case gl::active_uniform_type::unsigned_int_vec3:   return "uvec3  ";
   case gl::active_uniform_type::unsigned_int_vec4:   return "uvec4  ";
   case gl::active_uniform_type::float_:              return "float  ";
   case gl::active_uniform_type::float_vec2:          return "vec2   ";
   case gl::active_uniform_type::float_vec3:          return "vec3   ";
   case gl::active_uniform_type::float_vec4:          return "vec4   ";
   case gl::active_uniform_type::double_:             return "double ";
   case gl::active_uniform_type::double_vec2:         return "dvec2  ";
   case gl::active_uniform_type::double_vec3:         return "dvec3  ";
   case gl::active_uniform_type::double_vec4:         return "dvec4  ";
   default:
      throw runtime_error("unknown fragment outout type");
   }
}

void fragment_outputs::clear()
{
   m_outputs.clear();
}

void fragment_outputs::add(
   string const &name,
   gl::fragment_output_type::value type,
   unsigned int location
)
{
   m_outputs.push_back(
      make_shared<fragment_output>(
         name,
         type,
         location
      )
   );
}

void fragment_outputs::bind(program &program)
{
   for (auto i = m_outputs.begin(); i != m_outputs.end(); ++i)
      program.bind_frag_data_location((*i)->location(), (*i)->name());
}

string fragment_outputs::source(int glsl_version) const
{
   stringstream ss;

   for (auto i = m_outputs.begin(); i != m_outputs.end(); ++i)
   {
      auto &f = *i;
      if (glsl_version >= 330)
      {
         ss << "layout(location = " << f->location() << ") ";
      }
      ss << "out ";
      ss << gl_fragment_output_type_name(f->type()) << " ";
      ss << f->name() << ";\n";
   }

   return ss.str();
}

} }
