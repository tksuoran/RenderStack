#ifndef renderstack_toolkit__gles_ext_hpp
#define renderstack_toolkit__gles_ext_hpp

#include "platform.hpp"
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2)
#    include "GLES2/gl2.h"
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
#    include "GLES3/gl3.h"
#endif
#include "GLES2/gl2ext.h"

namespace gl
{
namespace detail
{

/*  GL_OES_get_program_binary */
typedef void(GL_APIENTRYP RS_ES2_PFNGLGETPROGRAMBINARYOESPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
typedef void(GL_APIENTRYP RS_ES2_PFNGLPROGRAMBINARYOESPROC)(GLuint program, GLenum binaryFormat, const GLvoid *binary, GLint length);

extern RS_ES2_PFNGLGETPROGRAMBINARYOESPROC glGetProgramBinaryOES;
extern RS_ES2_PFNGLPROGRAMBINARYOESPROC    glProgramBinaryOES;

/*  GL_OES_mapbuffer  */
typedef void *(GL_APIENTRYP RS_ES2_PFNGLMAPBUFFEROESPROC)(GLenum target, GLenum access);
typedef GLboolean(GL_APIENTRYP RS_ES2_PFNGLUNMAPBUFFEROESPROC)(GLenum target);
typedef void(GL_APIENTRYP RS_ES2_PFNGLGETBUFFERPOINTERVOESPROC)(GLenum target, GLenum pname, GLvoid **params);

extern RS_ES2_PFNGLMAPBUFFEROESPROC         glMapBufferOES;
extern RS_ES2_PFNGLUNMAPBUFFEROESPROC       glUnmapBufferOES;
extern RS_ES2_PFNGLGETBUFFERPOINTERVOESPROC glGetBufferPointervOES;

/*  GL_OES_texture_3D  */
typedef void(GL_APIENTRYP RS_ES2_PFNGLTEXIMAGE3DOESPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void(GL_APIENTRYP RS_ES2_PFNGLTEXSUBIMAGE3DOESPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
typedef void(GL_APIENTRYP RS_ES2_PFNGLCOPYTEXSUBIMAGE3DOESPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void(GL_APIENTRYP RS_ES2_PFNGLCOMPRESSEDTEXIMAGE3DOESPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void(GL_APIENTRYP RS_ES2_PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void(GL_APIENTRYP RS_ES2_PFNGLFRAMEBUFFERTEXTURE3DOES)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);

extern RS_ES2_PFNGLTEXIMAGE3DOESPROC              glTexImage3DOES;
extern RS_ES2_PFNGLTEXSUBIMAGE3DOESPROC           glTexSubImage3DOES;
extern RS_ES2_PFNGLCOPYTEXSUBIMAGE3DOESPROC       glCopyTexSubImage3DOES;
extern RS_ES2_PFNGLCOMPRESSEDTEXIMAGE3DOESPROC    glCompressedTexImage3DOES;
extern RS_ES2_PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC glCompressedTexSubImage3DOES;
extern RS_ES2_PFNGLFRAMEBUFFERTEXTURE3DOES        glFramebufferTexture3DOES;

/* GL_OES_vertex_array_object */
typedef void(GL_APIENTRYP RS_ES2_PFNGLBINDVERTEXARRAYOESPROC)(GLuint array);
typedef void(GL_APIENTRYP RS_ES2_PFNGLDELETEVERTEXARRAYSOESPROC)(GLsizei n, const GLuint *arrays);
typedef void(GL_APIENTRYP RS_ES2_PFNGLGENVERTEXARRAYSOESPROC)(GLsizei n, GLuint *arrays);
typedef GLboolean(GL_APIENTRYP RS_ES2_PFNGLISVERTEXARRAYOESPROC)(GLuint array);

extern RS_ES2_PFNGLBINDVERTEXARRAYOESPROC    glBindVertexArrayOES;
extern RS_ES2_PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
extern RS_ES2_PFNGLGENVERTEXARRAYSOESPROC    glGenVertexArraysOES;
extern RS_ES2_PFNGLISVERTEXARRAYOESPROC      glIsVertexArrayOES;

/* GL_EXT_discard_framebuffer */
typedef void(GL_APIENTRYP RS_ES2_PFNGLDISCARDFRAMEBUFFEREXTPROC)(GLenum target, GLsizei numAttachments, const GLenum *attachments);

extern RS_ES2_PFNGLDISCARDFRAMEBUFFEREXTPROC glDiscardFramebufferEXT;

/* GL_EXT_multisampled_render_to_texture */
typedef void(GL_APIENTRYP RS_ES2_PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void(GL_APIENTRYP RS_ES2_PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);

extern RS_ES2_PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC  glRenderbufferStorageMultisampleEXT;
extern RS_ES2_PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC glFramebufferTexture2DMultisampleEXT;

/* GL_EXT_occlusion_query_boolean */
typedef void(GL_APIENTRYP RS_ES2_PFNGLGENQUERIESEXTPROC)(GLsizei n, GLuint *ids);
typedef void(GL_APIENTRYP RS_ES2_PFNGLDELETEQUERIESEXTPROC)(GLsizei n, const GLuint *ids);
typedef GLboolean(GL_APIENTRYP RS_ES2_PFNGLISQUERYEXTPROC)(GLuint id);
typedef void(GL_APIENTRYP RS_ES2_PFNGLBEGINQUERYEXTPROC)(GLenum target, GLuint id);
typedef void(GL_APIENTRYP RS_ES2_PFNGLENDQUERYEXTPROC)(GLenum target);
typedef void(GL_APIENTRYP RS_ES2_PFNGLGETQUERYIVEXTPROC)(GLenum target, GLenum pname, GLint *params);
typedef void(GL_APIENTRYP RS_ES2_PFNGLGETQUERYOBJECTUIVEXTPROC)(GLuint id, GLenum pname, GLuint *params);

extern RS_ES2_PFNGLGENQUERIESEXTPROC        glGenQueriesEXT;
extern RS_ES2_PFNGLDELETEQUERIESEXTPROC     glDeleteQueriesEXT;
extern RS_ES2_PFNGLISQUERYEXTPROC           glIsQueryEXT;
extern RS_ES2_PFNGLBEGINQUERYEXTPROC        glBeginQueryEXT;
extern RS_ES2_PFNGLENDQUERYEXTPROC          glEndQueryEXT;
extern RS_ES2_PFNGLGETQUERYIVEXTPROC        glGetQueryivEXT;
extern RS_ES2_PFNGLGETQUERYOBJECTUIVEXTPROC glGetQueryObjectuivEXT;

/* GL_EXT_texture_storage */
typedef void(GL_APIENTRYP RS_ES2_PFNGLTEXSTORAGE1DEXTPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void(GL_APIENTRYP RS_ES2_PFNGLTEXSTORAGE2DEXTPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void(GL_APIENTRYP RS_ES2_PFNGLTEXSTORAGE3DEXTPROC)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void(GL_APIENTRYP RS_ES2_PFNGLTEXTURESTORAGE1DEXTPROC)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void(GL_APIENTRYP RS_ES2_PFNGLTEXTURESTORAGE2DEXTPROC)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void(GL_APIENTRYP RS_ES2_PFNGLTEXTURESTORAGE3DEXTPROC)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

extern RS_ES2_PFNGLTEXSTORAGE1DEXTPROC     glTexStorage1DEXT;
extern RS_ES2_PFNGLTEXSTORAGE2DEXTPROC     glTexStorage2DEXT;
extern RS_ES2_PFNGLTEXSTORAGE3DEXTPROC     glTexStorage3DEXT;
extern RS_ES2_PFNGLTEXTURESTORAGE1DEXTPROC glTextureStorage1DEXT;
extern RS_ES2_PFNGLTEXTURESTORAGE2DEXTPROC glTextureStorage2DEXT;
extern RS_ES2_PFNGLTEXTURESTORAGE3DEXTPROC glTextureStorage3DEXT;

} // namespace detail
} // namespace gl

#endif
