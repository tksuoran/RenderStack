#ifndef renderstack_toolkit__dynamic_opengl_es3_h
#define renderstack_toolkit__dynamic_opengl_es3_h

#include "platform.hpp"
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2)
# include "GLES2/gl2.h"
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
# include "GLES3/gl3.h"
#endif
#include "GLES2/gl2ext.h"
#include "GLES3/gl3ext.h"

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

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
typedef void           (GL_APIENTRY *RS_ES3_PFNGLREADBUFFER                     ) (GLenum mode);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLDRAWRANGEELEMENTS              ) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLTEXIMAGE3D                     ) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLTEXSUBIMAGE3D                  ) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLCOPYTEXSUBIMAGE3D              ) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLCOMPRESSEDTEXIMAGE3D           ) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLCOMPRESSEDTEXSUBIMAGE3D        ) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGENQUERIES                     ) (GLsizei n, GLuint* ids);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLDELETEQUERIES                  ) (GLsizei n, const GLuint* ids);
typedef GLboolean      (GL_APIENTRY *RS_ES3_PFNGLISQUERY                        ) (GLuint id);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLBEGINQUERY                     ) (GLenum target, GLuint id);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLENDQUERY                       ) (GLenum target);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETQUERYIV                     ) (GLenum target, GLenum pname, GLint* params);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETQUERYOBJECTUIV              ) (GLuint id, GLenum pname, GLuint* params);
typedef GLboolean      (GL_APIENTRY *RS_ES3_PFNGLUNMAPBUFFER                    ) (GLenum target);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETBUFFERPOINTERV              ) (GLenum target, GLenum pname, GLvoid** params);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLDRAWBUFFERS                    ) (GLsizei n, const GLenum* bufs);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORMMATRIX2X3FV             ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORMMATRIX3X2FV             ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORMMATRIX2X4FV             ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORMMATRIX4X2FV             ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORMMATRIX3X4FV             ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORMMATRIX4X3FV             ) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLBLITFRAMEBUFFER                ) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLRENDERBUFFERSTORAGEMULTISAMPLE ) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLFRAMEBUFFERTEXTURELAYER        ) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef GLvoid*        (GL_APIENTRY *RS_ES3_PFNGLMAPBUFFERRANGE                 ) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLFLUSHMAPPEDBUFFERRANGE         ) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLBINDVERTEXARRAY                ) (GLuint array);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLDELETEVERTEXARRAYS             ) (GLsizei n, const GLuint* arrays);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGENVERTEXARRAYS                ) (GLsizei n, GLuint* arrays);
typedef GLboolean      (GL_APIENTRY *RS_ES3_PFNGLISVERTEXARRAY                  ) (GLuint array);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETINTEGERI_V                  ) (GLenum target, GLuint index, GLint* data);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLBEGINTRANSFORMFEEDBACK         ) (GLenum primitiveMode);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLENDTRANSFORMFEEDBACK           ) (void);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLBINDBUFFERRANGE                ) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLBINDBUFFERBASE                 ) (GLenum target, GLuint index, GLuint buffer);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLTRANSFORMFEEDBACKVARYINGS      ) (GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETTRANSFORMFEEDBACKVARYING    ) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLVERTEXATTRIBIPOINTER           ) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETVERTEXATTRIBIIV             ) (GLuint index, GLenum pname, GLint* params);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETVERTEXATTRIBIUIV            ) (GLuint index, GLenum pname, GLuint* params);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLVERTEXATTRIBI4I                ) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLVERTEXATTRIBI4UI               ) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLVERTEXATTRIBI4IV               ) (GLuint index, const GLint* v);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLVERTEXATTRIBI4UIV              ) (GLuint index, const GLuint* v);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETUNIFORMUIV                  ) (GLuint program, GLint location, GLuint* params);
typedef GLint          (GL_APIENTRY *RS_ES3_PFNGLGETFRAGDATALOCATION            ) (GLuint program, const GLchar *name);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORM1UI                     ) (GLint location, GLuint v0);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORM2UI                     ) (GLint location, GLuint v0, GLuint v1);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORM3UI                     ) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORM4UI                     ) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORM1UIV                    ) (GLint location, GLsizei count, const GLuint* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORM2UIV                    ) (GLint location, GLsizei count, const GLuint* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORM3UIV                    ) (GLint location, GLsizei count, const GLuint* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORM4UIV                    ) (GLint location, GLsizei count, const GLuint* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLCLEARBUFFERIV                  ) (GLenum buffer, GLint drawbuffer, const GLint* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLCLEARBUFFERUIV                 ) (GLenum buffer, GLint drawbuffer, const GLuint* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLCLEARBUFFERFV                  ) (GLenum buffer, GLint drawbuffer, const GLfloat* value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLCLEARBUFFERFI                  ) (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef const GLubyte* (GL_APIENTRY *RS_ES3_PFNGLGETSTRINGI                     ) (GLenum name, GLuint index);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLCOPYBUFFERSUBDATA              ) (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETUNIFORMINDICES              ) (GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETACTIVEUNIFORMSIV            ) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
typedef GLuint         (GL_APIENTRY *RS_ES3_PFNGLGETUNIFORMBLOCKINDEX           ) (GLuint program, const GLchar* uniformBlockName);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETACTIVEUNIFORMBLOCKIV        ) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETACTIVEUNIFORMBLOCKNAME      ) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLUNIFORMBLOCKBINDING            ) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLDRAWARRAYSINSTANCED            ) (GLenum mode, GLint first, GLsizei count, GLsizei instanceCount);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLDRAWELEMENTSINSTANCED          ) (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instanceCount);
typedef GLsync         (GL_APIENTRY *RS_ES3_PFNGLFENCESYNC                      ) (GLenum condition, GLbitfield flags);
typedef GLboolean      (GL_APIENTRY *RS_ES3_PFNGLISSYNC                         ) (GLsync sync);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLDELETESYNC                     ) (GLsync sync);
typedef GLenum         (GL_APIENTRY *RS_ES3_PFNGLCLIENTWAITSYNC                 ) (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLWAITSYNC                       ) (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETINTEGER64V                  ) (GLenum pname, GLint64* params);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETSYNCIV                      ) (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETINTEGER64I_V                ) (GLenum target, GLuint index, GLint64* data);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETBUFFERPARAMETERI64V         ) (GLenum target, GLenum pname, GLint64* params);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGENSAMPLERS                    ) (GLsizei count, GLuint* samplers);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLDELETESAMPLERS                 ) (GLsizei count, const GLuint* samplers);
typedef GLboolean      (GL_APIENTRY *RS_ES3_PFNGLISSAMPLER                      ) (GLuint sampler);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLBINDSAMPLER                    ) (GLuint unit, GLuint sampler);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLSAMPLERPARAMETERI              ) (GLuint sampler, GLenum pname, GLint param);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLSAMPLERPARAMETERIV             ) (GLuint sampler, GLenum pname, const GLint* param);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLSAMPLERPARAMETERF              ) (GLuint sampler, GLenum pname, GLfloat param);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLSAMPLERPARAMETERFV             ) (GLuint sampler, GLenum pname, const GLfloat* param);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETSAMPLERPARAMETERIV          ) (GLuint sampler, GLenum pname, GLint* params);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETSAMPLERPARAMETERFV          ) (GLuint sampler, GLenum pname, GLfloat* params);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLVERTEXATTRIBDIVISOR            ) (GLuint index, GLuint divisor);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLBINDTRANSFORMFEEDBACK          ) (GLenum target, GLuint id);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLDELETETRANSFORMFEEDBACKS       ) (GLsizei n, const GLuint* ids);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGENTRANSFORMFEEDBACKS          ) (GLsizei n, GLuint* ids);
typedef GLboolean      (GL_APIENTRY *RS_ES3_PFNGLISTRANSFORMFEEDBACK            ) (GLuint id);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLPAUSETRANSFORMFEEDBACK         ) (void);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLRESUMETRANSFORMFEEDBACK        ) (void);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETPROGRAMBINARY               ) (GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLPROGRAMBINARY                  ) (GLuint program, GLenum binaryFormat, const GLvoid* binary, GLsizei length);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLPROGRAMPARAMETERI              ) (GLuint program, GLenum pname, GLint value);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLINVALIDATEFRAMEBUFFER          ) (GLenum target, GLsizei numAttachments, const GLenum* attachments);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLINVALIDATESUBFRAMEBUFFER       ) (GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLTEXSTORAGE2D                   ) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLTEXSTORAGE3D                   ) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void           (GL_APIENTRY *RS_ES3_PFNGLGETINTERNALFORMATIV            ) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);
#endif

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

