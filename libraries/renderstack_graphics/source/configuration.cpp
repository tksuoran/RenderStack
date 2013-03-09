#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <list>

log_category graphics_configuration(C_GREEN, C_GRAY, LOG_TRACE);
#define LOG_CATEGORY &graphics_configuration

namespace renderstack { namespace graphics {

using namespace std;

bool  configuration::use_gl1  = false;
bool  configuration::intel    = false;

int   configuration::gl_version;
int   configuration::glsl_version;
int   configuration::shader_model_version;

bool  configuration::core_profile                         = false;
bool  configuration::compatibility_profile                = false;
bool  configuration::forward_compatible                   = false;
bool  configuration::use_binary_shaders                   = false;
bool  configuration::use_integer_polygon_ids              = false;
bool  configuration::use_vertex_array_object              = true;
bool  configuration::must_use_vertex_array_object         = false;

configuration::can_use_t configuration::can_use;

bool  configuration::throw_program_exceptions             = true;
unsigned int   configuration::default_vao                 = 0;
int   configuration::max_texture_size                     = 64;
int   configuration::max_3d_texture_size                  = 0;
int   configuration::max_cube_map_texture_size            = 0;
int   configuration::max_texture_buffer_size              = 0;
int   configuration::max_texture_units                    = 0; /* fixed function */
int   configuration::max_texture_image_units              = 0; /* shaders */
int   configuration::max_combined_texture_image_units     = 0;
int   configuration::max_uniform_block_size               = 0;
int   configuration::max_uniform_buffer_bindings          = 8;
int   configuration::max_vertex_uniform_blocks            = 0;
int   configuration::max_fragment_uniform_blocks          = 0;
int   configuration::max_geometry_uniform_blocks          = 0;
int   configuration::max_tess_control_uniform_blocks      = 0;
int   configuration::max_tess_evaluation_uniform_blocks   = 0;
unsigned int configuration::uniform_buffer_offset_alignment      = 1;

static vector<string> split(string text, char separator)
{
   vector<string> result;
   size_t length = text.size();
   size_t span_start = numeric_limits<size_t>::max();
   for (size_t i = 0; i < length; ++i)
   {
      char c = text[i];
      if (c == separator)
      {
         if (span_start != numeric_limits<size_t>::max())
         {
            size_t span_length = i - span_start;
            if (span_length > 0)
            {
               string span = text.substr(span_start, span_length);
               result.push_back(span);
            }
            span_start = numeric_limits<size_t>::max();
         }
      }
      else
      {
         if (span_start == numeric_limits<size_t>::max())
         {
            span_start = i;
         }
      }
   }
   if (span_start != numeric_limits<size_t>::max())
   {
      if (length > span_start)
      {
         size_t span_length = length - span_start;
         string span = text.substr(span_start, span_length);
         result.push_back(span);
      }
   }
   return result;
}

static string digits_only(string s)
{
   size_t size = s.size();
   for (size_t i = 0; i < size; ++i)
   {
      if (::isdigit(s[i]) == 0)
      {
         if (i == 0)
            return "";
         else
            return s.substr(0, i);
      }
   }
   return s;
}

static bool contains(vector<string> const &collection, string const &key)
{
   auto i = find(collection.begin(), collection.end(), key);
   return i != collection.end();
}

static string get_string(int e)
{
   const GLubyte *gl_str = gl::get_string(static_cast<GLenum>(e));
   const char *c_str = reinterpret_cast<const char*>(gl_str);
   return (c_str != nullptr) ? string(c_str) : string();
}

void configuration::check(vector<string> const &extensions, bool &var, string const &name, int gl_min_ver, string const &gl_ext)
{
   if (gl_version >= gl_min_ver || (gl_ext.length() > 1 && contains(extensions, gl_ext)))
   {
      var = true;
      log_info(name.c_str());
   }
}

void configuration::initialize()
{
   vector<string> extensions;

   string vendor      = get_string(GL_VENDOR);
   string renderer    = get_string(GL_RENDERER);
   string version     = get_string(GL_VERSION);
   //bool gles20 = false; // TODO

   log_info("Vendor:   %s\n", vendor.c_str());
   log_info("Renderer: %s\n", renderer.c_str());
   log_info("Version:  %s\n", version.c_str());

   {
      int c = vendor.compare("Intel");
      if (c == 0)
         configuration::intel = true;
   }

   vector<string> versions = split(version, '.');

   int major = (versions.size() > 0) ? ::atoi(digits_only(versions[0]).c_str()) : 0;
   int minor = (versions.size() > 1) ? ::atoi(digits_only(versions[1]).c_str()) : 0;

   gl_version = (major * 100) + (minor * 10);

   gl::get_integer_v(GL_MAX_TEXTURE_SIZE, &max_texture_size);

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   // TODO Enable for GLES 3
   if (gl_version >= 300)
   {
      //  GL 3 introduced a new way to access extension strings
      GLint num_extensions;

      gl::get_integer_v(GL_NUM_EXTENSIONS, &num_extensions);
      if (num_extensions > 0)
      {
         extensions.reserve(num_extensions);
         for (unsigned int i = 0; i < static_cast<unsigned int>(num_extensions); ++i)
         {
            const GLubyte *extension = gl::get_string_i(GL_EXTENSIONS, i);
            string e = string(reinterpret_cast<const char*>(extension));
            extensions.push_back(e);
         }
      }
   } 
   else 
#endif
   {
      string e = get_string(GL_EXTENSIONS);
      extensions = split(e, ' ');
   }

   if (gl_version >= 200 || contains(extensions, "GL_ARB_shading_language_100"))
   {
      try
      {
         string shading_language_version = get_string(GL_SHADING_LANGUAGE_VERSION);
         versions = split(shading_language_version, '.');

         major = (versions.size() > 0) ? ::atoi(digits_only(versions[0]).c_str()) : 0;
         minor = (versions.size() > 1) ? ::atoi(digits_only(versions[1]).c_str()) : 0;
         glsl_version = (major * 100) + minor;
      }
      catch (...)
      {
         //  This should not happen; glsl_version is left to 0.
         //  Note: implicit value for glsl shader source version is 110 if not explicitly set
      }
   }

   log_trace("glVersion:   %d\n", gl_version);
   log_trace("glslVersion: %d\n", glsl_version);

#if 0
   if (gl_version < 300)
   {
      max_texture_units = 1;
      if (gl_version >= 121 || contains(extensions, "GL_ARB_multitexture"))
      {
         gl::get_integer_v(GL_MAX_TEXTURE_UNITS, &max_texture_units);
      }
   }
#endif

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   {
      if (gl_version >= 120 || contains(extensions, "GL_EXT_texture3D"))
      {
         gl::get_integer_v(GL_MAX_3D_TEXTURE_SIZE, &max_3d_texture_size);
      }
   }
#endif

   if (gl_version >= 130 || contains(extensions, "GL_ARB_texture_cube_map"))
   {
      gl::get_integer_v(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &max_cube_map_texture_size);
   }

   if (gl_version >= 200)
   {
      gl::get_integer_v(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units);
      gl::get_integer_v(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_combined_texture_image_units);
   }

   //  GL 3.0 introduced context flags
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   {
      if (gl_version >= 300)
      {
         int context_flags;
         gl::get_integer_v(GL_CONTEXT_FLAGS, &context_flags);
         if ((context_flags & (int)(GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)) != 0)
         {
            forward_compatible = true;
            log_info("forwardCompatible\n");
         }
      }
   }
#endif

   //  GL 3.3 introduced context profile mask
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   {
      if (gl_version >= 330)
      {
         int context_profile_mask;
         gl::get_integer_v(GL_CONTEXT_PROFILE_MASK, &context_profile_mask);
         if ((context_profile_mask & (int)(GL_CONTEXT_CORE_PROFILE_BIT)) != 0)
         {
            core_profile = true;
            log_info("core_profile\n");
         }
         if ((context_profile_mask & (int)(GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)) != 0)
         {
            compatibility_profile = true;
            log_info("compatibility_profile\n");
            //  This is for testing
            //  glslVersion = 120;
         }
      }
   }
#endif

   check(extensions, can_use.vertex_buffer,              "vertex_buffer",              150, "GL_ARB_vertex_buffer_object");
   check(extensions, can_use.conditional_render,         "conditional_render",         300, "GL_NV_conditional_render");
   check(extensions, can_use.gpu_shader4,                "gpu_shader4",                300, "GL_EXT_gpu_shader4");
   check(extensions, can_use.map_buffer_range,           "map_buffer_range",           300, "GL_ARB_map_buffer_range");
   check(extensions, can_use.framebuffer_object,         "framebuffer_object",         300, "GL_ARB_framebuffer_object");
   check(extensions, can_use.color_buffer_float,         "color_buffer_float",         300, "GL_ARB_color_buffer_float");
   check(extensions, can_use.depth_buffer_float,         "depth_buffer_float",         300, "GL_NV_depth_buffer_float");
   check(extensions, can_use.texture_float,              "texture_float",              300, "GL_ARB_texture_float");
   check(extensions, can_use.packed_float,               "packed_float",               300, "GL_EXT_packed_float");
   check(extensions, can_use.texture_shared_exponent,    "texture_shared_exponent",    300, "GL_EXT_texture_shared_exponent");
   check(extensions, can_use.half_float,                 "half_float",                 300, "GL_NV_half_float");
   check(extensions, can_use.half_float_pixel,           "half_float_pixel",           300, "GL_ARB_half_float_pixel");
   check(extensions, can_use.framebuffer_multisample,    "framebuffer_multisample",    300, "GL_EXT_framebuffer_multisample");
   check(extensions, can_use.framebuffer_blit,           "framebuffer_blit",           300, "GL_EXT_framebuffer_blit");
   check(extensions, can_use.texture_integer,            "texture_integer",            300, "GL_EXT_texture_integer");
   check(extensions, can_use.texture_array,              "texture_array",              300, "GL_EXT_texture_array");
   check(extensions, can_use.texture_r,                  "texture_r",                  300, "");
   check(extensions, can_use.texture_rg,                 "texture_rg",                 300, "");
   check(extensions, can_use.packed_depth_stencil,       "packed_depth_stencil",       300, "GL_EXT_packed_depth_stencil");
   check(extensions, can_use.draw_buffers2,              "draw_buffers2",              300, "GL_EXT_draw_buffers2");
   check(extensions, can_use.texture_compression_rgtc,   "texture_compression_rgtc",   300, "GL_EXT_texture_compression_rgtc");

   check(extensions, can_use.vertex_array_object,        "vertex_array_object",        300, "GL_ARB_vertex_array_object");
   check(extensions, can_use.integer_framebuffer_format, "integer_framebuffer_format", 300, "GL_EXT_gpu_shader4"); // GL_EXT_texture_integer too?
   check(extensions, can_use.bind_frag_data_location,    "bind_frag_data_location",    300, "");
   check(extensions, can_use.instanced_arrays,           "instanced_arrays",           300, "GL_ARB_instanced_arrays");
   can_use.bind_buffer_range = can_use.map_buffer_range; // TODO
   if (can_use.vertex_array_object)
   {
      //gl::gen_vertex_arrays(1, &default_vao);
      //gl::bind_vertex_array(default_vao);
   }

   check(extensions, can_use.draw_instanced,             "draw_instanced",             310, "GL_ARB_draw_instanced");
   check(extensions, can_use.texture_buffer_object,      "texture_buffer_object",      310, "GL_ARB_texture_buffer_object");
   check(extensions, can_use.uniform_buffer_object,      "uniform_buffer_object",      310, "GL_ARB_uniform_buffer_object");
   check(extensions, can_use.seamless_cube_map,          "seamless_cube_map",          320, "GL_ARB_seamless_cube_map");
   check(extensions, can_use.draw_elements_base_vertex,  "draw_elements_base_vertex",  320, "GL_ARB_draw_elements_base_vertex");
   check(extensions, can_use.geometry_shaders,           "geometry_shaders",           320, "GL_ARB_geometry_shader4");
   check(extensions, can_use.sampler_object,             "sampler_object",             330, "ARB_sampler_objects");
   check(extensions, can_use.timer_query,                "timer_query",                330, "GL_ARB_timer_query");
   check(extensions, can_use.gpu_shader5,                "gpu_shader5",                400, "GL_EXT_gpu_shader5");
   check(extensions, can_use.transform_feedback,         "transform_feedback",         400, "GL_ARB_transform_feedback3");
   check(extensions, can_use.tesselation_shaders,        "tesselation_shaders",        400, "GL_ARB_tesselation_shader");
   check(extensions, can_use.binary_shaders,             "binary_shaders",             410, "GL_ARB_get_program_binary");

   //  GL versions 3.0, 3.1 and 3.2 use forwardCompatible
   //  GL versions 3.3 and later have coreProfile
   //  Default VAO no longer exists in GL version 3.1 forward compatible contexts
   if (gl_version >= 310 && (core_profile || forward_compatible))
   {
      must_use_vertex_array_object = true;
      log_info("must_use_vertex_array_object\n");
   }

   if (contains(extensions, "GL_GREMEDY_frame_terminator"))
   {
      can_use.frame_terminator = true;
      log_info("can_use_frame_terminator\n");
   }
   if (contains(extensions, "GL_GREMEDY_string_marker"))
   {
      can_use.string_marker = true;
      log_info("can_use_string_marker\n");
   }

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3) && !defined(RENDERSTACK_USE_OPENGL_ES3)
   if (can_use.texture_buffer_object)
   {
      // \todo check gl core version requirement
      // \todo enable support for EXT version (different entry points)
      gl::get_integer_v(GL_MAX_TEXTURE_BUFFER_SIZE, &max_texture_buffer_size);
      log_info("can_use_texture_buffer_object (%d)\n", max_texture_buffer_size);
      //gl::get_integer_v(GetPName.MaxTextureBufferSize, out MaxTextureBufferSize);
   }
#endif

#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   if (can_use.uniform_buffer_object)
   {
      GLint alignment;
      gl::get_integer_v(GL_MAX_UNIFORM_BLOCK_SIZE,           &max_uniform_block_size);
      gl::get_integer_v(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,  &alignment);
      gl::get_integer_v(GL_MAX_UNIFORM_BUFFER_BINDINGS,      &max_uniform_buffer_bindings);
      gl::get_integer_v(GL_MAX_VERTEX_UNIFORM_BLOCKS,        &max_vertex_uniform_blocks);
      gl::get_integer_v(GL_MAX_FRAGMENT_UNIFORM_BLOCKS,      &max_fragment_uniform_blocks);
      uniform_buffer_offset_alignment = static_cast<GLuint>(alignment);
      if (can_use.geometry_shaders) {
         gl::get_integer_v(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &max_geometry_uniform_blocks);
      }
      if (can_use.tesselation_shaders) {
         gl::get_integer_v(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS, &max_tess_control_uniform_blocks);
         gl::get_integer_v(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS, &max_tess_evaluation_uniform_blocks);
      }
      log_info(
         "can_use_uniform_buffer_object " 
         "(max block size = %d"
         ", offset alignment = %d"
         ", max bindings = %d"
         ", max vertex blocks = %d"
         ", max fragment blocks = %d"
         ")\n",
         max_uniform_block_size,
         uniform_buffer_offset_alignment,
         max_uniform_buffer_bindings,
         max_vertex_uniform_blocks,
         max_fragment_uniform_blocks
      );
   }
#endif

