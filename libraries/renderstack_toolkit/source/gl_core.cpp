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

#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
   case GL_INVALID_FRAMEBUFFER_OPERATION : return "GL_INVALID_FRAMEBUFFER_OPERATION";

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

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   case GL_HALF_FLOAT        : return "GL_HALF_FLOAT";
#endif

#if defined(RENDERSTACK_GL_API_OPENGL)
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

#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_SAMPLER_1D        : return "GL_SAMPLER_1D";
#endif
   case GL_SAMPLER_2D        : return "GL_SAMPLER_2D";
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   case GL_SAMPLER_3D        : return "GL_SAMPLER_3D";
#endif
   case GL_SAMPLER_CUBE      : return "GL_SAMPLER_CUBE";

#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_SAMPLER_1D_SHADOW : return "GL_SAMPLER_1D_SHADOW";
   case GL_SAMPLER_2D_SHADOW : return "GL_SAMPLER_2D_SHADOW";
#endif

   case GL_FRAMEBUFFER_COMPLETE                     : return "GL_FRAMEBUFFER_COMPLETE";
   case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT        : return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
   case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER       : return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
   case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER       : return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
#endif
   case GL_FRAMEBUFFER_UNSUPPORTED                  : return "GL_FRAMEBUFFER_UNSUPPORTED";

   case GL_NONE:                   return "GL_POINTS / GL_NONE";
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   case GL_FRAMEBUFFER_DEFAULT:    return "GL_FRAMEBUFFER_DEFAULT";
#endif
   case GL_TEXTURE:                return "GL_TEXTURE";
   case GL_RENDERBUFFER:           return "GL_RENDERBUFFER";

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   case GL_SIGNED_NORMALIZED       : return "GL_SIGNED_NORMALIZED";
   case GL_UNSIGNED_NORMALIZED     : return "GL_UNSIGNED_NORMALIZED";
#endif

   case GL_LINEAR                  : return "GL_LINEAR";
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   case GL_SRGB                    : return "GL_SRGB";
#endif

   //  Texture targets
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_TEXTURE_1D              : return "GL_TEXTURE_1D";
#endif
   case GL_TEXTURE_2D              : return "GL_TEXTURE_2D";
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   case GL_TEXTURE_3D              : return "GL_TEXTURE_3D";
#endif
   case GL_TEXTURE_CUBE_MAP        : return "GL_TEXTURE_CUBE_MAP";

   //  Buffer targets
   case GL_ARRAY_BUFFER                : return "GL_ARRAY_BUFFER";
   case GL_ELEMENT_ARRAY_BUFFER        : return "GL_ELEMENT_ARRAY_BUFFER";
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   case GL_PIXEL_PACK_BUFFER           : return "GL_PIXEL_PACK_BUFFER";
   case GL_PIXEL_UNPACK_BUFFER         : return "GL_PIXEL_UNPACK_BUFFER";
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_TRANSFORM_FEEDBACK_BUFFER   : return "GL_TRANSFORM_FEEDBACK_BUFFER";
   case GL_SAMPLER_BUFFER              : return "GL_SAMPLER_BUFFER";
   case GL_INT_SAMPLER_BUFFER          : return "GL_INT_SAMPLER_BUFFER";
   case GL_UNSIGNED_INT_SAMPLER_BUFFER : return "GL_UNSIGNED_INT_SAMPLER_BUFFER";
   case GL_TEXTURE_BUFFER              : return "GL_TEXTURE_BUFFER";
   case GL_TEXTURE_BINDING_BUFFER      : return "GL_TEXTURE_BINDING_BUFFER";
#endif
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   case GL_UNIFORM_BUFFER              : return "GL_UNIFORM_BUFFER";
   case GL_COPY_READ_BUFFER            : return "GL_COPY_READ_BUFFER";
   case GL_COPY_WRITE_BUFFER           : return "GL_COPY_WRITE_BUFFER";
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_MULTISAMPLE                 : return "GL_MULTISAMPLE";
#endif
   case GL_SAMPLE_ALPHA_TO_COVERAGE    : return "GL_SAMPLE_ALPHA_TO_COVERAGE";
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_SAMPLE_ALPHA_TO_ONE         : return "GL_SAMPLE_ALPHA_TO_ONE";
#endif
   case GL_SAMPLE_COVERAGE             : return "GL_SAMPLE_COVERAGE";
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_DEPTH_CLAMP                 : return "GL_DEPTH_CLAMP";
   case GL_TEXTURE_CUBE_MAP_SEAMLESS   : return "GL_TEXTURE_CUBE_MAP_SEAMLESS";
   case GL_SAMPLE_SHADING              : return "GL_SAMPLE_SHADING";
#endif
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   case GL_RASTERIZER_DISCARD          : return "GL_RASTERIZER_DISCARD";
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_PATCHES                     : return "GL_PATCHES";
   case GL_LINES_ADJACENCY             : return "GL_LINES_ADJACENCY";
   case GL_LINE_STRIP_ADJACENCY        : return "GL_LINE_STRIP_ADJACENCY";
   case GL_TRIANGLES_ADJACENCY         : return "GL_TRIANGLES_ADJACENCY";
   case GL_TRIANGLE_STRIP_ADJACENCY    : return "GL_TRIANGLE_STRIP_ADJACENCY";
#endif

   case GL_BUFFER_SIZE                       : return "GL_BUFFER_SIZE";
   case GL_BUFFER_USAGE                      : return "GL_BUFFER_USAGE";
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_QUERY_COUNTER_BITS                : return "GL_QUERY_COUNTER_BITS";
#endif
   case GL_CURRENT_QUERY                     : return "GL_CURRENT_QUERY";
   case GL_QUERY_RESULT                      : return "GL_QUERY_RESULT";
   case GL_QUERY_RESULT_AVAILABLE            : return "GL_QUERY_RESULT_AVAILABLE";
   case GL_ARRAY_BUFFER_BINDING              : return "GL_ARRAY_BUFFER_BINDING";
   case GL_ELEMENT_ARRAY_BUFFER_BINDING      : return "GL_ELEMENT_ARRAY_BUFFER_BINDING";
   case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: return "GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING";
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_READ_ONLY                         : return "GL_READ_ONLY";
   case GL_WRITE_ONLY                        : return "GL_WRITE_ONLY";
   case GL_READ_WRITE                        : return "GL_READ_WRITE";
   case GL_BUFFER_ACCESS                     : return "GL_BUFFER_ACCESS";
#endif
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
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_SAMPLES_PASSED                    : return "GL_SAMPLES_PASSED";
   case GL_SRC1_ALPHA                        : return "GL_SRC1_ALPHA";
#endif
   case GL_FRAGMENT_SHADER                   : return "GL_FRAGMENT_SHADER";
   case GL_VERTEX_SHADER                     : return "GL_VERTEX_SHADER";
#if defined(RENDERSTACK_GL_API_OPENGL)
   case GL_GEOMETRY_SHADER                   : return "GL_GEOMETRY_SHADER";
   case GL_TESS_EVALUATION_SHADER            : return "GL_TESS_EVALUATION_SHADER";
   case GL_TESS_CONTROL_SHADER               : return "GL_TESS_CONTROL_SHADER";
#endif

   case GL_SHADER_TYPE        : return "GL_SHADER_TYPE";
   case GL_COMPILE_STATUS     : return "GL_COMPILE_STATUS";
   case GL_DELETE_STATUS      : return "GL_DELETE_STATUS";

   case GL_LINK_STATUS                             : return "GL_LINK_STATUS";
   case GL_VALIDATE_STATUS                         : return "GL_VALIDATE_STATUS";
   case GL_INFO_LOG_LENGTH                         : return "GL_INFO_LOG_LENGTH";
   case GL_ATTACHED_SHADERS                        : return "GL_ATTACHED_SHADERS";
   case GL_ACTIVE_ATTRIBUTES                       : return "GL_ACTIVE_ATTRIBUTES";
   case GL_ACTIVE_ATTRIBUTE_MAX_LENGTH             : return "GL_ACTIVE_ATTRIBUTE_MAX_LENGTH";
   case GL_ACTIVE_UNIFORMS                         : return "GL_ACTIVE_UNIFORMS";
   case GL_ACTIVE_UNIFORM_MAX_LENGTH               : return "GL_ACTIVE_UNIFORM_MAX_LENGTH";
   case GL_ACTIVE_UNIFORM_BLOCKS                   : return "GL_ACTIVE_UNIFORM_BLOCKS";
   case GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH    : return "GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH";
   case GL_TRANSFORM_FEEDBACK_BUFFER_MODE          : return "GL_TRANSFORM_FEEDBACK_BUFFER_MODE";
   case GL_TRANSFORM_FEEDBACK_VARYINGS             : return "GL_TRANSFORM_FEEDBACK_VARYINGS";
   case GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH   : return "GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH";

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