// OpenGL ES 3.0

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
extern RS_ES3_PFNGLREADBUFFER                     glReadBuffer                    ;
extern RS_ES3_PFNGLDRAWRANGEELEMENTS              glDrawRangeElements             ;
extern RS_ES3_PFNGLTEXIMAGE3D                     glTexImage3D                    ;
extern RS_ES3_PFNGLTEXSUBIMAGE3D                  glTexSubImage3D                 ;
extern RS_ES3_PFNGLCOPYTEXSUBIMAGE3D              glCopyTexSubImage3D             ;
extern RS_ES3_PFNGLCOMPRESSEDTEXIMAGE3D           glCompressedTexImage3D          ;
extern RS_ES3_PFNGLCOMPRESSEDTEXSUBIMAGE3D        glCompressedTexSubImage3D       ;
extern RS_ES3_PFNGLGENQUERIES                     glGenQueries                    ;
extern RS_ES3_PFNGLDELETEQUERIES                  glDeleteQueries                 ;
extern RS_ES3_PFNGLISQUERY                        glIsQuery                       ;
extern RS_ES3_PFNGLBEGINQUERY                     glBeginQuery                    ;
extern RS_ES3_PFNGLENDQUERY                       glEndQuery                      ;
extern RS_ES3_PFNGLGETQUERYIV                     glGetQueryiv                    ;
extern RS_ES3_PFNGLGETQUERYOBJECTUIV              glGetQueryObjectuiv             ;
extern RS_ES3_PFNGLUNMAPBUFFER                    glUnmapBuffer                   ;
extern RS_ES3_PFNGLGETBUFFERPOINTERV              glGetBufferPointerv             ;
extern RS_ES3_PFNGLDRAWBUFFERS                    glDrawBuffers                   ;
extern RS_ES3_PFNGLUNIFORMMATRIX2X3FV             glUniformMatrix2x3fv            ;
extern RS_ES3_PFNGLUNIFORMMATRIX3X2FV             glUniformMatrix3x2fv            ;
extern RS_ES3_PFNGLUNIFORMMATRIX2X4FV             glUniformMatrix2x4fv            ;
extern RS_ES3_PFNGLUNIFORMMATRIX4X2FV             glUniformMatrix4x2fv            ;
extern RS_ES3_PFNGLUNIFORMMATRIX3X4FV             glUniformMatrix3x4fv            ;
extern RS_ES3_PFNGLUNIFORMMATRIX4X3FV             glUniformMatrix4x3fv            ;
extern RS_ES3_PFNGLBLITFRAMEBUFFER                glBlitFramebuffer               ;
extern RS_ES3_PFNGLRENDERBUFFERSTORAGEMULTISAMPLE glRenderbufferStorageMultisample;
extern RS_ES3_PFNGLFRAMEBUFFERTEXTURELAYER        glFramebufferTextureLayer       ;
extern RS_ES3_PFNGLMAPBUFFERRANGE                 glMapBufferRange                ;
extern RS_ES3_PFNGLFLUSHMAPPEDBUFFERRANGE         glFlushMappedBufferRange        ;
extern RS_ES3_PFNGLBINDVERTEXARRAY                glBindVertexArray               ;
extern RS_ES3_PFNGLDELETEVERTEXARRAYS             glDeleteVertexArrays            ;
extern RS_ES3_PFNGLGENVERTEXARRAYS                glGenVertexArrays               ;
extern RS_ES3_PFNGLISVERTEXARRAY                  glIsVertexArray                 ;
extern RS_ES3_PFNGLGETINTEGERI_V                  glGetIntegeri_v                 ;
extern RS_ES3_PFNGLBEGINTRANSFORMFEEDBACK         glBeginTransformFeedback        ;
extern RS_ES3_PFNGLENDTRANSFORMFEEDBACK           glEndTransformFeedback          ;
extern RS_ES3_PFNGLBINDBUFFERRANGE                glBindBufferRange               ;
extern RS_ES3_PFNGLBINDBUFFERBASE                 glBindBufferBase                ;
extern RS_ES3_PFNGLTRANSFORMFEEDBACKVARYINGS      glTransformFeedbackVaryings     ;
extern RS_ES3_PFNGLGETTRANSFORMFEEDBACKVARYING    glGetTransformFeedbackVarying   ;
extern RS_ES3_PFNGLVERTEXATTRIBIPOINTER           glVertexAttribIPointer          ;
extern RS_ES3_PFNGLGETVERTEXATTRIBIIV             glGetVertexAttribIiv            ;
extern RS_ES3_PFNGLGETVERTEXATTRIBIUIV            glGetVertexAttribIuiv           ;
extern RS_ES3_PFNGLVERTEXATTRIBI4I                glVertexAttribI4i               ;
extern RS_ES3_PFNGLVERTEXATTRIBI4UI               glVertexAttribI4ui              ;
extern RS_ES3_PFNGLVERTEXATTRIBI4IV               glVertexAttribI4iv              ;
extern RS_ES3_PFNGLVERTEXATTRIBI4UIV              glVertexAttribI4uiv             ;
extern RS_ES3_PFNGLGETUNIFORMUIV                  glGetUniformuiv                 ;
extern RS_ES3_PFNGLGETFRAGDATALOCATION            glGetFragDataLocation           ;
extern RS_ES3_PFNGLUNIFORM1UI                     glUniform1ui                    ;
extern RS_ES3_PFNGLUNIFORM2UI                     glUniform2ui                    ;
extern RS_ES3_PFNGLUNIFORM3UI                     glUniform3ui                    ;
extern RS_ES3_PFNGLUNIFORM4UI                     glUniform4ui                    ;
extern RS_ES3_PFNGLUNIFORM1UIV                    glUniform1uiv                   ;
extern RS_ES3_PFNGLUNIFORM2UIV                    glUniform2uiv                   ;
extern RS_ES3_PFNGLUNIFORM3UIV                    glUniform3uiv                   ;
extern RS_ES3_PFNGLUNIFORM4UIV                    glUniform4uiv                   ;
extern RS_ES3_PFNGLCLEARBUFFERIV                  glClearBufferiv                 ;
extern RS_ES3_PFNGLCLEARBUFFERUIV                 glClearBufferuiv                ;
extern RS_ES3_PFNGLCLEARBUFFERFV                  glClearBufferfv                 ;
extern RS_ES3_PFNGLCLEARBUFFERFI                  glClearBufferfi                 ;
extern RS_ES3_PFNGLGETSTRINGI                     glGetStringi                    ;
extern RS_ES3_PFNGLCOPYBUFFERSUBDATA              glCopyBufferSubData             ;
extern RS_ES3_PFNGLGETUNIFORMINDICES              glGetUniformIndices             ;
extern RS_ES3_PFNGLGETACTIVEUNIFORMSIV            glGetActiveUniformsiv           ;
extern RS_ES3_PFNGLGETUNIFORMBLOCKINDEX           glGetUniformBlockIndex          ;
extern RS_ES3_PFNGLGETACTIVEUNIFORMBLOCKIV        glGetActiveUniformBlockiv       ;
extern RS_ES3_PFNGLGETACTIVEUNIFORMBLOCKNAME      glGetActiveUniformBlockName     ;
extern RS_ES3_PFNGLUNIFORMBLOCKBINDING            glUniformBlockBinding           ;
extern RS_ES3_PFNGLDRAWARRAYSINSTANCED            glDrawArraysInstanced           ;
extern RS_ES3_PFNGLDRAWELEMENTSINSTANCED          glDrawElementsInstanced         ;
extern RS_ES3_PFNGLFENCESYNC                      glFenceSync                     ;
extern RS_ES3_PFNGLISSYNC                         glIsSync                        ;
extern RS_ES3_PFNGLDELETESYNC                     glDeleteSync                    ;
extern RS_ES3_PFNGLCLIENTWAITSYNC                 glClientWaitSync                ;
extern RS_ES3_PFNGLWAITSYNC                       glWaitSync                      ;
extern RS_ES3_PFNGLGETINTEGER64V                  glGetInteger64v                 ;
extern RS_ES3_PFNGLGETSYNCIV                      glGetSynciv                     ;
extern RS_ES3_PFNGLGETINTEGER64I_V                glGetInteger64i_v               ;
extern RS_ES3_PFNGLGETBUFFERPARAMETERI64V         glGetBufferParameteri64v        ;
extern RS_ES3_PFNGLGENSAMPLERS                    glGenSamplers                   ;
extern RS_ES3_PFNGLDELETESAMPLERS                 glDeleteSamplers                ;
extern RS_ES3_PFNGLISSAMPLER                      glIsSampler                     ;
extern RS_ES3_PFNGLBINDSAMPLER                    glBindSampler                   ;
extern RS_ES3_PFNGLSAMPLERPARAMETERI              glSamplerParameteri             ;
extern RS_ES3_PFNGLSAMPLERPARAMETERIV             glSamplerParameteriv            ;
extern RS_ES3_PFNGLSAMPLERPARAMETERF              glSamplerParameterf             ;
extern RS_ES3_PFNGLSAMPLERPARAMETERFV             glSamplerParameterfv            ;
extern RS_ES3_PFNGLGETSAMPLERPARAMETERIV          glGetSamplerParameteriv         ;
extern RS_ES3_PFNGLGETSAMPLERPARAMETERFV          glGetSamplerParameterfv         ;
extern RS_ES3_PFNGLVERTEXATTRIBDIVISOR            glVertexAttribDivisor           ;
extern RS_ES3_PFNGLBINDTRANSFORMFEEDBACK          glBindTransformFeedback         ;
extern RS_ES3_PFNGLDELETETRANSFORMFEEDBACKS       glDeleteTransformFeedbacks      ;
extern RS_ES3_PFNGLGENTRANSFORMFEEDBACKS          glGenTransformFeedbacks         ;
extern RS_ES3_PFNGLISTRANSFORMFEEDBACK            glIsTransformFeedback           ;
extern RS_ES3_PFNGLPAUSETRANSFORMFEEDBACK         glPauseTransformFeedback        ;
extern RS_ES3_PFNGLRESUMETRANSFORMFEEDBACK        glResumeTransformFeedback       ;
extern RS_ES3_PFNGLGETPROGRAMBINARY               glGetProgramBinary              ;
extern RS_ES3_PFNGLPROGRAMBINARY                  glProgramBinary                 ;
extern RS_ES3_PFNGLPROGRAMPARAMETERI              glProgramParameteri             ;
extern RS_ES3_PFNGLINVALIDATEFRAMEBUFFER          glInvalidateFramebuffer         ;
extern RS_ES3_PFNGLINVALIDATESUBFRAMEBUFFER       glInvalidateSubFramebuffer      ;
extern RS_ES3_PFNGLTEXSTORAGE2D                   glTexStorage2D                  ;
extern RS_ES3_PFNGLTEXSTORAGE3D                   glTexStorage3D                  ;
extern RS_ES3_PFNGLGETINTERNALFORMATIV            glGetInternalformativ           ;
#endif

