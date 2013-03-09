#if defined(RENDERSTACK_USE_GLES2_OR_GLES3)

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/window.hpp"

#if defined(RENDERSTACK_USE_GLFW)
# include <GL/glfw3.h>
#else

# if defined(WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <tchar.h>
# endif // WIN32

# if defined(LINUX) || defined(ANDROID)

#  include <stdlib.h>
#  include <dlfcn.h>
# else
#  error "Bad platform"
# endif // LINUX || ANDROID_NDK

#endif

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

namespace gl { namespace detail {

   // OpenGL ES 2.0 function pointers
#if 1
   RS_ES2_PFNGLACTIVETEXTURE                         glActiveTexture                         = nullptr;
   RS_ES2_PFNGLATTACHSHADER                          glAttachShader                          = nullptr;
   RS_ES2_PFNGLBINDATTRIBLOCATION                    glBindAttribLocation                    = nullptr;
   RS_ES2_PFNGLBINDBUFFER                            glBindBuffer                            = nullptr;
   RS_ES2_PFNGLBINDFRAMEBUFFER                       glBindFramebuffer                       = nullptr;
   RS_ES2_PFNGLBINDRENDERBUFFER                      glBindRenderbuffer                      = nullptr;
   RS_ES2_PFNGLBINDTEXTURE                           glBindTexture                           = nullptr;
   RS_ES2_PFNGLBLENDCOLOR                            glBlendColor                            = nullptr;
   RS_ES2_PFNGLBLENDEQUATION                         glBlendEquation                         = nullptr;
   RS_ES2_PFNGLBLENDEQUATIONSEPARATE                 glBlendEquationSeparate                 = nullptr;
   RS_ES2_PFNGLBLENDFUNC                             glBlendFunc                             = nullptr;
   RS_ES2_PFNGLBLENDFUNCSEPARATE                     glBlendFuncSeparate                     = nullptr;
   RS_ES2_PFNGLBUFFERDATA                            glBufferData                            = nullptr;
   RS_ES2_PFNGLBUFFERSUBDATA                         glBufferSubData                         = nullptr;
   RS_ES2_PFNGLCHECKFRAMEBUFFERSTATUS                glCheckFramebufferStatus                = nullptr;
   RS_ES2_PFNGLCLEAR                                 glClear                                 = nullptr;
   RS_ES2_PFNGLCLEARCOLOR                            glClearColor                            = nullptr;
   RS_ES2_PFNGLCLEARDEPTHF                           glClearDepthf                           = nullptr;
   RS_ES2_PFNGLCLEARSTENCIL                          glClearStencil                          = nullptr;
   RS_ES2_PFNGLCOLORMASK                             glColorMask                             = nullptr;
   RS_ES2_PFNGLCOMPILESHADER                         glCompileShader                         = nullptr;
   RS_ES2_PFNGLCOMPRESSEDTEXIMAGE2D                  glCompressedTexImage2D                  = nullptr;
   RS_ES2_PFNGLCOMPRESSEDTEXSUBIMAGE2D               glCompressedTexSubImage2D               = nullptr;
   RS_ES2_PFNGLCOPYTEXIMAGE2D                        glCopyTexImage2D                        = nullptr;
   RS_ES2_PFNGLCOPYTEXSUBIMAGE2D                     glCopyTexSubImage2D                     = nullptr;
   RS_ES2_PFNGLCREATEPROGRAM                         glCreateProgram                         = nullptr;
   RS_ES2_PFNGLCREATESHADER                          glCreateShader                          = nullptr;
   RS_ES2_PFNGLCULLFACE                              glCullFace                              = nullptr;
   RS_ES2_PFNGLDELETEBUFFERS                         glDeleteBuffers                         = nullptr;
   RS_ES2_PFNGLDELETEFRAMEBUFFERS                    glDeleteFramebuffers                    = nullptr;
   RS_ES2_PFNGLDELETEPROGRAM                         glDeleteProgram                         = nullptr;
   RS_ES2_PFNGLDELETERENDERBUFFERS                   glDeleteRenderbuffers                   = nullptr;
   RS_ES2_PFNGLDELETESHADER                          glDeleteShader                          = nullptr;
   RS_ES2_PFNGLDELETETEXTURES                        glDeleteTextures                        = nullptr;
   RS_ES2_PFNGLDEPTHFUNC                             glDepthFunc                             = nullptr;
   RS_ES2_PFNGLDEPTHMASK                             glDepthMask                             = nullptr;
   RS_ES2_PFNGLDEPTHRANGEF                           glDepthRangef                           = nullptr;
   RS_ES2_PFNGLDETACHSHADER                          glDetachShader                          = nullptr;
   RS_ES2_PFNGLDISABLE                               glDisable                               = nullptr;
   RS_ES2_PFNGLDISABLEVERTEXATTRIBARRAY              glDisableVertexAttribArray              = nullptr;
   RS_ES2_PFNGLDRAWARRAYS                            glDrawArrays                            = nullptr;
   RS_ES2_PFNGLDRAWELEMENTS                          glDrawElements                          = nullptr;
   RS_ES2_PFNGLENABLE                                glEnable                                = nullptr;
   RS_ES2_PFNGLENABLEVERTEXATTRIBARRAY               glEnableVertexAttribArray               = nullptr;
   RS_ES2_PFNGLFINISH                                glFinish                                = nullptr;
   RS_ES2_PFNGLFLUSH                                 glFlush                                 = nullptr;
   RS_ES2_PFNGLFRAMEBUFFERRENDERBUFFER               glFramebufferRenderbuffer               = nullptr;
   RS_ES2_PFNGLFRAMEBUFFERTEXTURE2D                  glFramebufferTexture2D                  = nullptr;
   RS_ES2_PFNGLFRONTFACE                             glFrontFace                             = nullptr;
   RS_ES2_PFNGLGENBUFFERS                            glGenBuffers                            = nullptr;
   RS_ES2_PFNGLGENERATEMIPMAP                        glGenerateMipmap                        = nullptr;
   RS_ES2_PFNGLGENFRAMEBUFFERS                       glGenFramebuffers                       = nullptr;
   RS_ES2_PFNGLGENRENDERBUFFERS                      glGenRenderbuffers                      = nullptr;
   RS_ES2_PFNGLGENTEXTURES                           glGenTextures                           = nullptr;
   RS_ES2_PFNGLGETACTIVEATTRIB                       glGetActiveAttrib                       = nullptr;
   RS_ES2_PFNGLGETACTIVEUNIFORM                      glGetActiveUniform                      = nullptr;
   RS_ES2_PFNGLGETATTACHEDSHADERS                    glGetAttachedShaders                    = nullptr;
   RS_ES2_PFNGLGETATTRIBLOCATION                     glGetAttribLocation                     = nullptr;
   RS_ES2_PFNGLGETBOOLEANV                           glGetBooleanv                           = nullptr;
   RS_ES2_PFNGLGETBUFFERPARAMETERIV                  glGetBufferParameteriv                  = nullptr;
   RS_ES2_PFNGLGETERROR                              glGetError                              = nullptr;
   RS_ES2_PFNGLGETFLOATV                             glGetFloatv                             = nullptr;
   RS_ES2_PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV   glGetFramebufferAttachmentParameteriv   = nullptr;
   RS_ES2_PFNGLGETINTEGERV                           glGetIntegerv                           = nullptr;
   RS_ES2_PFNGLGETPROGRAMIV                          glGetProgramiv                          = nullptr;
   RS_ES2_PFNGLGETPROGRAMINFOLOG                     glGetProgramInfoLog                     = nullptr;
   RS_ES2_PFNGLGETRENDERBUFFERPARAMETERIV            glGetRenderbufferParameteriv            = nullptr;
   RS_ES2_PFNGLGETSHADERIV                           glGetShaderiv                           = nullptr;
   RS_ES2_PFNGLGETSHADERINFOLOG                      glGetShaderInfoLog                      = nullptr;
   RS_ES2_PFNGLGETSHADERPRECISIONFORMAT              glGetShaderPrecisionFormat              = nullptr;
   RS_ES2_PFNGLGETSHADERSOURCE                       glGetShaderSource                       = nullptr;
   RS_ES2_PFNGLGETSTRING                             glGetString                             = nullptr;
   RS_ES2_PFNGLGETTEXPARAMETERFV                     glGetTexParameterfv                     = nullptr;
   RS_ES2_PFNGLGETTEXPARAMETERIV                     glGetTexParameteriv                     = nullptr;
   RS_ES2_PFNGLGETUNIFORMFV                          glGetUniformfv                          = nullptr;
   RS_ES2_PFNGLGETUNIFORMIV                          glGetUniformiv                          = nullptr;
   RS_ES2_PFNGLGETUNIFORMLOCATION                    glGetUniformLocation                    = nullptr;
   RS_ES2_PFNGLGETVERTEXATTRIBFV                     glGetVertexAttribfv                     = nullptr;
   RS_ES2_PFNGLGETVERTEXATTRIBIV                     glGetVertexAttribiv                     = nullptr;
   RS_ES2_PFNGLGETVERTEXATTRIBPOINTERV               glGetVertexAttribPointerv               = nullptr;
   RS_ES2_PFNGLHINT                                  glHint                                  = nullptr;
   RS_ES2_PFNGLISBUFFER                              glIsBuffer                              = nullptr;
   RS_ES2_PFNGLISENABLED                             glIsEnabled                             = nullptr;
   RS_ES2_PFNGLISFRAMEBUFFER                         glIsFramebuffer                         = nullptr;
   RS_ES2_PFNGLISPROGRAM                             glIsProgram                             = nullptr;
   RS_ES2_PFNGLISRENDERBUFFER                        glIsRenderbuffer                        = nullptr;
   RS_ES2_PFNGLISSHADER                              glIsShader                              = nullptr;
   RS_ES2_PFNGLISTEXTURE                             glIsTexture                             = nullptr;
   RS_ES2_PFNGLLINEWIDTH                             glLineWidth                             = nullptr;
   RS_ES2_PFNGLLINKPROGRAM                           glLinkProgram                           = nullptr;
   RS_ES2_PFNGLPIXELSTOREI                           glPixelStorei                           = nullptr;
   RS_ES2_PFNGLPOLYGONOFFSET                         glPolygonOffset                         = nullptr;
   RS_ES2_PFNGLREADPIXELS                            glReadPixels                            = nullptr;
   RS_ES2_PFNGLRELEASESHADERCOMPILER                 glReleaseShaderCompiler                 = nullptr;
   RS_ES2_PFNGLRENDERBUFFERSTORAGE                   glRenderbufferStorage                   = nullptr;
   RS_ES2_PFNGLSAMPLECOVERAGE                        glSampleCoverage                        = nullptr;
   RS_ES2_PFNGLSCISSOR                               glScissor                               = nullptr;
   RS_ES2_PFNGLSHADERBINARY                          glShaderBinary                          = nullptr;
   RS_ES2_PFNGLSHADERSOURCE                          glShaderSource                          = nullptr;
   RS_ES2_PFNGLSTENCILFUNC                           glStencilFunc                           = nullptr;
   RS_ES2_PFNGLSTENCILFUNCSEPARATE                   glStencilFuncSeparate                   = nullptr;
   RS_ES2_PFNGLSTENCILMASK                           glStencilMask                           = nullptr;
   RS_ES2_PFNGLSTENCILMASKSEPARATE                   glStencilMaskSeparate                   = nullptr;
   RS_ES2_PFNGLSTENCILOP                             glStencilOp                             = nullptr;
   RS_ES2_PFNGLSTENCILOPSEPARATE                     glStencilOpSeparate                     = nullptr;
   RS_ES2_PFNGLTEXIMAGE2D                            glTexImage2D                            = nullptr;
   RS_ES2_PFNGLTEXPARAMETERF                         glTexParameterf                         = nullptr;
   RS_ES2_PFNGLTEXPARAMETERFV                        glTexParameterfv                        = nullptr;
   RS_ES2_PFNGLTEXPARAMETERI                         glTexParameteri                         = nullptr;
   RS_ES2_PFNGLTEXPARAMETERIV                        glTexParameteriv                        = nullptr;
   RS_ES2_PFNGLTEXSUBIMAGE2D                         glTexSubImage2D                         = nullptr;
   RS_ES2_PFNGLUNIFORM1F                             glUniform1f                             = nullptr;
   RS_ES2_PFNGLUNIFORM1FV                            glUniform1fv                            = nullptr;
   RS_ES2_PFNGLUNIFORM1I                             glUniform1i                             = nullptr;
   RS_ES2_PFNGLUNIFORM1IV                            glUniform1iv                            = nullptr;
   RS_ES2_PFNGLUNIFORM2F                             glUniform2f                             = nullptr;
   RS_ES2_PFNGLUNIFORM2FV                            glUniform2fv                            = nullptr;
   RS_ES2_PFNGLUNIFORM2I                             glUniform2i                             = nullptr;
   RS_ES2_PFNGLUNIFORM2IV                            glUniform2iv                            = nullptr;
   RS_ES2_PFNGLUNIFORM3F                             glUniform3f                             = nullptr;
   RS_ES2_PFNGLUNIFORM3FV                            glUniform3fv                            = nullptr;
   RS_ES2_PFNGLUNIFORM3I                             glUniform3i                             = nullptr;
   RS_ES2_PFNGLUNIFORM3IV                            glUniform3iv                            = nullptr;
   RS_ES2_PFNGLUNIFORM4F                             glUniform4f                             = nullptr;
   RS_ES2_PFNGLUNIFORM4FV                            glUniform4fv                            = nullptr;
   RS_ES2_PFNGLUNIFORM4I                             glUniform4i                             = nullptr;
   RS_ES2_PFNGLUNIFORM4IV                            glUniform4iv                            = nullptr;
   RS_ES2_PFNGLUNIFORMMATRIX2FV                      glUniformMatrix2fv                      = nullptr;
   RS_ES2_PFNGLUNIFORMMATRIX3FV                      glUniformMatrix3fv                      = nullptr;
   RS_ES2_PFNGLUNIFORMMATRIX4FV                      glUniformMatrix4fv                      = nullptr;
   RS_ES2_PFNGLUSEPROGRAM                            glUseProgram                            = nullptr;
   RS_ES2_PFNGLVALIDATEPROGRAM                       glValidateProgram                       = nullptr;
   RS_ES2_PFNGLVERTEXATTRIB1F                        glVertexAttrib1f                        = nullptr;
   RS_ES2_PFNGLVERTEXATTRIB1FV                       glVertexAttrib1fv                       = nullptr;
   RS_ES2_PFNGLVERTEXATTRIB2F                        glVertexAttrib2f                        = nullptr;
   RS_ES2_PFNGLVERTEXATTRIB2FV                       glVertexAttrib2fv                       = nullptr;
   RS_ES2_PFNGLVERTEXATTRIB3F                        glVertexAttrib3f                        = nullptr;
   RS_ES2_PFNGLVERTEXATTRIB3FV                       glVertexAttrib3fv                       = nullptr;
   RS_ES2_PFNGLVERTEXATTRIB4F                        glVertexAttrib4f                        = nullptr;
   RS_ES2_PFNGLVERTEXATTRIB4FV                       glVertexAttrib4fv                       = nullptr;
   RS_ES2_PFNGLVERTEXATTRIBPOINTER                   glVertexAttribPointer                   = nullptr;
   RS_ES2_PFNGLVIEWPORT                              glViewport                              = nullptr;
#endif

