#include "renderstack_toolkit/platform.hpp"

#include "renderstack_toolkit/gl_core.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cctype>
#include <iostream>

log_category gl_functions(C_DARK_GREEN, C_GRAY, LOG_TRACE);
#define LOG_CATEGORY &gl_functions


// #define LOG_GL             1
// #define LOG_GL_VAO         1
// #define LOG_GL_BUFFER      1
// #define LOG_GL_MAP_BUFFER  1
// #define LOG_GL_ATTRIB      1
// #define LOG_GL_TEXTURE     1
// #define LOG_GL_PROGRAM     1

// #define LOG_GL_UNIFORM
// #define LOG_GL_FBO

#if defined(LOG_GL)
#   define LOG_GL_FUNCTION(A) log_trace(A)
#else
#   define LOG_GL_FUNCTION(A)
#endif

#define ALWAYS_LOG_GL_FUNCTION(A) log_trace(A)

namespace gl {

#if defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   using namespace gl::detail;
#endif

const char *enum_string(GLenum e)
{
   switch (e) {
   case GL_INVALID_ENUM      : return "GL_INVALID_ENUM";
   case GL_INVALID_VALUE     : return "GL_INVALID_VALUE";
   case GL_INVALID_OPERATION : return "GL_INVALID_OPERATION";
   //case GL_STACK_OVERFLOW    : return "GL_STACK_OVERFLOW";
   //case GL_STACK_UNDERFLOW   : return "GL_STACK_UNDERFLOW";
   case GL_OUT_OF_MEMORY     : return "GL_OUT_OF_MEMORY";

   case GL_BYTE              : return "GL_BYTE";
   case GL_UNSIGNED_BYTE     : return "GL_UNSIGNED_BYTE";
   case GL_SHORT             : return "GL_SHORT";
   case GL_UNSIGNED_SHORT    : return "GL_UNSIGNED_SHORT";
   case GL_INT               : return "GL_INT";
   case GL_UNSIGNED_INT      : return "GL_UNSIGNED_INT";
   case GL_FLOAT             : return "GL_FLOAT";
   //case GL_2_BYTES           : return "GL_2_BYTES";
   //case GL_3_BYTES           : return "GL_3_BYTES";
   //case GL_4_BYTES           : return "GL_4_BYTES";

   case GL_HALF_FLOAT        : return "GL_HALF_FLOAT";

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_DOUBLE            : return "GL_DOUBLE";
#endif
   case GL_FLOAT_VEC2        : return "GL_FLOAT_VEC2";
   case GL_FLOAT_VEC3        : return "GL_FLOAT_VEC3";
   case GL_FLOAT_VEC4        : return "GL_FLOAT_VEC4";
   case GL_INT_VEC2          : return "GL_INT_VEC2";
   case GL_INT_VEC3          : return "GL_INT_VEC3";
   case GL_INT_VEC4          : return "GL_INT_VEC4";
   case GL_BOOL              : return "GL_BOOL";
   case GL_BOOL_VEC2         : return "GL_BOOL_VEC2";
   case GL_BOOL_VEC3         : return "GL_BOOL_VEC3";
   case GL_BOOL_VEC4         : return "GL_BOOL_VEC4";
   case GL_FLOAT_MAT2        : return "GL_FLOAT_MAT2";
   case GL_FLOAT_MAT3        : return "GL_FLOAT_MAT3";
   case GL_FLOAT_MAT4        : return "GL_FLOAT_MAT4";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_SAMPLER_1D        : return "GL_SAMPLER_1D";
#endif
   case GL_SAMPLER_2D        : return "GL_SAMPLER_2D";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_SAMPLER_3D        : return "GL_SAMPLER_3D";
#endif
   case GL_SAMPLER_CUBE      : return "GL_SAMPLER_CUBE";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_SAMPLER_1D_SHADOW : return "GL_SAMPLER_1D_SHADOW";
   case GL_SAMPLER_2D_SHADOW : return "GL_SAMPLER_2D_SHADOW";
#endif

   case GL_FRAMEBUFFER_COMPLETE                     : return "GL_FRAMEBUFFER_COMPLETE";
   case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT        : return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
   case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER       : return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
   case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER       : return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
#endif
   case GL_FRAMEBUFFER_UNSUPPORTED                  : return "GL_FRAMEBUFFER_UNSUPPORTED";

   case GL_NONE:                   return "GL_POINTS / GL_NONE";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_FRAMEBUFFER_DEFAULT:    return "GL_FRAMEBUFFER_DEFAULT";
#endif
   case GL_TEXTURE:                return "GL_TEXTURE";
   case GL_RENDERBUFFER:           return "GL_RENDERBUFFER";

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_SIGNED_NORMALIZED       : return "GL_SIGNED_NORMALIZED";
   case GL_UNSIGNED_NORMALIZED     : return "GL_UNSIGNED_NORMALIZED";
#endif

   case GL_LINEAR                  : return "GL_LINEAR";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_SRGB                    : return "GL_SRGB";
#endif

   //  Texture targets
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_TEXTURE_1D              : return "GL_TEXTURE_1D";
#endif
   case GL_TEXTURE_2D              : return "GL_TEXTURE_2D";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_TEXTURE_3D              : return "GL_TEXTURE_3D";
#endif
   case GL_TEXTURE_CUBE_MAP        : return "GL_TEXTURE_CUBE_MAP";

   //  Buffer targets
   case GL_ARRAY_BUFFER                : return "GL_ARRAY_BUFFER";
   case GL_ELEMENT_ARRAY_BUFFER        : return "GL_ELEMENT_ARRAY_BUFFER";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_PIXEL_PACK_BUFFER           : return "GL_PIXEL_PACK_BUFFER";
   case GL_PIXEL_UNPACK_BUFFER         : return "GL_PIXEL_UNPACK_BUFFER";
   case GL_TRANSFORM_FEEDBACK_BUFFER   : return "GL_TRANSFORM_FEEDBACK_BUFFER";
   case GL_SAMPLER_BUFFER              : return "GL_SAMPLER_BUFFER";
   case GL_INT_SAMPLER_BUFFER          : return "GL_INT_SAMPLER_BUFFER";
   case GL_UNSIGNED_INT_SAMPLER_BUFFER : return "GL_UNSIGNED_INT_SAMPLER_BUFFER";
   case GL_TEXTURE_BUFFER              : return "GL_TEXTURE_BUFFER";
   case GL_TEXTURE_BINDING_BUFFER      : return "GL_TEXTURE_BINDING_BUFFER";
   case GL_UNIFORM_BUFFER              : return "GL_UNIFORM_BUFFER";
   case GL_COPY_READ_BUFFER            : return "GL_COPY_READ_BUFFER";
   case GL_COPY_WRITE_BUFFER           : return "GL_COPY_WRITE_BUFFER";
   case GL_DRAW_INDIRECT_BUFFER        : return "GL_DRAW_INDIRECT_BUFFER";
   case GL_ATOMIC_COUNTER_BUFFER       : return "GL_ATOMIC_COUNTER_BUFFER";
   case GL_IMAGE_BUFFER                : return "GL_IMAGE_BUFFER";
   case GL_INT_IMAGE_BUFFER            : return "GL_INT_IMAGE_BUFFER";
   case GL_UNSIGNED_INT_IMAGE_BUFFER   : return "GL_UNSIGNED_INT_IMAGE_BUFFER";
#endif

   //  Enable targets
   case GL_CULL_FACE                   : return "GL_CULL_FACE";
   case GL_DEPTH_TEST                  : return "GL_DEPTH_TEST";
   case GL_STENCIL_TEST                : return "GL_STENCIL_TEST";
   case GL_DITHER                      : return "GL_DITHER";
   case GL_BLEND                       : return "GL_BLEND";
   case GL_SCISSOR_TEST                : return "GL_SCISSOR_TEST";
   case GL_POLYGON_OFFSET_FILL         : return "GL_POLYGON_OFFSET_FILL";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_MULTISAMPLE                 : return "GL_MULTISAMPLE";
#endif
   case GL_SAMPLE_ALPHA_TO_COVERAGE    : return "GL_SAMPLE_ALPHA_TO_COVERAGE";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_SAMPLE_ALPHA_TO_ONE         : return "GL_SAMPLE_ALPHA_TO_ONE";
#endif
   case GL_SAMPLE_COVERAGE             : return "GL_SAMPLE_COVERAGE";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_DEPTH_CLAMP                 : return "GL_DEPTH_CLAMP";
   case GL_TEXTURE_CUBE_MAP_SEAMLESS   : return "GL_TEXTURE_CUBE_MAP_SEAMLESS";
   case GL_SAMPLE_SHADING              : return "GL_SAMPLE_SHADING";
   case GL_RASTERIZER_DISCARD          : return "GL_RASTERIZER_DISCARD";
   case GL_FRAMEBUFFER_SRGB            : return "GL_FRAMEBUFFER_SRGB";
   case GL_SAMPLE_MASK                 : return "GL_SAMPLE_MASK";
   case GL_PRIMITIVE_RESTART           : return "GL_PRIMITIVE_RESTART";
#endif

   //  Begin modes
   //case GL_POINTS                      : return "GL_POINTS";
   case GL_LINES                       : return "GL_LINES";
   case GL_LINE_LOOP                   : return "GL_LINE_LOOP";
   case GL_LINE_STRIP                  : return "GL_LINE_STRIP";
   case GL_TRIANGLES                   : return "GL_TRIANGLES";
   case GL_TRIANGLE_STRIP              : return "GL_TRIANGLE_STRIP";
   case GL_TRIANGLE_FAN                : return "GL_TRIANGLE_FAN";
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
   case GL_PATCHES                     : return "GL_PATCHES";
   case GL_LINES_ADJACENCY             : return "GL_LINES_ADJACENCY";
   case GL_LINE_STRIP_ADJACENCY        : return "GL_LINE_STRIP_ADJACENCY";
   case GL_TRIANGLES_ADJACENCY         : return "GL_TRIANGLES_ADJACENCY";
   case GL_TRIANGLE_STRIP_ADJACENCY    : return "GL_TRIANGLE_STRIP_ADJACENCY";
#endif

   case GL_BUFFER_SIZE                       : return "GL_BUFFER_SIZE";
   case GL_BUFFER_USAGE                      : return "GL_BUFFER_USAGE";
   case GL_QUERY_COUNTER_BITS                : return "GL_QUERY_COUNTER_BITS";
   case GL_CURRENT_QUERY                     : return "GL_CURRENT_QUERY";
   case GL_QUERY_RESULT                      : return "GL_QUERY_RESULT";
   case GL_QUERY_RESULT_AVAILABLE            : return "GL_QUERY_RESULT_AVAILABLE";
   case GL_ARRAY_BUFFER_BINDING              : return "GL_ARRAY_BUFFER_BINDING";
   case GL_ELEMENT_ARRAY_BUFFER_BINDING      : return "GL_ELEMENT_ARRAY_BUFFER_BINDING";
   case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: return "GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING";
   case GL_READ_ONLY                         : return "GL_READ_ONLY";
   case GL_WRITE_ONLY                        : return "GL_WRITE_ONLY";
   case GL_READ_WRITE                        : return "GL_READ_WRITE";
   case GL_BUFFER_ACCESS                     : return "GL_BUFFER_ACCESS";
   case GL_BUFFER_MAPPED                     : return "GL_BUFFER_MAPPED";
   case GL_BUFFER_MAP_POINTER                : return "GL_BUFFER_MAP_POINTER";
   case GL_STREAM_DRAW                       : return "GL_STREAM_DRAW";
   case GL_STREAM_READ                       : return "GL_STREAM_READ";
   case GL_STREAM_COPY                       : return "GL_STREAM_COPY";
   case GL_STATIC_DRAW                       : return "GL_STATIC_DRAW";
   case GL_STATIC_READ                       : return "GL_STATIC_READ";
   case GL_STATIC_COPY                       : return "GL_STATIC_COPY";
   case GL_DYNAMIC_DRAW                      : return "GL_DYNAMIC_DRAW";
   case GL_DYNAMIC_READ                      : return "GL_DYNAMIC_READ";
   case GL_DYNAMIC_COPY                      : return "GL_DYNAMIC_COPY";
   case GL_SAMPLES_PASSED                    : return "GL_SAMPLES_PASSED";
   case GL_SRC1_ALPHA                        : return "GL_SRC1_ALPHA";
   case GL_FRAGMENT_SHADER                   : return "GL_FRAGMENT_SHADER";
   case GL_VERTEX_SHADER                     : return "GL_VERTEX_SHADER";
   case GL_GEOMETRY_SHADER                   : return "GL_GEOMETRY_SHADER";
   case GL_TESS_EVALUATION_SHADER            : return "GL_TESS_EVALUATION_SHADER";
   case GL_TESS_CONTROL_SHADER               : return "GL_TESS_CONTROL_SHADER";

   default: return "???"; 
}
}

static bool enable_error_checking = true;

void set_error_checking(bool enable)
{
   enable_error_checking = enable;
}

void check_error()
{
   if (enable_error_checking == false)
      return;

#if defined(SUPPORT_LEGACY_OPENGL) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   GLenum status = gl::detail::glGetError();
#else
   GLenum status = glGetError();
#endif
   if (status != GL_NO_ERROR)
   {
      set_text_color(C_RED);
      puts(enum_string(status));
      set_text_color(C_GREY);
   }
}

#if defined(SUPPORT_LEGACY_OPENGL)
void get_tex_env_fv(GLenum target, GLenum pname, GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(glGetTexEnvfv != nullptr);
   glGetTexEnvfv(target, pname, params);
   check_error();
}
void get_tex_env_iv(GLenum target, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(glGetTexEnviv != nullptr);
   glGetTexEnviv(target, pname, params);
   check_error();
}
void light_model_f(GLenum pname, GLfloat param);
void light_model_fv(GLenum pname, const GLfloat *params);
void light_model_i(GLenum pname, GLint param);
void light_model_iv(GLenum pname, const GLint *params);
void light_f(GLenum light, GLenum pname, GLfloat param);
void light_fv(GLenum light, GLenum pname, const GLfloat *params);
void light_i(GLenum light, GLenum pname, GLint param);
void light_iv(GLenum light, GLenum pname, const GLint *params);
void tex_env_f(GLenum target, GLenum pname, GLfloat param)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(glTexEnvf != nullptr);
   glTexEnvf(target, pname, param);
   check_error();
}
void tex_env_fv(GLenum target, GLenum pname, const GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(glTexEnvfv != nullptr);
   glTexEnvfv(target, pname, params);
   check_error();
}
void tex_env_i(GLenum target, GLenum pname, GLint param)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(glTexEnvi != nullptr);
   glTexEnvi(target, pname, param);
   check_error();
}
void tex_env_iv(GLenum target, GLenum pname, const GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(glTexEnviv != nullptr);
   glTexEnviv(target, pname, params);
   check_error();
}
#endif