#if defined(RENDERSTACK_GL_API_OPENGL_WITH_LEGACY) || defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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

#if defined(RENDERSTACK_GL_API_OPENGL_WITH_LEGACY)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glBindTexture != nullptr);
   gl::detail::glBindTexture(target, texture);
#else
   ::glBindTexture(target, texture);
#endif
   check_error();
}
void blend_func(GLenum sfactor, GLenum dfactor)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glBlendFunc != nullptr);
   gl::detail::glBlendFunc(sfactor, dfactor);
#else
   ::glBlendFunc(sfactor, dfactor);
#endif
   check_error();
}
void clear(GLbitfield mask)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glClear != nullptr);
   gl::detail::glClear(mask);
#else
   ::glClear(mask);
#endif
   check_error();
}
void clear_color(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glClearColor != nullptr);
   gl::detail::glClearColor(red, green, blue, alpha);
#else
   ::glClearColor(red, green, blue, alpha);
#endif
   check_error();
}
void clear_stencil(GLint s)
   {  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glClearStencil != nullptr);
   gl::detail::glClearStencil(s);
#else
   glClearStencil(s);
#endif
   check_error();
}
void color_mask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glColorMask != nullptr);
   gl::detail::glColorMask(red, green, blue, alpha);
#else
   glColorMask(red, green, blue, alpha);
#endif
   check_error();
}
void copy_tex_image_2d(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glCopyTexImage2D != nullptr);
   gl::detail::glCopyTexImage2D(target, level, internalFormat, x, y, width, height, border);
#else
   glCopyTexImage2D(target, level, internalFormat, x, y, width, height, border);
#endif
   check_error();
}
void copy_tex_sub_image_2d(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glCopyTexSubImage2D != nullptr);
   gl::detail::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
#else
   glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
#endif
   check_error();
}
void cull_face(GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glCullFace != nullptr);
   gl::detail::glCullFace(mode);
#else
   glCullFace(mode);
#endif
   check_error();
}
void delete_textures(GLsizei n, const GLuint *textures)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDeleteTextures != nullptr);
   gl::detail::glDeleteTextures(n, textures);
#else
   glDeleteTextures(n, textures);
#endif
   check_error();
}
void depth_func(GLenum func)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDepthFunc != nullptr);
   gl::detail::glDepthFunc(func);
#else
   glDepthFunc(func);
#endif
   check_error();
}
void depth_mask(GLboolean flag)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glEnable != nullptr);
   gl::detail::glEnable(cap);
#else
   ::glEnable(cap);
#endif
   check_error();
}
void finish(void)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glFinish != nullptr);
   gl::detail::glFinish();
#else
   ::glFinish();
#endif
   check_error();
}
void flush(void)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glFlush != nullptr);
   gl::detail::glFlush();
#else
   ::glFlush();
#endif
   check_error();
}
void front_face(GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetBooleanv != nullptr);
   gl::detail::glGetBooleanv(pname, params);
#else
   ::glGetBooleanv(pname, params);
#endif
   check_error();
}
GLenum get_error(void)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetError != nullptr);
   return gl::detail::glGetError();
#else
   return ::glGetError();
#endif
}
void get_float_v(GLenum pname, GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetFloatv != nullptr);
   gl::detail::glGetFloatv(pname, params);
#else
   ::glGetFloatv(pname, params);
#endif
   check_error();
}
void get_integer_v(GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetIntegerv != nullptr);
   gl::detail::glGetIntegerv(pname, params);
#else
   ::glGetIntegerv(pname, params);
#endif
   check_error();
}
const GLubyte *get_string(GLenum name)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetTexParameterfv != nullptr);
   gl::detail::glGetTexParameterfv(target, pname, params);
#else
   ::glGetTexParameterfv(target, pname, params);
#endif
   check_error();
}
void get_tex_parameter_iv(GLenum target, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glGetTexParameteriv != nullptr);
   gl::detail::glGetTexParameteriv(target, pname, params);
#else
   ::glGetTexParameteriv(target, pname, params);
#endif
   check_error();
}
void hint(GLenum target, GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glHint != nullptr);
   gl::detail::glHint(target, mode);
#else
   ::glHint(target, mode);
#endif
   check_error();
}
GLboolean is_enabled(GLenum cap)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glLineWidth != nullptr);
   gl::detail::glLineWidth(width);
#else
   ::glLineWidth(width);
#endif
   check_error();
}
void pixel_store_i(GLenum pname, GLint param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glPixelStorei != nullptr);
   gl::detail::glPixelStorei(pname, param);
#else
   ::glPixelStorei(pname, param);
#endif
   check_error();
}
void polygon_offset(GLfloat factor, GLfloat units)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glPolygonOffset != nullptr);
   gl::detail::glPolygonOffset(factor, units);
#else
   ::glPolygonOffset(factor, units);
#endif
   check_error();
}
void read_pixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glReadPixels != nullptr);
   gl::detail::glReadPixels(x, y, width, height, format, type, pixels);
#else
   ::glReadPixels(x, y, width, height, format, type, pixels);
#endif
   check_error();
}
void scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glScissor != nullptr);
   gl::detail::glScissor(x, y, width, height);
#else
   ::glScissor(x, y, width, height);
#endif
   check_error();
}
void stencil_func(GLenum func, GLint ref, GLuint mask)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glStencilFunc != nullptr);
   gl::detail::glStencilFunc(func, ref, mask);
#else
   ::glStencilFunc(func, ref, mask);
#endif
   check_error();
}
void stencil_mask(GLuint mask)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glStencilMask != nullptr);
   gl::detail::glStencilMask(mask);
#else
   ::glStencilMask(mask);
#endif
   check_error();
}
void stencil_op(GLenum fail, GLenum zfail, GLenum zpass)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glStencilOp != nullptr);
   gl::detail::glStencilOp(fail, zfail, zpass);
#else
   ::glStencilOp(fail, zfail, zpass);
#endif
   check_error();
}
void tex_image_2d(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexImage2D != nullptr);
   gl::detail::glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
#else
   ::glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
#endif
   check_error();
}
void tex_parameter_f(GLenum target, GLenum pname, GLfloat param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexParameterf != nullptr);
   gl::detail::glTexParameterf(target, pname, param);
#else
   ::glTexParameterf(target, pname, param);
#endif
   check_error();
}
void tex_parameter_fv(GLenum target, GLenum pname, const GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexParameterfv != nullptr);
   gl::detail::glTexParameterfv(target, pname, params);
#else
   ::glTexParameterfv(target, pname, params);
#endif
   check_error();
}
void tex_parameter_i(GLenum target, GLenum pname, GLint param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexParameteri != nullptr);
   gl::detail::glTexParameteri(target, pname, param);
#else
   ::glTexParameteri(target, pname, param);
#endif
   check_error();
}
void tex_parameter_iv(GLenum target, GLenum pname, const GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexParameteriv != nullptr);
   gl::detail::glTexParameteriv(target, pname, params);
#else
   ::glTexParameteriv(target, pname, params);
#endif
   check_error();
}
void tex_sub_image_2d(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glTexSubImage2D != nullptr);
   gl::detail::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
#else
   ::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
#endif
   check_error();
}
void viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glViewport != nullptr);
   gl::detail::glViewport(x, y, width, height);
#else
   ::glViewport(x, y, width, height);
#endif
   check_error();
}

#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void clear_depth(GLclampf depth)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glClearDepthf != nullptr);
   gl::detail::glClearDepthf(depth);
#else
   ::glClearDepthf(depth);
#endif
   check_error();
}
void depth_range(GLclampf zNear, GLclampf zFar)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glDepthRangef != nullptr);
   gl::detail::glDepthRangef(zNear, zFar);
#else
   ::glDepthRangef(zNear, zFar);
#endif
   check_error();
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void read_buffer(GLenum mode)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
   assert(gl::detail::glReadBuffer != nullptr);
   gl::detail::glReadBuffer(mode);
#else
   ::glReadBuffer(mode);
#endif
   check_error();
}
#endif

#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBlendColor != nullptr);
   gl::detail::glBlendColor(a, b, c, d);
#else
   ::glBlendColor(a, b, c, d);
#endif
   check_error();
}

void blend_equation(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBlendEquation != nullptr);
   gl::detail::glBlendEquation(a);
#else
   ::glBlendEquation(a);
#endif
   check_error();
}

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDrawRangeElements != nullptr);
   gl::detail::glDrawRangeElements(mode, start, end, count, type, indices);
#else
   ::glDrawRangeElements(mode, start, end, count, type, indices);
