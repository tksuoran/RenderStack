#include <GLES3/gl3.h>   

/* OpenGL ES 2.0 */

GL_APICALL void GL_APIENTRY glActiveTexture(GLenum texture)
{
  (void)texture;
}
GL_APICALL void GL_APIENTRY glAttachShader(GLuint program, GLuint shader)
{
  (void)program;
  (void)shader;
}
GL_APICALL void GL_APIENTRY glBindAttribLocation(GLuint program, GLuint index, const GLchar* name)
{
  (void)program;
  (void)index;
  (void)name;
}
GL_APICALL void GL_APIENTRY glBindBuffer(GLenum target, GLuint buffer)
{
  (void)target;
  (void)buffer;
}
GL_APICALL void GL_APIENTRY glBindFramebuffer(GLenum target, GLuint framebuffer)
{
  (void)target;
  (void)framebuffer;
}
GL_APICALL void GL_APIENTRY glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
  (void)target;
  (void)renderbuffer;
}
GL_APICALL void GL_APIENTRY glBindTexture(GLenum target, GLuint texture)
{
  (void)target;
  (void)texture;
}
GL_APICALL void GL_APIENTRY glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  (void)red;
  (void)green;
  (void)blue;
  (void)alpha;
}
GL_APICALL void GL_APIENTRY glBlendEquation(GLenum mode)
{
  (void)mode;
}
GL_APICALL void GL_APIENTRY glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
  (void)modeRGB;
}
GL_APICALL void GL_APIENTRY glBlendFunc(GLenum sfactor, GLenum dfactor)
{
  (void)sfactor;
  (void)dfactor;
}
GL_APICALL void GL_APIENTRY glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
  (void)srcRGB;
  (void)dstRGB;
  (void)srcAlpha;
  (void)dstAlpha;
}
GL_APICALL void GL_APIENTRY glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
  (void)target;
  (void)size;
  (void)data;
  (void)usage;
}
GL_APICALL void GL_APIENTRY glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
  (void)target;
  (void)offset;
  (void)size;
  (void)data;
}
GL_APICALL GLenum GL_APIENTRY glCheckFramebufferStatus(GLenum target)
{
  (void)target;
   return GL_NONE;
}
GL_APICALL void GL_APIENTRY glClear(GLbitfield mask)
{
  (void)mask;
}
GL_APICALL void GL_APIENTRY glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  (void)red;
  (void)green;
  (void)blue;
  (void)alpha;
}
GL_APICALL void GL_APIENTRY glClearDepthf(GLfloat depth)
{
  (void)depth;
}
GL_APICALL void GL_APIENTRY glClearStencil(GLint s)
{
  (void)s;
}
GL_APICALL void GL_APIENTRY glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
  (void)red;
  (void)green;
  (void)blue;
  (void)alpha;
}
GL_APICALL void GL_APIENTRY glCompileShader(GLuint shader)
{
  (void)shader;
}
GL_APICALL void GL_APIENTRY glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data)
{
  (void)target;
  (void)level;
  (void)internalformat;
  (void)width;
  (void)height;
  (void)border;
  (void)imageSize;
  (void)data;
}
GL_APICALL void GL_APIENTRY glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data)
{
  (void)target;
  (void)level;
  (void)xoffset;
  (void)yoffset;
  (void)width;
  (void)height;
  (void)format;
  (void)imageSize;
  (void)data;
}
GL_APICALL void GL_APIENTRY glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
  (void)target;
  (void)level;
  (void)internalformat;
  (void)x;
  (void)y;
  (void)width;
  (void)height;
  (void)border;
}
GL_APICALL void GL_APIENTRY glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
  (void)target;
  (void)level;
  (void)xoffset;
  (void)yoffset;
  (void)x;
  (void)y;
  (void)width;
  (void)height;
}
GL_APICALL GLuint GL_APIENTRY glCreateProgram(void)
{
   return 0;
}
GL_APICALL GLuint GL_APIENTRY glCreateShader(GLenum type)
{
  (void)type;
   return 0;
}
GL_APICALL void GL_APIENTRY glCullFace(GLenum mode)
{
  (void)mode;
}
GL_APICALL void GL_APIENTRY glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
  (void)n;
  (void)buffers;
}
GL_APICALL void GL_APIENTRY glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
  (void)n;
  (void)framebuffers;
}
GL_APICALL void GL_APIENTRY glDeleteProgram(GLuint program)
{
  (void)program;
}
GL_APICALL void GL_APIENTRY glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
  (void)n;
  (void)renderbuffers;
}
GL_APICALL void GL_APIENTRY glDeleteShader(GLuint shader)
{
  (void)shader;
}
GL_APICALL void GL_APIENTRY glDeleteTextures(GLsizei n, const GLuint* textures)
{
  (void)n;
  (void)textures;
}
GL_APICALL void GL_APIENTRY glDepthFunc(GLenum func)
{
  (void)func;
}
GL_APICALL void GL_APIENTRY glDepthMask(GLboolean flag)
{
  (void)flag;
}
GL_APICALL void GL_APIENTRY glDepthRangef(GLfloat n, GLfloat f)
{
  (void)n;
  (void)f;
}
GL_APICALL void GL_APIENTRY glDetachShader(GLuint program, GLuint shader)
{
  (void)program;
  (void)shader;
}
GL_APICALL void GL_APIENTRY glDisable(GLenum cap)
{
  (void)cap;
}
GL_APICALL void GL_APIENTRY glDisableVertexAttribArray(GLuint index)
{
  (void)index;
}
GL_APICALL void GL_APIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
  (void)mode;
  (void)first;
  (void)count;
}