/*  GL 1.0 ... 1.1  */ 
/*  GL_H  */ 
void bind_texture(GLenum target, GLuint texture)
{   
#if defined(LOG_GL) || defined(LOG_GL_TEXTURE)
   log_trace("bind_texture(%s, %u)", enum_string(target), texture);
#endif
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glBindTexture != nullptr);
   gl::detail::glBindTexture(target, texture);
#else
   ::glBindTexture(target, texture);
#endif
   check_error();
}
void blend_func(GLenum sfactor, GLenum dfactor)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glBlendFunc != nullptr);
   gl::detail::glBlendFunc(sfactor, dfactor);
#else
   ::glBlendFunc(sfactor, dfactor);
#endif
   check_error();
}
void clear(GLbitfield mask)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glClear != nullptr);
   gl::detail::glClear(mask);
#else
   ::glClear(mask);
#endif
   check_error();
}
void clear_color(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glClearColor != nullptr);
   gl::detail::glClearColor(red, green, blue, alpha);
#else
   ::glClearColor(red, green, blue, alpha);
#endif
   check_error();
}
void clear_stencil(GLint s)
   {  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glClearStencil != nullptr);
   gl::detail::glClearStencil(s);
#else
   glClearStencil(s);
#endif
   check_error();
}
void color_mask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glColorMask != nullptr);
   gl::detail::glColorMask(red, green, blue, alpha);
#else
   glColorMask(red, green, blue, alpha);
#endif
   check_error();
}
void copy_tex_image_2d(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glCopyTexImage2D != nullptr);
   gl::detail::glCopyTexImage2D(target, level, internalFormat, x, y, width, height, border);
#else
   glCopyTexImage2D(target, level, internalFormat, x, y, width, height, border);
#endif
   check_error();
}
void copy_tex_sub_image_2d(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glCopyTexSubImage2D != nullptr);
   gl::detail::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
#else
   glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
#endif
   check_error();
}
void cull_face(GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glCullFace != nullptr);
   gl::detail::glCullFace(mode);
#else
   glCullFace(mode);
#endif
   check_error();
}
void delete_textures(GLsizei n, const GLuint *textures)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDeleteTextures != nullptr);
   gl::detail::glDeleteTextures(n, textures);
#else
   glDeleteTextures(n, textures);
#endif
   check_error();
}
void depth_func(GLenum func)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDepthFunc != nullptr);
   gl::detail::glDepthFunc(func);
#else
   glDepthFunc(func);
#endif
   check_error();
}
void depth_mask(GLboolean flag)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDepthMask != nullptr);
   gl::detail::glDepthMask(flag);
#else
   glDepthMask(flag);
#endif
   check_error();
}
void disable(GLenum cap)
{
#if defined(LOG_GL)
   log_trace("disable(%s)", enum_string(cap));
#endif
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDisable != nullptr);
   gl::detail::glDisable(cap);
#else
   ::glDisable(cap);
#endif
   check_error();
}
void draw_arrays(GLenum mode, GLint first, GLsizei count)
{
#if defined(LOG_GL)
   log_trace("glDrawArrays(%s, %d, %u)",
      enum_string(mode),
      first,
      static_cast<unsigned int>(count));
#endif
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDrawArrays != nullptr);
   gl::detail::glDrawArrays(mode, first, count);
#else
   glDrawArrays(mode, first, count);
#endif
   check_error();
}
void draw_elements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
#if defined(LOG_GL)
   log_trace("draw_elements(%s, %u, %s, %p)",
      enum_string(mode),
      static_cast<unsigned int>(count),
      enum_string(type),
      indices);
#endif
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDrawElements != nullptr);
   gl::detail::glDrawElements(mode, count, type, indices);
#else
   ::glDrawElements(mode, count, type, indices);
#endif
   check_error();
}
void enable(GLenum cap)
{
#if defined(LOG_GL)
   log_trace("enable(%s)", enum_string(cap));
#endif
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glEnable != nullptr);
   gl::detail::glEnable(cap);
#else
   ::glEnable(cap);
#endif
   check_error();
}
void finish(void)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glFinish != nullptr);
   gl::detail::glFinish();
#else
   ::glFinish();
#endif
   check_error();
}
void flush(void)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glFlush != nullptr);
   gl::detail::glFlush();
#else
   ::glFlush();
#endif
   check_error();
}
void front_face(GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glFrontFace != nullptr);
   gl::detail::glFrontFace(mode);
#else
   ::glFrontFace(mode);
#endif
   check_error();
}
void gen_textures(GLsizei n, GLuint *textures)
{
#if defined(LOG_GL) || defined(LOG_GL_TEXTURE)
   slog_trace("gen_textures(%u, %p)",
      static_cast<unsigned int>(n),
      textures);
#endif
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGenTextures != nullptr);
   gl::detail::glGenTextures(n, textures);
#else
   ::glGenTextures(n, textures);
#endif
#if defined(LOG_GL) || defined(LOG_GL_TEXTURE)
   if (n > 0 && textures)
      log_trace(":%u", textures[0]);
#endif
   check_error();
}
void get_boolean_v(GLenum pname, GLboolean *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetBooleanv != nullptr);
   gl::detail::glGetBooleanv(pname, params);
#else
   ::glGetBooleanv(pname, params);
#endif
   check_error();
}
GLenum get_error(void)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetError != nullptr);
   return gl::detail::glGetError();
#else
   return ::glGetError();
#endif
}
void get_float_v(GLenum pname, GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetFloatv != nullptr);
   gl::detail::glGetFloatv(pname, params);
#else
   ::glGetFloatv(pname, params);
#endif
   check_error();
}
void get_integer_v(GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetIntegerv != nullptr);
   gl::detail::glGetIntegerv(pname, params);
#else
   ::glGetIntegerv(pname, params);
#endif
   check_error();
}
const GLubyte * get_string(GLenum name)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetString != nullptr);
   const GLubyte *res = gl::detail::glGetString(name);
#else
   const GLubyte *res = ::glGetString(name);
#endif
   check_error();
   return res;
}
void get_tex_parameter_fv(GLenum target, GLenum pname, GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetTexParameterfv != nullptr);
   gl::detail::glGetTexParameterfv(target, pname, params);
#else
   ::glGetTexParameterfv(target, pname, params);
#endif
   check_error();
}
void get_tex_parameter_iv(GLenum target, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetTexParameteriv != nullptr);
   gl::detail::glGetTexParameteriv(target, pname, params);
#else
   ::glGetTexParameteriv(target, pname, params);
#endif
   check_error();
}
void hint(GLenum target, GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glHint != nullptr);
   gl::detail::glHint(target, mode);
#else
   ::glHint(target, mode);
#endif
   check_error();
}
GLboolean is_enabled(GLenum cap)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glIsEnabled != nullptr);
   GLboolean res = gl::detail::glIsEnabled(cap);
#else
   GLboolean res = ::glIsEnabled(cap);
#endif
   check_error();
   return res;
}
GLboolean is_texture(GLuint texture)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glIsTexture != nullptr);
   GLboolean res = gl::detail::glIsTexture(texture);
#else
   GLboolean res = ::glIsTexture(texture);
#endif
   check_error();
   return res;
}
void line_width(GLfloat width)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glLineWidth != nullptr);
   gl::detail::glLineWidth(width);
#else
   ::glLineWidth(width);
#endif
   check_error();
}
void pixel_store_i(GLenum pname, GLint param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glPixelStorei != nullptr);
   gl::detail::glPixelStorei(pname, param);
#else
   ::glPixelStorei(pname, param);
#endif
   check_error();
}
void polygon_offset(GLfloat factor, GLfloat units)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glPolygonOffset != nullptr);
   gl::detail::glPolygonOffset(factor, units);
#else
   ::glPolygonOffset(factor, units);
#endif
   check_error();
}
void read_pixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glReadPixels != nullptr);
   gl::detail::glReadPixels(x, y, width, height, format, type, pixels);
#else
   ::glReadPixels(x, y, width, height, format, type, pixels);
#endif
   check_error();
}
void scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glScissor != nullptr);
   gl::detail::glScissor(x, y, width, height);
#else
   ::glScissor(x, y, width, height);
#endif
   check_error();
}
void stencil_func(GLenum func, GLint ref, GLuint mask)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glStencilFunc != nullptr);
   gl::detail::glStencilFunc(func, ref, mask);
#else
   ::glStencilFunc(func, ref, mask);
#endif
   check_error();
}
void stencil_mask(GLuint mask)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glStencilMask != nullptr);
   gl::detail::glStencilMask(mask);
#else
   ::glStencilMask(mask);
#endif
   check_error();
}
void stencil_op(GLenum fail, GLenum zfail, GLenum zpass)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glStencilOp != nullptr);
   gl::detail::glStencilOp(fail, zfail, zpass);
#else
   ::glStencilOp(fail, zfail, zpass);
#endif
   check_error();
}
void tex_image_2d(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexImage2D != nullptr);
   gl::detail::glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
#else
   ::glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
#endif
   check_error();
}
void tex_parameter_f(GLenum target, GLenum pname, GLfloat param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexParameterf != nullptr);
   gl::detail::glTexParameterf(target, pname, param);
#else
   ::glTexParameterf(target, pname, param);