// OpenGL ES 2.0 Extensions
#if 1
/*  GL_OES_get_program_binary */
extern PFNGLGETPROGRAMBINARYOESPROC                     glGetProgramBinaryOES               ;
extern PFNGLPROGRAMBINARYOESPROC                        glProgramBinaryOES                  ;
//#define glGetProgramBinary glGetProgramBinaryOES
//#define glProgramBinary    glProgramBinaryOES

/*  GL_OES_mapbuffer  */
extern PFNGLMAPBUFFEROESPROC                            glMapBufferOES                      ;
extern PFNGLUNMAPBUFFEROESPROC                          glUnmapBufferOES                    ;
extern PFNGLGETBUFFERPOINTERVOESPROC                    glGetBufferPointervOES              ;
//#define glMapBuffer               glMapBufferOES
//#define glUnmapBuffer             glUnmapBufferOES
//#define glGetBufferPointerv       glGetBufferPointervOES

/*  GL_OES_texture_3D  */
extern PFNGLTEXIMAGE3DOESPROC                           glTexImage3DOES                     ;
extern PFNGLTEXSUBIMAGE3DOESPROC                        glTexSubImage3DOES                  ;
extern PFNGLCOPYTEXSUBIMAGE3DOESPROC                    glCopyTexSubImage3DOES              ;
extern PFNGLCOMPRESSEDTEXIMAGE3DOESPROC                 glCompressedTexImage3DOES           ;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC              glCompressedTexSubImage3DOES        ;
extern PFNGLFRAMEBUFFERTEXTURE3DOES                     glFramebufferTexture3DOES           ;
//#define glTexImage3D              glTexImage3DOES               
//#define glTexSubImage3D           glTexSubImage3DOES            
//#define glCopyTexSubImage3D       glCopyTexSubImage3DOES        
//#define glCompressedTexImage3D    glCompressedTexImage3DOES     
//#define glCompressedTexSubImage3D glCompressedTexSubImage3DOES  
//#define glFramebufferTexture3D    glFramebufferTexture3DOES     