GL_APICALL void GL_APIENTRY glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
  (void)mode;
  (void)count;
  (void)type;
  (void)indices;
}
GL_APICALL void GL_APIENTRY glEnable(GLenum cap)
{
  (void)cap;
}
GL_APICALL void GL_APIENTRY glEnableVertexAttribArray(GLuint index)
{
  (void)index;
}
GL_APICALL void GL_APIENTRY glFinish(void)
{
}
GL_APICALL void GL_APIENTRY glFlush(void)
{
}
GL_APICALL void GL_APIENTRY glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  (void)target;
  (void)attachment;
  (void)renderbuffertarget;
  (void)renderbuffer;
}
GL_APICALL void GL_APIENTRY glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  (void)target;
  (void)attachment;
  (void)textarget;
  (void)texture;
  (void)level;
}
GL_APICALL void GL_APIENTRY glFrontFace(GLenum mode)
{
  (void)mode;
}
GL_APICALL void GL_APIENTRY glGenBuffers(GLsizei n, GLuint* buffers)
{
  (void)n;
  (void)buffers;
}
GL_APICALL void GL_APIENTRY glGenerateMipmap(GLenum target)
{
  (void)target;
}
GL_APICALL void GL_APIENTRY glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
  (void)n;
  (void)framebuffers;
}
GL_APICALL void GL_APIENTRY glGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
  (void)n;
  (void)renderbuffers;
}
GL_APICALL void GL_APIENTRY glGenTextures(GLsizei n, GLuint* textures)
{
  (void)n;
  (void)textures;
}
GL_APICALL void GL_APIENTRY glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
  (void)program;
  (void)index;
  (void)bufsize;
  (void)length;
  (void)size;
  (void)type;
  (void)name;
}
GL_APICALL void GL_APIENTRY glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
  (void)program;
  (void)index;
  (void)bufsize;
  (void)length;
  (void)size;
  (void)type;
  (void)name;
}
GL_APICALL void GL_APIENTRY glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
  (void)program;
  (void)maxcount;
  (void)count;
  (void)shaders;
}
GL_APICALL int GL_APIENTRY glGetAttribLocation(GLuint program, const GLchar* name)
{
  (void)program;
  (void)name;
   return -1;
}
GL_APICALL void GL_APIENTRY glGetBooleanv(GLenum pname, GLboolean* params)
{
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
  (void)target;
  (void)pname;
  (void)params;
}
GL_APICALL GLenum GL_APIENTRY glGetError(void)
{
   return GL_NO_ERROR;
}
GL_APICALL void GL_APIENTRY glGetFloatv(GLenum pname, GLfloat* params)
{
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
  (void)target;
  (void)attachment;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetIntegerv(GLenum pname, GLint* params)
{
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
  (void)program;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog)
{
  (void)program;
  (void)bufsize;
  (void)length;
  (void)infolog;
}
GL_APICALL void GL_APIENTRY glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
  (void)target;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
  (void)shader;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog)
{
  (void)shader;
  (void)bufsize;
  (void)length;
  (void)infolog;
}
GL_APICALL void GL_APIENTRY glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
  (void)shadertype;
  (void)precisiontype;
  (void)range;
  (void)precision;
}
GL_APICALL void GL_APIENTRY glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source)
{
  (void)shader;
  (void)bufsize;
  (void)length;
  (void)source;
}
GL_APICALL const GLubyte* GL_APIENTRY glGetString(GLenum name)
{
  (void)name;
   return(const GLubyte*)(0);
}
GL_APICALL void GL_APIENTRY glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
  (void)target;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetTexParameteriv(GLenum target, GLenum pname, GLint* params)
{
  (void)target;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetUniformfv(GLuint program, GLint location, GLfloat* params)
{
  (void)program;
  (void)location;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetUniformiv(GLuint program, GLint location, GLint* params)
{
  (void)program;
  (void)location;
  (void)params;
}
GL_APICALL int GL_APIENTRY glGetUniformLocation(GLuint program, const GLchar* name)
{
  (void)program;
  (void)name;
   return -1;
}
GL_APICALL void GL_APIENTRY glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params)
{
  (void)index;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params)
{
  (void)index;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid** pointer)
{
  (void)index;
  (void)pname;
  (void)pointer;
}
GL_APICALL void GL_APIENTRY glHint(GLenum target, GLenum mode)
{
  (void)target;
  (void)mode;
}
GL_APICALL GLboolean GL_APIENTRY glIsBuffer(GLuint buffer)
{
  (void)buffer;
   return GL_FALSE;
}
GL_APICALL GLboolean GL_APIENTRY glIsEnabled(GLenum cap)
{
  (void)cap;
   return GL_FALSE;
}
GL_APICALL GLboolean GL_APIENTRY glIsFramebuffer(GLuint framebuffer)
{
  (void)framebuffer;
   return GL_FALSE;
}
GL_APICALL GLboolean GL_APIENTRY glIsProgram(GLuint program)
{
  (void)program;
   return GL_FALSE;
}
GL_APICALL GLboolean GL_APIENTRY glIsRenderbuffer(GLuint renderbuffer)
{
  (void)renderbuffer;
   return GL_FALSE;
}
GL_APICALL GLboolean GL_APIENTRY glIsShader(GLuint shader)
{
  (void)shader;
   return GL_FALSE;
}
GL_APICALL GLboolean GL_APIENTRY glIsTexture(GLuint texture)
{
  (void)texture;
   return GL_FALSE;
}
GL_APICALL void GL_APIENTRY glLineWidth(GLfloat width)
{
  (void)width;
}
GL_APICALL void GL_APIENTRY glLinkProgram(GLuint program)
{
  (void)program;
}
GL_APICALL void GL_APIENTRY glPixelStorei(GLenum pname, GLint param)
{
  (void)pname;
  (void)param;
}
GL_APICALL void GL_APIENTRY glPolygonOffset(GLfloat factor, GLfloat units)
{
  (void)factor;
  (void)units;
}
GL_APICALL void GL_APIENTRY glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels)
{
  (void)x;
  (void)y;
  (void)width;
  (void)height;
  (void)format;
  (void)type;
  (void)pixels;
}
GL_APICALL void GL_APIENTRY glReleaseShaderCompiler(void)
{
}
GL_APICALL void GL_APIENTRY glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
  (void)target;
  (void)internalformat;
  (void)width;
  (void)height;
}
GL_APICALL void GL_APIENTRY glSampleCoverage(GLfloat value, GLboolean invert)
{
  (void)value;
  (void)invert;
}
GL_APICALL void GL_APIENTRY glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
  (void)x;
  (void)y;
  (void)width;
  (void)height;
}
GL_APICALL void GL_APIENTRY glShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length)
{
  (void)n;
  (void)shaders;
  (void)binaryformat;
  (void)binary;
  (void)length;
}
GL_APICALL void GL_APIENTRY glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
  (void)shader;
  (void)count;
  (void)string;
  (void)length;
}
GL_APICALL void GL_APIENTRY glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
  (void)func;
  (void)ref;
  (void)mask;
}
GL_APICALL void GL_APIENTRY glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
  (void)face;
  (void)func;
  (void)ref;
  (void)mask;
}
GL_APICALL void GL_APIENTRY glStencilMask(GLuint mask)
{
  (void)mask;
}
GL_APICALL void GL_APIENTRY glStencilMaskSeparate(GLenum face, GLuint mask)
{
  (void)face;
  (void)mask;
}
GL_APICALL void GL_APIENTRY glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
  (void)fail;
  (void)zfail;
  (void)zpass;
}
GL_APICALL void GL_APIENTRY glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
  (void)face;
  (void)fail;
  (void)zfail;
  (void)zpass;
}
GL_APICALL void GL_APIENTRY glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
  (void)target;
  (void)level;
  (void)internalformat;
  (void)width;
  (void)height;
  (void)border;
  (void)format;
  (void)type;
  (void)pixels;
}
GL_APICALL void GL_APIENTRY glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
  (void)target;
  (void)pname;
  (void)param;
}
GL_APICALL void GL_APIENTRY glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params)
{
  (void)target;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glTexParameteri(GLenum target, GLenum pname, GLint param)
{
  (void)target;
  (void)pname;
  (void)param;
}
GL_APICALL void GL_APIENTRY glTexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
  (void)target;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels)
{
  (void)target;
  (void)level;
  (void)xoffset;
  (void)yoffset;
  (void)width;
  (void)height;
  (void)format;
  (void)type;
  (void)pixels;
}
GL_APICALL void GL_APIENTRY glUniform1f(GLint location, GLfloat x)
{
  (void)location;
  (void)x;
}
GL_APICALL void GL_APIENTRY glUniform1fv(GLint location, GLsizei count, const GLfloat* v)
{
  (void)location;
  (void)count;
  (void)v;
}
GL_APICALL void GL_APIENTRY glUniform1i(GLint location, GLint x)
{
  (void)location;
  (void)x;
}
GL_APICALL void GL_APIENTRY glUniform1iv(GLint location, GLsizei count, const GLint* v)
{
  (void)location;
  (void)count;
  (void)v;
}
GL_APICALL void GL_APIENTRY glUniform2f(GLint location, GLfloat x, GLfloat y)
{
  (void)location;
  (void)x;
  (void)y;
}
GL_APICALL void GL_APIENTRY glUniform2fv(GLint location, GLsizei count, const GLfloat* v)
{
  (void)location;
  (void)count;
  (void)v;
}
GL_APICALL void GL_APIENTRY glUniform2i(GLint location, GLint x, GLint y)
{
  (void)location;
  (void)x;
  (void)y;
}
GL_APICALL void GL_APIENTRY glUniform2iv(GLint location, GLsizei count, const GLint* v)
{
  (void)location;
  (void)count;
  (void)v;
}
GL_APICALL void GL_APIENTRY glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
  (void)location;
  (void)x;
  (void)y;
  (void)z;
}
GL_APICALL void GL_APIENTRY glUniform3fv(GLint location, GLsizei count, const GLfloat* v)
{
  (void)location;
  (void)count;
  (void)v;
}
GL_APICALL void GL_APIENTRY glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
  (void)location;
  (void)x;
  (void)y;
  (void)z;
}
GL_APICALL void GL_APIENTRY glUniform3iv(GLint location, GLsizei count, const GLint* v)
{
  (void)location;
  (void)count;
  (void)v;
}
GL_APICALL void GL_APIENTRY glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  (void)location;
  (void)x;
  (void)y;
  (void)z;
  (void)w;
}
GL_APICALL void GL_APIENTRY glUniform4fv(GLint location, GLsizei count, const GLfloat* v)
{
  (void)location;
  (void)count;
  (void)v;
}
GL_APICALL void GL_APIENTRY glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
  (void)location;
  (void)x;
  (void)y;
  (void)z;
  (void)w;
}
GL_APICALL void GL_APIENTRY glUniform4iv(GLint location, GLsizei count, const GLint* v)
{
  (void)location;
  (void)count;
  (void)v;
}
GL_APICALL void GL_APIENTRY glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  (void)location;
  (void)count;
  (void)transpose;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  (void)location;
  (void)count;
  (void)transpose;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  (void)location;
  (void)count;
  (void)transpose;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUseProgram(GLuint program)
{
  (void)program;
}
GL_APICALL void GL_APIENTRY glValidateProgram(GLuint program)
{
  (void)program;
}
GL_APICALL void GL_APIENTRY glVertexAttrib1f(GLuint indx, GLfloat x)
{
  (void)indx;
  (void)x;
}
GL_APICALL void GL_APIENTRY glVertexAttrib1fv(GLuint indx, const GLfloat* values)
{
  (void)indx;
  (void)values;
}
GL_APICALL void GL_APIENTRY glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y)
{
  (void)indx;
  (void)x;
  (void)y;
}
GL_APICALL void GL_APIENTRY glVertexAttrib2fv(GLuint indx, const GLfloat* values)
{
  (void)indx;
  (void)values;
}
GL_APICALL void GL_APIENTRY glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
  (void)indx;
  (void)x;
  (void)y;
  (void)z;
}
GL_APICALL void GL_APIENTRY glVertexAttrib3fv(GLuint indx, const GLfloat* values)
{
  (void)indx;
  (void)values;
}
GL_APICALL void GL_APIENTRY glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  (void)indx;
  (void)x;
  (void)y;
  (void)z;
  (void)w;
}
GL_APICALL void GL_APIENTRY glVertexAttrib4fv(GLuint indx, const GLfloat* values)
{
  (void)indx;
  (void)values;
}
GL_APICALL void GL_APIENTRY glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr)
{
  (void)indx;
  (void)size;
  (void)type;
  (void)normalized;
  (void)stride;
  (void)ptr;
}
GL_APICALL void GL_APIENTRY glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
  (void)x;
  (void)y;
  (void)width;
  (void)height;
}