#endif
   check_error();
}
void tex_parameter_fv(GLenum target, GLenum pname, const GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexParameterfv != nullptr);
   gl::detail::glTexParameterfv(target, pname, params);
#else
   ::glTexParameterfv(target, pname, params);
#endif
   check_error();
}
void tex_parameter_i(GLenum target, GLenum pname, GLint param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexParameteri != nullptr);
   gl::detail::glTexParameteri(target, pname, param);
#else
   ::glTexParameteri(target, pname, param);
#endif
   check_error();
}
void tex_parameter_iv(GLenum target, GLenum pname, const GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexParameteriv != nullptr);
   gl::detail::glTexParameteriv(target, pname, params);
#else
   ::glTexParameteriv(target, pname, params);
#endif
   check_error();
}
void tex_sub_image_2d(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexSubImage2D != nullptr);
   gl::detail::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
#else
   ::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
#endif
   check_error();
}
void viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glViewport != nullptr);
   gl::detail::glViewport(x, y, width, height);
#else
   ::glViewport(x, y, width, height);
#endif
   check_error();
}

#if defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void clear_depth(GLclampf depth)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glClearDepthf != nullptr);
   gl::detail::glClearDepthf(depth);
   check_error();
}
void depth_range(GLclampf zNear, GLclampf zFar)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDepthRangef != nullptr);
   gl::detail::glDepthRangef(zNear, zFar);
   check_error();
}
#else
void clear_depth(GLclampd depth)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glClearDepth != nullptr);
   gl::detail::glClearDepth(depth);
#else
   ::glClearDepth(depth);
#endif
   check_error();
}
void depth_range(GLclampd zNear, GLclampd zFar)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDepthRange != nullptr);
   gl::detail::glDepthRange(zNear, zFar);
#else
   ::glDepthRange(zNear, zFar);
#endif
   check_error();
}
#endif

// OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void read_buffer(GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glReadBuffer != nullptr);
   gl::detail::glReadBuffer(mode);
#else
   ::glReadBuffer(mode);
#endif
   check_error();
}
#endif

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void copy_tex_image_1d(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glCopyTexImage1D != nullptr);
   gl::detail::glCopyTexImage1D(target, level, internalFormat, x, y, width, border);
#else
   ::glCopyTexImage1D(target, level, internalFormat, x, y, width, border);
#endif
   check_error();
}
void copy_tex_sub_image_1d(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glCopyTexSubImage1D != nullptr);
   gl::detail::glCopyTexSubImage1D(target, level, xoffset, x, y, width);
#else
   ::glCopyTexSubImage1D(target, level, xoffset, x, y, width);
#endif
   check_error();
}
void draw_buffer(GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(::gl::detail::glDrawBuffer != nullptr);
   gl::detail::glDrawBuffer(mode);
#else
   ::glDrawBuffer(mode);
#endif
   check_error();
}
void get_double_v(GLenum pname, GLdouble *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetDoublev != nullptr);
   gl::detail::glGetDoublev(pname, params);
#else
   ::glGetDoublev(pname, params);
#endif
   check_error();
}
void get_pointer_v(GLenum pname, GLvoid* *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetPointerv != nullptr);
   gl::detail::glGetPointerv(pname, params);
#else
   ::glGetPointerv(pname, params);
#endif
   check_error();
}
void get_tex_image(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetTexImage != nullptr);
   gl::detail::glGetTexImage(target, level, format, type, pixels);
#else
   ::glGetTexImage(target, level, format, type, pixels);
#endif
   check_error();
}
void get_tex_level_parameter_fv(GLenum target, GLint level, GLenum pname, GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetTexLevelParameterfv != nullptr);
   gl::detail::glGetTexLevelParameterfv(target, level, pname, params);
#else
   ::glGetTexLevelParameterfv(target, level, pname, params);
#endif
   check_error();
}
void get_tex_level_parameter_iv(GLenum target, GLint level, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetTexLevelParameteriv != nullptr);
   gl::detail::glGetTexLevelParameteriv(target, level, pname, params);
#else
   ::glGetTexLevelParameteriv(target, level, pname, params);
#endif
check_error();
}
void logic_op(GLenum opcode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glLogicOp != nullptr);
   gl::detail::glLogicOp(opcode);
#else
   ::glLogicOp(opcode);
#endif
   check_error();
}
void pixel_store_f(GLenum pname, GLfloat param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glPixelStoref != nullptr);
   gl::detail::glPixelStoref(pname, param);
#else
   ::glPixelStoref(pname, param);
#endif
   check_error();
}
void point_size(GLfloat size)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glPointSize != nullptr);
   gl::detail::glPointSize(size);
#else
   ::glPointSize(size);
#endif
   check_error();
}
void polygon_mode(GLenum face, GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glPolygonMode != nullptr);
   gl::detail::glPolygonMode(face, mode);
#else
   ::glPolygonMode(face, mode);
#endif
   check_error();
}
void tex_image_1d(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexImage1D != nullptr);
   gl::detail::glTexImage1D(target, level, internalformat, width, border, format, type, pixels);
#else
   ::glTexImage1D(target, level, internalformat, width, border, format, type, pixels);
#endif
   check_error();
}
void tex_sub_image_1d(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexSubImage1D != nullptr);
   gl::detail::glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
#else
   ::glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
#endif
   check_error();
}
#endif

/*  GL_VERSION_1_2  */ 
void blend_color(GLclampf a, GLclampf b, GLclampf c, GLclampf d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBlendColor != nullptr);
   gl::detail::glBlendColor(a, b, c, d);
   check_error();
}

void blend_equation(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBlendEquation != nullptr);
   gl::detail::glBlendEquation(a);
   check_error();
}

// OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void draw_range_elements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices)
{
#if defined(LOG_GL)
   log_trace("draw_range_elements(%s, %u, %u, %u, %s, %p)",
      enum_string(mode),
      start,
      end,
      static_cast<unsigned int>(count),
      enum_string(type),
      indices);
#endif
   assert(gl::detail::glDrawRangeElements != nullptr);
   gl::detail::glDrawRangeElements(mode, start, end, count, type, indices);
   check_error();
}
void tex_image_3d(GLenum a, GLint b, GLint c, GLsizei d, GLsizei e, GLsizei f, GLint g, GLenum h, GLenum i, const GLvoid *j)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glTexImage3D != nullptr);
   gl::detail::glTexImage3D(a, b, c, d, e, f, g, h, i, j);
   check_error();
}
void tex_sub_image_3d(GLenum a, GLint b, GLint c, GLint d, GLint e, GLsizei f, GLsizei g, GLsizei h, GLenum i, GLenum j, const GLvoid *k)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glTexSubImage3D != nullptr);
   gl::detail::glTexSubImage3D(a, b, c, d, e, f, g, h, i, j, k);
   check_error();
}
void copy_tex_sub_image_3d(GLenum a, GLint b, GLint c, GLint d, GLint e, GLint f, GLint g, GLsizei h, GLsizei i)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glCopyTexSubImage3D != nullptr);
   gl::detail::glCopyTexSubImage3D(a, b, c, d, e, f, g, h, i);
   check_error();
}
#endif

/*  GL_VERSION_1_3  */ 
void active_texture(GLenum a)
{
#if defined(LOG_GL)
   log_trace("active_texture(GL_TEXTURE%d)", a - GL_TEXTURE0);
#endif
   assert(a >= GL_TEXTURE0);
   assert(gl::detail::glActiveTexture != nullptr);
   gl::detail::glActiveTexture(a);
   check_error();
}
void sample_coverage(GLclampf a, GLboolean b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glSampleCoverage != nullptr);
   gl::detail::glSampleCoverage(a, b);
   check_error();
}
void compressed_tex_image_2d(GLenum a, GLint b, GLenum c, GLsizei d, GLsizei e, GLint f, GLsizei g, const GLvoid *h)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glCompressedTexImage2D != nullptr);
   gl::detail::glCompressedTexImage2D(a, b, c, d, e, f, g, h);
   check_error();
}
void compressed_tex_sub_image_2d(GLenum a, GLint b, GLint c, GLint d, GLsizei e, GLsizei f, GLenum g, GLsizei h, const GLvoid *i)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glCompressedTexSubImage2D != nullptr);
   gl::detail::glCompressedTexSubImage2D(a, b, c, d, e, f, g, h, i);
   check_error();
}

   // OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void compressed_tex_image_3d(GLenum a, GLint b, GLenum c, GLsizei d, GLsizei e, GLsizei f, GLint g, GLsizei h, const GLvoid *i)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glCompressedTexImage3D != nullptr);
   gl::detail::glCompressedTexImage3D(a, b, c, d, e, f, g, h, i);
   check_error();
}
void compressed_tex_sub_image_3d(GLenum a, GLint b, GLint c, GLint d, GLint e, GLsizei f, GLsizei g, GLsizei h, GLenum i, GLsizei j, const GLvoid *k)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glCompressedTexSubImage3D != nullptr);
   gl::detail::glCompressedTexSubImage3D(a, b, c, d, e, f, g, h, i, j, k);
   check_error();
}
#endif

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void compressed_tex_image_1d(GLenum a, GLint b, GLenum c, GLsizei d, GLint e, GLsizei f, const GLvoid *g)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glCompressedTexImage1D != nullptr);
   gl::detail::glCompressedTexImage1D(a, b, c, d, e, f, g);
   check_error();
}
void compressed_tex_sub_image_1d(GLenum a, GLint b, GLint c, GLsizei d, GLenum e, GLsizei f, const GLvoid *g)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glCompressedTexSubImage1D != nullptr);
   gl::detail::glCompressedTexSubImage1D(a, b, c, d, e, f, g);
   check_error();
}
void get_compressed_tex_image(GLenum a, GLint b, GLvoid *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetCompressedTexImage != nullptr);
   gl::detail::glGetCompressedTexImage(a, b, c);
   check_error();
}
#endif

#if defined(SUPPORT_LEGACY_OPENGL)
void client_active_texture(GLenum texture)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glClientActiveTexture != nullptr);
   gl::detail::glClientActiveTexture(a, b, c);
   check_error();
}
#endif

/*  GL_VERSION_1_4  */ 
void blend_func_separate(GLenum a, GLenum b, GLenum c, GLenum d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBlendFuncSeparate != nullptr);
   gl::detail::glBlendFuncSeparate(a, b, c, d);
   check_error();
}

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void multi_draw_arrays(GLenum a, GLint *b, GLsizei *c, GLsizei d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glMultiDrawArrays != nullptr);
   gl::detail::glMultiDrawArrays(a, b, c, d);
   check_error();
}
void multi_draw_elements(GLenum a, const GLsizei *b, GLenum c, const GLvoid* *d, GLsizei e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glMultiDrawElements != nullptr);
   gl::detail::glMultiDrawElements(a, b, c, d, e);
   check_error();
}
void point_parameter_f(GLenum a, GLfloat b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glPointParameterf != nullptr);
   gl::detail::glPointParameterf(a, b);
   check_error();
}
void point_parameter_fv(GLenum a, const GLfloat *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glPointParameterfv != nullptr);
   gl::detail::glPointParameterfv(a, b);
   check_error();
}
void point_parameter_i(GLenum a, GLint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glPointParameteri != nullptr);
   gl::detail::glPointParameteri(a, b);
   check_error();
}
void point_parameter_iv(GLenum a, const GLint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glPointParameteriv != nullptr);
   gl::detail::glPointParameteriv(a, b);
   check_error();
}
#endif

/*  GL_VERSION_1_5  */ 

void bind_buffer(GLenum target, GLuint buffer)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   log_trace("bind_buffer(%s, %u)", enum_string(target), buffer);
#endif
   assert(gl::detail::glBindBuffer != nullptr);
   gl::detail::glBindBuffer(target, buffer);
   check_error();
}
void delete_buffers(GLsizei n, const GLuint *buffers)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   log_trace("delete_buffers(n = %u, buffers = %p)",
      static_cast<unsigned int>(n),
      buffers);