/* GL_OES_vertex_array_object */
extern PFNGLBINDVERTEXARRAYOESPROC                      glBindVertexArrayOES                ;
extern PFNGLDELETEVERTEXARRAYSOESPROC                   glDeleteVertexArraysOES             ;
extern PFNGLGENVERTEXARRAYSOESPROC                      glGenVertexArraysOES                ;
extern PFNGLISVERTEXARRAYOESPROC                        glIsVertexArrayOES                  ;
//#define glBindVertexArray      glBindVertexArrayOES     
//#define glDeleteVertexArrays   glDeleteVertexArraysOES  
//#define glGenVertexArrays      glGenVertexArraysOES     
//#define glIsVertexArray        glIsVertexArrayOES       

/* GL_EXT_discard_framebuffer */
extern PFNGLDISCARDFRAMEBUFFEREXTPROC                   glDiscardFramebufferEXT             ;
//#define glDiscardFramebuffer glDiscardFramebufferEXT

/* GL_EXT_multisampled_render_to_texture */
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC       glRenderbufferStorageMultisampleEXT     ;
extern PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC      glFramebufferTexture2DMultisampleEXT    ;

/* GL_EXT_multi_draw_arrays  */
extern PFNGLMULTIDRAWARRAYSEXTPROC                      glMultiDrawArraysEXT                ;
extern PFNGLMULTIDRAWELEMENTSEXTPROC                    glMultiDrawElementsEXT              ;
//#define glMultiDrawArrays   glMultiDrawArraysEXT  
//#define glMultiDrawElements glMultiDrawElementsEXT