#endif
   check_error();
}
void tex_image_3d(GLenum a, GLint b, GLint c, GLsizei d, GLsizei e, GLsizei f, GLint g, GLenum h, GLenum i, const GLvoid *j)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glTexImage3D != nullptr);
   gl::detail::glTexImage3D(a, b, c, d, e, f, g, h, i, j);
#else
   ::glTexImage3D(a, b, c, d, e, f, g, h, i, j);
#endif
   check_error();
}
void tex_sub_image_3d(GLenum a, GLint b, GLint c, GLint d, GLint e, GLsizei f, GLsizei g, GLsizei h, GLenum i, GLenum j, const GLvoid *k)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glTexSubImage3D != nullptr);
   gl::detail::glTexSubImage3D(a, b, c, d, e, f, g, h, i, j, k);
#else
   ::glTexSubImage3D(a, b, c, d, e, f, g, h, i, j, k);
#endif
   check_error();
}
void copy_tex_sub_image_3d(GLenum a, GLint b, GLint c, GLint d, GLint e, GLint f, GLint g, GLsizei h, GLsizei i)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCopyTexSubImage3D != nullptr);
   gl::detail::glCopyTexSubImage3D(a, b, c, d, e, f, g, h, i);
#else
   ::glCopyTexSubImage3D(a, b, c, d, e, f, g, h, i);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glActiveTexture != nullptr);
   gl::detail::glActiveTexture(a);
#else
   ::glActiveTexture(a);
#endif
   check_error();
}
void sample_coverage(GLclampf a, GLboolean b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glSampleCoverage != nullptr);
   gl::detail::glSampleCoverage(a, b);
#else
   ::glSampleCoverage(a, b);
#endif
   check_error();
}
void compressed_tex_image_2d(GLenum a, GLint b, GLenum c, GLsizei d, GLsizei e, GLint f, GLsizei g, const GLvoid *h)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCompressedTexImage2D != nullptr);
   gl::detail::glCompressedTexImage2D(a, b, c, d, e, f, g, h);
#else
   ::glCompressedTexImage2D(a, b, c, d, e, f, g, h);
#endif
   check_error();
}
void compressed_tex_sub_image_2d(GLenum a, GLint b, GLint c, GLint d, GLsizei e, GLsizei f, GLenum g, GLsizei h, const GLvoid *i)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCompressedTexSubImage2D != nullptr);
   gl::detail::glCompressedTexSubImage2D(a, b, c, d, e, f, g, h, i);
#else
   ::glCompressedTexSubImage2D(a, b, c, d, e, f, g, h, i);
#endif
   check_error();
}

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void compressed_tex_image_3d(GLenum a, GLint b, GLenum c, GLsizei d, GLsizei e, GLsizei f, GLint g, GLsizei h, const GLvoid *i)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCompressedTexImage3D != nullptr);
   gl::detail::glCompressedTexImage3D(a, b, c, d, e, f, g, h, i);
#else
   ::glCompressedTexImage3D(a, b, c, d, e, f, g, h, i);
#endif
   check_error();
}
void compressed_tex_sub_image_3d(GLenum a, GLint b, GLint c, GLint d, GLint e, GLsizei f, GLsizei g, GLsizei h, GLenum i, GLsizei j, const GLvoid *k)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCompressedTexSubImage3D != nullptr);
   gl::detail::glCompressedTexSubImage3D(a, b, c, d, e, f, g, h, i, j, k);
#else
   ::glCompressedTexSubImage3D(a, b, c, d, e, f, g, h, i, j, k);
#endif
   check_error();
}
#endif

#if defined(RENDERSTACK_GL_API_OPENGL)
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

#if defined(RENDERSTACK_GL_API_OPENGL_WITH_LEGACYUPPORT_LEGACY_OPENGL)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBlendFuncSeparate != nullptr);
   gl::detail::glBlendFuncSeparate(a, b, c, d);
#else
   ::glBlendFuncSeparate(a, b, c, d);
#endif
   check_error();
}

#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBindBuffer != nullptr);
   gl::detail::glBindBuffer(target, buffer);
#else
   ::glBindBuffer(target, buffer);
#endif
   check_error();
}
void delete_buffers(GLsizei n, const GLuint *buffers)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   log_trace("delete_buffers(n = %u, buffers = %p)",
      static_cast<unsigned int>(n),
      buffers);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteBuffers != nullptr);
   gl::detail::glDeleteBuffers(n, buffers);
#else
   ::glDeleteBuffers(n, buffers);
#endif
   check_error();
}
void gen_buffers(GLsizei n, GLuint *buffers)
{   
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   slog_trace("gen_buffers(n = %u, buffers = %p)",
      static_cast<unsigned int>(n),
      buffers);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGenBuffers != nullptr);
   gl::detail::glGenBuffers(n, buffers);
#else
   ::glGenBuffers(n, buffers);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsBuffer != nullptr);
   GLboolean res = gl::detail::glIsBuffer(buffer);
#else
   GLboolean res = ::glIsBuffer(buffer);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBufferData != nullptr);
   gl::detail::glBufferData(target, size, data, usage);
#else
   ::glBufferData(target, size, data, usage);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBufferSubData != nullptr);
   gl::detail::glBufferSubData(target, offset, size, data);
#else
   ::glBufferSubData(target, offset, size, data);
#endif
   check_error();
}
void get_buffer_parameter_iv(GLenum target, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetBufferParameteriv != nullptr);
   gl::detail::glGetBufferParameteriv(target, pname, params);
#else
   ::glGetBufferParameteriv(target, pname, params);
#endif
   check_error();
}

// GL_OES_mapbuffer
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2)
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
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void gen_queries(GLsizei a, GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGenQueries != nullptr);
   gl::detail::glGenQueries(a, b);
#else
   ::glGenQueries(a, b);
#endif
   check_error();
}
void delete_queries(GLsizei a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteQueries != nullptr);
   gl::detail::glDeleteQueries(a, b);
#else
   ::glDeleteQueries(a, b);
#endif
   check_error();
}
GLboolean is_query(GLuint a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsQuery != nullptr);
   GLboolean res = gl::detail::glIsQuery(a);
#else
   GLboolean res = ::glIsQuery(a);
#endif
   check_error();
   return res;
}
void begin_query(GLenum a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBeginQuery != nullptr);
   gl::detail::glBeginQuery(a, b);
#else
   ::glBeginQuery(a, b);
#endif
   check_error();
}
void end_query(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glEndQuery != nullptr);
   gl::detail::glEndQuery(a);
#else
   ::glEndQuery(a);
#endif
   check_error();
}
void get_query_iv(GLenum a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetQueryiv != nullptr);
   gl::detail::glGetQueryiv(a, b, c);
#else
   ::glGetQueryiv(a, b, c);
#endif
   check_error();
}
void get_query_object_uiv(GLuint a, GLenum b, GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetQueryObjectuiv != nullptr);
   gl::detail::glGetQueryObjectuiv(a, b, c);
#else
   ::glGetQueryObjectuiv(a, b, c);
#endif
   check_error();
}
void get_buffer_pointer_v(GLenum target, GLenum pname, GLvoid **params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetBufferPointerv != nullptr);
   gl::detail::glGetBufferPointerv(target, pname, params);
#else
   ::glGetBufferPointerv(target, pname, params);
#endif
   check_error();
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#endif
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
GLboolean unmap_buffer(GLenum target)
{
#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   slog_trace("unmap_buffer(target = %s)", enum_string(target));
#endif

#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUnmapBuffer != nullptr);
   GLboolean res = gl::detail::glUnmapBuffer(target);
#else
   GLboolean res = ::glUnmapBuffer(target);
#endif

#if defined(LOG_GL) || defined(LOG_GL_MAP_BUFFER) || defined(LOG_GL_BUFFER)
   log_trace(":%d", res);
#endif
   check_error();
   return res;
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBlendEquationSeparate != nullptr);
   gl::detail::glBlendEquationSeparate(a, b);
#else
   ::glBlendEquationSeparate(a, b);
#endif
   check_error();
}
void stencil_op_separate(GLenum a, GLenum b, GLenum c, GLenum d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glStencilOpSeparate != nullptr);
   gl::detail::glStencilOpSeparate(a, b, c, d);
#else
   ::glStencilOpSeparate(a, b, c, d);
#endif
   check_error();
}
void stencil_func_Separate(GLenum a, GLenum b, GLint c, GLuint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glStencilFuncSeparate != nullptr);
   gl::detail::glStencilFuncSeparate(a, b, c, d);
#else
   ::glStencilFuncSeparate(a, b, c, d);
#endif
   check_error();
}
void stencil_mask_separate(GLenum a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glStencilMaskSeparate != nullptr);
   gl::detail::glStencilMaskSeparate(a, b);
#else
   ::glStencilMaskSeparate(a, b);