#endif
   assert(gl::detail::glDeleteBuffers != nullptr);
   gl::detail::glDeleteBuffers(n, buffers);
   check_error();
}
void gen_buffers(GLsizei n, GLuint *buffers)
{   
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   slog_trace("gen_buffers(n = %u, buffers = %p)",
      static_cast<unsigned int>(n),
      buffers);
#endif
   assert(gl::detail::glGenBuffers != nullptr);
   gl::detail::glGenBuffers(n, buffers);
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   if (n > 0 && buffers)
      log_trace(":%u", buffers[0]);
#endif
   check_error();
}
GLboolean is_buffer(GLuint buffer)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   log_trace("is_buffer(%u)", buffer);
#endif
   assert(gl::detail::glIsBuffer != nullptr);
   GLboolean res = gl::detail::glIsBuffer(buffer);
   check_error();
   return res;
}
void buffer_data(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   log_trace("buffer_data(%s, %u, %p, 0x%x)",
      enum_string(target),
      static_cast<unsigned int>(size),
      data,
      usage);
#endif
   assert(gl::detail::glBufferData != nullptr);
   gl::detail::glBufferData(target, size, data, usage);
   check_error();
}
void buffer_sub_data(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   slog_trace("buffer_sub_data(%s, %u, %u, %p)",
      enum_string(target),
      static_cast<unsigned int>(offset),
      static_cast<unsigned int>(size),
      data);

   if ((size == 0) || (data == nullptr))
      log_error("oops?");
#endif
   assert(gl::detail::glBufferSubData != nullptr);
   gl::detail::glBufferSubData(target, offset, size, data);
   check_error();
}
void get_buffer_parameter_iv(GLenum target, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetBufferParameteriv != nullptr);
   gl::detail::glGetBufferParameteriv(target, pname, params);
   check_error();
}

   // GL_OES_mapbuffer
#if defined(RENDERSTACK_USE_GLES2_OR_GLES3)
GLvoid* map_buffer_oes(GLenum target, GLenum access)
{
#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   log_trace("map_buffer_oes(target = %s, access = 0x%04x)", enum_string(target), enum_string(access));
#endif

   assert(gl::detail::glMapBufferOES != nullptr);
   GLvoid* res = gl::detail::glMapBufferOES(target, access);

#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   log_trace(":%p", res);
#endif
   check_error();
   return res;
}
void get_buffer_pointer_v_oes(GLenum target, GLenum pname, GLvoid **params)
{
#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   slog_trace("get_buffer_pointer_v_oes(target = %s, pname = %s, params = %p)",
      enum_string(target), enum_string(pname), params);
#endif

   assert(gl::detail::glGetBufferPointervOES != nullptr);
   gl::detail::glGetBufferPointervOES(target, pname, params);
   check_error();
}
GLboolean unmap_buffer_oes(GLenum target)
{
#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   slog_trace("unmap_buffer_oes(target = %s)", enum_string(target));
#endif

   assert(gl::detail::glUnmapBufferOES != nullptr);
   GLboolean res = gl::detail::glUnmapBufferOES(target);
   check_error();

#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   log_trace(":%d", res);
#endif
   return res;
}
#endif

// OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void gen_queries(GLsizei a, GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGenQueries != nullptr);
   gl::detail::glGenQueries(a, b);
   check_error();
}
void delete_queries(GLsizei a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDeleteQueries != nullptr);
   gl::detail::glDeleteQueries(a, b);
   check_error();
}
GLboolean is_query(GLuint a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glIsQuery != nullptr);
   GLboolean res = gl::detail::glIsQuery(a);
   check_error();
   return res;
}
void begin_query(GLenum a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBeginQuery != nullptr);
   gl::detail::glBeginQuery(a, b);
   check_error();
}
void end_query(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glEndQuery != nullptr);
   gl::detail::glEndQuery(a);
   check_error();
}
void get_query_iv(GLenum a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetQueryiv != nullptr);
   gl::detail::glGetQueryiv(a, b, c);
   check_error();
}
void get_query_object_uiv(GLuint a, GLenum b, GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetQueryObjectuiv != nullptr);
   gl::detail::glGetQueryObjectuiv(a, b, c);
   check_error();
}
void get_buffer_pointer_v(GLenum target, GLenum pname, GLvoid **params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetBufferPointerv != nullptr);
   gl::detail::glGetBufferPointerv(target, pname, params);
   check_error();
}
GLvoid* map_buffer(GLenum target, GLenum access)
{
#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   slog_trace("map_buffer(target = %s, access = 0x%04x)", enum_string(target), access);
#endif

   assert(gl::detail::glMapBuffer != nullptr);
   GLvoid* res = gl::detail::glMapBuffer(target, access);

#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   log_trace(":%p", res);
#endif
   check_error();
   return res;
}
GLboolean unmap_buffer(GLenum target)
{
#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   slog_trace("unmap_buffer(target = %s)", enum_string(target));
#endif

   assert(gl::detail::glUnmapBuffer != nullptr);
   GLboolean res = gl::detail::glUnmapBuffer(target);

#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   log_trace(":%d", res);
#endif
   check_error();
   return res;
}
#endif
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void get_buffer_sub_data(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   log_trace("get_buffer_data(target = %s, offset = %u, size = %u, data = %p)",
      enum_string(target),
      static_cast<unsigned int>(offset),
      static_cast<unsigned int>(size),
      data);
#endif
   assert(gl::detail::glGetBufferSubData != nullptr);
   gl::detail::glGetBufferSubData(target, offset, size, data);
   check_error();
}
void get_query_object_iv(GLuint a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetQueryObjectiv != nullptr);
   gl::detail::glGetQueryObjectiv(a, b, c);
   check_error();
}
#endif


/*  GL_VERSION_2_0  */ 
void blend_equation_separate(GLenum a, GLenum b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBlendEquationSeparate != nullptr);
   gl::detail::glBlendEquationSeparate(a, b);
   check_error();
}
void stencil_op_separate(GLenum a, GLenum b, GLenum c, GLenum d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glStencilOpSeparate != nullptr);
   gl::detail::glStencilOpSeparate(a, b, c, d);
   check_error();
}
void stencil_func_Separate(GLenum a, GLenum b, GLint c, GLuint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glStencilFuncSeparate != nullptr);
   gl::detail::glStencilFuncSeparate(a, b, c, d);
   check_error();
}
void stencil_mask_separate(GLenum a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glStencilMaskSeparate != nullptr);
   gl::detail::glStencilMaskSeparate(a, b);
   check_error();
}
void attach_shader(GLuint program, GLuint shader)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("attach_shader(%u, %u)", program, shader);
#endif
   assert(gl::detail::glAttachShader != nullptr);
   gl::detail::glAttachShader(program, shader);
   check_error();
}
void bind_attrib_location(GLuint program, GLuint index, const GLchar *name)
{
   assert(name != nullptr);
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB)
   log_trace("bind_attrib_location(program = %u, index = %u, name = %s)",
      program, index, name != nullptr ? name : "(null)");
#endif
   assert(gl::detail::glBindAttribLocation != nullptr);
   gl::detail::glBindAttribLocation(program, index, name);
   check_error();
}
void compile_shader(GLuint shader)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("compile_shader(shader = %u)", shader);
#endif
   assert(gl::detail::glCompileShader != nullptr);
   gl::detail::glCompileShader(shader);
   check_error();
}
GLuint create_program(void)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   slog_trace("create_program()");
#endif
   assert(gl::detail::glCreateProgram != nullptr);
   GLuint res = gl::detail::glCreateProgram();
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace(":%u", res);
#endif
   check_error();
   return res;
}
GLuint create_shader(GLenum type)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("create_shader(type = %s)", gl::enum_string(type));
#endif
   assert(gl::detail::glCreateShader != nullptr);
   GLuint res = gl::detail::glCreateShader(type);
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace(":%u", res);
#endif
   check_error();
   return res;
}
void delete_program(GLuint program)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("delete_program(program = %u)", program);
#endif
   assert(gl::detail::glDeleteProgram != nullptr);
   gl::detail::glDeleteProgram(program);
   check_error();
}
void delete_shader(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace(__FUNCTION__);
#endif
   assert(gl::detail::glDeleteShader != nullptr);
   gl::detail::glDeleteShader(a);
   check_error();
}
void detach_shader(GLuint program, GLuint shader)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("detach_shader(program = %u, shader = %u)", program, shader);
#endif
   assert(gl::detail::glDetachShader != nullptr);
   gl::detail::glDetachShader(program, shader);
   check_error();
}
void disable_vertex_attrib_array(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB)
   log_trace("disable_vertex_attrib_array(%u)", a);
#endif
   assert(gl::detail::glDisableVertexAttribArray != nullptr);
   gl::detail::glDisableVertexAttribArray(a);
   check_error();
}
void enable_vertex_attrib_array(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB)
   log_trace("enable_vertex_attrib_array(%u)", a);
#endif
   assert(gl::detail::glEnableVertexAttribArray != nullptr);
   gl::detail::glEnableVertexAttribArray(a);
   check_error();
}
void get_active_attrib(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLint *e, GLenum *f, GLchar *g)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetActiveAttrib != nullptr);
   gl::detail::glGetActiveAttrib(a, b, c, d, e, f, g);
   check_error();
}
void get_active_uniform(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLint *e, GLenum *f, GLchar *g)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetActiveUniform != nullptr);
   gl::detail::glGetActiveUniform(a, b, c, d, e, f, g);
   check_error();
}
void get_attached_shaders(GLuint a, GLsizei b, GLsizei *c, GLuint *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetAttachedShaders != nullptr);
   gl::detail::glGetAttachedShaders(a, b, c, d);
   check_error();
}
GLint get_attrib_location(GLuint a, const GLchar *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetAttribLocation != nullptr);
   GLint res = gl::detail::glGetAttribLocation(a, b);
   check_error();
   return res;
}
void get_program_iv(GLuint a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetProgramiv != nullptr);
   gl::detail::glGetProgramiv(a, b, c);
   check_error();
}
void get_program_info_log(GLuint a, GLsizei b, GLsizei *c, GLchar *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetProgramInfoLog != nullptr);
   gl::detail::glGetProgramInfoLog(a, b, c, d);
   check_error();
}
void get_shader_iv(GLuint a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetShaderiv != nullptr);
   gl::detail::glGetShaderiv(a, b, c);
   check_error();
}
void get_shader_info_log(GLuint a, GLsizei b, GLsizei *c, GLchar *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetShaderInfoLog != nullptr);
   gl::detail::glGetShaderInfoLog(a, b, c, d);
   check_error();
}
void get_shader_source(GLuint a, GLsizei b, GLsizei *c, GLchar *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetShaderSource != nullptr);
   gl::detail::glGetShaderSource(a, b, c, d);
   check_error();
}
GLint get_uniform_location(GLuint program, const GLchar *name)
{
   assert(name != nullptr);
#if defined(LOG_GL)
   slog_trace("get_uniform_location(program = %u, name = %s)",
      program, name != nullptr ? name : "(null)");
#endif
   assert(gl::detail::glGetUniformLocation != nullptr);
   GLint res = gl::detail::glGetUniformLocation(program, name);
#if defined(LOG_GL)
   log_trace(":%d", res);
#endif
   check_error();
   return res;
}
void get_uniform_fv(GLuint a, GLint b, GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetUniformfv != nullptr);
   gl::detail::glGetUniformfv(a, b, c);
   check_error();
}
void get_uniform_iv(GLuint a, GLint b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetUniformiv != nullptr);
   gl::detail::glGetUniformiv(a, b, c);
   check_error();
}

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void get_vertex_attrib_dv(GLuint a, GLenum b, GLdouble *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetVertexAttribdv != nullptr);
   gl::detail::glGetVertexAttribdv(a, b, c);
   check_error();
}
#endif