   /*  GL_OES_get_program_binary */
   PFNGLGETPROGRAMBINARYOESPROC                      glGetProgramBinaryOES               ;
   PFNGLPROGRAMBINARYOESPROC                         glProgramBinaryOES                  ;

   /*  GL_OES_mapbuffer  */
   PFNGLMAPBUFFEROESPROC                             glMapBufferOES                      ;
   PFNGLUNMAPBUFFEROESPROC                           glUnmapBufferOES                    ;
   PFNGLGETBUFFERPOINTERVOESPROC                     glGetBufferPointervOES              ;

   /* GL_EXT_discard_framebuffer */
   PFNGLDISCARDFRAMEBUFFEREXTPROC                    glDiscardFramebufferEXT             ;

   /* GL_EXT_multisampled_render_to_texture */
   PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC        glRenderbufferStorageMultisampleEXT     ;
   PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC       glFramebufferTexture2DMultisampleEXT    ;
   
   // OpenGL ES 3.0 function pointers
#if defined(RENDERSTACK_USE_GLES3)
   RS_ES3_PFNGLREADBUFFER                             glReadBuffer                     = nullptr; // From OpenGL 1.0

   RS_ES3_PFNGLDRAWRANGEELEMENTS                      glDrawRangeElements              = nullptr; // From OpenGL 1.2
   RS_ES3_PFNGLTEXIMAGE3D                             glTexImage3D                     = nullptr;
   RS_ES3_PFNGLTEXSUBIMAGE3D                          glTexSubImage3D                  = nullptr;
   RS_ES3_PFNGLCOPYTEXSUBIMAGE3D                      glCopyTexSubImage3D              = nullptr;

   RS_ES3_PFNGLCOMPRESSEDTEXIMAGE3D                   glCompressedTexImage3D           = nullptr; // From OpenGL 1.3
   RS_ES3_PFNGLCOMPRESSEDTEXSUBIMAGE3D                glCompressedTexSubImage3D        = nullptr;

   RS_ES3_PFNGLGENQUERIES                             glGenQueries                     = nullptr; // From OpenGL 1.5
   RS_ES3_PFNGLDELETEQUERIES                          glDeleteQueries                  = nullptr;
   RS_ES3_PFNGLISQUERY                                glIsQuery                        = nullptr;
   RS_ES3_PFNGLBEGINQUERY                             glBeginQuery                     = nullptr;
   RS_ES3_PFNGLENDQUERY                               glEndQuery                       = nullptr;
   RS_ES3_PFNGLGETQUERYIV                             glGetQueryiv                     = nullptr;
   RS_ES3_PFNGLGETQUERYOBJECTUIV                      glGetQueryObjectuiv              = nullptr;
   RS_ES3_PFNGLUNMAPBUFFER                            glUnmapBuffer                    = nullptr;
   RS_ES3_PFNGLGETBUFFERPOINTERV                      glGetBufferPointerv              = nullptr;

   RS_ES3_PFNGLDRAWBUFFERS                            glDrawBuffers                    = nullptr;  // From OpenGL 2.0

   RS_ES3_PFNGLUNIFORMMATRIX2X3FV                     glUniformMatrix2x3fv             = nullptr;  // From OpenGL 2.1
   RS_ES3_PFNGLUNIFORMMATRIX3X2FV                     glUniformMatrix3x2fv             = nullptr;
   RS_ES3_PFNGLUNIFORMMATRIX2X4FV                     glUniformMatrix2x4fv             = nullptr;
   RS_ES3_PFNGLUNIFORMMATRIX4X2FV                     glUniformMatrix4x2fv             = nullptr;
   RS_ES3_PFNGLUNIFORMMATRIX3X4FV                     glUniformMatrix3x4fv             = nullptr;
   RS_ES3_PFNGLUNIFORMMATRIX4X3FV                     glUniformMatrix4x3fv             = nullptr;

   RS_ES3_PFNGLBLITFRAMEBUFFER                        glBlitFramebuffer                = nullptr; // From OpenGL 3.0 / ARB_framebuffer_object
   RS_ES3_PFNGLRENDERBUFFERSTORAGEMULTISAMPLE         glRenderbufferStorageMultisample = nullptr;
   RS_ES3_PFNGLFRAMEBUFFERTEXTURELAYER                glFramebufferTextureLayer        = nullptr;