#endif
   check_error();
}
void attach_shader(GLuint program, GLuint shader)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("attach_shader(%u, %u)", program, shader);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glAttachShader != nullptr);
   gl::detail::glAttachShader(program, shader);
#else
   ::glAttachShader(program, shader);
#endif
   check_error();
}
void bind_attrib_location(GLuint program, GLuint index, const GLchar *name)
{
   assert(name != nullptr);
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB)
   log_trace("bind_attrib_location(program = %u, index = %u, name = %s)",
      program, index, name != nullptr ? name : "(null)");
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBindAttribLocation != nullptr);
   gl::detail::glBindAttribLocation(program, index, name);
#else
   ::glBindAttribLocation(program, index, name);
#endif
   check_error();
}
void compile_shader(GLuint shader)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("compile_shader(shader = %u)", shader);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCompileShader != nullptr);
   gl::detail::glCompileShader(shader);
#else
   ::glCompileShader(shader);
#endif
   check_error();
}
GLuint create_program(void)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   slog_trace("create_program()");
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCreateProgram != nullptr);
   GLuint res = gl::detail::glCreateProgram();
#else
   GLuint res = ::glCreateProgram();
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCreateShader != nullptr);
   GLuint res = gl::detail::glCreateShader(type);
#else
   GLuint res = ::glCreateShader(type);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteProgram != nullptr);
   gl::detail::glDeleteProgram(program);
#else
   ::glDeleteProgram(program);
#endif
   check_error();
}
void delete_shader(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace(__FUNCTION__);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteShader != nullptr);
   gl::detail::glDeleteShader(a);
#else
   ::glDeleteShader(a);
#endif
   check_error();
}
void detach_shader(GLuint program, GLuint shader)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("detach_shader(program = %u, shader = %u)", program, shader);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDetachShader != nullptr);
   gl::detail::glDetachShader(program, shader);
#else
   ::glDetachShader(program, shader);
#endif
   check_error();
}
void disable_vertex_attrib_array(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB)
   log_trace("disable_vertex_attrib_array(%u)", a);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDisableVertexAttribArray != nullptr);
   gl::detail::glDisableVertexAttribArray(a);
#else
   ::glDisableVertexAttribArray(a);
#endif
   check_error();
}
void enable_vertex_attrib_array(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB)
   log_trace("enable_vertex_attrib_array(%u)", a);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glEnableVertexAttribArray != nullptr);
   gl::detail::glEnableVertexAttribArray(a);
#else
   ::glEnableVertexAttribArray(a);
#endif
   check_error();
}
void get_active_attrib(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLint *e, GLenum *f, GLchar *g)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetActiveAttrib != nullptr);
   gl::detail::glGetActiveAttrib(a, b, c, d, e, f, g);
#else
   ::glGetActiveAttrib(a, b, c, d, e, f, g);
#endif
   check_error();
}
void get_active_uniform(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLint *e, GLenum *f, GLchar *g)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetActiveUniform != nullptr);
   gl::detail::glGetActiveUniform(a, b, c, d, e, f, g);
#else
   ::glGetActiveUniform(a, b, c, d, e, f, g);
#endif
   check_error();
}
void get_attached_shaders(GLuint a, GLsizei b, GLsizei *c, GLuint *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetAttachedShaders != nullptr);
   gl::detail::glGetAttachedShaders(a, b, c, d);
#else
   ::glGetAttachedShaders(a, b, c, d);
#endif
   check_error();
}
GLint get_attrib_location(GLuint a, const GLchar *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetAttribLocation != nullptr);
   GLint res = gl::detail::glGetAttribLocation(a, b);
#else
   GLint res = ::glGetAttribLocation(a, b);
#endif
   check_error();
   return res;
}
void get_program_iv(GLuint program, GLenum pname, GLint *params)
{
#if defined(LOG_GL)
   log_trace("get_program_iv(shader = %u, pname = %s, params = %p)", program, gl::enum_string(pname), params);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetProgramiv != nullptr);
   gl::detail::glGetProgramiv(program, pname, params);
#else
   ::glGetProgramiv(program, pname, params);
#endif
   check_error();
}
void get_program_info_log(GLuint a, GLsizei b, GLsizei *c, GLchar *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetProgramInfoLog != nullptr);
   gl::detail::glGetProgramInfoLog(a, b, c, d);
#else
   ::glGetProgramInfoLog(a, b, c, d);
#endif
   check_error();
}
void get_shader_iv(GLuint shader, GLenum pname, GLint *params)
{
#if defined(LOG_GL)
   log_trace("get_shader_iv(shader = %u, pname = %s, params = %p)", shader, gl::enum_string(pname), params);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetShaderiv != nullptr);
   gl::detail::glGetShaderiv(shader, pname, params);
#else
   ::glGetShaderiv(shader, pname, params);
#endif
   check_error();
}
void get_shader_info_log(GLuint a, GLsizei b, GLsizei *c, GLchar *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetShaderInfoLog != nullptr);
   gl::detail::glGetShaderInfoLog(a, b, c, d);
#else
   ::glGetShaderInfoLog(a, b, c, d);
#endif
   check_error();
}
void get_shader_source(GLuint a, GLsizei b, GLsizei *c, GLchar *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetShaderSource != nullptr);
   gl::detail::glGetShaderSource(a, b, c, d);
#else
   ::glGetShaderSource(a, b, c, d);
#endif
   check_error();
}
GLint get_uniform_location(GLuint program, const GLchar *name)
{
   assert(name != nullptr);
#if defined(LOG_GL)
   slog_trace("get_uniform_location(program = %u, name = %s)",
      program, name != nullptr ? name : "(null)");
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetUniformLocation != nullptr);
   GLint res = gl::detail::glGetUniformLocation(program, name);
#else
   GLint res = ::glGetUniformLocation(program, name);
#endif
#if defined(LOG_GL)
   log_trace(":%d", res);
#endif
   check_error();
   return res;
}
void get_uniform_fv(GLuint a, GLint b, GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetUniformfv != nullptr);
   gl::detail::glGetUniformfv(a, b, c);
#else
   ::glGetUniformfv(a, b, c);
#endif
   check_error();
}
void get_uniform_iv(GLuint a, GLint b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetUniformiv != nullptr);
   gl::detail::glGetUniformiv(a, b, c);
#else
   ::glGetUniformiv(a, b, c);
#endif
   check_error();
}

#if defined(RENDERSTACK_GL_API_OPENGL)
void get_vertex_attrib_dv(GLuint a, GLenum b, GLdouble *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetVertexAttribdv != nullptr);
   gl::detail::glGetVertexAttribdv(a, b, c);
   check_error();
}
#endif

void get_vertex_attrib_fv(GLuint a, GLenum b, GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetVertexAttribfv != nullptr);
   gl::detail::glGetVertexAttribfv(a, b, c);
#else
   ::glGetVertexAttribfv(a, b, c);
#endif
   check_error();
}
void get_vertex_attrib_iv(GLuint a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetVertexAttribiv != nullptr);
   gl::detail::glGetVertexAttribiv(a, b, c);
#else
   ::glGetVertexAttribiv(a, b, c);
#endif
   check_error();
}
void get_vertex_attrib_pointer_v(GLuint a, GLenum b, GLvoid* *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetVertexAttribPointerv != nullptr);
   gl::detail::glGetVertexAttribPointerv(a, b, c);
#else
   ::glGetVertexAttribPointerv(a, b, c);
#endif
   check_error();
}
GLboolean is_program(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("is_program(%u)", a);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsProgram != nullptr);
   GLboolean res = gl::detail::glIsProgram(a);
#else
   GLboolean res = ::glIsProgram(a);
#endif
   check_error();
   return res;
}
GLboolean is_shader(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("is_shader(%u)", a);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsShader != nullptr);
   GLboolean res = gl::detail::glIsShader(a);
#else
   GLboolean res = ::glIsShader(a);
#endif
   check_error();
   return res;
}
void link_program(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("link_program(%u)", a);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glLinkProgram != nullptr);
   gl::detail::glLinkProgram(a);
#else
   ::glLinkProgram(a);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glShaderSource != nullptr);
   gl::detail::glShaderSource(shader, count, string, length);
#else
   ::glShaderSource(shader, count, string, length);
#endif
   check_error();
}
void use_program(GLuint a)
{
#if defined(LOG_GL) || defined(LOG_GL_PROGRAM)
   log_trace("use_program(program = %u)", a);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUseProgram != nullptr);
   gl::detail::glUseProgram(a);
#else
   ::glUseProgram(a);
#endif
   check_error();
}
void uniform_1f(GLint a, GLfloat b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform1f != nullptr);
   gl::detail::glUniform1f(a, b);
#else
   ::glUniform1f(a, b);
#endif
   check_error();
}
void uniform_2f(GLint a, GLfloat b, GLfloat c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform2f != nullptr);
   gl::detail::glUniform2f(a, b, c);