void get_vertex_attrib_fv(GLuint a, GLenum b, GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetVertexAttribfv != nullptr);
   gl::detail::glGetVertexAttribfv(a, b, c);
   check_error();
}
void get_vertex_attrib_iv(GLuint a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetVertexAttribiv != nullptr);
   gl::detail::glGetVertexAttribiv(a, b, c);
   check_error();
}
void get_vertex_attrib_pointer_v(GLuint a, GLenum b, GLvoid* *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetVertexAttribPointerv != nullptr);
   gl::detail::glGetVertexAttribPointerv(a, b, c);
   check_error();
}
GLboolean is_program(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("is_program(%u)", a);
#endif
   assert(gl::detail::glIsProgram != nullptr);
   GLboolean res = gl::detail::glIsProgram(a);
   check_error();
   return res;
}
GLboolean is_shader(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("is_shader(%u)", a);
#endif
   assert(gl::detail::glIsShader != nullptr);
   GLboolean res = gl::detail::glIsShader(a);
   check_error();
   return res;
}
void link_program(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("link_program(%u)", a);
#endif
   assert(gl::detail::glLinkProgram != nullptr);
   gl::detail::glLinkProgram(a);
   check_error();
}
void shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("shader_source(shader = %u, count = %u, string = %p, length = %p)",
      shader,
      static_cast<unsigned int>(count),
      string,
      length);
#endif
   assert(gl::detail::glShaderSource != nullptr);
   gl::detail::glShaderSource(shader, count, string, length);
   check_error();
}
void use_program(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("use_program(program = %u)", a);
#endif
   assert(gl::detail::glUseProgram != nullptr);
   gl::detail::glUseProgram(a);
   check_error();
}
void uniform_1f(GLint a, GLfloat b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform1f != nullptr);
   gl::detail::glUniform1f(a, b);
   check_error();
}
void uniform_2f(GLint a, GLfloat b, GLfloat c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform2f != nullptr);
   gl::detail::glUniform2f(a, b, c);
   check_error();
}
void uniform_3f(GLint a, GLfloat b, GLfloat c, GLfloat d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform3f != nullptr);
   gl::detail::glUniform3f(a, b, c, d);
   check_error();
}
void uniform_4f(GLint a, GLfloat b, GLfloat c, GLfloat d, GLfloat e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform4f != nullptr);
   gl::detail::glUniform4f(a, b, c, d, e);
   check_error();
}
void uniform_1i(GLint location, GLint v0)
{
#if defined(LOG_GL) || defined(LOG_GL_UNIFORM)
   log_trace("uniform_1i(location = %d, v0 = %d)", location, v0);
#endif
   assert(gl::detail::glUniform1i != nullptr);
   gl::detail::glUniform1i(location, v0);
   check_error();
}
void uniform_2i(GLint a, GLint b, GLint c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform2i != nullptr);
   gl::detail::glUniform2i(a, b, c);
   check_error();
}
void uniform_3i(GLint a, GLint b, GLint c, GLint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform3i != nullptr);
   gl::detail::glUniform3i(a, b, c, d);
   check_error();
}
void uniform_4i(GLint a, GLint b, GLint c, GLint d, GLint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform4i != nullptr);
   gl::detail::glUniform4i(a, b, c, d, e);
   check_error();
}
void uniform_1fv(GLint a, GLsizei b, const GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform1fv != nullptr);
   gl::detail::glUniform1fv(a, b, c);
   check_error();
}
void uniform_2fv(GLint a, GLsizei b, const GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform2fv != nullptr);
   gl::detail::glUniform2fv(a, b, c);
   check_error();
}
void uniform_3fv(GLint location, GLsizei count, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform3fv != nullptr);
   gl::detail::glUniform3fv(location, count, value);
   check_error();
}
void uniform_4fv(GLint location, GLsizei count, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform4fv != nullptr);
   gl::detail::glUniform4fv(location, count, value);
   check_error();
}
void uniform_1iv(GLint location, GLsizei count, const GLint *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform1iv != nullptr);
   gl::detail::glUniform1iv(location, count, value);
   check_error();
}
void uniform_2iv(GLint a, GLsizei b, const GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform2iv != nullptr);
   gl::detail::glUniform2iv(a, b, c);
   check_error();
}
void uniform_3iv(GLint a, GLsizei b, const GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform3iv != nullptr);
   gl::detail::glUniform3iv(a, b, c);
   check_error();
}
void uniform_4iv(GLint a, GLsizei b, const GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform4iv != nullptr);
   gl::detail::glUniform4iv(a, b, c);
   check_error();
}
void uniform_matrix_2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformMatrix2fv != nullptr);
   gl::detail::glUniformMatrix2fv(location, count, transpose, value);
   check_error();
}
void uniform_matrix_3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformMatrix3fv != nullptr);
   gl::detail::glUniformMatrix3fv(location, count, transpose, value);
   check_error();
}
void uniform_matrix_4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformMatrix4fv != nullptr);
   gl::detail::glUniformMatrix4fv(location, count, transpose, value);
   check_error();
}
void validate_program(GLuint program)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glValidateProgram != nullptr);
   gl::detail::glValidateProgram(program);
   check_error();
}
void vertex_attrib_1f(GLuint a, GLfloat b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib1f != nullptr);
   gl::detail::glVertexAttrib1f(a, b);
   check_error();
}
void vertex_attrib_1fv(GLuint a, const GLfloat *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib1fv != nullptr);
   gl::detail::glVertexAttrib1fv(a, b);
   check_error();
}
void vertex_attrib_2f(GLuint a, GLfloat b, GLfloat c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib2f != nullptr);
   gl::detail::glVertexAttrib2f(a, b, c);
   check_error();
}
void vertex_attrib_2fv(GLuint a, const GLfloat *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib2fv != nullptr);
   gl::detail::glVertexAttrib2fv(a, b);
   check_error();
}
void vertex_attrib_3f(GLuint a, GLfloat b, GLfloat c, GLfloat d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib3f != nullptr);
   gl::detail::glVertexAttrib3f(a, b, c, d);
   check_error();
}
void vertex_attrib_3fv(GLuint a, const GLfloat *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib3fv != nullptr);
   gl::detail::glVertexAttrib3fv(a, b);
   check_error();
}
void vertex_attrib_4f(GLuint a, GLfloat b, GLfloat c, GLfloat d, GLfloat e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4f != nullptr);
   gl::detail::glVertexAttrib4f(a, b, c, d, e);
   check_error();
}
void vertex_attrib_4fv(GLuint a, const GLfloat *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4fv != nullptr);
   gl::detail::glVertexAttrib4fv(a, b);
   check_error();
}
void vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB)
   log_trace("vertex_attrib_pointer(index = %u, size = %d, type = %s, normalized = %d, stride = %d, pointer = %p)",
      index, size, enum_string(type), normalized, static_cast<unsigned int>(stride), pointer);
#endif
   assert(gl::detail::glVertexAttribPointer != nullptr);
   gl::detail::glVertexAttribPointer(index, size, type, normalized, stride, pointer);
   check_error();
}

//  OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void draw_buffers(GLsizei a, const GLenum * b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawBuffers != nullptr);
   gl::detail::glDrawBuffers(a, b);
   check_error();
}
#endif
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void vertex_attrib_1d(GLuint a, GLdouble b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib1d != nullptr);
   gl::detail::glVertexAttrib1d(a, b);
   check_error();
}
void vertex_attrib_1dv(GLuint a, const GLdouble *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib1dv != nullptr);
   gl::detail::glVertexAttrib1dv(a, b);
   check_error();
}
void vertex_attrib_1s(GLuint a, GLshort b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib1s != nullptr);
   gl::detail::glVertexAttrib1s(a, b);
   check_error();
}
void vertex_attrib_1sv(GLuint a, const GLshort *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib1sv != nullptr);
   gl::detail::glVertexAttrib1sv(a, b);
   check_error();
}
void vertex_attrib_2d(GLuint a, GLdouble b, GLdouble c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib2d != nullptr);
   gl::detail::glVertexAttrib2d(a, b, c);
   check_error();
}
void vertex_attrib_2dv(GLuint a, const GLdouble *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib2dv != nullptr);
   gl::detail::glVertexAttrib2dv(a, b);
   check_error();
}
void vertex_attrib_2s(GLuint a, GLshort b, GLshort c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib2s != nullptr);
   gl::detail::glVertexAttrib2s(a, b, c);
   check_error();
}
void vertex_attrib_2sv(GLuint a, const GLshort *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib2sv != nullptr);
   gl::detail::glVertexAttrib2sv(a, b);
   check_error();
}
void vertex_attrib_3d(GLuint a, GLdouble b, GLdouble c, GLdouble d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib3d != nullptr);
   gl::detail::glVertexAttrib3d(a, b, c, d);
   check_error();
}
void vertex_attrib_3dv(GLuint a, const GLdouble *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib3dv != nullptr);
   gl::detail::glVertexAttrib3dv(a, b);
   check_error();
}
void vertex_attrib_3s(GLuint a, GLshort b, GLshort c, GLshort d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib3s != nullptr);
   gl::detail::glVertexAttrib3s(a, b, c, d);
   check_error();
}
void vertex_attrib_3sv(GLuint a, const GLshort *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib3sv != nullptr);
   gl::detail::glVertexAttrib3sv(a, b);
   check_error();
}
void vertex_attrib_4Nbv(GLuint a, const GLbyte *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4Nbv != nullptr);
   gl::detail::glVertexAttrib4Nbv(a, b);
   check_error();
}
void vertex_attrib_4Niv(GLuint a, const GLint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4Niv != nullptr);
   gl::detail::glVertexAttrib4Niv(a, b);
   check_error();
}
void vertex_attrib_4Nsv(GLuint a, const GLshort *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4Nsv != nullptr);
   gl::detail::glVertexAttrib4Nsv(a, b);
   check_error();
}
void vertex_attrib_4Nub(GLuint a, GLubyte b, GLubyte c, GLubyte d, GLubyte e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4Nub != nullptr);
   gl::detail::glVertexAttrib4Nub(a, b, c, d, e);
   check_error();
}
void vertex_attrib_4Nubv(GLuint a, const GLubyte *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4Nubv != nullptr);
   gl::detail::glVertexAttrib4Nubv(a, b);
   check_error();
}
void vertex_attrib_4Nuiv(GLuint a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4Nuiv != nullptr);
   gl::detail::glVertexAttrib4Nuiv(a, b);
   check_error();
}
void vertex_attrib_4Nusv(GLuint a, const GLushort *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4Nusv != nullptr);
   gl::detail::glVertexAttrib4Nusv(a, b);
   check_error();
}
void vertex_attrib_4bv(GLuint a, const GLbyte *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4bv != nullptr);
   gl::detail::glVertexAttrib4bv(a, b);
   check_error();
}
void vertex_attrib_4d(GLuint a, GLdouble b, GLdouble c, GLdouble d, GLdouble e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4d != nullptr);
   gl::detail::glVertexAttrib4d(a, b, c, d, e);
   check_error();
}
void vertex_attrib_4dv(GLuint a, const GLdouble *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4dv != nullptr);
   gl::detail::glVertexAttrib4dv(a, b);
   check_error();
}
void vertex_attrib_4iv(GLuint a, const GLint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4iv != nullptr);
   gl::detail::glVertexAttrib4iv(a, b);
   check_error();
}
void vertex_attrib_4s(GLuint a, GLshort b, GLshort c, GLshort d, GLshort e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4s != nullptr);
   gl::detail::glVertexAttrib4s(a, b, c, d, e);
   check_error();
}
void vertex_attrib_4sv(GLuint a, const GLshort *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4sv != nullptr);
   gl::detail::glVertexAttrib4sv(a, b);
   check_error();
}
void vertex_attrib_4ubv(GLuint a, const GLubyte *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4ubv != nullptr);
   gl::detail::glVertexAttrib4ubv(a, b);
   check_error();
}
void vertex_attrib_4uiv(GLuint a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4uiv != nullptr);
   gl::detail::glVertexAttrib4uiv(a, b);
   check_error();
}
void vertex_attrib_4usv(GLuint a, const GLushort *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttrib4usv != nullptr);
   gl::detail::glVertexAttrib4usv(a, b);
   check_error();
}
#endif

/*  GL_VERSION_2_1  */ 