   RS_ES3_PFNGLMAPBUFFERRANGE                         glMapBufferRange                 = nullptr; // From OpenGL 3.0 / ARB_map_buffer_range
   RS_ES3_PFNGLFLUSHMAPPEDBUFFERRANGE                 glFlushMappedBufferRange         = nullptr;

   RS_ES3_PFNGLBINDVERTEXARRAY                        glBindVertexArray                = nullptr; // From OpenGL 3.0 / ARB_vertex_array_object
   RS_ES3_PFNGLDELETEVERTEXARRAYS                     glDeleteVertexArrays             = nullptr;
   RS_ES3_PFNGLGENVERTEXARRAYS                        glGenVertexArrays                = nullptr;
   RS_ES3_PFNGLISVERTEXARRAY                          glIsVertexArray                  = nullptr;

   RS_ES3_PFNGLGETINTEGERI_V                          glGetIntegeri_v                  = nullptr; // From OpenGL 3.0
   RS_ES3_PFNGLBEGINTRANSFORMFEEDBACK                 glBeginTransformFeedback         = nullptr;
   RS_ES3_PFNGLENDTRANSFORMFEEDBACK                   glEndTransformFeedback           = nullptr;
   RS_ES3_PFNGLBINDBUFFERRANGE                        glBindBufferRange                = nullptr;
   RS_ES3_PFNGLBINDBUFFERBASE                         glBindBufferBase                 = nullptr;
   RS_ES3_PFNGLTRANSFORMFEEDBACKVARYINGS              glTransformFeedbackVaryings      = nullptr;
   RS_ES3_PFNGLGETTRANSFORMFEEDBACKVARYING            glGetTransformFeedbackVarying    = nullptr;
   RS_ES3_PFNGLVERTEXATTRIBIPOINTER                   glVertexAttribIPointer           = nullptr;
   RS_ES3_PFNGLGETVERTEXATTRIBIIV                     glGetVertexAttribIiv             = nullptr;
   RS_ES3_PFNGLGETVERTEXATTRIBIUIV                    glGetVertexAttribIuiv            = nullptr;
   RS_ES3_PFNGLVERTEXATTRIBI4I                        glVertexAttribI4i                = nullptr;
   RS_ES3_PFNGLVERTEXATTRIBI4UI                       glVertexAttribI4ui               = nullptr;
   RS_ES3_PFNGLVERTEXATTRIBI4IV                       glVertexAttribI4iv               = nullptr;
   RS_ES3_PFNGLVERTEXATTRIBI4UIV                      glVertexAttribI4uiv              = nullptr;
   RS_ES3_PFNGLGETUNIFORMUIV                          glGetUniformuiv                  = nullptr;
   RS_ES3_PFNGLGETFRAGDATALOCATION                    glGetFragDataLocation            = nullptr;
   RS_ES3_PFNGLUNIFORM1UI                             glUniform1ui                     = nullptr;
   RS_ES3_PFNGLUNIFORM2UI                             glUniform2ui                     = nullptr;
   RS_ES3_PFNGLUNIFORM3UI                             glUniform3ui                     = nullptr;
   RS_ES3_PFNGLUNIFORM4UI                             glUniform4ui                     = nullptr;
   RS_ES3_PFNGLUNIFORM1UIV                            glUniform1uiv                    = nullptr;
   RS_ES3_PFNGLUNIFORM2UIV                            glUniform2uiv                    = nullptr;
   RS_ES3_PFNGLUNIFORM3UIV                            glUniform3uiv                    = nullptr;
   RS_ES3_PFNGLUNIFORM4UIV                            glUniform4uiv                    = nullptr;
   RS_ES3_PFNGLCLEARBUFFERIV                          glClearBufferiv                  = nullptr;
   RS_ES3_PFNGLCLEARBUFFERUIV                         glClearBufferuiv                 = nullptr;
   RS_ES3_PFNGLCLEARBUFFERFV                          glClearBufferfv                  = nullptr;
   RS_ES3_PFNGLCLEARBUFFERFI                          glClearBufferfi                  = nullptr;
   RS_ES3_PFNGLGETSTRINGI                             glGetStringi                     = nullptr;

   RS_ES3_PFNGLCOPYBUFFERSUBDATA                      glCopyBufferSubData              = nullptr; // From OpenGL 3.1 / ARB_copy_buffer

   RS_ES3_PFNGLGETUNIFORMINDICES                      glGetUniformIndices              = nullptr; // From OpenGL 3.1 / ARB_uniform_buffer_object
   RS_ES3_PFNGLGETACTIVEUNIFORMSIV                    glGetActiveUniformsiv            = nullptr;
   RS_ES3_PFNGLGETUNIFORMBLOCKINDEX                   glGetUniformBlockIndex           = nullptr;
   RS_ES3_PFNGLGETACTIVEUNIFORMBLOCKIV                glGetActiveUniformBlockiv        = nullptr;
   RS_ES3_PFNGLGETACTIVEUNIFORMBLOCKNAME              glGetActiveUniformBlockName      = nullptr;
   RS_ES3_PFNGLUNIFORMBLOCKBINDING                    glUniformBlockBinding            = nullptr;

   RS_ES3_PFNGLDRAWARRAYSINSTANCED                    glDrawArraysInstanced            = nullptr; // From OpenGL 3.1 / GL_ARB_draw_instanced
   RS_ES3_PFNGLDRAWELEMENTSINSTANCED                  glDrawElementsInstanced          = nullptr; 

   RS_ES3_PFNGLFENCESYNC                              glFenceSync                      = nullptr; // From OpenGL 3.2 / ARB_sync
   RS_ES3_PFNGLISSYNC                                 glIsSync                         = nullptr;
   RS_ES3_PFNGLDELETESYNC                             glDeleteSync                     = nullptr;
   RS_ES3_PFNGLCLIENTWAITSYNC                         glClientWaitSync                 = nullptr;
   RS_ES3_PFNGLWAITSYNC                               glWaitSync                       = nullptr;
   RS_ES3_PFNGLGETINTEGER64V                          glGetInteger64v                  = nullptr;
   RS_ES3_PFNGLGETSYNCIV                              glGetSynciv                      = nullptr;

   RS_ES3_PFNGLGETINTEGER64I_V                        glGetInteger64i_v                = nullptr; // From OpenGL 3.2
   RS_ES3_PFNGLGETBUFFERPARAMETERI64V                 glGetBufferParameteri64v         = nullptr;

   RS_ES3_PFNGLGENSAMPLERS                            glGenSamplers                    = nullptr; // From OpenGL 3.2 / GL_ARB_sampler_objects
   RS_ES3_PFNGLDELETESAMPLERS                         glDeleteSamplers                 = nullptr;
   RS_ES3_PFNGLISSAMPLER                              glIsSampler                      = nullptr;
   RS_ES3_PFNGLBINDSAMPLER                            glBindSampler                    = nullptr;
   RS_ES3_PFNGLSAMPLERPARAMETERI                      glSamplerParameteri              = nullptr;
   RS_ES3_PFNGLSAMPLERPARAMETERIV                     glSamplerParameteriv             = nullptr;
   RS_ES3_PFNGLSAMPLERPARAMETERF                      glSamplerParameterf              = nullptr;
   RS_ES3_PFNGLSAMPLERPARAMETERFV                     glSamplerParameterfv             = nullptr;
   RS_ES3_PFNGLGETSAMPLERPARAMETERIV                  glGetSamplerParameteriv          = nullptr;
   RS_ES3_PFNGLGETSAMPLERPARAMETERFV                  glGetSamplerParameterfv          = nullptr;

   RS_ES3_PFNGLVERTEXATTRIBDIVISOR                    glVertexAttribDivisor            = nullptr;  // From OpenGL 3.3 / GL_ARB_instanced_arrays

   RS_ES3_PFNGLBINDTRANSFORMFEEDBACK                  glBindTransformFeedback          = nullptr; // GL_ARB_transform_feedback2 (minus draw_transform_feedback)
   RS_ES3_PFNGLDELETETRANSFORMFEEDBACKS               glDeleteTransformFeedbacks       = nullptr;
   RS_ES3_PFNGLGENTRANSFORMFEEDBACKS                  glGenTransformFeedbacks          = nullptr;
   RS_ES3_PFNGLISTRANSFORMFEEDBACK                    glIsTransformFeedback            = nullptr;
   RS_ES3_PFNGLPAUSETRANSFORMFEEDBACK                 glPauseTransformFeedback         = nullptr;
   RS_ES3_PFNGLRESUMETRANSFORMFEEDBACK                glResumeTransformFeedback        = nullptr;

   RS_ES3_PFNGLGETPROGRAMBINARY                       glGetProgramBinary               = nullptr; // GL_ARB_get_program_binary
   RS_ES3_PFNGLPROGRAMBINARY                          glProgramBinary                  = nullptr;
   RS_ES3_PFNGLPROGRAMPARAMETERI                      glProgramParameteri              = nullptr;

   RS_ES3_PFNGLINVALIDATEFRAMEBUFFER                  glInvalidateFramebuffer          = nullptr;  // In OpenGL 4.3 / GL_ARB_invalidate_subdata
   RS_ES3_PFNGLINVALIDATESUBFRAMEBUFFER               glInvalidateSubFramebuffer       = nullptr;

   RS_ES3_PFNGLTEXSTORAGE2D                           glTexStorage2D                   = nullptr;  // In ARB_texture_storage
   RS_ES3_PFNGLTEXSTORAGE3D                           glTexStorage3D                   = nullptr;

   RS_ES3_PFNGLGETINTERNALFORMATIV                    glGetInternalformativ            = nullptr;  // In GL_ARB_internalformat_query

   /*  GL_OES_get_program_binary */
   PFNGLGETPROGRAMBINARYOESPROC                      glGetProgramBinaryOES                   = nullptr;
   PFNGLPROGRAMBINARYOESPROC                         glProgramBinaryOES                      = nullptr;