#else
   ::glUniform2f(a, b, c);
#endif
   check_error();
}
void uniform_3f(GLint a, GLfloat b, GLfloat c, GLfloat d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform3f != nullptr);
   gl::detail::glUniform3f(a, b, c, d);
#else
   ::glUniform3f(a, b, c, d);
#endif
   check_error();
}
void uniform_4f(GLint a, GLfloat b, GLfloat c, GLfloat d, GLfloat e)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform4f != nullptr);
   gl::detail::glUniform4f(a, b, c, d, e);
#else
   ::glUniform4f(a, b, c, d, e);
#endif
   check_error();
}
void uniform_1i(GLint location, GLint v0)
{
#if defined(LOG_GL) || defined(LOG_GL_UNIFORM)
   log_trace("uniform_1i(location = %d, v0 = %d)", location, v0);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform1i != nullptr);
   gl::detail::glUniform1i(location, v0);
#else
   ::glUniform1i(location, v0);
#endif
   check_error();
}
void uniform_2i(GLint a, GLint b, GLint c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform2i != nullptr);
   gl::detail::glUniform2i(a, b, c);
#else
   ::glUniform2i(a, b, c);
#endif
   check_error();
}
void uniform_3i(GLint a, GLint b, GLint c, GLint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform3i != nullptr);
   gl::detail::glUniform3i(a, b, c, d);
#else
   ::glUniform3i(a, b, c, d);
#endif
   check_error();
}
void uniform_4i(GLint a, GLint b, GLint c, GLint d, GLint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform4i != nullptr);
   gl::detail::glUniform4i(a, b, c, d, e);
#else
   ::glUniform4i(a, b, c, d, e);
#endif
   check_error();
}
void uniform_1fv(GLint a, GLsizei b, const GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform1fv != nullptr);
   gl::detail::glUniform1fv(a, b, c);
#else
   ::glUniform1fv(a, b, c);
#endif
   check_error();
}
void uniform_2fv(GLint a, GLsizei b, const GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform2fv != nullptr);
   gl::detail::glUniform2fv(a, b, c);
#else
   ::glUniform2fv(a, b, c);
#endif
   check_error();
}
void uniform_3fv(GLint location, GLsizei count, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform3fv != nullptr);
   gl::detail::glUniform3fv(location, count, value);
#else
   ::glUniform3fv(location, count, value);
#endif
   check_error();
}
void uniform_4fv(GLint location, GLsizei count, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform4fv != nullptr);
   gl::detail::glUniform4fv(location, count, value);
#else
   ::glUniform4fv(location, count, value);
#endif
   check_error();
}
void uniform_1iv(GLint location, GLsizei count, const GLint *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform1iv != nullptr);
   gl::detail::glUniform1iv(location, count, value);
#else
   ::glUniform1iv(location, count, value);
#endif
   check_error();
}
void uniform_2iv(GLint a, GLsizei b, const GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform2iv != nullptr);
   gl::detail::glUniform2iv(a, b, c);
#else
   ::glUniform2iv(a, b, c);
#endif
   check_error();
}
void uniform_3iv(GLint a, GLsizei b, const GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform3iv != nullptr);
   gl::detail::glUniform3iv(a, b, c);
#else
   ::glUniform3iv(a, b, c);
#endif
   check_error();
}
void uniform_4iv(GLint a, GLsizei b, const GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform4iv != nullptr);
   gl::detail::glUniform4iv(a, b, c);
#else
   ::glUniform4iv(a, b, c);
#endif
   check_error();
}
void uniform_matrix_2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformMatrix2fv != nullptr);
   gl::detail::glUniformMatrix2fv(location, count, transpose, value);
#else
   ::glUniformMatrix2fv(location, count, transpose, value);
#endif
   check_error();
}
void uniform_matrix_3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformMatrix3fv != nullptr);
   gl::detail::glUniformMatrix3fv(location, count, transpose, value);
#else
   ::glUniformMatrix3fv(location, count, transpose, value);
#endif
   check_error();
}
void uniform_matrix_4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformMatrix4fv != nullptr);
   gl::detail::glUniformMatrix4fv(location, count, transpose, value);
#else
   ::glUniformMatrix4fv(location, count, transpose, value);
#endif
   check_error();
}
void validate_program(GLuint program)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glValidateProgram != nullptr);
   gl::detail::glValidateProgram(program);
#else
   ::glValidateProgram(program);
#endif
   check_error();
}
void vertex_attrib_1f(GLuint a, GLfloat b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttrib1f != nullptr);
   gl::detail::glVertexAttrib1f(a, b);
#else
   ::glVertexAttrib1f(a, b);
#endif
   check_error();
}
void vertex_attrib_1fv(GLuint a, const GLfloat *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttrib1fv != nullptr);
   gl::detail::glVertexAttrib1fv(a, b);
#else
   ::glVertexAttrib1fv(a, b);
#endif
   check_error();
}
void vertex_attrib_2f(GLuint a, GLfloat b, GLfloat c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttrib2f != nullptr);
   gl::detail::glVertexAttrib2f(a, b, c);
#else
   ::glVertexAttrib2f(a, b, c);
#endif
   check_error();
}
void vertex_attrib_2fv(GLuint a, const GLfloat *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttrib2fv != nullptr);
   gl::detail::glVertexAttrib2fv(a, b);
#else
   ::glVertexAttrib2fv(a, b);
#endif
   check_error();
}
void vertex_attrib_3f(GLuint a, GLfloat b, GLfloat c, GLfloat d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttrib3f != nullptr);
   gl::detail::glVertexAttrib3f(a, b, c, d);
#else
   ::glVertexAttrib3f(a, b, c, d);
#endif
   check_error();
}
void vertex_attrib_3fv(GLuint a, const GLfloat *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttrib3fv != nullptr);
   gl::detail::glVertexAttrib3fv(a, b);
#else
   ::glVertexAttrib3fv(a, b);
#endif
   check_error();
}
void vertex_attrib_4f(GLuint a, GLfloat b, GLfloat c, GLfloat d, GLfloat e)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttrib4f != nullptr);
   gl::detail::glVertexAttrib4f(a, b, c, d, e);
#else
   ::glVertexAttrib4f(a, b, c, d, e);
#endif
   check_error();
}
void vertex_attrib_4fv(GLuint a, const GLfloat *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttrib4fv != nullptr);
   gl::detail::glVertexAttrib4fv(a, b);
#else
   ::glVertexAttrib4fv(a, b);
#endif
   check_error();
}
void vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB)
   log_trace("vertex_attrib_pointer(index = %u, size = %d, type = %s, normalized = %d, stride = %d, pointer = %p)",
      index, size, enum_string(type), normalized, static_cast<unsigned int>(stride), pointer);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttribPointer != nullptr);
   gl::detail::glVertexAttribPointer(index, size, type, normalized, stride, pointer);
#else
   ::glVertexAttribPointer(index, size, type, normalized, stride, pointer);
#endif
   check_error();
}

//  OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void draw_buffers(GLsizei a, const GLenum * b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDrawBuffers != nullptr);
   gl::detail::glDrawBuffers(a, b);
#else
   ::glDrawBuffers(a, b);
#endif
   check_error();
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void uniform_matrix_2x3fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformMatrix2x3fv != nullptr);
   gl::detail::glUniformMatrix2x3fv(a, b, c, d);
#else
   ::glUniformMatrix2x3fv(a, b, c, d);
#endif
   check_error();
}
void uniform_matrix_3x2fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformMatrix2x3fv != nullptr);
   gl::detail::glUniformMatrix3x2fv(a, b, c, d);
#else
   ::glUniformMatrix3x2fv(a, b, c, d);
#endif
   check_error();
}
void niform_matrix_2x4fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformMatrix2x4fv != nullptr);
   gl::detail::glUniformMatrix2x4fv(a, b, c, d);
#else
   ::glUniformMatrix2x4fv(a, b, c, d);
#endif
   check_error();
}
void uniform_matrix_4x2fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformMatrix4x2fv != nullptr);
   gl::detail::glUniformMatrix4x2fv(a, b, c, d);
#else
   ::glUniformMatrix4x2fv(a, b, c, d);
#endif
   check_error();
}
void uniform_matrix_3x4fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformMatrix3x4fv != nullptr);
   gl::detail::glUniformMatrix3x4fv(a, b, c, d);
#else
   ::glUniformMatrix3x4fv(a, b, c, d);
#endif
   check_error();
}
void uniform_matrix_4x3fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformMatrix4x3fv != nullptr);
   gl::detail::glUniformMatrix4x3fv(a, b, c, d);
#else
   ::glUniformMatrix4x3fv(a, b, c, d);
#endif
   check_error();
}
#endif