//  OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void uniform_matrix_2x3fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformMatrix2x3fv != nullptr);
   gl::detail::glUniformMatrix2x3fv(a, b, c, d);
   check_error();
}
void uniform_matrix_3x2fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformMatrix2x3fv != nullptr);
   gl::detail::glUniformMatrix3x2fv(a, b, c, d);
   check_error();
}
void niform_matrix_2x4fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformMatrix2x4fv != nullptr);
   gl::detail::glUniformMatrix2x4fv(a, b, c, d);
   check_error();
}
void uniform_matrix_4x2fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformMatrix4x2fv != nullptr);
   gl::detail::glUniformMatrix4x2fv(a, b, c, d);
   check_error();
}
void uniform_matrix_3x4fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformMatrix3x4fv != nullptr);
   gl::detail::glUniformMatrix3x4fv(a, b, c, d);
   check_error();
}
void uniform_matrix_4x3fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformMatrix4x3fv != nullptr);
   gl::detail::glUniformMatrix4x3fv(a, b, c, d);
   check_error();
}
#endif

/*  GL_VERSION_3_0  */ 

/* ARB_framebuffer_object */ 
GLboolean is_renderbuffer(GLuint a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glIsRenderbuffer != nullptr);
   GLboolean res = gl::detail::glIsRenderbuffer(a);
   check_error();
   return res;
}
void bind_renderbuffer(GLenum target, GLuint renderbuffer)
{
#if defined(LOG_GL)
   log_trace("bind_renderbuffer(target = %s, renderbuffer = %u)",
      enum_string(target), renderbuffer);
#endif
   assert(gl::detail::glBindRenderbuffer);
   gl::detail::glBindRenderbuffer(target, renderbuffer);
   check_error();
}
void delete_renderbuffers(GLsizei a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDeleteRenderbuffers != nullptr);
   gl::detail::glDeleteRenderbuffers(a, b);
   check_error();
}
void gen_renderbuffers(GLsizei a, GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGenRenderbuffers != nullptr);
   gl::detail::glGenRenderbuffers(a, b);
   check_error();
}
void renderbuffer_storage(GLenum a, GLenum b, GLsizei c, GLsizei d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glRenderbufferStorage != nullptr);
   gl::detail::glRenderbufferStorage(a, b, c, d);
   check_error();
}
void get_renderbuffer_parameter_iv(GLenum a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetRenderbufferParameteriv != nullptr);
   gl::detail::glGetRenderbufferParameteriv(a, b, c);
   check_error();
}
GLboolean is_framebuffer(GLuint a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glIsFramebuffer != nullptr);
   GLboolean res = gl::detail::glIsFramebuffer(a);
   check_error();
   return res;
}
void bind_framebuffer(GLenum target, GLuint framebuffer)
{
#if defined(LOG_GL)
   log_trace("bind_framebuffer(target = %s, framebuffer = %d)",
      enum_string(target), framebuffer);
#endif
   assert(gl::detail::glBindFramebuffer != nullptr);
   gl::detail::glBindFramebuffer(target, framebuffer);
   check_error();
}
void delete_framebuffers(GLsizei a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDeleteFramebuffers != nullptr);
   gl::detail::glDeleteFramebuffers(a, b);
   check_error();
}
void gen_framebuffers(GLsizei a, GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGenFramebuffers != nullptr);
   gl::detail::glGenFramebuffers(a, b);
   check_error();
}
   GLenum check_framebuffer_status(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glCheckFramebufferStatus != nullptr);
   GLenum res = gl::detail::glCheckFramebufferStatus(a);
   check_error();
   return res;
}
void framebuffer_texture_2d(GLenum a, GLenum b, GLenum c, GLuint d, GLint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glFramebufferTexture2D != nullptr);
   gl::detail::glFramebufferTexture2D(a, b, c, d, e);
   check_error();
}
void framebuffer_renderbuffer(GLenum a, GLenum b, GLenum c, GLuint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glFramebufferRenderbuffer != nullptr);
   gl::detail::glFramebufferRenderbuffer(a, b, c, d);
   check_error();
}
void get_framebuffer_attachment_parameter_iv(GLenum a, GLenum b, GLenum c, GLint *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetFramebufferAttachmentParameteriv != nullptr);
   gl::detail::glGetFramebufferAttachmentParameteriv(a, b, c, d);
   check_error();
}
void generate_mipmap(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGenerateMipmap != nullptr);
   gl::detail::glGenerateMipmap(a);
   check_error();
}

//  OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void blit_framebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBlitFramebuffer != nullptr);
   gl::detail::glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
   check_error();
}
void renderbuffer_storage_multisample(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glRenderbufferStorageMultisample != nullptr);
   gl::detail::glRenderbufferStorageMultisample(a, b, c, d, e);
   check_error();
}
void framebuffer_texture_layer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glFramebufferTextureLayer != nullptr);
   gl::detail::glFramebufferTextureLayer(target, attachment, texture, level, layer);
   check_error();
}
#endif
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void framebuffer_texture_1d(GLenum a, GLenum b, GLenum c, GLuint d, GLint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glFramebufferTexture1D != nullptr);
   gl::detail::glFramebufferTexture1D(a, b, c, d, e);
   check_error();
}
void framebuffer_texture_3d(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLint f)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glFramebufferTexture3D != nullptr);
   gl::detail::glFramebufferTexture3D(a, b, c, d, e, f);
   check_error();
}
#endif

/* ARB_map_buffer_range */

// OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
GLvoid* map_buffer_range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
   static unsigned long s_map_buffer_range_counter = 0;

#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER)
   slog_trace("map_buffer_range(target = %s, offset = %u, length = %u, access = 0x%x)  counter = %lu",
      enum_string(target),
      static_cast<unsigned int>(offset),
      static_cast<unsigned int>(length),
      access,
      s_map_buffer_range_counter);
   if (access & GL_MAP_READ_BIT)                log_trace(":read");
   if (access & GL_MAP_WRITE_BIT)               log_trace(":write");
   if (access & GL_MAP_INVALIDATE_RANGE_BIT)    log_trace(":invalidate_range");
   if (access & GL_MAP_INVALIDATE_BUFFER_BIT)   log_trace(":invalidate_buffer");
   if (access & GL_MAP_FLUSH_EXPLICIT_BIT)      log_trace(":flush_explicit");
   if (access & GL_MAP_UNSYNCHRONIZED_BIT)      log_trace(":unsynchronized");
#endif

   //if (s_map_buffer_range_counter == ~0UL)
   //   log_trace("counter breakpoint");
   ++s_map_buffer_range_counter;

   assert(gl::detail::glMapBufferRange != nullptr);
   GLvoid* res = gl::detail::glMapBufferRange(target, offset, length, access);
#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER)
   log_trace(":%p", res);
#endif
   check_error();
   return res;
}
void flush_mapped_buffer_range(GLenum target, GLintptr offset, GLsizeiptr length)
{
#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER)
   log_trace("flush_mapped_buffer_range(target = %s, offset = %u, length = %u)",
      enum_string(target),
      static_cast<unsigned int>(offset),
      static_cast<unsigned int>(length));
#endif
   assert(gl::detail::glFlushMappedBufferRange != nullptr);
   gl::detail::glFlushMappedBufferRange(target, offset, length);
   check_error();
}
#endif

/* ARB_vertex_array_object */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void bind_vertex_array(GLuint array_)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB) || defined(LOG_GL_VAO)
   log_trace("bind_vertex_array(array = %u)", array_);
#endif
   assert(gl::detail::glBindVertexArray != nullptr);
   gl::detail::glBindVertexArray(array_);
   check_error();
}
void delete_vertex_arrays(GLsizei n, const GLuint *arrays)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB) || defined(LOG_GL_VAO)
   log_trace("delete_vertex_arrays(n = %u, arrays = %p)",
      static_cast<unsigned int>(n), arrays);
#endif
   assert(gl::detail::glDeleteVertexArrays != nullptr);
   gl::detail::glDeleteVertexArrays(n, arrays);
   check_error();
}
void gen_vertex_arrays(GLsizei n, GLuint *arrays)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB) || defined(LOG_GL_VAO)
   slog_trace("gen_vertex_arrays(n = %u, arrays = %p)", 
      static_cast<unsigned int>(n), arrays);
#endif
   assert(gl::detail::glGenVertexArrays != nullptr);
   gl::detail::glGenVertexArrays(n, arrays);
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB) || defined(LOG_GL_VAO)
   if (n > 0 && arrays)
      log_trace(":%u", arrays[0]);
#endif
   check_error();
}
GLboolean is_vertex_array(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB) || defined(LOG_GL_VAO)
   log_trace("is_vertex_array(array = %u)", a);
#endif

   assert(gl::detail::glIsVertexArray != nullptr);
   GLboolean res = gl::detail::glIsVertexArray(a);
   check_error();
   return res;
}
void get_integer_i_v(GLenum a, GLuint b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetIntegeri_v != nullptr);
   gl::detail::glGetIntegeri_v(a, b, c);
   check_error();
}
void begin_transform_feedback(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBeginTransformFeedback != nullptr);
   gl::detail::glBeginTransformFeedback(a);
   check_error();
}
void end_transform_feedback(void)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glEndTransformFeedback != nullptr);
   gl::detail::glEndTransformFeedback();
   check_error();
}
void bind_buffer_range(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   log_trace("bind_buffer_range(%s, %u, %u, %u, %u)",
      enum_string(target),
      index,
      buffer,
      static_cast<unsigned int>(offset),
      static_cast<unsigned int>(size));
#endif
   assert(gl::detail::glBindBufferRange != nullptr);
   gl::detail::glBindBufferRange(target, index, buffer, offset, size);
   check_error();
}
void bind_buffer_base(GLenum target, GLuint index, GLuint buffer)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   log_trace("bind_buffer_base(target = %s, index = %u, buffer = %u)", enum_string(target), index, buffer);
#endif
   assert(gl::detail::glBindBufferBase != nullptr);
   gl::detail::glBindBufferBase(target, index, buffer);
   check_error();
}
void transform_feedback_varyings(GLuint a, GLsizei b, const GLchar* *c, GLenum d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glTransformFeedbackVaryings != nullptr);
   gl::detail::glTransformFeedbackVaryings(a, b, c, d);
   check_error();
}
void get_transform_feedback_varying(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLsizei *e, GLenum *f, GLchar *g)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetTransformFeedbackVarying != nullptr);
   gl::detail::glGetTransformFeedbackVarying(a, b, c, d, e, f, g);
   check_error();
}
void vertex_attrib_i_pointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB)
   log_trace("vertex_attrib_i_pointer(index = %u, size = %d, type = %s, stride = %u, pointer = %p)",
      index,
      size,
      enum_string(type),
      static_cast<unsigned int>(stride),
      pointer);