   /*  GL_OES_mapbuffer  */
   PFNGLMAPBUFFEROESPROC                             glMapBufferOES                          = nullptr;
   PFNGLUNMAPBUFFEROESPROC                           glUnmapBufferOES                        = nullptr;
   PFNGLGETBUFFERPOINTERVOESPROC                     glGetBufferPointervOES                  = nullptr;

   /*  GL_OES_texture_3D  */
   PFNGLTEXIMAGE3DOESPROC                            glTexImage3DOES                         = nullptr;
   PFNGLTEXSUBIMAGE3DOESPROC                         glTexSubImage3DOES                      = nullptr;
   PFNGLCOPYTEXSUBIMAGE3DOESPROC                     glCopyTexSubImage3DOES                  = nullptr;
   PFNGLCOMPRESSEDTEXIMAGE3DOESPROC                  glCompressedTexImage3DOES               = nullptr;
   PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC               glCompressedTexSubImage3DOES            = nullptr;
   PFNGLFRAMEBUFFERTEXTURE3DOES                      glFramebufferTexture3DOES               = nullptr;

   /* GL_OES_vertex_array_object */
   PFNGLBINDVERTEXARRAYOESPROC                       glBindVertexArrayOES                    = nullptr;
   PFNGLDELETEVERTEXARRAYSOESPROC                    glDeleteVertexArraysOES                 = nullptr;
   PFNGLGENVERTEXARRAYSOESPROC                       glGenVertexArraysOES                    = nullptr;
   PFNGLISVERTEXARRAYOESPROC                         glIsVertexArrayOES                      = nullptr;

   /* GL_EXT_discard_framebuffer */
   PFNGLDISCARDFRAMEBUFFEREXTPROC                    glDiscardFramebufferEXT                 = nullptr;

   /* GL_EXT_multisampled_render_to_texture */
   PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC        glRenderbufferStorageMultisampleEXT     = nullptr;
   PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC       glFramebufferTexture2DMultisampleEXT    = nullptr; 

   /* GL_EXT_multi_draw_arrays  */
   PFNGLMULTIDRAWARRAYSEXTPROC                       glMultiDrawArraysEXT                    = nullptr;
   PFNGLMULTIDRAWELEMENTSEXTPROC                     glMultiDrawElementsEXT                  = nullptr;

   /* GL_EXT_occlusion_query_boolean */
   PFNGLGENQUERIESEXTPROC                            glGenQueriesEXT                         = nullptr;
   PFNGLDELETEQUERIESEXTPROC                         glDeleteQueriesEXT                      = nullptr;
   PFNGLISQUERYEXTPROC                               glIsQueryEXT                            = nullptr;
   PFNGLBEGINQUERYEXTPROC                            glBeginQueryEXT                         = nullptr;
   PFNGLENDQUERYEXTPROC                              glEndQueryEXT                           = nullptr;
   PFNGLGETQUERYIVEXTPROC                            glGetQueryivEXT                         = nullptr;
   PFNGLGETQUERYOBJECTUIVEXTPROC                     glGetQueryObjectuivEXT                  = nullptr;

   /* GL_EXT_separate_shader_objects */
   PFNGLUSEPROGRAMSTAGESEXTPROC                      glUseProgramStagesEXT                   = nullptr;
   PFNGLACTIVESHADERPROGRAMEXTPROC                   glActiveShaderProgramEXT                = nullptr;
   PFNGLCREATESHADERPROGRAMVEXTPROC                  glCreateShaderProgramvEXT               = nullptr;
   PFNGLBINDPROGRAMPIPELINEEXTPROC                   glBindProgramPipelineEXT                = nullptr;
   PFNGLDELETEPROGRAMPIPELINESEXTPROC                glDeleteProgramPipelinesEXT             = nullptr;
   PFNGLGENPROGRAMPIPELINESEXTPROC                   glGenProgramPipelinesEXT                = nullptr;
   PFNGLISPROGRAMPIPELINEEXTPROC                     glIsProgramPipelineEXT                  = nullptr;
   PFNGLPROGRAMPARAMETERIEXTPROC                     glProgramParameteriEXT                  = nullptr;
   PFNGLGETPROGRAMPIPELINEIVEXTPROC                  glGetProgramPipelineivEXT               = nullptr;
   PFNGLPROGRAMUNIFORM1IEXTPROC                      glProgramUniform1iEXT                   = nullptr;
   PFNGLPROGRAMUNIFORM2IEXTPROC                      glProgramUniform2iEXT                   = nullptr;
   PFNGLPROGRAMUNIFORM3IEXTPROC                      glProgramUniform3iEXT                   = nullptr;
   PFNGLPROGRAMUNIFORM4IEXTPROC                      glProgramUniform4iEXT                   = nullptr;
   PFNGLPROGRAMUNIFORM1FEXTPROC                      glProgramUniform1fEXT                   = nullptr;
   PFNGLPROGRAMUNIFORM2FEXTPROC                      glProgramUniform2fEXT                   = nullptr;
   PFNGLPROGRAMUNIFORM3FEXTPROC                      glProgramUniform3fEXT                   = nullptr;
   PFNGLPROGRAMUNIFORM4FEXTPROC                      glProgramUniform4fEXT                   = nullptr;
   PFNGLPROGRAMUNIFORM1IVEXTPROC                     glProgramUniform1ivEXT                  = nullptr;
   PFNGLPROGRAMUNIFORM2IVEXTPROC                     glProgramUniform2ivEXT                  = nullptr;
   PFNGLPROGRAMUNIFORM3IVEXTPROC                     glProgramUniform3ivEXT                  = nullptr;
   PFNGLPROGRAMUNIFORM4IVEXTPROC                     glProgramUniform4ivEXT                  = nullptr;
   PFNGLPROGRAMUNIFORM1FVEXTPROC                     glProgramUniform1fvEXT                  = nullptr;
   PFNGLPROGRAMUNIFORM2FVEXTPROC                     glProgramUniform2fvEXT                  = nullptr;
   PFNGLPROGRAMUNIFORM3FVEXTPROC                     glProgramUniform3fvEXT                  = nullptr;
   PFNGLPROGRAMUNIFORM4FVEXTPROC                     glProgramUniform4fvEXT                  = nullptr;
   PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC               glProgramUniformMatrix2fvEXT            = nullptr;
   PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC               glProgramUniformMatrix3fvEXT            = nullptr;
   PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC               glProgramUniformMatrix4fvEXT            = nullptr;
   PFNGLVALIDATEPROGRAMPIPELINEEXTPROC               glValidateProgramPipelineEXT            = nullptr;
   PFNGLGETPROGRAMPIPELINEINFOLOGEXTPROC             glGetProgramPipelineInfoLogEXT          = nullptr;

   /* GL_EXT_texture_storage */
   PFNGLTEXSTORAGE1DEXTPROC                          glTexStorage1DEXT                       = nullptr;
   PFNGLTEXSTORAGE2DEXTPROC                          glTexStorage2DEXT                       = nullptr;
   PFNGLTEXSTORAGE3DEXTPROC                          glTexStorage3DEXT                       = nullptr;
   PFNGLTEXTURESTORAGE1DEXTPROC                      glTextureStorage1DEXT                   = nullptr;
   PFNGLTEXTURESTORAGE2DEXTPROC                      glTextureStorage2DEXT                   = nullptr;
   PFNGLTEXTURESTORAGE3DEXTPROC                      glTextureStorage3DEXT                   = nullptr;
#endif

} }

#if defined(WIN32)
static void *get_sym(HMODULE es, const char *fn)
{
   return (void *)GetProcAddress(es, fn);
}
#endif

#if defined(LINUX) || defined(ANDROID)
static void *get_sym(void *es, const char *fn)
{
   return (void *)dlsym(es, fn);
}
#endif

