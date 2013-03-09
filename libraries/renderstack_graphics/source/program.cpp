#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_toolkit/file.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstdarg>
#include <cassert>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

#define LOG_CATEGORY &log_program

// #define LOG_LINK 1

namespace renderstack { namespace graphics {

using namespace std;
using namespace gl;
using namespace renderstack::toolkit;

static void log_function(char const *format, ...);

static void log_function(char const *format, ...)
{
   char    buf[4096];
   char    *p = buf;
   va_list args;
   int     n;

   va_start(args, format);
   n = ::vsnprintf(p, sizeof buf - 3, format, args);
   va_end(args);

   p += (n < 0) ? sizeof(buf) - 3 : static_cast<unsigned int>(n);

   while ((p > buf) && isspace(p[-1]))
      *--p = '\0';

#if defined(_WIN32)
   *p++ = '\r';
#endif
   *p++ = '\n';
   *p   = '\0';

   ::printf("%s", buf);
#if defined(_WIN32)
   ::OutputDebugStringA(buf);
#endif
}

string program::format(string const &source)
{
   int         line = 1;
   const char  *head = source.c_str();
   char        c;

   stringstream sb;
   sb << setw(3) << setfill(' ') << line << ": ";

   for (;;)
   {
      c = *head;
      ++head;
      if (c == 0)
         break;

      if (c == '\n')
      {
         ++line;
         sb << '\n' << setw(3) << setfill(' ') << line << ": ";
         continue;
      }
      sb << c;
   }
   return sb.str();
}

static std::string string_replace_all(
   std::string s,
   std::string old,
   std::string new_
)
{
   for (;;)
   {
      auto pos = s.find(old);
      if (pos == string::npos)
         return s;

      s.replace(pos, old.length(), new_);
   }
}

unsigned int program::make_shader(
   shader_type::value   type, 
   string               source_str, 
   string               &compiled_src
)
{
   int status;

   stringstream sb;
   sb << "#version " << m_glsl_version << "\n";
   for (auto i = m_defines.cbegin(); i != m_defines.cend(); ++i)
      sb << "#define " << i->first << " " << i->second << "\n";

   // TODO If we can not use uniform buffers, replace "block_name." with "block_name_"
   bool use_uniform_buffers = 
      renderstack::graphics::configuration::can_use.uniform_buffer_object && 
      (m_glsl_version >= 140) &&
      (renderstack::graphics::configuration::shader_model_version >= 4);

   for (auto i = m_uniform_blocks.cbegin(); i != m_uniform_blocks.cend(); ++i)
   {
      auto block = i->second.lock();
      sb << block->source(m_glsl_version);

      if (!use_uniform_buffers)
      {
         source_str = string_replace_all(
            source_str,
            block->name() + ".",
            block->name() + "_"
         );
      }
   }

   if (m_samplers)
      sb << m_samplers->str();

   sb << source_str;

   unsigned int shader = gl::create_shader(type);
   if (shader == 0)
      throw runtime_error("create_shader() failed\n");

   {
      compiled_src = sb.str();  
      const char *c_source = compiled_src.c_str();
      const GLchar **source = &c_source;

      gl::shader_source(shader, 1, source, nullptr);
      gl::compile_shader(shader);
      gl::get_shader_iv(shader, gl::shader_parameter::compile_status, &status);
      if (status != GL_TRUE)
      {
         GLint length;
         gl::get_shader_iv(shader, gl::shader_parameter::info_log_length, &length);
         vector<char> log(length + 1, 0);
         gl::get_shader_info_log(shader, length, nullptr, &log[0]);
         string f_source = format(*source);
         log_function("Shader compilation failed: ");
         log_function(&log[0]);
         log_function("\n");
         //if(m_defines.size() > 0)
         {
            log_function(f_source.c_str());
            log_function("\n");
         }
         cout << flush;
         cerr << flush;
         throw runtime_error("shader compilation failed");
      }
   }
   return shader;
}

program::program(string const &name, int glsl_version, shared_ptr<class samplers> samplers, shared_ptr<class vertex_stream_mappings> mappings)
:  m_name         (name)
,  m_glsl_version (glsl_version)
,  m_program      (0)
,  m_samplers     (samplers)
{
   m_program = gl::create_program();

   if (mappings)
      mappings->bind_attrib_locations(*this);
   else
      context::current()->global_vertex_stream_mappings()->bind_attrib_locations(*this);
}

void program::bind_attrib_location(int location, string const name)
{
   gl::bind_attrib_location(m_program, location, name.c_str());
}

// TODO We must configure outputs programmatically
void program::bind_frag_data_location(int location, string const &name)
{
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (configuration::shader_model_version >= 4)
   {
      gl::bind_frag_data_location(m_program, location, name.c_str());
   }
   else
#endif
   {
      if (location == 0)
         define(name, "gl_FragColor");
      else
         throw runtime_error("MRT not supported");
   }
}

void program::define(string const &key, string const &value)
{
   m_defines.push_back(make_pair(key, value));
}
int program::get_uniform_location(const char *name)
{
   return gl::get_uniform_location(m_program, name);
}

void program::dump_shaders() const
{
   log() << "Shaders for " << m_name << ":\n";
   for (auto i = m_loaded_shaders.begin(); i != m_loaded_shaders.end(); ++i)
   {
      auto     resource = *i;
      string   f_source = format(resource.compiled_src);

      //  detach old shader, load, compile and attach new
      log() << gl::enum_string(resource.type) << " compiled from " << resource.path << ":\n";
      log() << f_source;
      log() << "\n";
   }
}

void program::set_shader(shader_type::value type, string const &source)
{
   string compiled_src;
   GLuint shader = make_shader(type, source, compiled_src);

   gl::attach_shader(m_program, shader);
}
void program::load_shader(shader_type::value type, string const &path)
{
   string source = read(path);
   string compiled_src;
   GLuint shader = make_shader(type, source, compiled_src);

   gl::attach_shader(m_program, shader);

   loaded_shader resource;
   resource.type              = type;
   resource.path              = path;
   resource.shader            = shader;
   resource.last_good_shader  = shader;
   resource.source            = source;
   resource.compiled_src      = compiled_src;

   m_loaded_shaders.push_back(resource);

   context::current()->shader_monitor().add(resource.path, this);
}
void program::reload()
{
   // TODO resource.source and resource.compiled_src get messedup if loading fails!

   try
   {
      //  Reload all shaders
      for (auto i = m_loaded_shaders.begin(); i != m_loaded_shaders.end(); ++i)
      {
         auto resource = *i;

         //  detach old shader, load, compile and attach new
         cout << "reload detach old " << resource.shader << '\n';
         gl::detach_shader(m_program, resource.shader);
         //  Mark current shader as 0 before compilation in order to
         //  know when to attach the shader back if compilation fails.
         resource.shader = 0;
         string source = read(resource.path);
         string compiled_src;
         resource.shader = make_shader(resource.type, source, compiled_src);
         resource.compiled_src = compiled_src;
         cout << "reload attach new" << resource.shader << '\n';
         gl::attach_shader(m_program, resource.shader);
      }

      link();

      //  Since we are here, linking succeeded. 
      //  Update last known good shader
      for (auto i = m_loaded_shaders.begin(); i != m_loaded_shaders.end(); ++i)
      {
         loaded_shader &resource = *i;

         resource.last_good_shader = resource.shader;
         cout << "considering " << resource.shader << "last good\n";
      }

   }
   catch (...)
   {
      //  New shaders did not work, revert to last known good
      try
      {
         for (auto i = m_loaded_shaders.begin(); i != m_loaded_shaders.end(); ++i)
         {
            auto resource = *i;

            if (resource.shader != resource.last_good_shader)
            {
               log_warn(
                  "fail, detaching and deleting %u and attaching %u",
                  resource.shader, resource.last_good_shader
               );

               if (resource.shader != 0)
               {
                  gl::detach_shader(m_program, resource.shader);
                  gl::delete_shader(resource.shader);
               }
               gl::attach_shader(m_program, resource.last_good_shader);

               resource.shader = resource.last_good_shader;
            }
         }

         log_warn("fail, linking with last known good shaders");
         link();
         log_warn("restoring last known good shaders succeeded");

      }
      catch (...)
      {
         log_error("this should not happen");
      }
   }
}
program &program::load_vs(string const &path)
{
   load_shader(shader_type::vertex_shader, path);
   return *this;
}
program &program::load_tcs(string const &path)
{
   if (m_glsl_version >= 400)
   {
      load_shader(shader_type::tess_control_shader, path);
      return *this;
   }
   else
   {
      throw runtime_error("tesselation shaders are not supported / require GLSL version 4.00");
   }
}
program &program::load_tes(string const &path)
{
   if (m_glsl_version >= 400)
   {
      load_shader(shader_type::tess_evaluation_shader, path);
      return *this;
   }
   else
   {
      throw runtime_error("tesselation shaders are not supported / require GLSL version 4.00");
   }
}
program &program::load_gs(string const &path)
{
   if (m_glsl_version >= 150 /*configuration::can_use.geometry_shaders*/)
   {
      load_shader(shader_type::geometry_shader, path);
      return *this;
   }
   else
   {
      throw runtime_error("geometry shaders are not supported / require GLSL version 1.50");
   }
}
program &program::load_fs(string const &path)
{
   load_shader(shader_type::fragment_shader, path);
   return *this;
}
program &program::set_vs(string const &source)
{
   set_shader(shader_type::vertex_shader, source);
   return *this;
}
program &program::set_tcs(string const &source)
{
   set_shader(shader_type::tess_control_shader, source);
   return *this;
}
program &program::set_tes(string const &source)
{
   set_shader(shader_type::tess_evaluation_shader, source);
   return *this;
}
program &program::set_gs(string const &source)
{
   set_shader(shader_type::geometry_shader, source);
   return *this;
}
program &program::set_fs(string const &source)
{
   set_shader(shader_type::fragment_shader, source);
   return *this;
}
void program::transform_feedback(vector<string> varyings, GLenum buffer_mode)
{
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (m_glsl_version >= 400 /*configuration::can_use.transform_feedback*/)
   {
      vector<char const *> c_array(varyings.size());
      for (size_t i = 0; i < varyings.size(); ++i)
         c_array[i] = varyings[i].c_str();

      gl::transform_feedback_varyings(m_program, static_cast<GLsizei>(varyings.size()), &c_array[0], buffer_mode);
   }
   else
#endif
   {
      (void)varyings;
      (void)buffer_mode;
      throw runtime_error("transform feedback is not supported / require GLSL version 4.00");
   }
}
static char const * const attrib_type_str(GLenum type)
{
   switch (type) {
   case gl::active_attrib_type::int_             : return "int_";
   case gl::active_attrib_type::unsigned_int     : return "unsigned_int";
   case gl::active_attrib_type::float_           : return "float_";
   case gl::active_attrib_type::double_          : return "double_";
   case gl::active_attrib_type::float_vec2       : return "float_vec2";
   case gl::active_attrib_type::float_vec3       : return "float_vec3";
   case gl::active_attrib_type::float_vec4       : return "float_vec4";
   case gl::active_attrib_type::int_vec2         : return "int_vec2";
   case gl::active_attrib_type::int_vec3         : return "int_vec3";
   case gl::active_attrib_type::int_vec4         : return "int_vec4";
   case gl::active_attrib_type::float_mat2       : return "float_mat2";
   case gl::active_attrib_type::float_mat3       : return "float_mat3";
   case gl::active_attrib_type::float_mat4       : return "float_mat4";
   case gl::active_attrib_type::unsigned_int_vec2: return "unsigned_int_vec2";
   case gl::active_attrib_type::unsigned_int_vec3: return "unsigned_int_vec3";
   case gl::active_attrib_type::unsigned_int_vec4: return "unsigned_int_vec4";
   case gl::active_attrib_type::double_mat2      : return "double_mat2";
   case gl::active_attrib_type::double_mat3      : return "double_mat3";
   case gl::active_attrib_type::double_mat4      : return "double_mat4";
   case gl::active_attrib_type::double_mat2x3    : return "double_mat2x3";
   case gl::active_attrib_type::double_mat2x4    : return "double_mat2x4";
   case gl::active_attrib_type::double_mat3x2    : return "double_mat3x2";
   case gl::active_attrib_type::double_mat3x4    : return "double_mat3x4";
   case gl::active_attrib_type::double_mat4x2    : return "double_mat4x2";
   case gl::active_attrib_type::double_mat4x3    : return "double_mat4x3";
   case gl::active_attrib_type::double_vec2      : return "double_vec2";
   case gl::active_attrib_type::double_vec3      : return "double_vec3";
   case gl::active_attrib_type::double_vec4      : return "double_vec4";
   default: return "?";
   };
}
void program::add(weak_ptr<class uniform_block> uniform_block)
{
   m_uniform_blocks[uniform_block.lock()->block_name()] = uniform_block;
}
void program::link()
{
#if defined(LOG_LINK)
   cout << "Linking program " << m_program << ":\n";
   for (auto i = m_loaded_shaders.cbegin(); i != m_loaded_shaders.cend(); ++i)
   {
      cout << i->path << ":\n--------------------------------------------------------------------\n";
      string f_source = format(i->source);
      cout << f_source << "\n--------------------------------------------------------------------\n";
   }

   cout << "\n";
#endif

   gl::link_program(m_program);
   int status;
   gl::get_program_iv(m_program, gl::program_parameter::link_status, &status);
   if (status != GL_TRUE)
   {
      GLint length;
      gl::get_program_iv(m_program, gl::program_parameter::info_log_length, &length);
      string log(length + 1, 0);
      gl::get_program_info_log(m_program, length, nullptr, &log[0]);
      log_function("Program linking failed: ");
      log_function(&log[0]);
      log_function("\n");
      throw runtime_error("program linking failed");
   }

   //  This is required by gl::uniform_1i() calls
   gl::use_program(m_program);

   int buffer_size = 128;
   gl::get_program_iv(
      m_program, 
      gl::program_parameter::active_uniform_max_length,
      &buffer_size
   );

   bool use_uniform_buffers = 
      renderstack::graphics::configuration::can_use.uniform_buffer_object && 
      (m_glsl_version >= 140) &&
      (renderstack::graphics::configuration::shader_model_version >= 4);

   if (use_uniform_buffers)
   {
      int buffer_size2 = 128;
      gl::get_program_iv(
         m_program, 
         gl::program_parameter::active_uniform_block_max_name_length,
         &buffer_size2
      );
      if (buffer_size2 > buffer_size)
         buffer_size = buffer_size2;
   }
   vector<char> buffer(buffer_size + 1);

   int uniform_count;

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (use_uniform_buffers)
   {
      int active_uniform_blocks;
      gl::get_program_iv(m_program, gl::program_parameter::active_uniform_blocks, &active_uniform_blocks);
      if (active_uniform_blocks < 0)
      {
         assert(0);
         active_uniform_blocks = 0;
      }
      //m_uniform_block_names.clear();
      for (unsigned int i = 0; i < static_cast<unsigned int>(active_uniform_blocks); ++i)
      {
         GLsizei length;
         gl::get_active_uniform_block_name(m_program, i, buffer_size, &length, &buffer[0]);
         gl::get_active_uniform_block_iv(
            m_program,
            i,
            gl::active_uniform_block_parameter::uniform_block_active_uniforms,
            &uniform_count
         );

         //  This should never happen
         if (length > buffer_size)
            length = buffer_size;

         buffer[length] = 0;

         string name(&buffer[0], static_cast<size_t>(length));

         auto fi = m_uniform_blocks.find(name);
         if (fi != m_uniform_blocks.end())
         {
            auto block = fi->second.lock();
            unsigned int binding_point = block->binding_point();

            gl::uniform_block_binding(m_program, i, binding_point);
         }
         else
         {
            throw runtime_error("uniform block error");
         }

      }
   }
#endif

   gl::get_program_iv(
      m_program,
      gl::program_parameter::active_uniforms,
      &uniform_count
   );

   GLint   size;
   GLenum  type;
   m_uniforms.clear();
   for (unsigned int i = 0; i < static_cast<unsigned int>(uniform_count); ++i)
   {
      GLsizei length;

      gl::get_active_uniform(
         m_program,
         i,
         buffer_size,
         &length,
         &size,
         &type,
         &buffer[0]
      );

      //  This should never happen
      if (length > buffer_size)
         length = buffer_size;

      buffer[length] = 0;

      int index = gl::get_uniform_location(m_program, &buffer[0]);

      if (index >= 0)
      {
         //Trace.WriteLine("\tUniform " + name + " index " + index);
         string name_str(&buffer[0], static_cast<size_t>(length));

         cout << "name = " << &buffer[0] << " index = " << index << " name_str = " << name_str << "\n";

         m_uniforms[name_str] = make_shared<class uniform>(
            name_str,
            index,
            static_cast<size_t>(size),
            static_cast<gl::active_uniform_type::value>(type)
         );

         if (m_samplers)
         {
            auto sampler_uniform = m_samplers->sampler(name_str);
            if (sampler_uniform)
            {
               GLint texture_unit = sampler_uniform->texture_unit_index();
               //printf("program %s uniform %s bound to texture unit %d\n", m_name.c_str(), name_str.c_str(), texture_unit);
               gl::uniform_1i(index, texture_unit);
            }
         }

      }
   }

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (m_glsl_version >= 400)
   {
      int transform_feedback_varyings = 0;
      int transform_feedback_varying_max_length = 0;
      gl::get_program_iv(m_program, gl::program_parameter::transform_feedback_varyings,  &transform_feedback_varyings);
      gl::get_program_iv(m_program, gl::program_parameter::transform_feedback_varying_max_length, &transform_feedback_varying_max_length);
      if (transform_feedback_varyings < 0)
      {
         assert(0);
         transform_feedback_varyings = 0;
      }
      if (transform_feedback_varying_max_length < 0)
      {
         assert(0);
      }
      string buffer_(transform_feedback_varying_max_length + 1, 0);
      ::GLsizei length;
      ::GLsizei size2;
      ::GLenum  type2;
      for (unsigned int i = 0; i < static_cast<unsigned int>(transform_feedback_varyings); ++i)
      {
         gl::get_transform_feedback_varying(
            m_program,
            i,
            transform_feedback_varying_max_length,
            &length,
            &size2,
            &type2,
            &buffer_[0]
         );

         if (size2 > 1)
            log_function("transform feedback varying %d %s %s[%d]\n", i, attrib_type_str(type2), &buffer_[0], size2);
         else
            log_function("transform feedback varying %d %s %s\n", i, attrib_type_str(type2), &buffer_[0]);

      }
   }
#endif
}
shared_ptr<class uniform> program::uniform(string const &name)
{
   return m_uniforms[name];
}

void program::use()
{
   gl::use_program(m_program);
}

} }