#endif
   assert(gl::detail::glVertexAttribIPointer != nullptr);
   gl::detail::glVertexAttribIPointer(index, size, type, stride, pointer);
   check_error();
}
void get_vertex_attrib_iiv(GLuint a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetVertexAttribIiv != nullptr);
   gl::detail::glGetVertexAttribIiv(a, b, c);
   check_error();
}
void get_vertex_attrib_iuiv(GLuint a, GLenum b, GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetVertexAttribIuiv != nullptr);
   gl::detail::glGetVertexAttribIuiv(a, b, c);
   check_error();
}
void vertex_attrib_i4i(GLuint a, GLint b, GLint c, GLint d, GLint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI4i != nullptr);
   gl::detail::glVertexAttribI4i(a, b, c, d, e);
   check_error();
}
void vertex_attrib_i4ui(GLuint a, GLuint b, GLuint c, GLuint d, GLuint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI4ui != nullptr);
   gl::detail::glVertexAttribI4ui(a, b, c, d, e);
   check_error();
}
void vertex_attrib_i4iv(GLuint a, const GLint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI4iv != nullptr);
   gl::detail::glVertexAttribI4iv(a, b);
   check_error();
}
void vertex_attrib_i4uiv(GLuint a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI4uiv != nullptr);
   gl::detail::glVertexAttribI4uiv(a, b);
   check_error();
}
void get_uniform_uiv(GLuint a, GLint b, GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetUniformuiv != nullptr);
   gl::detail::glGetUniformuiv(a, b, c);
   check_error();
}
GLint get_frag_data_location(GLuint a, const GLchar *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetFragDataLocation != nullptr);
   GLint res = gl::detail::glGetFragDataLocation(a, b);
   check_error();
   return res;
}
void uniform_1ui(GLint a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform1ui != nullptr);
   gl::detail::glUniform1ui(a, b);
   check_error();
}
void uniform_2ui(GLint a, GLuint b, GLuint c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform2ui != nullptr);
   gl::detail::glUniform2ui(a, b, c);
   check_error();
}
void uniform_3ui(GLint a, GLuint b, GLuint c, GLuint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform3ui != nullptr);
   gl::detail::glUniform3ui(a, b, c, d);
   check_error();
}
void uniform_4ui(GLint a, GLuint b, GLuint c, GLuint d, GLuint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform4ui != nullptr);
   gl::detail::glUniform4ui(a, b, c, d, e);
   check_error();
}
void uniform_1uiv(GLint a, GLsizei b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform1uiv != nullptr);
   gl::detail::glUniform1uiv(a, b, c);
   check_error();
}
void uniform_2uiv(GLint a, GLsizei b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform2uiv != nullptr);
   gl::detail::glUniform2uiv(a, b, c);
   check_error();
}
void uniform_3uiv(GLint a, GLsizei b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform3uiv != nullptr);
   gl::detail::glUniform3uiv(a, b, c);
   check_error();
}
void uniform_4uiv(GLint a, GLsizei b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniform4uiv != nullptr);
   gl::detail::glUniform4uiv(a, b, c);
   check_error();
}
void clear_buffer_iv(GLenum a, GLint b, const GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glClearBufferiv != nullptr);
   gl::detail::glClearBufferiv(a, b, c);
   check_error();
}
void clear_buffer_uiv(GLenum a, GLint b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glClearBufferuiv != nullptr);
   gl::detail::glClearBufferuiv(a, b, c);
   check_error();
}
void clear_buffer_fv(GLenum a, GLint b, const GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glClearBufferfv != nullptr);
   gl::detail::glClearBufferfv(a, b, c);
   check_error();
}
void clear_buffer_fi(GLenum a, GLint b, GLfloat c, GLint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glClearBufferfi != nullptr);
   gl::detail::glClearBufferfi(a, b, c, d);
   check_error();
}
   const GLubyte * get_string_i(GLenum a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetStringi != nullptr);
   const GLubyte * res = gl::detail::glGetStringi(a, b);
   check_error();
   return res;
}
#endif
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void color_mask_i(GLuint a, GLboolean b, GLboolean c, GLboolean d, GLboolean e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glColorMaski != nullptr);
   gl::detail::glColorMaski(a, b, c, d, e);
   check_error();
}
void get_boolean_i_v(GLenum a, GLuint b, GLboolean *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetBooleani_v != nullptr);
   gl::detail::glGetBooleani_v(a, b, c);
   check_error();
}
void enable_i(GLenum a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glEnablei != nullptr);
   gl::detail::glEnablei(a, b);
   check_error();
}
void disable_i(GLenum a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDisablei != nullptr);
   gl::detail::glDisablei(a, b);
   check_error();
}
   GLboolean is_enabled_i(GLenum a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glIsEnabledi != nullptr);
   GLboolean res = gl::detail::glIsEnabledi(a, b);
   check_error();
   return res;
}
void clamp_color(GLenum a, GLenum b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glClampColor != nullptr);
   gl::detail::glClampColor(a, b);
   check_error();
}
void begin_conditional_render(GLuint a, GLenum b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBeginConditionalRender != nullptr);
   gl::detail::glBeginConditionalRender(a, b);
   check_error();
}
void end_conditional_render(void)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glEndConditionalRender != nullptr);
   gl::detail::glEndConditionalRender();
   check_error();
}
void vertex_attrib_i1i(GLuint a, GLint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI1i != nullptr);
   gl::detail::glVertexAttribI1i(a, b);
   check_error();
}
void vertex_attrib_i2i(GLuint a, GLint b, GLint c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI2i != nullptr);
   gl::detail::glVertexAttribI2i(a, b, c);
   check_error();
}
void vertex_attrib_i3i(GLuint a, GLint b, GLint c, GLint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI3i != nullptr);
   gl::detail::glVertexAttribI3i(a, b, c, d);
   check_error();
}
void vertex_attrib_i1ui(GLuint a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI1ui != nullptr);
   gl::detail::glVertexAttribI1ui(a, b);
   check_error();
}
void vertex_attrib_i2ui(GLuint a, GLuint b, GLuint c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI2ui != nullptr);
   gl::detail::glVertexAttribI2ui(a, b, c);
   check_error();
}
void vertex_attrib_i3ui(GLuint a, GLuint b, GLuint c, GLuint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI3ui != nullptr);
   gl::detail::glVertexAttribI3ui(a, b, c, d);
   check_error();
}
void vertex_attrib_i1iv(GLuint a, const GLint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI1iv != nullptr);
   gl::detail::glVertexAttribI1iv(a, b);
   check_error();
}
void vertex_attrib_i2iv(GLuint a, const GLint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI2iv != nullptr);
   gl::detail::glVertexAttribI2iv(a, b);
   check_error();
}
void vertex_attrib_i3iv(GLuint a, const GLint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI3iv != nullptr);
   gl::detail::glVertexAttribI3iv(a, b);
   check_error();
}
void vertex_attrib_i1uiv(GLuint a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI1uiv != nullptr);
   gl::detail::glVertexAttribI1uiv(a, b);
   check_error();
}
void vertex_attrib_i2uiv(GLuint a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI2uiv != nullptr);
   gl::detail::glVertexAttribI2uiv(a, b);
   check_error();
}
void vertex_attrib_i3uiv(GLuint a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI3uiv != nullptr);
   gl::detail::glVertexAttribI3uiv(a, b);
   check_error();
}
void vertex_attrib_i4bv(GLuint a, const GLbyte *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI4bv != nullptr);
   gl::detail::glVertexAttribI4bv(a, b);
   check_error();
}
void vertex_attrib_i4sv(GLuint a, const GLshort *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI4sv != nullptr);
   gl::detail::glVertexAttribI4sv(a, b);
   check_error();
}
void vertex_attrib_i4ubv(GLuint a, const GLubyte *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI4ubv != nullptr);
   gl::detail::glVertexAttribI4ubv(a, b);
   check_error();
}
void vertex_attrib_i4usv(GLuint a, const GLushort *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glVertexAttribI4usv != nullptr);
   gl::detail::glVertexAttribI4usv(a, b);
   check_error();
}
void bind_frag_data_location(GLuint program, GLuint colorNumber, const GLchar *name)
{
   assert(name != nullptr);
#if defined(LOG_GL)
   log_trace("bind_frag_data_location(program = %u, colorNumber = %u, name = %s)",
      program, colorNumber, name != nullptr ? name : "(null)");
#endif
   assert(gl::detail::glBindFragDataLocation != nullptr);
   gl::detail::glBindFragDataLocation(program, colorNumber, name);
   check_error();
}
void tex_parameter_iiv(GLenum a, GLenum b, const GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glTexParameterIiv != nullptr);
   gl::detail::glTexParameterIiv(a, b, c);
   check_error();
}
void tex_parameter_iuiv(GLenum a, GLenum b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glTexParameterIuiv != nullptr);
   gl::detail::glTexParameterIuiv(a, b, c);
   check_error();
}
void get_tex_parameter_iiv(GLenum a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetTexParameterIiv != nullptr);
   gl::detail::glGetTexParameterIiv(a, b, c);
   check_error();
}
void get_tex_parameter_iuiv(GLenum a, GLenum b, GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetTexParameterIuiv != nullptr);
   gl::detail::glGetTexParameterIuiv(a, b, c);
   check_error();
}
#endif


/*  GL_VERSION_3_1  */ 

/*  ARB_copy_buffer */ 

//  OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void copy_buffer_sub_data(GLenum a, GLenum b, GLintptr c, GLintptr d, GLsizeiptr e) 
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glCopyBufferSubData != nullptr);
   gl::detail::glCopyBufferSubData(a, b, c, d, e);
   check_error();
}
#endif
/* ARB_uniform_buffer_object */ 

//  OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void get_uniform_indices(GLuint program, GLsizei uniformCount, const GLchar* *uniformNames, GLuint *uniformIndices)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetUniformIndices != nullptr);
   gl::detail::glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
   check_error();
}
void get_active_uniforms_iv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetActiveUniformsiv != nullptr);
   gl::detail::glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
   check_error();
}
GLuint get_uniform_block_index(GLuint program, const GLchar *uniformBlockName)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetUniformBlockIndex != nullptr);
   GLuint res = gl::detail::glGetUniformBlockIndex (program, uniformBlockName);
   check_error();
   return res;
}
void get_active_uniform_block_iv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetActiveUniformBlockiv != nullptr);
   gl::detail::glGetActiveUniformBlockiv (program, uniformBlockIndex, pname, params);
   check_error();
}
void get_active_uniform_block_name(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetActiveUniformBlockName != nullptr);
   gl::detail::glGetActiveUniformBlockName (program, uniformBlockIndex, bufSize, length, uniformBlockName);
   check_error();
}
void uniform_block_binding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glUniformBlockBinding != nullptr);
   gl::detail::glUniformBlockBinding (program, uniformBlockIndex, uniformBlockBinding);
   check_error();
}
#endif
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void get_active_uniform_name(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetActiveUniformName != nullptr);
   gl::detail::glGetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
   check_error();
}
#endif

//  GL_ARB_draw_instanced

//  OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void draw_arrays_instanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount)
{
#if defined(LOG_GL) || defined(LOG_GL_DRAW)
   log_trace("draw_arrays_instanced(mode = %s, first = %d, count = %u, primcount = %u)",
      enum_string(mode),
      first,
      static_cast<unsigned int>(count),
      static_cast<unsigned int>(primcount));
#endif
   assert(gl::detail::glDrawArraysInstanced != nullptr);
   gl::detail::glDrawArraysInstanced(mode, first, count, primcount);
   check_error();
}
void draw_elements_instanced(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount)
{
#if defined(LOG_GL)
   log_trace("draw_elements_instanced(mode = %s, count = %u, type = %s, indices = %p, primcout = %u)",
      enum_string(mode),
      static_cast<unsigned int>(count),
      enum_string(type),
      indices,
      primcount);
#endif
   assert(gl::detail::glDrawElementsInstanced != nullptr);
   gl::detail::glDrawElementsInstanced(mode, count, type, indices, primcount);
   check_error();
}
#endif
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void tex_buffer(GLenum a, GLenum b, GLuint c)
{
   LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glTexBuffer != nullptr);
   gl::detail::glTexBuffer(a, b, c);
   check_error();
}
void primitive_restart_index(GLuint a)
{
   LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glPrimitiveRestartIndex != nullptr);
   gl::detail::glPrimitiveRestartIndex(a);
   check_error();
}
#endif


/*  GL_VERSION_3_2  */ 

/* ARB_sync */ 

//  OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
GLsync fence_sync(GLenum a, GLbitfield b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glFenceSync != nullptr);
   GLsync res = gl::detail::glFenceSync(a, b);
   check_error();
   return res;
}
GLboolean is_sync(GLsync a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glIsSync != nullptr);
   GLboolean res = gl::detail::glIsSync(a);
   check_error();
   return res;
}
void delete_sync(GLsync a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDeleteSync != nullptr);
   gl::detail::glDeleteSync(a);
   check_error();
}
GLenum client_wait_sync(GLsync a, GLbitfield b, GLuint64 c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glClientWaitSync != nullptr);
   GLenum res = gl::detail::glClientWaitSync(a, b, c);
   check_error();
   return res;
}
void wait_sync(GLsync a, GLbitfield b, GLuint64 c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glWaitSync != nullptr);
   gl::detail::glWaitSync(a, b, c);
   check_error();
}
void get_integer_64v(GLenum a, GLint64 *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetInteger64v != nullptr);
   gl::detail::glGetInteger64v(a, b);
   check_error();
}
void get_sync_iv(GLsync a, GLenum b, GLsizei c, GLsizei *d, GLint *e)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetSynciv != nullptr);
   gl::detail::glGetSynciv(a, b, c, d, e);
   check_error();
}
void get_integer_64i_v(GLenum a, GLuint b, GLint64 *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetInteger64i_v != nullptr);
   gl::detail::glGetInteger64i_v(a, b, c);
   check_error();
}
void get_buffer_parameter_i64v(GLenum a, GLenum b, GLint64 *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetBufferParameteri64v != nullptr);
   gl::detail::glGetBufferParameteri64v(a, b, c);
   check_error();
}
#endif

