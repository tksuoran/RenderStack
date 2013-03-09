#ifndef renderstack_toolkit__dynamic_opengl_es2_h
#define renderstack_toolkit__dynamic_opengl_es2_h

#include "platform.hpp"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

namespace gl { namespace detail {

typedef void            (GL_APIENTRY *RS_ES2_PFNGLACTIVETEXTURE                         ) (GLenum texture);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLATTACHSHADER                          ) (GLuint program, GLuint shader);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBINDATTRIBLOCATION                    ) (GLuint program, GLuint index, const char* name);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBINDBUFFER                            ) (GLenum target, GLuint buffer);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBINDFRAMEBUFFER                       ) (GLenum target, GLuint framebuffer);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBINDRENDERBUFFER                      ) (GLenum target, GLuint renderbuffer);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBINDTEXTURE                           ) (GLenum target, GLuint texture);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBLENDCOLOR                            ) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBLENDEQUATION                         ) (GLenum mode);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBLENDEQUATIONSEPARATE                 ) (GLenum modeRGB, GLenum modeAlpha);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBLENDFUNC                             ) (GLenum sfactor, GLenum dfactor);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBLENDFUNCSEPARATE                     ) (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBUFFERDATA                            ) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLBUFFERSUBDATA                         ) (GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef GLenum          (GL_APIENTRY *RS_ES2_PFNGLCHECKFRAMEBUFFERSTATUS                ) (GLenum target);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCLEAR                                 ) (GLbitfield mask);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCLEARCOLOR                            ) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCLEARDEPTHF                           ) (GLclampf depth);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCLEARSTENCIL                          ) (GLint s);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCOLORMASK                             ) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCOMPILESHADER                         ) (GLuint shader);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCOMPRESSEDTEXIMAGE2D                  ) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCOMPRESSEDTEXSUBIMAGE2D               ) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCOPYTEXIMAGE2D                        ) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCOPYTEXSUBIMAGE2D                     ) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef GLuint          (GL_APIENTRY *RS_ES2_PFNGLCREATEPROGRAM                         ) (void);
typedef GLuint          (GL_APIENTRY *RS_ES2_PFNGLCREATESHADER                          ) (GLenum type);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLCULLFACE                              ) (GLenum mode);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDELETEBUFFERS                         ) (GLsizei n, const GLuint* buffers);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDELETEFRAMEBUFFERS                    ) (GLsizei n, const GLuint* framebuffers);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDELETEPROGRAM                         ) (GLuint program);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDELETERENDERBUFFERS                   ) (GLsizei n, const GLuint* renderbuffers);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDELETESHADER                          ) (GLuint shader);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDELETETEXTURES                        ) (GLsizei n, const GLuint* textures);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDEPTHFUNC                             ) (GLenum func);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDEPTHMASK                             ) (GLboolean flag);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDEPTHRANGEF                           ) (GLclampf zNear, GLclampf zFar);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDETACHSHADER                          ) (GLuint program, GLuint shader);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDISABLE                               ) (GLenum cap);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDISABLEVERTEXATTRIBARRAY              ) (GLuint index);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDRAWARRAYS                            ) (GLenum mode, GLint first, GLsizei count);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLDRAWELEMENTS                          ) (GLenum mode, GLsizei count, GLenum type, const void* indices);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLENABLE                                ) (GLenum cap);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLENABLEVERTEXATTRIBARRAY               ) (GLuint index);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLFINISH                                ) (void);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLFLUSH                                 ) (void);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLFRAMEBUFFERRENDERBUFFER               ) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLFRAMEBUFFERTEXTURE2D                  ) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLFRONTFACE                             ) (GLenum mode);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGENBUFFERS                            ) (GLsizei n, GLuint* buffers);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGENERATEMIPMAP                        ) (GLenum target);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGENFRAMEBUFFERS                       ) (GLsizei n, GLuint* framebuffers);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGENRENDERBUFFERS                      ) (GLsizei n, GLuint* renderbuffers);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGENTEXTURES                               ) (GLsizei n, GLuint* textures);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETACTIVEATTRIB                       ) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETACTIVEUNIFORM                      ) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETATTACHEDSHADERS                    ) (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
typedef int             (GL_APIENTRY *RS_ES2_PFNGLGETATTRIBLOCATION                     ) (GLuint program, const char* name);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETBOOLEANV                           ) (GLenum pname, GLboolean* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETBUFFERPARAMETERIV                  ) (GLenum target, GLenum pname, GLint* params);
typedef GLenum          (GL_APIENTRY *RS_ES2_PFNGLGETERROR                              ) (void);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETFLOATV                             ) (GLenum pname, GLfloat* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV   ) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETINTEGERV                           ) (GLenum pname, GLint* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETPROGRAMIV                          ) (GLuint program, GLenum pname, GLint* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETPROGRAMINFOLOG                     ) (GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETRENDERBUFFERPARAMETERIV            ) (GLenum target, GLenum pname, GLint* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETSHADERIV                           ) (GLuint shader, GLenum pname, GLint* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETSHADERINFOLOG                      ) (GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETSHADERPRECISIONFORMAT              ) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETSHADERSOURCE                       ) (GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
typedef const GLubyte*  (GL_APIENTRY *RS_ES2_PFNGLGETSTRING                             ) (GLenum name);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETTEXPARAMETERFV                     ) (GLenum target, GLenum pname, GLfloat* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETTEXPARAMETERIV                     ) (GLenum target, GLenum pname, GLint* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETUNIFORMFV                          ) (GLuint program, GLint location, GLfloat* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETUNIFORMIV                          ) (GLuint program, GLint location, GLint* params);
typedef int             (GL_APIENTRY *RS_ES2_PFNGLGETUNIFORMLOCATION                    ) (GLuint program, const char* name);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETVERTEXATTRIBFV                     ) (GLuint index, GLenum pname, GLfloat* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETVERTEXATTRIBIV                     ) (GLuint index, GLenum pname, GLint* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLGETVERTEXATTRIBPOINTERV               ) (GLuint index, GLenum pname, void** pointer);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLHINT                                  ) (GLenum target, GLenum mode);
typedef GLboolean       (GL_APIENTRY *RS_ES2_PFNGLISBUFFER                              ) (GLuint buffer);
typedef GLboolean       (GL_APIENTRY *RS_ES2_PFNGLISENABLED                             ) (GLenum cap);
typedef GLboolean       (GL_APIENTRY *RS_ES2_PFNGLISFRAMEBUFFER                         ) (GLuint framebuffer);
typedef GLboolean       (GL_APIENTRY *RS_ES2_PFNGLISPROGRAM                             ) (GLuint program);
typedef GLboolean       (GL_APIENTRY *RS_ES2_PFNGLISRENDERBUFFER                        ) (GLuint renderbuffer);
typedef GLboolean       (GL_APIENTRY *RS_ES2_PFNGLISSHADER                              ) (GLuint shader);
typedef GLboolean       (GL_APIENTRY *RS_ES2_PFNGLISTEXTURE                             ) (GLuint texture);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLLINEWIDTH                             ) (GLfloat width);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLLINKPROGRAM                           ) (GLuint program);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLPIXELSTOREI                           ) (GLenum pname, GLint param);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLPOLYGONOFFSET                         ) (GLfloat factor, GLfloat units);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLREADPIXELS                            ) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLRELEASESHADERCOMPILER                 ) (void);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLRENDERBUFFERSTORAGE                   ) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSAMPLECOVERAGE                        ) (GLclampf value, GLboolean invert);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSCISSOR                               ) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSHADERBINARY                          ) (GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSHADERSOURCE                          ) (GLuint shader, GLsizei count, const char** string, const GLint* length);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSTENCILFUNC                           ) (GLenum func, GLint ref, GLuint mask);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSTENCILFUNCSEPARATE                   ) (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSTENCILMASK                           ) (GLuint mask);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSTENCILMASKSEPARATE                   ) (GLenum face, GLuint mask);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSTENCILOP                             ) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLSTENCILOPSEPARATE                     ) (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLTEXIMAGE2D                            ) (GLenum target, GLint level, GLint internalformat,  GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLTEXPARAMETERF                         ) (GLenum target, GLenum pname, GLfloat param);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLTEXPARAMETERFV                        ) (GLenum target, GLenum pname, const GLfloat* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLTEXPARAMETERI                         ) (GLenum target, GLenum pname, GLint param);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLTEXPARAMETERIV                        ) (GLenum target, GLenum pname, const GLint* params);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLTEXSUBIMAGE2D                         ) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM1F                             ) (GLint location, GLfloat x);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM1FV                            ) (GLint location, GLsizei count, const GLfloat* v);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM1I                             ) (GLint location, GLint x);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM1IV                            ) (GLint location, GLsizei count, const GLint* v);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM2F                             ) (GLint location, GLfloat x, GLfloat y);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM2FV                            ) (GLint location, GLsizei count, const GLfloat* v);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM2I                             ) (GLint location, GLint x, GLint y);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM2IV                            ) (GLint location, GLsizei count, const GLint* v);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM3F                             ) (GLint location, GLfloat x, GLfloat y, GLfloat z);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM3FV                            ) (GLint location, GLsizei count, const GLfloat* v);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM3I                             ) (GLint location, GLint x, GLint y, GLint z);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM3IV                            ) (GLint location, GLsizei count, const GLint* v);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM4F                             ) (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM4FV                            ) (GLint location, GLsizei count, const GLfloat* v);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM4I                             ) (GLint location, GLint x, GLint y, GLint z, GLint w);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORM4IV                            ) (GLint location, GLsizei count, const GLint* v);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORMMATRIX2FV                      ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORMMATRIX3FV                      ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUNIFORMMATRIX4FV                      ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLUSEPROGRAM                            ) (GLuint program);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVALIDATEPROGRAM                       ) (GLuint program);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVERTEXATTRIB1F                        ) (GLuint indx, GLfloat x);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVERTEXATTRIB1FV                       ) (GLuint indx, const GLfloat* values);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVERTEXATTRIB2F                        ) (GLuint indx, GLfloat x, GLfloat y);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVERTEXATTRIB2FV                       ) (GLuint indx, const GLfloat* values);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVERTEXATTRIB3F                        ) (GLuint indx, GLfloat x, GLfloat y, GLfloat z);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVERTEXATTRIB3FV                       ) (GLuint indx, const GLfloat* values);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVERTEXATTRIB4F                        ) (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVERTEXATTRIB4FV                       ) (GLuint indx, const GLfloat* values);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVERTEXATTRIBPOINTER                   ) (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
typedef void            (GL_APIENTRY *RS_ES2_PFNGLVIEWPORT                              ) (GLint x, GLint y, GLsizei width, GLsizei height);

extern RS_ES2_PFNGLACTIVETEXTURE                         glActiveTexture                         ;
extern RS_ES2_PFNGLATTACHSHADER                          glAttachShader                          ;
extern RS_ES2_PFNGLBINDATTRIBLOCATION                    glBindAttribLocation                    ;
extern RS_ES2_PFNGLBINDBUFFER                            glBindBuffer                            ;
extern RS_ES2_PFNGLBINDFRAMEBUFFER                       glBindFramebuffer                       ;
extern RS_ES2_PFNGLBINDRENDERBUFFER                      glBindRenderbuffer                      ;
extern RS_ES2_PFNGLBINDTEXTURE                           glBindTexture                           ;
extern RS_ES2_PFNGLBLENDCOLOR                            glBlendColor                            ;
extern RS_ES2_PFNGLBLENDEQUATION                         glBlendEquation                         ;
extern RS_ES2_PFNGLBLENDEQUATIONSEPARATE                 glBlendEquationSeparate                 ;
extern RS_ES2_PFNGLBLENDFUNC                             glBlendFunc                             ;
extern RS_ES2_PFNGLBLENDFUNCSEPARATE                     glBlendFuncSeparate                     ;
extern RS_ES2_PFNGLBUFFERDATA                            glBufferData                            ;
extern RS_ES2_PFNGLBUFFERSUBDATA                         glBufferSubData                         ;
extern RS_ES2_PFNGLCHECKFRAMEBUFFERSTATUS                glCheckFramebufferStatus                ;
extern RS_ES2_PFNGLCLEAR                                 glClear                                 ;
extern RS_ES2_PFNGLCLEARCOLOR                            glClearColor                            ;
extern RS_ES2_PFNGLCLEARDEPTHF                           glClearDepthf                           ;
extern RS_ES2_PFNGLCLEARSTENCIL                          glClearStencil                          ;
extern RS_ES2_PFNGLCOLORMASK                             glColorMask                             ;
extern RS_ES2_PFNGLCOMPILESHADER                         glCompileShader                         ;
extern RS_ES2_PFNGLCOMPRESSEDTEXIMAGE2D                  glCompressedTexImage2D                  ;
extern RS_ES2_PFNGLCOMPRESSEDTEXSUBIMAGE2D               glCompressedTexSubImage2D               ;
extern RS_ES2_PFNGLCOPYTEXIMAGE2D                        glCopyTexImage2D                        ;
extern RS_ES2_PFNGLCOPYTEXSUBIMAGE2D                     glCopyTexSubImage2D                     ;
extern RS_ES2_PFNGLCREATEPROGRAM                         glCreateProgram                         ;
extern RS_ES2_PFNGLCREATESHADER                          glCreateShader                          ;
extern RS_ES2_PFNGLCULLFACE                              glCullFace                              ;
extern RS_ES2_PFNGLDELETEBUFFERS                         glDeleteBuffers                         ;
extern RS_ES2_PFNGLDELETEFRAMEBUFFERS                    glDeleteFramebuffers                    ;
extern RS_ES2_PFNGLDELETEPROGRAM                         glDeleteProgram                         ;
extern RS_ES2_PFNGLDELETERENDERBUFFERS                   glDeleteRenderbuffers                   ;
extern RS_ES2_PFNGLDELETESHADER                          glDeleteShader                          ;
extern RS_ES2_PFNGLDELETETEXTURES                        glDeleteTextures                        ;
extern RS_ES2_PFNGLDEPTHFUNC                             glDepthFunc                             ;
extern RS_ES2_PFNGLDEPTHMASK                             glDepthMask                             ;
extern RS_ES2_PFNGLDEPTHRANGEF                           glDepthRangef                           ;
extern RS_ES2_PFNGLDETACHSHADER                          glDetachShader                          ;
extern RS_ES2_PFNGLDISABLE                               glDisable                               ;
extern RS_ES2_PFNGLDISABLEVERTEXATTRIBARRAY              glDisableVertexAttribArray              ;
extern RS_ES2_PFNGLDRAWARRAYS                            glDrawArrays                            ;
extern RS_ES2_PFNGLDRAWELEMENTS                          glDrawElements                          ;
extern RS_ES2_PFNGLENABLE                                glEnable                                ;
extern RS_ES2_PFNGLENABLEVERTEXATTRIBARRAY               glEnableVertexAttribArray               ;
extern RS_ES2_PFNGLFINISH                                glFinish                                ;
extern RS_ES2_PFNGLFLUSH                                 glFlush                                 ;
extern RS_ES2_PFNGLFRAMEBUFFERRENDERBUFFER               glFramebufferRenderbuffer               ;
extern RS_ES2_PFNGLFRAMEBUFFERTEXTURE2D                  glFramebufferTexture2D                  ;
extern RS_ES2_PFNGLFRONTFACE                             glFrontFace                             ;
extern RS_ES2_PFNGLGENBUFFERS                            glGenBuffers                            ;
extern RS_ES2_PFNGLGENERATEMIPMAP                        glGenerateMipmap                        ;
extern RS_ES2_PFNGLGENFRAMEBUFFERS                       glGenFramebuffers                       ;
extern RS_ES2_PFNGLGENRENDERBUFFERS                      glGenRenderbuffers                      ;
extern RS_ES2_PFNGLGENTEXTURES                           glGenTextures                           ;
extern RS_ES2_PFNGLGETACTIVEATTRIB                       glGetActiveAttrib                       ;
extern RS_ES2_PFNGLGETACTIVEUNIFORM                      glGetActiveUniform                      ;
extern RS_ES2_PFNGLGETATTACHEDSHADERS                    glGetAttachedShaders                    ;
extern RS_ES2_PFNGLGETATTRIBLOCATION                     glGetAttribLocation                     ;
extern RS_ES2_PFNGLGETBOOLEANV                           glGetBooleanv                           ;
extern RS_ES2_PFNGLGETBUFFERPARAMETERIV                  glGetBufferParameteriv                  ;
extern RS_ES2_PFNGLGETERROR                              glGetError                              ;
extern RS_ES2_PFNGLGETFLOATV                             glGetFloatv                             ;
extern RS_ES2_PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV   glGetFramebufferAttachmentParameteriv   ;
extern RS_ES2_PFNGLGETINTEGERV                           glGetIntegerv                           ;
extern RS_ES2_PFNGLGETPROGRAMIV                          glGetProgramiv                          ;
extern RS_ES2_PFNGLGETPROGRAMINFOLOG                     glGetProgramInfoLog                     ;
extern RS_ES2_PFNGLGETRENDERBUFFERPARAMETERIV            glGetRenderbufferParameteriv            ;
extern RS_ES2_PFNGLGETSHADERIV                           glGetShaderiv                           ;
extern RS_ES2_PFNGLGETSHADERINFOLOG                      glGetShaderInfoLog                      ;
extern RS_ES2_PFNGLGETSHADERPRECISIONFORMAT              glGetShaderPrecisionFormat              ;
extern RS_ES2_PFNGLGETSHADERSOURCE                       glGetShaderSource                       ;
extern RS_ES2_PFNGLGETSTRING                             glGetString                             ;
extern RS_ES2_PFNGLGETTEXPARAMETERFV                     glGetTexParameterfv                     ;
extern RS_ES2_PFNGLGETTEXPARAMETERIV                     glGetTexParameteriv                     ;
extern RS_ES2_PFNGLGETUNIFORMFV                          glGetUniformfv                          ;
extern RS_ES2_PFNGLGETUNIFORMIV                          glGetUniformiv                          ;
extern RS_ES2_PFNGLGETUNIFORMLOCATION                    glGetUniformLocation                    ;
extern RS_ES2_PFNGLGETVERTEXATTRIBFV                     glGetVertexAttribfv                     ;
extern RS_ES2_PFNGLGETVERTEXATTRIBIV                     glGetVertexAttribiv                     ;
extern RS_ES2_PFNGLGETVERTEXATTRIBPOINTERV               glGetVertexAttribPointerv               ;
extern RS_ES2_PFNGLHINT                                  glHint                                  ;
extern RS_ES2_PFNGLISBUFFER                              glIsBuffer                              ;
extern RS_ES2_PFNGLISENABLED                             glIsEnabled                             ;
extern RS_ES2_PFNGLISFRAMEBUFFER                         glIsFramebuffer                         ;
extern RS_ES2_PFNGLISPROGRAM                             glIsProgram                             ;
extern RS_ES2_PFNGLISRENDERBUFFER                        glIsRenderbuffer                        ;
extern RS_ES2_PFNGLISSHADER                              glIsShader                              ;
extern RS_ES2_PFNGLISTEXTURE                             glIsTexture                             ;
extern RS_ES2_PFNGLLINEWIDTH                             glLineWidth                             ;
extern RS_ES2_PFNGLLINKPROGRAM                           glLinkProgram                           ;
extern RS_ES2_PFNGLPIXELSTOREI                           glPixelStorei                           ;
extern RS_ES2_PFNGLPOLYGONOFFSET                         glPolygonOffset                         ;
extern RS_ES2_PFNGLREADPIXELS                            glReadPixels                            ;
extern RS_ES2_PFNGLRELEASESHADERCOMPILER                 glReleaseShaderCompiler                 ;
extern RS_ES2_PFNGLRENDERBUFFERSTORAGE                   glRenderbufferStorage                   ;
extern RS_ES2_PFNGLSAMPLECOVERAGE                        glSampleCoverage                        ;
extern RS_ES2_PFNGLSCISSOR                               glScissor                               ;
extern RS_ES2_PFNGLSHADERBINARY                          glShaderBinary                          ;
extern RS_ES2_PFNGLSHADERSOURCE                          glShaderSource                          ;
extern RS_ES2_PFNGLSTENCILFUNC                           glStencilFunc                           ;
extern RS_ES2_PFNGLSTENCILFUNCSEPARATE                   glStencilFuncSeparate                   ;
extern RS_ES2_PFNGLSTENCILMASK                           glStencilMask                           ;
extern RS_ES2_PFNGLSTENCILMASKSEPARATE                   glStencilMaskSeparate                   ;
extern RS_ES2_PFNGLSTENCILOP                             glStencilOp                             ;
extern RS_ES2_PFNGLSTENCILOPSEPARATE                     glStencilOpSeparate                     ;
extern RS_ES2_PFNGLTEXIMAGE2D                            glTexImage2D                            ;
extern RS_ES2_PFNGLTEXPARAMETERF                         glTexParameterf                         ;
extern RS_ES2_PFNGLTEXPARAMETERFV                        glTexParameterfv                        ;
extern RS_ES2_PFNGLTEXPARAMETERI                         glTexParameteri                         ;
extern RS_ES2_PFNGLTEXPARAMETERIV                        glTexParameteriv                        ;
extern RS_ES2_PFNGLTEXSUBIMAGE2D                         glTexSubImage2D                         ;
extern RS_ES2_PFNGLUNIFORM1F                             glUniform1f                             ;
extern RS_ES2_PFNGLUNIFORM1FV                            glUniform1fv                            ;
extern RS_ES2_PFNGLUNIFORM1I                             glUniform1i                             ;
extern RS_ES2_PFNGLUNIFORM1IV                            glUniform1iv                            ;
extern RS_ES2_PFNGLUNIFORM2F                             glUniform2f                             ;
extern RS_ES2_PFNGLUNIFORM2FV                            glUniform2fv                            ;
extern RS_ES2_PFNGLUNIFORM2I                             glUniform2i                             ;
extern RS_ES2_PFNGLUNIFORM2IV                            glUniform2iv                            ;
extern RS_ES2_PFNGLUNIFORM3F                             glUniform3f                             ;
extern RS_ES2_PFNGLUNIFORM3FV                            glUniform3fv                            ;
extern RS_ES2_PFNGLUNIFORM3I                             glUniform3i                             ;
extern RS_ES2_PFNGLUNIFORM3IV                            glUniform3iv                            ;
extern RS_ES2_PFNGLUNIFORM4F                             glUniform4f                             ;
extern RS_ES2_PFNGLUNIFORM4FV                            glUniform4fv                            ;
extern RS_ES2_PFNGLUNIFORM4I                             glUniform4i                             ;
extern RS_ES2_PFNGLUNIFORM4IV                            glUniform4iv                            ;
extern RS_ES2_PFNGLUNIFORMMATRIX2FV                      glUniformMatrix2fv                      ;
extern RS_ES2_PFNGLUNIFORMMATRIX3FV                      glUniformMatrix3fv                      ;
extern RS_ES2_PFNGLUNIFORMMATRIX4FV                      glUniformMatrix4fv                      ;
extern RS_ES2_PFNGLUSEPROGRAM                            glUseProgram                            ;
extern RS_ES2_PFNGLVALIDATEPROGRAM                       glValidateProgram                       ;
extern RS_ES2_PFNGLVERTEXATTRIB1F                        glVertexAttrib1f                        ;
extern RS_ES2_PFNGLVERTEXATTRIB1FV                       glVertexAttrib1fv                       ;
extern RS_ES2_PFNGLVERTEXATTRIB2F                        glVertexAttrib2f                        ;
extern RS_ES2_PFNGLVERTEXATTRIB2FV                       glVertexAttrib2fv                       ;
extern RS_ES2_PFNGLVERTEXATTRIB3F                        glVertexAttrib3f                        ;
extern RS_ES2_PFNGLVERTEXATTRIB3FV                       glVertexAttrib3fv                       ;
extern RS_ES2_PFNGLVERTEXATTRIB4F                        glVertexAttrib4f                        ;
extern RS_ES2_PFNGLVERTEXATTRIB4FV                       glVertexAttrib4fv                       ;
extern RS_ES2_PFNGLVERTEXATTRIBPOINTER                   glVertexAttribPointer                   ;
extern RS_ES2_PFNGLVIEWPORT                              glViewport                              ;

/*  GL_OES_get_program_binary */
extern PFNGLGETPROGRAMBINARYOESPROC                     glGetProgramBinaryOES               ;
extern PFNGLPROGRAMBINARYOESPROC                        glProgramBinaryOES                  ;

/*  GL_OES_mapbuffer  */
extern PFNGLMAPBUFFEROESPROC                            glMapBufferOES                      ;
extern PFNGLUNMAPBUFFEROESPROC                          glUnmapBufferOES                    ;
extern PFNGLGETBUFFERPOINTERVOESPROC                    glGetBufferPointervOES              ;

/* GL_EXT_discard_framebuffer */
extern PFNGLDISCARDFRAMEBUFFEREXTPROC                   glDiscardFramebufferEXT             ;

/* GL_EXT_multisampled_render_to_texture */
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC       glRenderbufferStorageMultisampleEXT     ;
extern PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC      glFramebufferTexture2DMultisampleEXT    ;


} }

#endif