namespace renderstack { namespace toolkit {

using namespace gl::detail;
using namespace std;

void window::get_extensions()
{
#if defined(WIN32)
   HMODULE es = LoadLibraryA("libGLESv2.dll");
#elif defined(LINUX) || defined(ANDROID)
   void *es = dlopen("libGLESv2.so", RTLD_NOW);
#else
# error "Unsupported platform"
#endif
   if (es == NULL)
      throw std::runtime_error("Could not open libGLESv2");

#if 1 // OpenGL ES 2.0
   gl::detail::glActiveTexture                           = (RS_ES2_PFNGLACTIVETEXTURE                         )::get_sym(es, "glActiveTexture");
   gl::detail::glAttachShader                            = (RS_ES2_PFNGLATTACHSHADER                          )::get_sym(es, "glAttachShader");
   gl::detail::glBindAttribLocation                      = (RS_ES2_PFNGLBINDATTRIBLOCATION                    )::get_sym(es, "glBindAttribLocation");
   gl::detail::glBindBuffer                              = (RS_ES2_PFNGLBINDBUFFER                            )::get_sym(es, "glBindBuffer");
   gl::detail::glBindFramebuffer                         = (RS_ES2_PFNGLBINDFRAMEBUFFER                       )::get_sym(es, "glBindFramebuffer");
   gl::detail::glBindRenderbuffer                        = (RS_ES2_PFNGLBINDRENDERBUFFER                      )::get_sym(es, "glBindRenderbuffer");
   gl::detail::glBindTexture                             = (RS_ES2_PFNGLBINDTEXTURE                           )::get_sym(es, "glBindTexture");
   gl::detail::glBlendColor                              = (RS_ES2_PFNGLBLENDCOLOR                            )::get_sym(es, "glBlendColor");
   gl::detail::glBlendEquation                           = (RS_ES2_PFNGLBLENDEQUATION                         )::get_sym(es, "glBlendEquation");
   gl::detail::glBlendEquationSeparate                   = (RS_ES2_PFNGLBLENDEQUATIONSEPARATE                 )::get_sym(es, "glBlendEquationSeparate");
   gl::detail::glBlendFunc                               = (RS_ES2_PFNGLBLENDFUNC                             )::get_sym(es, "glBlendFunc");
   gl::detail::glBlendFuncSeparate                       = (RS_ES2_PFNGLBLENDFUNCSEPARATE                     )::get_sym(es, "glBlendFuncSeparate");
   gl::detail::glBufferData                              = (RS_ES2_PFNGLBUFFERDATA                            )::get_sym(es, "glBufferData");
   gl::detail::glBufferSubData                           = (RS_ES2_PFNGLBUFFERSUBDATA                         )::get_sym(es, "glBufferSubData");
   gl::detail::glCheckFramebufferStatus                  = (RS_ES2_PFNGLCHECKFRAMEBUFFERSTATUS                )::get_sym(es, "glCheckFramebufferStatus");
   gl::detail::glClear                                   = (RS_ES2_PFNGLCLEAR                                 )::get_sym(es, "glClear");
   gl::detail::glClearColor                              = (RS_ES2_PFNGLCLEARCOLOR                            )::get_sym(es, "glClearColor");
   gl::detail::glClearDepthf                             = (RS_ES2_PFNGLCLEARDEPTHF                           )::get_sym(es, "glClearDepthf");
   gl::detail::glClearStencil                            = (RS_ES2_PFNGLCLEARSTENCIL                          )::get_sym(es, "glClearStencil");
   gl::detail::glColorMask                               = (RS_ES2_PFNGLCOLORMASK                             )::get_sym(es, "glColorMask");
   gl::detail::glCompileShader                           = (RS_ES2_PFNGLCOMPILESHADER                         )::get_sym(es, "glCompileShader");
   gl::detail::glCompressedTexImage2D                    = (RS_ES2_PFNGLCOMPRESSEDTEXIMAGE2D                  )::get_sym(es, "glCompressedTexImage2D");
   gl::detail::glCompressedTexSubImage2D                 = (RS_ES2_PFNGLCOMPRESSEDTEXSUBIMAGE2D               )::get_sym(es, "glCompressedTexSubImage2D");
   gl::detail::glCopyTexImage2D                          = (RS_ES2_PFNGLCOPYTEXIMAGE2D                        )::get_sym(es, "glCopyTexImage2D");
   gl::detail::glCopyTexSubImage2D                       = (RS_ES2_PFNGLCOPYTEXSUBIMAGE2D                     )::get_sym(es, "glCopyTexSubImage2D");
   gl::detail::glCreateProgram                           = (RS_ES2_PFNGLCREATEPROGRAM                         )::get_sym(es, "glCreateProgram");
   gl::detail::glCreateShader                            = (RS_ES2_PFNGLCREATESHADER                          )::get_sym(es, "glCreateShader");
   gl::detail::glCullFace                                = (RS_ES2_PFNGLCULLFACE                              )::get_sym(es, "glCullFace");
   gl::detail::glDeleteBuffers                           = (RS_ES2_PFNGLDELETEBUFFERS                         )::get_sym(es, "glDeleteBuffers");
   gl::detail::glDeleteFramebuffers                      = (RS_ES2_PFNGLDELETEFRAMEBUFFERS                    )::get_sym(es, "glDeleteFramebuffers");
   gl::detail::glDeleteProgram                           = (RS_ES2_PFNGLDELETEPROGRAM                         )::get_sym(es, "glDeleteProgram");
   gl::detail::glDeleteRenderbuffers                     = (RS_ES2_PFNGLDELETERENDERBUFFERS                   )::get_sym(es, "glDeleteRenderbuffers");
   gl::detail::glDeleteShader                            = (RS_ES2_PFNGLDELETESHADER                          )::get_sym(es, "glDeleteShader");
   gl::detail::glDeleteTextures                          = (RS_ES2_PFNGLDELETETEXTURES                        )::get_sym(es, "glDeleteTextures");
   gl::detail::glDepthFunc                               = (RS_ES2_PFNGLDEPTHFUNC                             )::get_sym(es, "glDepthFunc");
   gl::detail::glDepthMask                               = (RS_ES2_PFNGLDEPTHMASK                             )::get_sym(es, "glDepthMask");
   gl::detail::glDepthRangef                             = (RS_ES2_PFNGLDEPTHRANGEF                           )::get_sym(es, "glDepthRangef");
   gl::detail::glDetachShader                            = (RS_ES2_PFNGLDETACHSHADER                          )::get_sym(es, "glDetachShader");
   gl::detail::glDisable                                 = (RS_ES2_PFNGLDISABLE                               )::get_sym(es, "glDisable");
   gl::detail::glDisableVertexAttribArray                = (RS_ES2_PFNGLDISABLEVERTEXATTRIBARRAY              )::get_sym(es, "glDisableVertexAttribArray");
   gl::detail::glDrawArrays                              = (RS_ES2_PFNGLDRAWARRAYS                            )::get_sym(es, "glDrawArrays");
   gl::detail::glDrawElements                            = (RS_ES2_PFNGLDRAWELEMENTS                          )::get_sym(es, "glDrawElements");
   gl::detail::glEnable                                  = (RS_ES2_PFNGLENABLE                                )::get_sym(es, "glEnable");
   gl::detail::glEnableVertexAttribArray                 = (RS_ES2_PFNGLENABLEVERTEXATTRIBARRAY               )::get_sym(es, "glEnableVertexAttribArray");
   gl::detail::glFinish                                  = (RS_ES2_PFNGLFINISH                                )::get_sym(es, "glFinish");
   gl::detail::glFlush                                   = (RS_ES2_PFNGLFLUSH                                 )::get_sym(es, "glFlush");
   gl::detail::glFramebufferRenderbuffer                 = (RS_ES2_PFNGLFRAMEBUFFERRENDERBUFFER               )::get_sym(es, "glFramebufferRenderbuffer");
   gl::detail::glFramebufferTexture2D                    = (RS_ES2_PFNGLFRAMEBUFFERTEXTURE2D                  )::get_sym(es, "glFramebufferTexture2D");
   gl::detail::glFrontFace                               = (RS_ES2_PFNGLFRONTFACE                             )::get_sym(es, "glFrontFace");
   gl::detail::glGenBuffers                              = (RS_ES2_PFNGLGENBUFFERS                            )::get_sym(es, "glGenBuffers");
   gl::detail::glGenerateMipmap                          = (RS_ES2_PFNGLGENERATEMIPMAP                        )::get_sym(es, "glGenerateMipmap");
   gl::detail::glGenFramebuffers                         = (RS_ES2_PFNGLGENFRAMEBUFFERS                       )::get_sym(es, "glGenFramebuffers");
   gl::detail::glGenRenderbuffers                        = (RS_ES2_PFNGLGENRENDERBUFFERS                      )::get_sym(es, "glGenRenderbuffers");
   gl::detail::glGenTextures                             = (RS_ES2_PFNGLGENTEXTURES                           )::get_sym(es, "glGenTextures");
   gl::detail::glGetActiveAttrib                         = (RS_ES2_PFNGLGETACTIVEATTRIB                       )::get_sym(es, "glGetActiveAttrib");
   gl::detail::glGetActiveUniform                        = (RS_ES2_PFNGLGETACTIVEUNIFORM                      )::get_sym(es, "glGetActiveUniform");
   gl::detail::glGetAttachedShaders                      = (RS_ES2_PFNGLGETATTACHEDSHADERS                    )::get_sym(es, "glGetAttachedShaders");
   gl::detail::glGetAttribLocation                       = (RS_ES2_PFNGLGETATTRIBLOCATION                     )::get_sym(es, "glGetAttribLocation");
   gl::detail::glGetBooleanv                             = (RS_ES2_PFNGLGETBOOLEANV                           )::get_sym(es, "glGetBooleanv");
   gl::detail::glGetBufferParameteriv                    = (RS_ES2_PFNGLGETBUFFERPARAMETERIV                  )::get_sym(es, "glGetBufferParameteriv");
   gl::detail::glGetError                                = (RS_ES2_PFNGLGETERROR                              )::get_sym(es, "glGetError");
   gl::detail::glGetFloatv                               = (RS_ES2_PFNGLGETFLOATV                             )::get_sym(es, "glGetFloatv");
   gl::detail::glGetFramebufferAttachmentParameteriv     = (RS_ES2_PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV   )::get_sym(es, "glGetFramebufferAttachmentParameteriv");
   gl::detail::glGetIntegerv                             = (RS_ES2_PFNGLGETINTEGERV                           )::get_sym(es, "glGetIntegerv");
   gl::detail::glGetProgramiv                            = (RS_ES2_PFNGLGETPROGRAMIV                          )::get_sym(es, "glGetProgramiv");
   gl::detail::glGetProgramInfoLog                       = (RS_ES2_PFNGLGETPROGRAMINFOLOG                     )::get_sym(es, "glGetProgramInfoLog");
   gl::detail::glGetRenderbufferParameteriv              = (RS_ES2_PFNGLGETRENDERBUFFERPARAMETERIV            )::get_sym(es, "glGetRenderbufferParameteriv");
   gl::detail::glGetShaderiv                             = (RS_ES2_PFNGLGETSHADERIV                           )::get_sym(es, "glGetShaderiv");
   gl::detail::glGetShaderInfoLog                        = (RS_ES2_PFNGLGETSHADERINFOLOG                      )::get_sym(es, "glGetShaderInfoLog");
   gl::detail::glGetShaderPrecisionFormat                = (RS_ES2_PFNGLGETSHADERPRECISIONFORMAT              )::get_sym(es, "glGetShaderPrecisionFormat");
   gl::detail::glGetShaderSource                         = (RS_ES2_PFNGLGETSHADERSOURCE                       )::get_sym(es, "glGetShaderSource");
   gl::detail::glGetString                               = (RS_ES2_PFNGLGETSTRING                             )::get_sym(es, "glGetString");
   gl::detail::glGetTexParameterfv                       = (RS_ES2_PFNGLGETTEXPARAMETERFV                     )::get_sym(es, "glGetTexParameterfv");
   gl::detail::glGetTexParameteriv                       = (RS_ES2_PFNGLGETTEXPARAMETERIV                     )::get_sym(es, "glGetTexParameteriv");
   gl::detail::glGetUniformfv                            = (RS_ES2_PFNGLGETUNIFORMFV                          )::get_sym(es, "glGetUniformfv");
   gl::detail::glGetUniformiv                            = (RS_ES2_PFNGLGETUNIFORMIV                          )::get_sym(es, "glGetUniformiv");
   gl::detail::glGetUniformLocation                      = (RS_ES2_PFNGLGETUNIFORMLOCATION                    )::get_sym(es, "glGetUniformLocation");
   gl::detail::glGetVertexAttribfv                       = (RS_ES2_PFNGLGETVERTEXATTRIBFV                     )::get_sym(es, "glGetVertexAttribfv");
   gl::detail::glGetVertexAttribiv                       = (RS_ES2_PFNGLGETVERTEXATTRIBIV                     )::get_sym(es, "glGetVertexAttribiv");
   gl::detail::glGetVertexAttribPointerv                 = (RS_ES2_PFNGLGETVERTEXATTRIBPOINTERV               )::get_sym(es, "glGetVertexAttribPointerv");
   gl::detail::glHint                                    = (RS_ES2_PFNGLHINT                                  )::get_sym(es, "glHint");
   gl::detail::glIsBuffer                                = (RS_ES2_PFNGLISBUFFER                              )::get_sym(es, "glIsBuffer");
   gl::detail::glIsEnabled                               = (RS_ES2_PFNGLISENABLED                             )::get_sym(es, "glIsEnabled");
   gl::detail::glIsFramebuffer                           = (RS_ES2_PFNGLISFRAMEBUFFER                         )::get_sym(es, "glIsFramebuffer");
   gl::detail::glIsProgram                               = (RS_ES2_PFNGLISPROGRAM                             )::get_sym(es, "glIsProgram");
   gl::detail::glIsRenderbuffer                          = (RS_ES2_PFNGLISRENDERBUFFER                        )::get_sym(es, "glIsRenderbuffer");
   gl::detail::glIsShader                                = (RS_ES2_PFNGLISSHADER                              )::get_sym(es, "glIsShader");
   gl::detail::glIsTexture                               = (RS_ES2_PFNGLISTEXTURE                             )::get_sym(es, "glIsTexture");
   gl::detail::glLineWidth                               = (RS_ES2_PFNGLLINEWIDTH                             )::get_sym(es, "glLineWidth");
   gl::detail::glLinkProgram                             = (RS_ES2_PFNGLLINKPROGRAM                           )::get_sym(es, "glLinkProgram");
   gl::detail::glPixelStorei                             = (RS_ES2_PFNGLPIXELSTOREI                           )::get_sym(es, "glPixelStorei");
   gl::detail::glPolygonOffset                           = (RS_ES2_PFNGLPOLYGONOFFSET                         )::get_sym(es, "glPolygonOffset");
   gl::detail::glReadPixels                              = (RS_ES2_PFNGLREADPIXELS                            )::get_sym(es, "glReadPixels");
   gl::detail::glReleaseShaderCompiler                   = (RS_ES2_PFNGLRELEASESHADERCOMPILER                 )::get_sym(es, "glReleaseShaderCompiler");
   gl::detail::glRenderbufferStorage                     = (RS_ES2_PFNGLRENDERBUFFERSTORAGE                   )::get_sym(es, "glRenderbufferStorage");
   gl::detail::glSampleCoverage                          = (RS_ES2_PFNGLSAMPLECOVERAGE                        )::get_sym(es, "glSampleCoverage");
   gl::detail::glScissor                                 = (RS_ES2_PFNGLSCISSOR                               )::get_sym(es, "glScissor");
   gl::detail::glShaderBinary                            = (RS_ES2_PFNGLSHADERBINARY                          )::get_sym(es, "glShaderBinary");
   gl::detail::glShaderSource                            = (RS_ES2_PFNGLSHADERSOURCE                          )::get_sym(es, "glShaderSource");
   gl::detail::glStencilFunc                             = (RS_ES2_PFNGLSTENCILFUNC                           )::get_sym(es, "glStencilFunc");
   gl::detail::glStencilFuncSeparate                     = (RS_ES2_PFNGLSTENCILFUNCSEPARATE                   )::get_sym(es, "glStencilFuncSeparate");
   gl::detail::glStencilMask                             = (RS_ES2_PFNGLSTENCILMASK                           )::get_sym(es, "glStencilMask");
   gl::detail::glStencilMaskSeparate                     = (RS_ES2_PFNGLSTENCILMASKSEPARATE                   )::get_sym(es, "glStencilMaskSeparate");
   gl::detail::glStencilOp                               = (RS_ES2_PFNGLSTENCILOP                             )::get_sym(es, "glStencilOp");
   gl::detail::glStencilOpSeparate                       = (RS_ES2_PFNGLSTENCILOPSEPARATE                     )::get_sym(es, "glStencilOpSeparate");
   gl::detail::glTexImage2D                              = (RS_ES2_PFNGLTEXIMAGE2D                            )::get_sym(es, "glTexImage2D");
   gl::detail::glTexParameterf                           = (RS_ES2_PFNGLTEXPARAMETERF                         )::get_sym(es, "glTexParameterf");
   gl::detail::glTexParameterfv                          = (RS_ES2_PFNGLTEXPARAMETERFV                        )::get_sym(es, "glTexParameterfv");
   gl::detail::glTexParameteri                           = (RS_ES2_PFNGLTEXPARAMETERI                         )::get_sym(es, "glTexParameteri");
   gl::detail::glTexParameteriv                          = (RS_ES2_PFNGLTEXPARAMETERIV                        )::get_sym(es, "glTexParameteriv");
   gl::detail::glTexSubImage2D                           = (RS_ES2_PFNGLTEXSUBIMAGE2D                         )::get_sym(es, "glTexSubImage2D");
   gl::detail::glUniform1f                               = (RS_ES2_PFNGLUNIFORM1F                             )::get_sym(es, "glUniform1f");
   gl::detail::glUniform1fv                              = (RS_ES2_PFNGLUNIFORM1FV                            )::get_sym(es, "glUniform1fv");
   gl::detail::glUniform1i                               = (RS_ES2_PFNGLUNIFORM1I                             )::get_sym(es, "glUniform1i");
   gl::detail::glUniform1iv                              = (RS_ES2_PFNGLUNIFORM1IV                            )::get_sym(es, "glUniform1iv");
   gl::detail::glUniform2f                               = (RS_ES2_PFNGLUNIFORM2F                             )::get_sym(es, "glUniform2f");
   gl::detail::glUniform2fv                              = (RS_ES2_PFNGLUNIFORM2FV                            )::get_sym(es, "glUniform2fv");
   gl::detail::glUniform2i                               = (RS_ES2_PFNGLUNIFORM2I                             )::get_sym(es, "glUniform2i");
   gl::detail::glUniform2iv                              = (RS_ES2_PFNGLUNIFORM2IV                            )::get_sym(es, "glUniform2iv");
   gl::detail::glUniform3f                               = (RS_ES2_PFNGLUNIFORM3F                             )::get_sym(es, "glUniform3f");
   gl::detail::glUniform3fv                              = (RS_ES2_PFNGLUNIFORM3FV                            )::get_sym(es, "glUniform3fv");
   gl::detail::glUniform3i                               = (RS_ES2_PFNGLUNIFORM3I                             )::get_sym(es, "glUniform3i");
   gl::detail::glUniform3iv                              = (RS_ES2_PFNGLUNIFORM3IV                            )::get_sym(es, "glUniform3iv");
   gl::detail::glUniform4f                               = (RS_ES2_PFNGLUNIFORM4F                             )::get_sym(es, "glUniform4f");
   gl::detail::glUniform4fv                              = (RS_ES2_PFNGLUNIFORM4FV                            )::get_sym(es, "glUniform4fv");
   gl::detail::glUniform4i                               = (RS_ES2_PFNGLUNIFORM4I                             )::get_sym(es, "glUniform4i");
   gl::detail::glUniform4iv                              = (RS_ES2_PFNGLUNIFORM4IV                            )::get_sym(es, "glUniform4iv");
   gl::detail::glUniformMatrix2fv                        = (RS_ES2_PFNGLUNIFORMMATRIX2FV                      )::get_sym(es, "glUniformMatrix2fv");
   gl::detail::glUniformMatrix3fv                        = (RS_ES2_PFNGLUNIFORMMATRIX3FV                      )::get_sym(es, "glUniformMatrix3fv");
   gl::detail::glUniformMatrix4fv                        = (RS_ES2_PFNGLUNIFORMMATRIX4FV                      )::get_sym(es, "glUniformMatrix4fv");
   gl::detail::glUseProgram                              = (RS_ES2_PFNGLUSEPROGRAM                            )::get_sym(es, "glUseProgram");
   gl::detail::glValidateProgram                         = (RS_ES2_PFNGLVALIDATEPROGRAM                       )::get_sym(es, "glValidateProgram");
   gl::detail::glVertexAttrib1f                          = (RS_ES2_PFNGLVERTEXATTRIB1F                        )::get_sym(es, "glVertexAttrib1f");
   gl::detail::glVertexAttrib1fv                         = (RS_ES2_PFNGLVERTEXATTRIB1FV                       )::get_sym(es, "glVertexAttrib1fv");
   gl::detail::glVertexAttrib2f                          = (RS_ES2_PFNGLVERTEXATTRIB2F                        )::get_sym(es, "glVertexAttrib2f");
   gl::detail::glVertexAttrib2fv                         = (RS_ES2_PFNGLVERTEXATTRIB2FV                       )::get_sym(es, "glVertexAttrib2fv");
   gl::detail::glVertexAttrib3f                          = (RS_ES2_PFNGLVERTEXATTRIB3F                        )::get_sym(es, "glVertexAttrib3f");
   gl::detail::glVertexAttrib3fv                         = (RS_ES2_PFNGLVERTEXATTRIB3FV                       )::get_sym(es, "glVertexAttrib3fv");
   gl::detail::glVertexAttrib4f                          = (RS_ES2_PFNGLVERTEXATTRIB4F                        )::get_sym(es, "glVertexAttrib4f");
   gl::detail::glVertexAttrib4fv                         = (RS_ES2_PFNGLVERTEXATTRIB4FV                       )::get_sym(es, "glVertexAttrib4fv");
   gl::detail::glVertexAttribPointer                     = (RS_ES2_PFNGLVERTEXATTRIBPOINTER                   )::get_sym(es, "glVertexAttribPointer");
   gl::detail::glViewport                                = (RS_ES2_PFNGLVIEWPORT                              )::get_sym(es, "glViewport");
#endif

   // OpenGL ES 2.0 extensions
#if 1
   gl::detail::glGetProgramBinaryOES                     = (PFNGLGETPROGRAMBINARYOESPROC                  )::get_sym(es, "glGetProgramBinaryOES");
   gl::detail::glProgramBinaryOES                        = (PFNGLPROGRAMBINARYOESPROC                     )::get_sym(es, "glProgramBinaryOES");
   gl::detail::glMapBufferOES                            = (PFNGLMAPBUFFEROESPROC                         )::get_sym(es, "glMapBufferOES");
   gl::detail::glUnmapBufferOES                          = (PFNGLUNMAPBUFFEROESPROC                       )::get_sym(es, "glUnmapBufferOES");
   gl::detail::glGetBufferPointervOES                    = (PFNGLGETBUFFERPOINTERVOESPROC                 )::get_sym(es, "glGetBufferPointervOES");
   gl::detail::glDiscardFramebufferEXT                   = (PFNGLDISCARDFRAMEBUFFEREXTPROC                )::get_sym(es, "glDiscardFramebufferEXT");
   gl::detail::glRenderbufferStorageMultisampleEXT       = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC    )::get_sym(es, "glRenderbufferStorageMultisampleEXT");
   gl::detail::glFramebufferTexture2DMultisampleEXT      = (PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC   )::get_sym(es, "glFramebufferTexture2DMultisampleEXT");
#endif