/*  GL_VERSION_3_0  */ 

/* ARB_framebuffer_object */ 
GLboolean is_renderbuffer(GLuint a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsRenderbuffer != nullptr);
   GLboolean res = gl::detail::glIsRenderbuffer(a);
#else
   GLboolean res = ::glIsRenderbuffer(a);
#endif
   check_error();
   return res;
}
void bind_renderbuffer(GLenum target, GLuint renderbuffer)
{
#if defined(LOG_GL)
   log_trace("bind_renderbuffer(target = %s, renderbuffer = %u)",
      enum_string(target), renderbuffer);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBindRenderbuffer);
   gl::detail::glBindRenderbuffer(target, renderbuffer);
#else
   ::glBindRenderbuffer(target, renderbuffer);
#endif
   check_error();
}
void delete_renderbuffers(GLsizei a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteRenderbuffers != nullptr);
   gl::detail::glDeleteRenderbuffers(a, b);
#else
   ::glDeleteRenderbuffers(a, b);
#endif
   check_error();
}
void gen_renderbuffers(GLsizei a, GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGenRenderbuffers != nullptr);
   gl::detail::glGenRenderbuffers(a, b);
#else
   ::glGenRenderbuffers(a, b);
#endif
   check_error();
}
void renderbuffer_storage(GLenum a, GLenum b, GLsizei c, GLsizei d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glRenderbufferStorage != nullptr);
   gl::detail::glRenderbufferStorage(a, b, c, d);
#else
   ::glRenderbufferStorage(a, b, c, d);
#endif
   check_error();
}
void get_renderbuffer_parameter_iv(GLenum a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetRenderbufferParameteriv != nullptr);
   gl::detail::glGetRenderbufferParameteriv(a, b, c);
#else
   ::glGetRenderbufferParameteriv(a, b, c);
#endif
   check_error();
}
GLboolean is_framebuffer(GLuint a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsFramebuffer != nullptr);
   GLboolean res = gl::detail::glIsFramebuffer(a);
#else
   GLboolean res = ::glIsFramebuffer(a);
#endif
   check_error();
   return res;
}
void bind_framebuffer(GLenum target, GLuint framebuffer)
{
#if defined(LOG_GL)
   log_trace("bind_framebuffer(target = %s, framebuffer = %d)",
      enum_string(target), framebuffer);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBindFramebuffer != nullptr);
   gl::detail::glBindFramebuffer(target, framebuffer);
#else
   ::glBindFramebuffer(target, framebuffer);
#endif
   check_error();
}
void delete_framebuffers(GLsizei a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteFramebuffers != nullptr);
   gl::detail::glDeleteFramebuffers(a, b);
#else
   ::glDeleteFramebuffers(a, b);
#endif
   check_error();
}
void gen_framebuffers(GLsizei a, GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGenFramebuffers != nullptr);
   gl::detail::glGenFramebuffers(a, b);
#else
   ::glGenFramebuffers(a, b);
#endif
   check_error();
}
GLenum check_framebuffer_status(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCheckFramebufferStatus != nullptr);
   GLenum res = gl::detail::glCheckFramebufferStatus(a);
#else
   GLenum res = ::glCheckFramebufferStatus(a);
#endif
   check_error();
   return res;
}
void framebuffer_texture_2d(GLenum a, GLenum b, GLenum c, GLuint d, GLint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glFramebufferTexture2D != nullptr);
   gl::detail::glFramebufferTexture2D(a, b, c, d, e);
#else
   ::glFramebufferTexture2D(a, b, c, d, e);
#endif
   check_error();
}
void framebuffer_renderbuffer(GLenum a, GLenum b, GLenum c, GLuint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glFramebufferRenderbuffer != nullptr);
   gl::detail::glFramebufferRenderbuffer(a, b, c, d);
#else
   ::glFramebufferRenderbuffer(a, b, c, d);
#endif
   check_error();
}
void get_framebuffer_attachment_parameter_iv(GLenum a, GLenum b, GLenum c, GLint *d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetFramebufferAttachmentParameteriv != nullptr);
   gl::detail::glGetFramebufferAttachmentParameteriv(a, b, c, d);
#else
   ::glGetFramebufferAttachmentParameteriv(a, b, c, d);
#endif
   check_error();
}
void generate_mipmap(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGenerateMipmap != nullptr);
   gl::detail::glGenerateMipmap(a);
#else
   ::glGenerateMipmap(a);
#endif
   check_error();
}

//  OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void blit_framebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBlitFramebuffer != nullptr);
   gl::detail::glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
#else
   ::glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
#endif
   check_error();
}
void renderbuffer_storage_multisample(GLenum a, GLsizei b, GLenum c, GLsizei d, GLsizei e)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glRenderbufferStorageMultisample != nullptr);
   gl::detail::glRenderbufferStorageMultisample(a, b, c, d, e);
#else
   ::glRenderbufferStorageMultisample(a, b, c, d, e);
#endif
   check_error();
}
void framebuffer_texture_layer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glFramebufferTextureLayer != nullptr);
   gl::detail::glFramebufferTextureLayer(target, attachment, texture, level, layer);
#else
   ::glFramebufferTextureLayer(target, attachment, texture, level, layer);
#endif
   check_error();
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
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

#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glMapBufferRange != nullptr);
   GLvoid* res = gl::detail::glMapBufferRange(target, offset, length, access);
#else
   GLvoid* res = ::glMapBufferRange(target, offset, length, access);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glFlushMappedBufferRange != nullptr);
   gl::detail::glFlushMappedBufferRange(target, offset, length);
#else
   ::glFlushMappedBufferRange(target, offset, length);
#endif
   check_error();
}
#endif

/* ARB_vertex_array_object */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void bind_vertex_array(GLuint array_)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB) || defined(LOG_GL_VAO)
   log_trace("bind_vertex_array(array = %u)", array_);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBindVertexArray != nullptr);
   gl::detail::glBindVertexArray(array_);
#else
   ::glBindVertexArray(array_);
#endif
   check_error();
}
void delete_vertex_arrays(GLsizei n, const GLuint *arrays)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB) || defined(LOG_GL_VAO)
   log_trace("delete_vertex_arrays(n = %u, arrays = %p)",
      static_cast<unsigned int>(n), arrays);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteVertexArrays != nullptr);
   gl::detail::glDeleteVertexArrays(n, arrays);
#else
   ::glDeleteVertexArrays(n, arrays);
#endif
   check_error();
}
void gen_vertex_arrays(GLsizei n, GLuint *arrays)
{
#if defined(LOG_GL) || defined(LOG_GL_ATTRIB) || defined(LOG_GL_VAO)
   slog_trace("gen_vertex_arrays(n = %u, arrays = %p)", 
      static_cast<unsigned int>(n), arrays);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGenVertexArrays != nullptr);
   gl::detail::glGenVertexArrays(n, arrays);
#else
   ::glGenVertexArrays(n, arrays);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsVertexArray != nullptr);
   GLboolean res = gl::detail::glIsVertexArray(a);
#else
   GLboolean res = ::glIsVertexArray(a);
#endif
   check_error();
   return res;
}
void get_integer_i_v(GLenum a, GLuint b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetIntegeri_v != nullptr);
   gl::detail::glGetIntegeri_v(a, b, c);
#else
   ::glGetIntegeri_v(a, b, c);
#endif
   check_error();
}
void begin_transform_feedback(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBeginTransformFeedback != nullptr);
   gl::detail::glBeginTransformFeedback(a);
#else
   ::glBeginTransformFeedback(a);
#endif
   check_error();
}
void end_transform_feedback(void)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glEndTransformFeedback != nullptr);
   gl::detail::glEndTransformFeedback();
#else
   ::glEndTransformFeedback();
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBindBufferRange != nullptr);
   gl::detail::glBindBufferRange(target, index, buffer, offset, size);
#else
   ::glBindBufferRange(target, index, buffer, offset, size);
#endif
   check_error();
}
void bind_buffer_base(GLenum target, GLuint index, GLuint buffer)
{
#if defined(LOG_GL) || defined(LOG_GL_BUFFER)
   log_trace("bind_buffer_base(target = %s, index = %u, buffer = %u)", enum_string(target), index, buffer);
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBindBufferBase != nullptr);
   gl::detail::glBindBufferBase(target, index, buffer);
#else
   ::glBindBufferBase(target, index, buffer);
#endif
   check_error();
}
void transform_feedback_varyings(GLuint a, GLsizei b, const GLchar* *c, GLenum d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glTransformFeedbackVaryings != nullptr);
   gl::detail::glTransformFeedbackVaryings(a, b, c, d);
#else
   ::glTransformFeedbackVaryings(a, b, c, d);