/* GL_EXT_occlusion_query_boolean */
extern PFNGLGENQUERIESEXTPROC                           glGenQueriesEXT                         ;
extern PFNGLDELETEQUERIESEXTPROC                        glDeleteQueriesEXT                      ;
extern PFNGLISQUERYEXTPROC                              glIsQueryEXT                            ;
extern PFNGLBEGINQUERYEXTPROC                           glBeginQueryEXT                         ;
extern PFNGLENDQUERYEXTPROC                             glEndQueryEXT                           ;
extern PFNGLGETQUERYIVEXTPROC                           glGetQueryivEXT                         ;
extern PFNGLGETQUERYOBJECTUIVEXTPROC                    glGetQueryObjectuivEXT                  ;

/* GL_EXT_separate_shader_objects */
extern PFNGLUSEPROGRAMSTAGESEXTPROC                     glUseProgramStagesEXT                   ;
extern PFNGLACTIVESHADERPROGRAMEXTPROC                  glActiveShaderProgramEXT                ;
extern PFNGLCREATESHADERPROGRAMVEXTPROC                 glCreateShaderProgramvEXT               ;
extern PFNGLBINDPROGRAMPIPELINEEXTPROC                  glBindProgramPipelineEXT                ;
extern PFNGLDELETEPROGRAMPIPELINESEXTPROC               glDeleteProgramPipelinesEXT             ;
extern PFNGLGENPROGRAMPIPELINESEXTPROC                  glGenProgramPipelinesEXT                ;
extern PFNGLISPROGRAMPIPELINEEXTPROC                    glIsProgramPipelineEXT                  ;
extern PFNGLPROGRAMPARAMETERIEXTPROC                    glProgramParameteriEXT                  ;
extern PFNGLGETPROGRAMPIPELINEIVEXTPROC                 glGetProgramPipelineivEXT               ;
extern PFNGLPROGRAMUNIFORM1IEXTPROC                     glProgramUniform1iEXT                   ;
extern PFNGLPROGRAMUNIFORM2IEXTPROC                     glProgramUniform2iEXT                   ;
extern PFNGLPROGRAMUNIFORM3IEXTPROC                     glProgramUniform3iEXT                   ;
extern PFNGLPROGRAMUNIFORM4IEXTPROC                     glProgramUniform4iEXT                   ;
extern PFNGLPROGRAMUNIFORM1FEXTPROC                     glProgramUniform1fEXT                   ;
extern PFNGLPROGRAMUNIFORM2FEXTPROC                     glProgramUniform2fEXT                   ;
extern PFNGLPROGRAMUNIFORM3FEXTPROC                     glProgramUniform3fEXT                   ;
extern PFNGLPROGRAMUNIFORM4FEXTPROC                     glProgramUniform4fEXT                   ;
extern PFNGLPROGRAMUNIFORM1IVEXTPROC                    glProgramUniform1ivEXT                  ;
extern PFNGLPROGRAMUNIFORM2IVEXTPROC                    glProgramUniform2ivEXT                  ;
extern PFNGLPROGRAMUNIFORM3IVEXTPROC                    glProgramUniform3ivEXT                  ;
extern PFNGLPROGRAMUNIFORM4IVEXTPROC                    glProgramUniform4ivEXT                  ;
extern PFNGLPROGRAMUNIFORM1FVEXTPROC                    glProgramUniform1fvEXT                  ;
extern PFNGLPROGRAMUNIFORM2FVEXTPROC                    glProgramUniform2fvEXT                  ;
extern PFNGLPROGRAMUNIFORM3FVEXTPROC                    glProgramUniform3fvEXT                  ;
extern PFNGLPROGRAMUNIFORM4FVEXTPROC                    glProgramUniform4fvEXT                  ;
extern PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC              glProgramUniformMatrix2fvEXT            ;
extern PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC              glProgramUniformMatrix3fvEXT            ;
extern PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC              glProgramUniformMatrix4fvEXT            ;
extern PFNGLVALIDATEPROGRAMPIPELINEEXTPROC              glValidateProgramPipelineEXT            ;
extern PFNGLGETPROGRAMPIPELINEINFOLOGEXTPROC            glGetProgramPipelineInfoLogEXT          ;

/* GL_EXT_texture_storage */
extern PFNGLTEXSTORAGE1DEXTPROC                         glTexStorage1DEXT                       ;
extern PFNGLTEXSTORAGE2DEXTPROC                         glTexStorage2DEXT                       ;
extern PFNGLTEXSTORAGE3DEXTPROC                         glTexStorage3DEXT                       ;
extern PFNGLTEXTURESTORAGE1DEXTPROC                     glTextureStorage1DEXT                   ;
extern PFNGLTEXTURESTORAGE2DEXTPROC                     glTextureStorage2DEXT                   ;
extern PFNGLTEXTURESTORAGE3DEXTPROC                     glTextureStorage3DEXT                   ;
#endif

} }

#endif