   //  Shader model detection could do a lot better job that this... 
   shader_model_version = 0;
   if (can_use.gpu_shader4) {
      shader_model_version = 4;
   }
   if (can_use.gpu_shader5)
      shader_model_version = 5;

   // TODO this is for debugging..
   shader_model_version = 4;

   //can_use.map_buffer_range      = true;
   can_use.map_buffer_range      = false;

   //can_use.texture_rg            = false;
   can_use.uniform_buffer_object = false;
#if 0
   shader_model_version = 0;

   use_binary_shaders                   = false;
   use_integer_polygon_ids              = false;
   use_vertex_array_object              = false;

   can_use.binary_shaders               = false;
   can_use.draw_elements_base_vertex    = false;
   can_use.texture_float                = false;
   can_use.framebuffer_object           = false;
   can_use.frame_terminator             = false;
   can_use.geometry_shaders             = false;
   can_use.draw_instanced               = false;
   can_use.integer_framebuffer_format   = false;
   can_use.map_buffer_range             = false;
   can_use.pixel_buffer_object          = false;
   can_use.sampler_object               = false;
   can_use.seamless_cube_map            = false;
   can_use.string_marker                = false;
   can_use.tesselation_shaders          = false;
   can_use.texture_array                = false;
   can_use.texture_buffer_object        = false;
   can_use.timer_query                  = false;
   can_use.transform_feedback           = false;
   can_use.uniform_buffer_object        = false;
   can_use.vertex_array_object          = false;
#endif

   extensions.clear();
   ::set_text_color(C_GRAY);
}

} }