   // OpenGL ES 3.0
#if defined(RENDERSTACK_USE_GLES3)
   gl::detail::glReadBuffer                    = (RS_ES3_PFNGLREADBUFFER                     )::get_sym(es, "glReadBuffer");
   gl::detail::glDrawRangeElements             = (RS_ES3_PFNGLDRAWRANGEELEMENTS              )::get_sym(es, "glDrawRangeElements");
   gl::detail::glTexImage3D                    = (RS_ES3_PFNGLTEXIMAGE3D                     )::get_sym(es, "glTexImage3D");
   gl::detail::glTexSubImage3D                 = (RS_ES3_PFNGLTEXSUBIMAGE3D                  )::get_sym(es, "glTexSubImage3D");
   gl::detail::glCopyTexSubImage3D             = (RS_ES3_PFNGLCOPYTEXSUBIMAGE3D              )::get_sym(es, "glCopyTexSubImage3D");
   gl::detail::glCompressedTexImage3D          = (RS_ES3_PFNGLCOMPRESSEDTEXIMAGE3D           )::get_sym(es, "glCompressedTexImage3D");
   gl::detail::glCompressedTexSubImage3D       = (RS_ES3_PFNGLCOMPRESSEDTEXSUBIMAGE3D        )::get_sym(es, "glCompressedTexSubImage3D");
   gl::detail::glGenQueries                    = (RS_ES3_PFNGLGENQUERIES                     )::get_sym(es, "glGenQueries");
   gl::detail::glDeleteQueries                 = (RS_ES3_PFNGLDELETEQUERIES                  )::get_sym(es, "glDeleteQueries");
   gl::detail::glIsQuery                       = (RS_ES3_PFNGLISQUERY                        )::get_sym(es, "glIsQuery");
   gl::detail::glBeginQuery                    = (RS_ES3_PFNGLBEGINQUERY                     )::get_sym(es, "glBeginQuery");
   gl::detail::glEndQuery                      = (RS_ES3_PFNGLENDQUERY                       )::get_sym(es, "glEndQuery");
   gl::detail::glGetQueryiv                    = (RS_ES3_PFNGLGETQUERYIV                     )::get_sym(es, "glGetQueryiv");
   gl::detail::glGetQueryObjectuiv             = (RS_ES3_PFNGLGETQUERYOBJECTUIV              )::get_sym(es, "glGetQueryObjectuiv");
   gl::detail::glUnmapBuffer                   = (RS_ES3_PFNGLUNMAPBUFFER                    )::get_sym(es, "glUnmapBuffer");
   gl::detail::glGetBufferPointerv             = (RS_ES3_PFNGLGETBUFFERPOINTERV              )::get_sym(es, "glGetBufferPointerv");
   gl::detail::glDrawBuffers                   = (RS_ES3_PFNGLDRAWBUFFERS                    )::get_sym(es, "glDrawBuffers");
   gl::detail::glUniformMatrix2x3fv            = (RS_ES3_PFNGLUNIFORMMATRIX2X3FV             )::get_sym(es, "glUniformMatrix2x3fv");
   gl::detail::glUniformMatrix3x2fv            = (RS_ES3_PFNGLUNIFORMMATRIX3X2FV             )::get_sym(es, "glUniformMatrix3x2fv");
   gl::detail::glUniformMatrix2x4fv            = (RS_ES3_PFNGLUNIFORMMATRIX2X4FV             )::get_sym(es, "glUniformMatrix2x4fv");
   gl::detail::glUniformMatrix4x2fv            = (RS_ES3_PFNGLUNIFORMMATRIX4X2FV             )::get_sym(es, "glUniformMatrix4x2fv");
   gl::detail::glUniformMatrix3x4fv            = (RS_ES3_PFNGLUNIFORMMATRIX3X4FV             )::get_sym(es, "glUniformMatrix3x4fv");
   gl::detail::glUniformMatrix4x3fv            = (RS_ES3_PFNGLUNIFORMMATRIX4X3FV             )::get_sym(es, "glUniformMatrix4x3fv");
   gl::detail::glBlitFramebuffer               = (RS_ES3_PFNGLBLITFRAMEBUFFER                )::get_sym(es, "glBlitFramebuffer");
   gl::detail::glRenderbufferStorageMultisample= (RS_ES3_PFNGLRENDERBUFFERSTORAGEMULTISAMPLE )::get_sym(es, "glRenderbufferStorageMultisample");
   gl::detail::glFramebufferTextureLayer       = (RS_ES3_PFNGLFRAMEBUFFERTEXTURELAYER        )::get_sym(es, "glFramebufferTextureLayer");
   gl::detail::glMapBufferRange                = (RS_ES3_PFNGLMAPBUFFERRANGE                 )::get_sym(es, "glMapBufferRange");
   gl::detail::glFlushMappedBufferRange        = (RS_ES3_PFNGLFLUSHMAPPEDBUFFERRANGE         )::get_sym(es, "glFlushMappedBufferRange");
   gl::detail::glBindVertexArray               = (RS_ES3_PFNGLBINDVERTEXARRAY                )::get_sym(es, "glBindVertexArray");
   gl::detail::glDeleteVertexArrays            = (RS_ES3_PFNGLDELETEVERTEXARRAYS             )::get_sym(es, "glDeleteVertexArrays");
   gl::detail::glGenVertexArrays               = (RS_ES3_PFNGLGENVERTEXARRAYS                )::get_sym(es, "glGenVertexArrays");
   gl::detail::glIsVertexArray                 = (RS_ES3_PFNGLISVERTEXARRAY                  )::get_sym(es, "glIsVertexArray");
   gl::detail::glGetIntegeri_v                 = (RS_ES3_PFNGLGETINTEGERI_V                  )::get_sym(es, "glGetIntegeri_v");
   gl::detail::glBeginTransformFeedback        = (RS_ES3_PFNGLBEGINTRANSFORMFEEDBACK         )::get_sym(es, "glBeginTransformFeedback");
   gl::detail::glEndTransformFeedback          = (RS_ES3_PFNGLENDTRANSFORMFEEDBACK           )::get_sym(es, "glEndTransformFeedback");
   gl::detail::glBindBufferRange               = (RS_ES3_PFNGLBINDBUFFERRANGE                )::get_sym(es, "glBindBufferRange");
   gl::detail::glBindBufferBase                = (RS_ES3_PFNGLBINDBUFFERBASE                 )::get_sym(es, "glBindBufferBase");
   gl::detail::glTransformFeedbackVaryings     = (RS_ES3_PFNGLTRANSFORMFEEDBACKVARYINGS      )::get_sym(es, "glTransformFeedbackVaryings");
   gl::detail::glGetTransformFeedbackVarying   = (RS_ES3_PFNGLGETTRANSFORMFEEDBACKVARYING    )::get_sym(es, "glGetTransformFeedbackVarying");
   gl::detail::glVertexAttribIPointer          = (RS_ES3_PFNGLVERTEXATTRIBIPOINTER           )::get_sym(es, "glVertexAttribIPointer");
   gl::detail::glGetVertexAttribIiv            = (RS_ES3_PFNGLGETVERTEXATTRIBIIV             )::get_sym(es, "glGetVertexAttribIiv");
   gl::detail::glGetVertexAttribIuiv           = (RS_ES3_PFNGLGETVERTEXATTRIBIUIV            )::get_sym(es, "glGetVertexAttribIuiv");
   gl::detail::glVertexAttribI4i               = (RS_ES3_PFNGLVERTEXATTRIBI4I                )::get_sym(es, "glVertexAttribI4i");
   gl::detail::glVertexAttribI4ui              = (RS_ES3_PFNGLVERTEXATTRIBI4UI               )::get_sym(es, "glVertexAttribI4ui");
   gl::detail::glVertexAttribI4iv              = (RS_ES3_PFNGLVERTEXATTRIBI4IV               )::get_sym(es, "glVertexAttribI4iv");
   gl::detail::glVertexAttribI4uiv             = (RS_ES3_PFNGLVERTEXATTRIBI4UIV              )::get_sym(es, "glVertexAttribI4uiv");
   gl::detail::glGetUniformuiv                 = (RS_ES3_PFNGLGETUNIFORMUIV                  )::get_sym(es, "glGetUniformuiv");
   gl::detail::glGetFragDataLocation           = (RS_ES3_PFNGLGETFRAGDATALOCATION            )::get_sym(es, "glGetFragDataLocation");
   gl::detail::glUniform1ui                    = (RS_ES3_PFNGLUNIFORM1UI                     )::get_sym(es, "glUniform1ui");
   gl::detail::glUniform2ui                    = (RS_ES3_PFNGLUNIFORM2UI                     )::get_sym(es, "glUniform2ui");
   gl::detail::glUniform3ui                    = (RS_ES3_PFNGLUNIFORM3UI                     )::get_sym(es, "glUniform3ui");
   gl::detail::glUniform4ui                    = (RS_ES3_PFNGLUNIFORM4UI                     )::get_sym(es, "glUniform4ui");
   gl::detail::glUniform1uiv                   = (RS_ES3_PFNGLUNIFORM1UIV                    )::get_sym(es, "glUniform1uiv");
   gl::detail::glUniform2uiv                   = (RS_ES3_PFNGLUNIFORM2UIV                    )::get_sym(es, "glUniform2uiv");
   gl::detail::glUniform3uiv                   = (RS_ES3_PFNGLUNIFORM3UIV                    )::get_sym(es, "glUniform3uiv");
   gl::detail::glUniform4uiv                   = (RS_ES3_PFNGLUNIFORM4UIV                    )::get_sym(es, "glUniform4uiv");
   gl::detail::glClearBufferiv                 = (RS_ES3_PFNGLCLEARBUFFERIV                  )::get_sym(es, "glClearBufferiv");
   gl::detail::glClearBufferuiv                = (RS_ES3_PFNGLCLEARBUFFERUIV                 )::get_sym(es, "glClearBufferuiv");
   gl::detail::glClearBufferfv                 = (RS_ES3_PFNGLCLEARBUFFERFV                  )::get_sym(es, "glClearBufferfv");
   gl::detail::glClearBufferfi                 = (RS_ES3_PFNGLCLEARBUFFERFI                  )::get_sym(es, "glClearBufferfi");
   gl::detail::glGetStringi                    = (RS_ES3_PFNGLGETSTRINGI                     )::get_sym(es, "glGetStringi");
   gl::detail::glCopyBufferSubData             = (RS_ES3_PFNGLCOPYBUFFERSUBDATA              )::get_sym(es, "glCopyBufferSubData");
   gl::detail::glGetUniformIndices             = (RS_ES3_PFNGLGETUNIFORMINDICES              )::get_sym(es, "glGetUniformIndices");
   gl::detail::glGetActiveUniformsiv           = (RS_ES3_PFNGLGETACTIVEUNIFORMSIV            )::get_sym(es, "glGetActiveUniformsiv");
   gl::detail::glGetUniformBlockIndex          = (RS_ES3_PFNGLGETUNIFORMBLOCKINDEX           )::get_sym(es, "glGetUniformBlockIndex");
   gl::detail::glGetActiveUniformBlockiv       = (RS_ES3_PFNGLGETACTIVEUNIFORMBLOCKIV        )::get_sym(es, "glGetActiveUniformBlockiv");
   gl::detail::glGetActiveUniformBlockName     = (RS_ES3_PFNGLGETACTIVEUNIFORMBLOCKNAME      )::get_sym(es, "glGetActiveUniformBlockName");
   gl::detail::glUniformBlockBinding           = (RS_ES3_PFNGLUNIFORMBLOCKBINDING            )::get_sym(es, "glUniformBlockBinding");
   gl::detail::glDrawArraysInstanced           = (RS_ES3_PFNGLDRAWARRAYSINSTANCED            )::get_sym(es, "glDrawArraysInstanced");
   gl::detail::glDrawElementsInstanced         = (RS_ES3_PFNGLDRAWELEMENTSINSTANCED          )::get_sym(es, "glDrawElementsInstanced");
   gl::detail::glFenceSync                     = (RS_ES3_PFNGLFENCESYNC                      )::get_sym(es, "glFenceSync");
   gl::detail::glIsSync                        = (RS_ES3_PFNGLISSYNC                         )::get_sym(es, "glIsSync");
   gl::detail::glDeleteSync                    = (RS_ES3_PFNGLDELETESYNC                     )::get_sym(es, "glDeleteSync");
   gl::detail::glClientWaitSync                = (RS_ES3_PFNGLCLIENTWAITSYNC                 )::get_sym(es, "glClientWaitSync");
   gl::detail::glWaitSync                      = (RS_ES3_PFNGLWAITSYNC                       )::get_sym(es, "glWaitSync");
   gl::detail::glGetInteger64v                 = (RS_ES3_PFNGLGETINTEGER64V                  )::get_sym(es, "glGetInteger64v");
   gl::detail::glGetSynciv                     = (RS_ES3_PFNGLGETSYNCIV                      )::get_sym(es, "glGetSynciv");
   gl::detail::glGetInteger64i_v               = (RS_ES3_PFNGLGETINTEGER64I_V                )::get_sym(es, "glGetInteger64i_v");
   gl::detail::glGetBufferParameteri64v        = (RS_ES3_PFNGLGETBUFFERPARAMETERI64V         )::get_sym(es, "glGetBufferParameteri64v");
   gl::detail::glGenSamplers                   = (RS_ES3_PFNGLGENSAMPLERS                    )::get_sym(es, "glGenSamplers");
   gl::detail::glDeleteSamplers                = (RS_ES3_PFNGLDELETESAMPLERS                 )::get_sym(es, "glDeleteSamplers");
   gl::detail::glIsSampler                     = (RS_ES3_PFNGLISSAMPLER                      )::get_sym(es, "glIsSampler");
   gl::detail::glBindSampler                   = (RS_ES3_PFNGLBINDSAMPLER                    )::get_sym(es, "glBindSampler");
   gl::detail::glSamplerParameteri             = (RS_ES3_PFNGLSAMPLERPARAMETERI              )::get_sym(es, "glSamplerParameteri");
   gl::detail::glSamplerParameteriv            = (RS_ES3_PFNGLSAMPLERPARAMETERIV             )::get_sym(es, "glSamplerParameteriv");
   gl::detail::glSamplerParameterf             = (RS_ES3_PFNGLSAMPLERPARAMETERF              )::get_sym(es, "glSamplerParameterf");
   gl::detail::glSamplerParameterfv            = (RS_ES3_PFNGLSAMPLERPARAMETERFV             )::get_sym(es, "glSamplerParameterfv");
   gl::detail::glGetSamplerParameteriv         = (RS_ES3_PFNGLGETSAMPLERPARAMETERIV          )::get_sym(es, "glGetSamplerParameteriv");
   gl::detail::glGetSamplerParameterfv         = (RS_ES3_PFNGLGETSAMPLERPARAMETERFV          )::get_sym(es, "glGetSamplerParameterfv");
   gl::detail::glVertexAttribDivisor           = (RS_ES3_PFNGLVERTEXATTRIBDIVISOR            )::get_sym(es, "glVertexAttribDivisor");
   gl::detail::glBindTransformFeedback         = (RS_ES3_PFNGLBINDTRANSFORMFEEDBACK          )::get_sym(es, "glBindTransformFeedback");
   gl::detail::glDeleteTransformFeedbacks      = (RS_ES3_PFNGLDELETETRANSFORMFEEDBACKS       )::get_sym(es, "glDeleteTransformFeedbacks");
   gl::detail::glGenTransformFeedbacks         = (RS_ES3_PFNGLGENTRANSFORMFEEDBACKS          )::get_sym(es, "glGenTransformFeedbacks");
   gl::detail::glIsTransformFeedback           = (RS_ES3_PFNGLISTRANSFORMFEEDBACK            )::get_sym(es, "glIsTransformFeedback");
   gl::detail::glPauseTransformFeedback        = (RS_ES3_PFNGLPAUSETRANSFORMFEEDBACK         )::get_sym(es, "glPauseTransformFeedback");
   gl::detail::glResumeTransformFeedback       = (RS_ES3_PFNGLRESUMETRANSFORMFEEDBACK        )::get_sym(es, "glResumeTransformFeedback");
   gl::detail::glGetProgramBinary              = (RS_ES3_PFNGLGETPROGRAMBINARY               )::get_sym(es, "glGetProgramBinary");
   gl::detail::glProgramBinary                 = (RS_ES3_PFNGLPROGRAMBINARY                  )::get_sym(es, "glProgramBinary");
   gl::detail::glProgramParameteri             = (RS_ES3_PFNGLPROGRAMPARAMETERI              )::get_sym(es, "glProgramParameteri");
   gl::detail::glInvalidateFramebuffer         = (RS_ES3_PFNGLINVALIDATEFRAMEBUFFER          )::get_sym(es, "glInvalidateFramebuffer");
   gl::detail::glInvalidateSubFramebuffer      = (RS_ES3_PFNGLINVALIDATESUBFRAMEBUFFER       )::get_sym(es, "glInvalidateSubFramebuffer");
   gl::detail::glTexStorage2D                  = (RS_ES3_PFNGLTEXSTORAGE2D                   )::get_sym(es, "glTexStorage2D");
   gl::detail::glTexStorage3D                  = (RS_ES3_PFNGLTEXSTORAGE3D                   )::get_sym(es, "glTexStorage3D");
   gl::detail::glGetInternalformativ           = (RS_ES3_PFNGLGETINTERNALFORMATIV            )::get_sym(es, "glGetInternalformativ");
#endif

   //check_gl_error();
}

} }
#endif