/* OpenGL ES 3.0 */

GL_APICALL void GL_APIENTRY glReadBuffer(GLenum mode)
{
  (void)mode;
}
GL_APICALL void GL_APIENTRY glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices)
{
  (void)mode;
  (void)start;
  (void)end;
  (void)count;
  (void)type;
  (void)indices;
}
GL_APICALL void GL_APIENTRY glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
  (void)target;
  (void)level;
  (void)internalformat;
  (void)width;
  (void)height;
  (void)depth;
  (void)border;
  (void)format;
  (void)type;
  (void)pixels;
}
GL_APICALL void GL_APIENTRY glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels)
{
  (void)target;
  (void)level;
  (void)xoffset;
  (void)yoffset;
  (void)zoffset;
  (void)width;
  (void)height;
  (void)depth;
  (void)format;
  (void)type;
  (void)pixels;
}
GL_APICALL void GL_APIENTRY glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
  (void)target;
  (void)level;
  (void)xoffset;
  (void)yoffset;
  (void)zoffset;
  (void)x;
  (void)y;
  (void)width;
  (void)height;
}
GL_APICALL void GL_APIENTRY glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data)
{
  (void)target;
  (void)level;
  (void)internalformat;
  (void)width;
  (void)height;
  (void)depth;
  (void)border;
  (void)imageSize;
  (void)data;
}
GL_APICALL void GL_APIENTRY glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data)
{
  (void)target;
  (void)level;
  (void)xoffset;
  (void)yoffset;
  (void)zoffset;
  (void)width;
  (void)height;
  (void)depth;
  (void)format;
  (void)imageSize;
  (void)data;
}
GL_APICALL void GL_APIENTRY glGenQueries(GLsizei n, GLuint* ids)
{
  (void)n;
  (void)ids;
}
GL_APICALL void GL_APIENTRY glDeleteQueries(GLsizei n, const GLuint* ids)
{
  (void)n;
  (void)ids;
}
GL_APICALL GLboolean GL_APIENTRY glIsQuery(GLuint id)
{
  (void)id;
   return GL_FALSE;
}
GL_APICALL void GL_APIENTRY glBeginQuery(GLenum target, GLuint id)
{
  (void)target;
  (void)id;
}
GL_APICALL void GL_APIENTRY glEndQuery(GLenum target)
{
  (void)target;
}
GL_APICALL void GL_APIENTRY glGetQueryiv(GLenum target, GLenum pname, GLint* params)
{
  (void)target;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params)
{
  (void)id;
  (void)pname;
  (void)params;
}
GL_APICALL GLboolean GL_APIENTRY glUnmapBuffer(GLenum target)
{
  (void)target;
   return GL_FALSE;
}
GL_APICALL void GL_APIENTRY glGetBufferPointerv(GLenum target, GLenum pname, GLvoid** params)
{
  (void)target;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glDrawBuffers(GLsizei n, const GLenum* bufs)
{
  (void)n;
  (void)bufs;
}
GL_APICALL void GL_APIENTRY glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  (void)location;
  (void)count;
  (void)transpose;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  (void)location;
  (void)count;
  (void)transpose;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  (void)location;
  (void)count;
  (void)transpose;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  (void)location;
  (void)count;
  (void)transpose;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  (void)location;
  (void)count;
  (void)transpose;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  (void)location;
  (void)count;
  (void)transpose;
  (void)value;
}
GL_APICALL void GL_APIENTRY glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
  (void)srcX0;
  (void)srcY0;
  (void)srcX1;
  (void)srcY1;
  (void)dstX0;
  (void)dstY0;
  (void)dstX1;
  (void)dstY1;
  (void)mask;
  (void)filter;
}
GL_APICALL void GL_APIENTRY glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
  (void)target;
  (void)samples;
  (void)internalformat;
  (void)width;
  (void)height;
}
GL_APICALL void GL_APIENTRY glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
  (void)target;
  (void)attachment;
  (void)texture;
  (void)level;
  (void)layer;
}
GL_APICALL GLvoid* GL_APIENTRY glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
  (void)target;
  (void)offset;
  (void)length;
  (void)access;
   return(GLvoid*)(0);
}
GL_APICALL void GL_APIENTRY glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
{
  (void)target;
  (void)offset;
  (void)length;
}
GL_APICALL void GL_APIENTRY glBindVertexArray(GLuint array)
{
  (void)array;
}
GL_APICALL void GL_APIENTRY glDeleteVertexArrays(GLsizei n, const GLuint* arrays)
{
  (void)n;
  (void)arrays;
}
GL_APICALL void GL_APIENTRY glGenVertexArrays(GLsizei n, GLuint* arrays)
{
  (void)n;
  (void)arrays;
}
GL_APICALL GLboolean GL_APIENTRY glIsVertexArray(GLuint array)
{
  (void)array;
   return GL_FALSE;
}
GL_APICALL void GL_APIENTRY glGetIntegeri_v(GLenum target, GLuint index, GLint* data)
{
  (void)target;
  (void)index;
  (void)data;
}
GL_APICALL void GL_APIENTRY glBeginTransformFeedback(GLenum primitiveMode)
{
  (void)primitiveMode;
}
GL_APICALL void GL_APIENTRY glEndTransformFeedback(void)
{
}
GL_APICALL void GL_APIENTRY glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
  (void)target;
  (void)index;
  (void)buffer;
  (void)offset;
  (void)size;
}
GL_APICALL void GL_APIENTRY glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
  (void)target;
  (void)index;
  (void)buffer;
}
GL_APICALL void GL_APIENTRY glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode)
{
  (void)program;
  (void)count;
  (void)varyings;
  (void)bufferMode;
}
GL_APICALL void GL_APIENTRY glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name)
{
  (void)program;
  (void)index;
  (void)bufSize;
  (void)length;
  (void)size;
  (void)type;
  (void)name;
}
GL_APICALL void GL_APIENTRY glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
  (void)index;
  (void)size;
  (void)type;
  (void)stride;
  (void)pointer;
}
GL_APICALL void GL_APIENTRY glGetVertexAttribIiv(GLuint index, GLenum pname, GLint* params)
{
  (void)index;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint* params)
{
  (void)index;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
{
  (void)index;
  (void)x;
  (void)y;
  (void)z;
  (void)w;
}
GL_APICALL void GL_APIENTRY glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)
{
  (void)index;
  (void)x;
  (void)y;
  (void)z;
  (void)w;
}
GL_APICALL void GL_APIENTRY glVertexAttribI4iv(GLuint index, const GLint* v)
{
  (void)index;
  (void)v;
}
GL_APICALL void GL_APIENTRY glVertexAttribI4uiv(GLuint index, const GLuint* v)
{
  (void)index;
  (void)v;
}
GL_APICALL void GL_APIENTRY glGetUniformuiv(GLuint program, GLint location, GLuint* params)
{
  (void)program;
  (void)location;
  (void)params;
}
GL_APICALL GLint GL_APIENTRY glGetFragDataLocation(GLuint program, const GLchar *name)
{
  (void)program;
  (void)name;
   return -1;
}
GL_APICALL void GL_APIENTRY glUniform1ui(GLint location, GLuint v0)
{
  (void)location;
  (void)v0;
}
GL_APICALL void GL_APIENTRY glUniform2ui(GLint location, GLuint v0, GLuint v1)
{
  (void)location;
  (void)v0;
  (void)v1;
}
GL_APICALL void GL_APIENTRY glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
  (void)location;
  (void)v0;
  (void)v1;
  (void)v2;
}
GL_APICALL void GL_APIENTRY glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
  (void)location;
  (void)v0;
  (void)v1;
  (void)v2;
  (void)v3;
}
GL_APICALL void GL_APIENTRY glUniform1uiv(GLint location, GLsizei count, const GLuint* value)
{
  (void)location;
  (void)count;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniform2uiv(GLint location, GLsizei count, const GLuint* value)
{
  (void)location;
  (void)count;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniform3uiv(GLint location, GLsizei count, const GLuint* value)
{
  (void)location;
  (void)count;
  (void)value;
}
GL_APICALL void GL_APIENTRY glUniform4uiv(GLint location, GLsizei count, const GLuint* value)
{
  (void)location;
  (void)count;
  (void)value;
}
GL_APICALL void GL_APIENTRY glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value)
{
  (void)buffer;
  (void)drawbuffer;
  (void)value;
}
GL_APICALL void GL_APIENTRY glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value)
{
  (void)buffer;
  (void)drawbuffer;
  (void)value;
}
GL_APICALL void GL_APIENTRY glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value)
{
  (void)buffer;
  (void)drawbuffer;
  (void)value;
}
GL_APICALL void GL_APIENTRY glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
  (void)buffer;
  (void)drawbuffer;
  (void)depth;
  (void)stencil;
}
GL_APICALL const GLubyte* GL_APIENTRY glGetStringi(GLenum name, GLuint index)
{
  (void)name;
  (void)index;
   return(const GLubyte*)(0);
}
GL_APICALL void GL_APIENTRY glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
  (void)readTarget;
  (void)writeTarget;
  (void)readOffset;
  (void)writeOffset;
  (void)size;
}
GL_APICALL void GL_APIENTRY glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices)
{
  (void)program;
  (void)uniformCount;
  (void)uniformNames;
  (void)uniformIndices;
}
GL_APICALL void GL_APIENTRY glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params)
{
  (void)program;
  (void)uniformCount;
  (void)uniformIndices;
  (void)pname;
  (void)params;
}
GL_APICALL GLuint GL_APIENTRY glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName)
{
  (void)program;
  (void)uniformBlockName;
   return 0;
}
GL_APICALL void GL_APIENTRY glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params)
{
  (void)program;
  (void)uniformBlockIndex;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName)
{
  (void)program;
  (void)uniformBlockIndex;
  (void)bufSize;
  (void)length;
  (void)uniformBlockName;
}
GL_APICALL void GL_APIENTRY glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
  (void)program;
  (void)uniformBlockIndex;
  (void)uniformBlockBinding;
}
GL_APICALL void GL_APIENTRY glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount)
{
  (void)mode;
  (void)first;
  (void)count;
  (void)instanceCount;
}
GL_APICALL void GL_APIENTRY glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instanceCount)
{
  (void)mode;
  (void)count;
  (void)type;
  (void)indices;
  (void)instanceCount;
}
GL_APICALL GLsync GL_APIENTRY glFenceSync(GLenum condition, GLbitfield flags)
{
  (void)condition;
  (void)flags;
   return 0;
}
GL_APICALL GLboolean GL_APIENTRY glIsSync(GLsync sync)
{
  (void)sync;
   return GL_FALSE;
}
GL_APICALL void GL_APIENTRY glDeleteSync(GLsync sync)
{
  (void)sync;
}
GL_APICALL GLenum GL_APIENTRY glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
  (void)sync;
  (void)flags;
  (void)timeout;
   return GL_WAIT_FAILED;
}
GL_APICALL void GL_APIENTRY glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
  (void)sync;
  (void)flags;
  (void)timeout;
}
GL_APICALL void GL_APIENTRY glGetInteger64v(GLenum pname, GLint64* params)
{
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values)
{
  (void)sync;
  (void)pname;
  (void)bufSize;
  (void)length;
  (void)values;
}
GL_APICALL void GL_APIENTRY glGetInteger64i_v(GLenum target, GLuint index, GLint64* data)
{
  (void)target;
  (void)index;
  (void)data;
}
GL_APICALL void GL_APIENTRY glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64* params)
{
  (void)target;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGenSamplers(GLsizei count, GLuint* samplers)
{
  (void)count;
  (void)samplers;
}
GL_APICALL void GL_APIENTRY glDeleteSamplers(GLsizei count, const GLuint* samplers)
{
  (void)count;
  (void)samplers;
}
GL_APICALL GLboolean GL_APIENTRY glIsSampler(GLuint sampler)
{
  (void)sampler;
   return GL_FALSE;
}
GL_APICALL void GL_APIENTRY glBindSampler(GLuint unit, GLuint sampler)
{
  (void)unit;
  (void)sampler;
}
GL_APICALL void GL_APIENTRY glSamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
  (void)sampler;
  (void)pname;
  (void)param;
}
GL_APICALL void GL_APIENTRY glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint* param)
{
  (void)sampler;
  (void)pname;
  (void)param;
}
GL_APICALL void GL_APIENTRY glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
{
  (void)sampler;
  (void)pname;
  (void)param;
}
GL_APICALL void GL_APIENTRY glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat* param)
{
  (void)sampler;
  (void)pname;
  (void)param;
}
GL_APICALL void GL_APIENTRY glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint* params)
{
  (void)sampler;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat* params)
{
  (void)sampler;
  (void)pname;
  (void)params;
}
GL_APICALL void GL_APIENTRY glVertexAttribDivisor(GLuint index, GLuint divisor)
{
  (void)index;
  (void)divisor;
}
GL_APICALL void GL_APIENTRY glBindTransformFeedback(GLenum target, GLuint id)
{
  (void)target;
  (void)id;
}
GL_APICALL void GL_APIENTRY glDeleteTransformFeedbacks(GLsizei n, const GLuint* ids)
{
  (void)n;
  (void)ids;
}
GL_APICALL void GL_APIENTRY glGenTransformFeedbacks(GLsizei n, GLuint* ids)
{
  (void)n;
  (void)ids;
}
GL_APICALL GLboolean GL_APIENTRY glIsTransformFeedback(GLuint id)
{
  (void)id;
   return GL_FALSE;
}
GL_APICALL void GL_APIENTRY glPauseTransformFeedback(void)
{
}
GL_APICALL void GL_APIENTRY glResumeTransformFeedback(void)
{
}
GL_APICALL void GL_APIENTRY glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary)
{
  (void)program;
  (void)bufSize;
  (void)length;
  (void)binaryFormat;
  (void)binary;
}
GL_APICALL void GL_APIENTRY glProgramBinary(GLuint program, GLenum binaryFormat, const GLvoid* binary, GLsizei length)
{
  (void)program;
  (void)binaryFormat;
  (void)binary;
  (void)length;
}
GL_APICALL void GL_APIENTRY glProgramParameteri(GLuint program, GLenum pname, GLint value)
{
  (void)program;
  (void)pname;
  (void)value;
}
GL_APICALL void GL_APIENTRY glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments)
{
  (void)target;
  (void)numAttachments;
  (void)attachments;
}
GL_APICALL void GL_APIENTRY glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
  (void)target;
  (void)numAttachments;
  (void)attachments;
  (void)x;
  (void)y;
  (void)width;
  (void)height;
}
GL_APICALL void GL_APIENTRY glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
  (void)target;
  (void)levels;
  (void)internalformat;
  (void)width;
  (void)height;
}
GL_APICALL void GL_APIENTRY glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
  (void)target;
  (void)levels;
  (void)internalformat;
  (void)width;
  (void)height;
  (void)depth;
}
GL_APICALL void GL_APIENTRY glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params)
{
  (void)target;
  (void)internalformat;
  (void)pname;
  (void)bufSize;
  (void)params;
}