#endif
   check_error();
}
void get_transform_feedback_varying(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLsizei *e, GLenum *f, GLchar *g)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetTransformFeedbackVarying != nullptr);
   gl::detail::glGetTransformFeedbackVarying(a, b, c, d, e, f, g);
#else
   ::glGetTransformFeedbackVarying(a, b, c, d, e, f, g);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttribIPointer != nullptr);
   gl::detail::glVertexAttribIPointer(index, size, type, stride, pointer);
#else
   ::glVertexAttribIPointer(index, size, type, stride, pointer);
#endif
   check_error();
}
void get_vertex_attrib_iiv(GLuint a, GLenum b, GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetVertexAttribIiv != nullptr);
   gl::detail::glGetVertexAttribIiv(a, b, c);
#else
   ::glGetVertexAttribIiv(a, b, c);
#endif
   check_error();
}
void get_vertex_attrib_iuiv(GLuint a, GLenum b, GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetVertexAttribIuiv != nullptr);
   gl::detail::glGetVertexAttribIuiv(a, b, c);
#else
   ::glGetVertexAttribIuiv(a, b, c);
#endif
   check_error();
}
void vertex_attrib_i4i(GLuint a, GLint b, GLint c, GLint d, GLint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttribI4i != nullptr);
   gl::detail::glVertexAttribI4i(a, b, c, d, e);
#else
   ::glVertexAttribI4i(a, b, c, d, e);
#endif
   check_error();
}
void vertex_attrib_i4ui(GLuint a, GLuint b, GLuint c, GLuint d, GLuint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttribI4ui != nullptr);
   gl::detail::glVertexAttribI4ui(a, b, c, d, e);
#else
   ::glVertexAttribI4ui(a, b, c, d, e);
#endif
   check_error();
}
void vertex_attrib_i4iv(GLuint a, const GLint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttribI4iv != nullptr);
   gl::detail::glVertexAttribI4iv(a, b);
#else
   ::glVertexAttribI4iv(a, b);
#endif
   check_error();
}
void vertex_attrib_i4uiv(GLuint a, const GLuint *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glVertexAttribI4uiv != nullptr);
   gl::detail::glVertexAttribI4uiv(a, b);
#else
   ::glVertexAttribI4uiv(a, b);
#endif
   check_error();
}
void get_uniform_uiv(GLuint a, GLint b, GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetUniformuiv != nullptr);
   gl::detail::glGetUniformuiv(a, b, c);
#else
   ::glGetUniformuiv(a, b, c);
#endif
   check_error();
}
GLint get_frag_data_location(GLuint a, const GLchar *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetFragDataLocation != nullptr);
   GLint res = gl::detail::glGetFragDataLocation(a, b);
#else
   GLint res = ::glGetFragDataLocation(a, b);
#endif
   check_error();
   return res;
}
void uniform_1ui(GLint a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform1ui != nullptr);
   gl::detail::glUniform1ui(a, b);
#else
   ::glUniform1ui(a, b);
#endif
   check_error();
}
void uniform_2ui(GLint a, GLuint b, GLuint c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform2ui != nullptr);
   gl::detail::glUniform2ui(a, b, c);
#else
   ::glUniform2ui(a, b, c);
#endif
   check_error();
}
void uniform_3ui(GLint a, GLuint b, GLuint c, GLuint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform3ui != nullptr);
   gl::detail::glUniform3ui(a, b, c, d);
#else
   ::glUniform3ui(a, b, c, d);
#endif
   check_error();
}
void uniform_4ui(GLint a, GLuint b, GLuint c, GLuint d, GLuint e)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform4ui != nullptr);
   gl::detail::glUniform4ui(a, b, c, d, e);
#else
   ::glUniform4ui(a, b, c, d, e);
#endif
   check_error();
}
void uniform_1uiv(GLint a, GLsizei b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform1uiv != nullptr);
   gl::detail::glUniform1uiv(a, b, c);
#else
   ::glUniform1uiv(a, b, c);
#endif
   check_error();
}
void uniform_2uiv(GLint a, GLsizei b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform2uiv != nullptr);
   gl::detail::glUniform2uiv(a, b, c);
#else
   ::glUniform2uiv(a, b, c);
#endif
   check_error();
}
void uniform_3uiv(GLint a, GLsizei b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform3uiv != nullptr);
   gl::detail::glUniform3uiv(a, b, c);
#else
   ::glUniform3uiv(a, b, c);
#endif
   check_error();
}
void uniform_4uiv(GLint a, GLsizei b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniform4uiv != nullptr);
   gl::detail::glUniform4uiv(a, b, c);
#else
   ::glUniform4uiv(a, b, c);
#endif
   check_error();
}
void clear_buffer_iv(GLenum a, GLint b, const GLint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glClearBufferiv != nullptr);
   gl::detail::glClearBufferiv(a, b, c);
#else
   ::glClearBufferiv(a, b, c);
#endif
   check_error();
}
void clear_buffer_uiv(GLenum a, GLint b, const GLuint *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glClearBufferuiv != nullptr);
   gl::detail::glClearBufferuiv(a, b, c);
#else
   ::glClearBufferuiv(a, b, c);
#endif
   check_error();
}
void clear_buffer_fv(GLenum a, GLint b, const GLfloat *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glClearBufferfv != nullptr);
   gl::detail::glClearBufferfv(a, b, c);
#else
   ::glClearBufferfv(a, b, c);
#endif
   check_error();
}
void clear_buffer_fi(GLenum a, GLint b, GLfloat c, GLint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glClearBufferfi != nullptr);
   gl::detail::glClearBufferfi(a, b, c, d);
#else
   ::glClearBufferfi(a, b, c, d);
#endif
   check_error();
}
   const GLubyte * get_string_i(GLenum a, GLuint b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetStringi != nullptr);
   const GLubyte *res = gl::detail::glGetStringi(a, b);
#else
   const GLubyte *res = ::glGetStringi(a, b);
#endif
   check_error();
   return res;
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void copy_buffer_sub_data(GLenum a, GLenum b, GLintptr c, GLintptr d, GLsizeiptr e) 
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glCopyBufferSubData != nullptr);
   gl::detail::glCopyBufferSubData(a, b, c, d, e);
#else
   ::glCopyBufferSubData(a, b, c, d, e);
#endif
   check_error();
}
#endif
/* ARB_uniform_buffer_object */ 

//  OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void get_uniform_indices(GLuint program, GLsizei uniformCount, const GLchar* *uniformNames, GLuint *uniformIndices)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetUniformIndices != nullptr);
   gl::detail::glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
#else
   ::glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
#endif
   check_error();
}
void get_active_uniforms_iv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetActiveUniformsiv != nullptr);
   gl::detail::glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
#else
   ::glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
#endif
   check_error();
}
GLuint get_uniform_block_index(GLuint program, const GLchar *uniformBlockName)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetUniformBlockIndex != nullptr);
   GLuint res = gl::detail::glGetUniformBlockIndex (program, uniformBlockName);
#else
   GLuint res = ::glGetUniformBlockIndex (program, uniformBlockName);
#endif
   check_error();
   return res;
}
void get_active_uniform_block_iv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetActiveUniformBlockiv != nullptr);
   gl::detail::glGetActiveUniformBlockiv (program, uniformBlockIndex, pname, params);
#else
   ::glGetActiveUniformBlockiv (program, uniformBlockIndex, pname, params);
#endif
   check_error();
}
void get_active_uniform_block_name(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetActiveUniformBlockName != nullptr);
   gl::detail::glGetActiveUniformBlockName (program, uniformBlockIndex, bufSize, length, uniformBlockName);
#else
   ::glGetActiveUniformBlockName (program, uniformBlockIndex, bufSize, length, uniformBlockName);
#endif
   check_error();
}
void uniform_block_binding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glUniformBlockBinding != nullptr);
   gl::detail::glUniformBlockBinding (program, uniformBlockIndex, uniformBlockBinding);
#else
   ::glUniformBlockBinding (program, uniformBlockIndex, uniformBlockBinding);
#endif
   check_error();
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void get_active_uniform_name(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glGetActiveUniformName != nullptr);
   gl::detail::glGetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
   check_error();
}
#endif

//  GL_ARB_draw_instanced

//  OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void draw_arrays_instanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount)
{
#if defined(LOG_GL) || defined(LOG_GL_DRAW)
   log_trace("draw_arrays_instanced(mode = %s, first = %d, count = %u, primcount = %u)",
      enum_string(mode),
      first,
      static_cast<unsigned int>(count),
      static_cast<unsigned int>(primcount));
#endif
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDrawArraysInstanced != nullptr);
   gl::detail::glDrawArraysInstanced(mode, first, count, primcount);
#else
   ::glDrawArraysInstanced(mode, first, count, primcount);
#endif
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
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDrawElementsInstanced != nullptr);
   gl::detail::glDrawElementsInstanced(mode, count, type, indices, primcount);