/*  GL_ARB_sampler_objects  */ 

//  OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void gen_samplers(GLsizei count, GLuint *samplers)
{
   LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGenSamplers != nullptr);
   gl::detail::glGenSamplers(count, samplers);
   check_error();
}
void delete_samplers(GLsizei count, const GLuint *samplers)
{
   LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDeleteSamplers != nullptr);
   gl::detail::glDeleteSamplers(count, samplers);
   check_error();
}
GLboolean is_sampler(GLuint sampler)
{
   LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glIsSampler != nullptr);
   GLboolean res = gl::detail::glIsSampler(sampler);
   check_error();
   return res;
}
void bind_sampler(GLuint unit, GLuint sampler)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBindSampler != nullptr);
   gl::detail::glBindSampler(unit, sampler);
   check_error();
}
void sampler_parameter_i(GLuint sampler, GLenum pname, GLint param)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glSamplerParameteri != nullptr);
   gl::detail::glSamplerParameteri(sampler, pname, param);
   check_error();
}
void sampler_parameter_iv(GLuint sampler, GLenum pname, const GLint *param)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glSamplerParameteriv != nullptr);
   gl::detail::glSamplerParameteriv(sampler, pname, param);
   check_error();
}
void sampler_parameter_f(GLuint sampler, GLenum pname, GLfloat param)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glSamplerParameterf != nullptr);
   gl::detail::glSamplerParameterf(sampler, pname, param);
   check_error();
}
void sampler_parameter_fv(GLuint sampler, GLenum pname, const GLfloat *param)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glSamplerParameterfv != nullptr);
   gl::detail::glSamplerParameterfv(sampler, pname, param);
   check_error();
}
void get_sampler_parameter_iv(GLuint sampler, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetSamplerParameteriv != nullptr);
   gl::detail::glGetSamplerParameteriv(sampler, pname, params);
   check_error();
}
void get_sampler_parameter_fv(GLuint sampler, GLenum pname, GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetSamplerParameterfv != nullptr);
   gl::detail::glGetSamplerParameterfv(sampler, pname, params);
   check_error();
}
#endif
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void sampler_parameter_i_iv(GLuint sampler, GLenum pname, const GLint *param)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glSamplerParameterIiv != nullptr);
   gl::detail::glSamplerParameterIiv(sampler, pname, param);
   check_error();
}
void sampler_parameter_i_uiv(GLuint sampler, GLenum pname, const GLuint *param)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glSamplerParameterIuiv != nullptr);
   gl::detail::glSamplerParameterIuiv(sampler, pname, param);
   check_error();
}
void get_sampler_parameter_i_iv(GLuint sampler, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetSamplerParameterIiv != nullptr);
   gl::detail::glGetSamplerParameterIiv(sampler, pname, params);
   check_error();
}
void get_sampler_parameter_i_uiv(GLuint sampler, GLenum pname, GLuint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetSamplerParameterIuiv != nullptr);
   gl::detail::glGetSamplerParameterIuiv(sampler, pname, params);
   check_error();
}
#endif

#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void framebuffer_texture(GLenum a, GLenum b, GLuint c, GLint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glFramebufferTexture != nullptr);
   gl::detail::glFramebufferTexture(a, b, c, d);
   check_error();
}
#endif

/*  3.2  */ 
/* ARB_draw_elements_base_vertex */ 
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void draw_elements_base_vertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex)
{
#if defined(LOG_GL)
   log_trace("draw_elements_base_vertex(mode = %s, count = %d, type = %s, indices = %p, basevertex = %u)",
      enum_string(mode),
      static_cast<unsigned int>(count),
      enum_string(type),
      indices,
      static_cast<unsigned int>(basevertex));
#endif
   assert(gl::detail::glDrawElementsBaseVertex != nullptr);
   gl::detail::glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
   check_error();
}
void draw_range_elements_base_vertex(GLenum a, GLuint b, GLuint c, GLsizei d, GLenum e, const GLvoid *f, GLint g)
{
   LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawRangeElementsBaseVertex != nullptr);
   gl::detail::glDrawRangeElementsBaseVertex(a, b, c, d, e, f, g);
   check_error();
}
void draw_elements_instanced_base_vertex(GLenum a, GLsizei b, GLenum c, const GLvoid *d, GLsizei e, GLint f)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawElementsInstancedBaseVertex != nullptr);
   gl::detail::glDrawElementsInstancedBaseVertex(a, b, c, d, e, f);
   check_error();
}
void multi_draw_elements_base_vertex(GLenum a, const GLsizei *b, GLenum c, const GLvoid* *d, GLsizei e, const GLint *f)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glMultiDrawElementsBaseVertex != nullptr);
   gl::detail::glMultiDrawElementsBaseVertex(a, b, c, d, e, f);
   check_error();
}
#endif

/* ARB_provoking_vertex */ 
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void provoking_vertex(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glProvokingVertex != nullptr);
   gl::detail::glProvokingVertex(a);
   check_error();
}
#endif

/* ARB_texture_multisample */
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void tex_image_2d_multisample(GLenum a, GLsizei b, GLint c, GLsizei d, GLsizei e, GLboolean f)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glTexImage2DMultisample != nullptr);
   gl::detail::glTexImage2DMultisample(a, b, c, d, e, f);
   check_error();
}
void tex_image_3d_multisample(GLenum a, GLsizei b, GLint c, GLsizei d, GLsizei e, GLsizei f, GLboolean g)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glTexImage3DMultisample != nullptr);
   gl::detail::glTexImage3DMultisample(a, b, c, d, e, f, g);
   check_error();
}
void get_multisample_fv(GLenum a, GLuint b, GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetMultisamplefv != nullptr);
   gl::detail::glGetMultisamplefv(a, b, c);
   check_error();
}
void sample_mask_i(GLuint a, GLbitfield b)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glSampleMaski != nullptr);
   gl::detail::glSampleMaski(a, b);
   check_error();
}
#endif

/*  GL_ARB_sample_shading  */ 
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void min_sample_shading(GLclampf value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glMinSampleShading != nullptr);
   gl::detail::glMinSampleShading(value);
   check_error();
}
#endif

/*  GL_ARB_timer_query  */ 
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void query_counter(GLuint id, GLenum target)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glQueryCounter != nullptr);
   gl::detail::glQueryCounter(id, target);
   check_error();
}
void get_query_object_i64v(GLuint id, GLenum pname, GLint64 *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetQueryObjecti64v != nullptr);
   gl::detail::glGetQueryObjecti64v(id, pname, params);
   check_error();
}
void get_query_object_ui64v(GLuint id, GLenum pname, GLuint64 *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetQueryObjectui64v != nullptr);
   gl::detail::glGetQueryObjectui64v(id, pname, params);
   check_error();
}
#endif

/*  GL_ARB_tessellation_shader  */
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void patch_parameter_i(GLenum pname, GLint value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glPatchParameteri != nullptr);
   gl::detail::glPatchParameteri(pname, value);
   check_error();
}
void patch_parameter_fv(GLenum pname, const GLfloat *values)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glPatchParameterfv != nullptr);
   gl::detail::glPatchParameterfv(pname, values);
   check_error();
}
#endif

/*  GL_ARB_get_program_binary  */
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void get_program_binary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetProgramBinary != nullptr);
   gl::detail::glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
   check_error();
}
void program_binary(GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glProgramBinary != nullptr);
   gl::detail::glProgramBinary(program, binaryFormat, binary, length);
   check_error();
}
#endif
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void program_parameter_i(GLuint program, GLenum pname, GLint value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glProgramParameteri != nullptr);
   gl::detail::glProgramParameteri(program, pname, value);
   check_error();
}
#endif

/*  GL_ARB_base_instance  */
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void draw_arrays_instanced_base_instance(GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawArraysInstancedBaseInstance != nullptr);
   gl::detail::glDrawArraysInstancedBaseInstance(mode, first, count, primcount, baseinstance);
   check_error();
}
void draw_elements_instanced_base_instance(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLuint baseinstance)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawElementsInstancedBaseInstance != nullptr);
   gl::detail::glDrawElementsInstancedBaseInstance(mode, count, type, indices, primcount, baseinstance);
   check_error();
}
void draw_elements_instanced_base_vertex_base_instance(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawElementsInstancedBaseVertexBaseInstance != nullptr);
   gl::detail::glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, primcount, basevertex, baseinstance);
   check_error();
}
#endif

/*  GL_ARB_transform_feedback2  */
#if defined(RENDERSTACK_USE_GLES3) || !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void bind_transform_feedback(GLenum target, GLuint id)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBindTransformFeedback != nullptr);
   gl::detail::glBindTransformFeedback(target, id);
   check_error();
}
void delete_transform_feedbacks(GLsizei n, const GLuint *ids)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDeleteTransformFeedbacks != nullptr);
   gl::detail::glDeleteTransformFeedbacks(n, ids);
   check_error();
}
void gen_transform_feedbacks(GLsizei n, GLuint *ids)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGenTransformFeedbacks != nullptr);
   gl::detail::glGenTransformFeedbacks(n, ids);
   check_error();
}
GLboolean is_transform_feedback(GLuint id)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glIsTransformFeedback != nullptr);
   GLboolean res = gl::detail::glIsTransformFeedback(id);
   check_error();
   return res;
}
void pause_transform_feedback()
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glPauseTransformFeedback != nullptr);
   gl::detail::glPauseTransformFeedback();
   check_error();
}
void resume_transform_feedback ()
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glResumeTransformFeedback != nullptr);
   gl::detail::glResumeTransformFeedback();
   check_error();
}
#endif
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void draw_transform_feedback(GLenum mode, GLuint id)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawTransformFeedback != nullptr);
   gl::detail::glDrawTransformFeedback(mode, id);
   check_error();
}
#endif


/*  GL_ARB_transform_feedback3  */
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void draw_transform_feedback_stream(GLenum mode, GLuint id, GLuint stream)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawTransformFeedbackStream != nullptr);
   gl::detail::glDrawTransformFeedbackStream(mode, id, stream);
   check_error();
}
void begin_query_indexed(GLenum target, GLuint index, GLuint id)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glBeginQueryIndexed != nullptr);
   gl::detail::glBeginQueryIndexed(target, index, id);
   check_error();
}
void end_query_indexed(GLenum target, GLuint index)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glEndQueryIndexed != nullptr);
   gl::detail::glEndQueryIndexed(target, index);
   check_error();
}
void get_query_indexed_iv (GLenum target, GLuint index, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetQueryIndexediv != nullptr);
   gl::detail::glGetQueryIndexediv(target, index, pname, params);
   check_error();
}
#endif


/*  GL_ARB_transform_feedback_instanced  */
#if !defined(RENDERSTACK_USE_GLES2_OR_GLES3)
void draw_transform_feedback_instanced(GLenum mode, GLuint id, GLsizei primcount)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawTransformFeedbackInstanced != nullptr);
   gl::detail::glDrawTransformFeedbackInstanced(mode, id, primcount);
   check_error();
}
void draw_transform_feedback_stream_instanced(GLenum mode, GLuint id, GLuint stream, GLsizei primcount)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawTransformFeedbackStreamInstanced != nullptr);
   gl::detail::glDrawTransformFeedbackStreamInstanced(mode, id, stream, primcount);
   check_error();
}
#endif


}