#else
   ::glDrawElementsInstanced(mode, count, type, indices, primcount);
#endif
   check_error();
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
GLsync fence_sync(GLenum a, GLbitfield b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glFenceSync != nullptr);
   GLsync res = gl::detail::glFenceSync(a, b);
#else
   GLsync res = ::glFenceSync(a, b);
#endif
   check_error();
   return res;
}
GLboolean is_sync(GLsync a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsSync != nullptr);
   GLboolean res = gl::detail::glIsSync(a);
#else
   GLboolean res = ::glIsSync(a);
#endif
   check_error();
   return res;
}
void delete_sync(GLsync a)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteSync != nullptr);
   gl::detail::glDeleteSync(a);
#else
   ::glDeleteSync(a);
#endif
   check_error();
}
GLenum client_wait_sync(GLsync a, GLbitfield b, GLuint64 c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glClientWaitSync != nullptr);
   GLenum res = gl::detail::glClientWaitSync(a, b, c);
#else
   GLenum res = ::glClientWaitSync(a, b, c);
#endif
   check_error();
   return res;
}
void wait_sync(GLsync a, GLbitfield b, GLuint64 c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glWaitSync != nullptr);
   gl::detail::glWaitSync(a, b, c);
#else
   ::glWaitSync(a, b, c);
#endif
   check_error();
}
void get_integer_64v(GLenum a, GLint64 *b)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetInteger64v != nullptr);
   gl::detail::glGetInteger64v(a, b);
#else
   ::glGetInteger64v(a, b);
#endif
   check_error();
}
void get_sync_iv(GLsync a, GLenum b, GLsizei c, GLsizei *d, GLint *e)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetSynciv != nullptr);
   gl::detail::glGetSynciv(a, b, c, d, e);
#else
   ::glGetSynciv(a, b, c, d, e);
#endif
   check_error();
}
void get_integer_64i_v(GLenum a, GLuint b, GLint64 *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetInteger64i_v != nullptr);
   gl::detail::glGetInteger64i_v(a, b, c);
#else
   ::glGetInteger64i_v(a, b, c);
#endif
   check_error();
}
void get_buffer_parameter_i64v(GLenum a, GLenum b, GLint64 *c)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetBufferParameteri64v != nullptr);
   gl::detail::glGetBufferParameteri64v(a, b, c);
#else
   ::glGetBufferParameteri64v(a, b, c);
#endif
   check_error();
}
#endif

/*  GL_ARB_sampler_objects  */ 

//  OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void gen_samplers(GLsizei count, GLuint *samplers)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGenSamplers != nullptr);
   gl::detail::glGenSamplers(count, samplers);
#else
   ::glGenSamplers(count, samplers);
#endif
   check_error();
}
void delete_samplers(GLsizei count, const GLuint *samplers)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteSamplers != nullptr);
   gl::detail::glDeleteSamplers(count, samplers);
#else
   ::glDeleteSamplers(count, samplers);
#endif
   check_error();
}
GLboolean is_sampler(GLuint sampler)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsSampler != nullptr);
   GLboolean res = gl::detail::glIsSampler(sampler);
#else
   GLboolean res = ::glIsSampler(sampler);
#endif
   check_error();
   return res;
}
void bind_sampler(GLuint unit, GLuint sampler)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBindSampler != nullptr);
   gl::detail::glBindSampler(unit, sampler);
#else
   ::glBindSampler(unit, sampler);
#endif
   check_error();
}
void sampler_parameter_i(GLuint sampler, GLenum pname, GLint param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glSamplerParameteri != nullptr);
   gl::detail::glSamplerParameteri(sampler, pname, param);
#else
   ::glSamplerParameteri(sampler, pname, param);
#endif
   check_error();
}
void sampler_parameter_iv(GLuint sampler, GLenum pname, const GLint *param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glSamplerParameteriv != nullptr);
   gl::detail::glSamplerParameteriv(sampler, pname, param);
#else
   ::glSamplerParameteriv(sampler, pname, param);
#endif
   check_error();
}
void sampler_parameter_f(GLuint sampler, GLenum pname, GLfloat param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glSamplerParameterf != nullptr);
   gl::detail::glSamplerParameterf(sampler, pname, param);
#else
   ::glSamplerParameterf(sampler, pname, param);
#endif
   check_error();
}
void sampler_parameter_fv(GLuint sampler, GLenum pname, const GLfloat *param)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glSamplerParameterfv != nullptr);
   gl::detail::glSamplerParameterfv(sampler, pname, param);
#else
   ::glSamplerParameterfv(sampler, pname, param);
#endif
   check_error();
}
void get_sampler_parameter_iv(GLuint sampler, GLenum pname, GLint *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetSamplerParameteriv != nullptr);
   gl::detail::glGetSamplerParameteriv(sampler, pname, params);
#else
   ::glGetSamplerParameteriv(sampler, pname, params);
#endif
   check_error();
}
void get_sampler_parameter_fv(GLuint sampler, GLenum pname, GLfloat *params)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetSamplerParameterfv != nullptr);
   gl::detail::glGetSamplerParameterfv(sampler, pname, params);
#else
   ::glGetSamplerParameterfv(sampler, pname, params);
#endif
   check_error();
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
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

#if defined(RENDERSTACK_GL_API_OPENGL)
void framebuffer_texture(GLenum a, GLenum b, GLuint c, GLint d)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glFramebufferTexture != nullptr);
   gl::detail::glFramebufferTexture(a, b, c, d);
   check_error();
}
#endif

/*  3.2  */ 
/* ARB_draw_elements_base_vertex */ 
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL)
void provoking_vertex(GLenum a)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glProvokingVertex != nullptr);
   gl::detail::glProvokingVertex(a);
   check_error();
}
#endif

/* ARB_texture_multisample */
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL)
void min_sample_shading(GLclampf value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glMinSampleShading != nullptr);
   gl::detail::glMinSampleShading(value);
   check_error();
}
#endif

/*  GL_ARB_timer_query  */ 
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void get_program_binary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGetProgramBinary != nullptr);
   gl::detail::glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
#else
   ::glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
#endif
   check_error();
}
void program_binary(GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glProgramBinary != nullptr);
   gl::detail::glProgramBinary(program, binaryFormat, binary, length);
#else
   ::glProgramBinary(program, binaryFormat, binary, length);
#endif
   check_error();
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void program_parameter_i(GLuint program, GLenum pname, GLint value)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glProgramParameteri != nullptr);
   gl::detail::glProgramParameteri(program, pname, value);
   check_error();
}
#endif

/*  GL_ARB_base_instance  */
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void bind_transform_feedback(GLenum target, GLuint id)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glBindTransformFeedback != nullptr);
   gl::detail::glBindTransformFeedback(target, id);
#else
   ::glBindTransformFeedback(target, id);
#endif
   check_error();
}
void delete_transform_feedbacks(GLsizei n, const GLuint *ids)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glDeleteTransformFeedbacks != nullptr);
   gl::detail::glDeleteTransformFeedbacks(n, ids);
#else
   ::glDeleteTransformFeedbacks(n, ids);
#endif
   check_error();
}
void gen_transform_feedbacks(GLsizei n, GLuint *ids)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glGenTransformFeedbacks != nullptr);
   gl::detail::glGenTransformFeedbacks(n, ids);
#else
   ::glGenTransformFeedbacks(n, ids);
#endif
   check_error();
}
GLboolean is_transform_feedback(GLuint id)
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glIsTransformFeedback != nullptr);
   GLboolean res = gl::detail::glIsTransformFeedback(id);
#else
   GLboolean res = ::glIsTransformFeedback(id);
#endif
   check_error();
   return res;
}
void pause_transform_feedback()
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glPauseTransformFeedback != nullptr);
   gl::detail::glPauseTransformFeedback();
#else
   ::glPauseTransformFeedback();
#endif
   check_error();
}
void resume_transform_feedback ()
{  LOG_GL_FUNCTION(__FUNCTION__);
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS) || defined(RENDERSTACK_DLOAD_WINDOWS_GL_SYMBOLS)
   assert(gl::detail::glResumeTransformFeedback != nullptr);
   gl::detail::glResumeTransformFeedback();
#else
   ::glResumeTransformFeedback();
#endif
   check_error();
}
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void draw_transform_feedback(GLenum mode, GLuint id)
{  LOG_GL_FUNCTION(__FUNCTION__);
   assert(gl::detail::glDrawTransformFeedback != nullptr);
   gl::detail::glDrawTransformFeedback(mode, id);
   check_error();
}
#endif


/*  GL_ARB_transform_feedback3  */
#if defined(RENDERSTACK_GL_API_OPENGL)
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
#if defined(RENDERSTACK_GL_API_OPENGL)
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

