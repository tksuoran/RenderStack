#if defined(RENDERSTACK_GL_API_OPENGL)

#include "renderstack_toolkit/platform.hpp"
#if defined(_WIN32)
# include <windows.h>
#endif

#include <GL/glcorearb.h>

#if defined(RENDERSTACK_USE_GLFW)
# include <GL/glfw3.h>
#endif

#include "renderstack_toolkit/window.hpp"

#include <cstdio>
#include <cstdlib>

//#define EXT_SUBSET 0
#define EXT_FULL 1

#if 1 /* GL functions */

namespace gl { namespace detail {

/*  GL_VERSION_1_0  */ 
#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
PFNGLCULLFACEPROC                                       glCullFace                                      = nullptr;
PFNGLFRONTFACEPROC                                      glFrontFace                                     = nullptr;
PFNGLHINTPROC                                           glHint                                          = nullptr;
PFNGLLINEWIDTHPROC                                      glLineWidth                                     = nullptr;
PFNGLPOINTSIZEPROC                                      glPointSize                                     = nullptr;
PFNGLPOLYGONMODEPROC                                    glPolygonMode                                   = nullptr;
PFNGLSCISSORPROC                                        glScissor                                       = nullptr;
PFNGLTEXPARAMETERFPROC                                  glTexParameterf                                 = nullptr;
PFNGLTEXPARAMETERFVPROC                                 glTexParameterfv                                = nullptr;
PFNGLTEXPARAMETERIPROC                                  glTexParameteri                                 = nullptr;
PFNGLTEXPARAMETERIVPROC                                 glTexParameteriv                                = nullptr;
PFNGLTEXIMAGE1DPROC                                     glTexImage1D                                    = nullptr;
PFNGLTEXIMAGE2DPROC                                     glTexImage2D                                    = nullptr;
PFNGLDRAWBUFFERPROC                                     glDrawBuffer                                    = nullptr;
PFNGLCLEARPROC                                          glClear                                         = nullptr;
PFNGLCLEARCOLORPROC                                     glClearColor                                    = nullptr;
PFNGLCLEARSTENCILPROC                                   glClearStencil                                  = nullptr;
PFNGLCLEARDEPTHPROC                                     glClearDepth                                    = nullptr;
PFNGLSTENCILMASKPROC                                    glStencilMask                                   = nullptr;
PFNGLCOLORMASKPROC                                      glColorMask                                     = nullptr;
PFNGLDEPTHMASKPROC                                      glDepthMask                                     = nullptr;
PFNGLDISABLEPROC                                        glDisable                                       = nullptr;
PFNGLENABLEPROC                                         glEnable                                        = nullptr;
PFNGLFINISHPROC                                         glFinish                                        = nullptr;
PFNGLFLUSHPROC                                          glFlush                                         = nullptr;
PFNGLBLENDFUNCPROC                                      glBlendFunc                                     = nullptr;
PFNGLLOGICOPPROC                                        glLogicOp                                       = nullptr;
PFNGLSTENCILFUNCPROC                                    glStencilFunc                                   = nullptr;
PFNGLSTENCILOPPROC                                      glStencilOp                                     = nullptr;
PFNGLDEPTHFUNCPROC                                      glDepthFunc                                     = nullptr;
PFNGLPIXELSTOREFPROC                                    glPixelStoref                                   = nullptr;
PFNGLPIXELSTOREIPROC                                    glPixelStorei                                   = nullptr;
PFNGLREADBUFFERPROC                                     glReadBuffer                                    = nullptr;
PFNGLREADPIXELSPROC                                     glReadPixels                                    = nullptr;
PFNGLGETBOOLEANVPROC                                    glGetBooleanv                                   = nullptr;
PFNGLGETDOUBLEVPROC                                     glGetDoublev                                    = nullptr;
PFNGLGETERRORPROC                                       glGetError                                      = nullptr;
PFNGLGETFLOATVPROC                                      glGetFloatv                                     = nullptr;
PFNGLGETINTEGERVPROC                                    glGetIntegerv                                   = nullptr;
PFNGLGETSTRINGPROC                                      glGetString                                     = nullptr;
PFNGLGETTEXIMAGEPROC                                    glGetTexImage                                   = nullptr;
PFNGLGETTEXPARAMETERFVPROC                              glGetTexParameterfv                             = nullptr;
PFNGLGETTEXPARAMETERIVPROC                              glGetTexParameteriv                             = nullptr;
PFNGLGETTEXLEVELPARAMETERFVPROC                         glGetTexLevelParameterfv                        = nullptr; 
PFNGLGETTEXLEVELPARAMETERIVPROC                         glGetTexLevelParameteriv                        = nullptr;
PFNGLISENABLEDPROC                                      glIsEnabled                                     = nullptr;
PFNGLDEPTHRANGEPROC                                     glDepthRange                                    = nullptr;
PFNGLVIEWPORTPROC                                       glViewport                                      = nullptr;

/*  GL_VERSION_1_1  */
PFNGLDRAWARRAYSPROC                                     glDrawArrays                                    = nullptr;
PFNGLDRAWELEMENTSPROC                                   glDrawElements                                  = nullptr;
PFNGLGETPOINTERVPROC                                    glGetPointerv                                   = nullptr;
PFNGLPOLYGONOFFSETPROC                                  glPolygonOffset                                 = nullptr;
PFNGLCOPYTEXIMAGE1DPROC                                 glCopyTexImage1D                                = nullptr;
PFNGLCOPYTEXIMAGE2DPROC                                 glCopyTexImage2D                                = nullptr;
PFNGLCOPYTEXSUBIMAGE1DPROC                              glCopyTexSubImage1D                             = nullptr;
PFNGLCOPYTEXSUBIMAGE2DPROC                              glCopyTexSubImage2D                             = nullptr;
PFNGLTEXSUBIMAGE1DPROC                                  glTexSubImage1D                                 = nullptr;
PFNGLTEXSUBIMAGE2DPROC                                  glTexSubImage2D                                 = nullptr;
PFNGLBINDTEXTUREPROC                                    glBindTexture                                   = nullptr;
PFNGLDELETETEXTURESPROC                                 glDeleteTextures                                = nullptr;
PFNGLGENTEXTURESPROC                                    glGenTextures                                   = nullptr;
PFNGLISTEXTUREPROC                                      glIsTexture                                     = nullptr;
#endif

/*  GL_VERSION_1_2  */ 
#if 1
PFNGLBLENDCOLORPROC                                     glBlendColor                                    = nullptr;
PFNGLBLENDEQUATIONPROC                                  glBlendEquation                                 = nullptr;
PFNGLDRAWRANGEELEMENTSPROC                              glDrawRangeElements                             = nullptr;
PFNGLTEXIMAGE3DPROC                                     glTexImage3D                                    = nullptr;
PFNGLTEXSUBIMAGE3DPROC                                  glTexSubImage3D                                 = nullptr;
PFNGLCOPYTEXSUBIMAGE3DPROC                              glCopyTexSubImage3D                             = nullptr;
#endif

/*  GL_VERSION_1_3  */ 
#if 1
PFNGLACTIVETEXTUREPROC                                  glActiveTexture                                 = nullptr;
PFNGLSAMPLECOVERAGEPROC                                 glSampleCoverage                                = nullptr;
PFNGLCOMPRESSEDTEXIMAGE3DPROC                           glCompressedTexImage3D                          = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DPROC                           glCompressedTexImage2D                          = nullptr;
PFNGLCOMPRESSEDTEXIMAGE1DPROC                           glCompressedTexImage1D                          = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC                        glCompressedTexSubImage3D                       = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC                        glCompressedTexSubImage2D                       = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC                        glCompressedTexSubImage1D                       = nullptr;
PFNGLGETCOMPRESSEDTEXIMAGEPROC                          glGetCompressedTexImage                         = nullptr;
#endif

/*  GL_VERSION_1_4  */ 
#if 1
PFNGLBLENDFUNCSEPARATEPROC                              glBlendFuncSeparate                             = nullptr;
PFNGLMULTIDRAWARRAYSPROC                                glMultiDrawArrays                               = nullptr;
PFNGLMULTIDRAWELEMENTSPROC                              glMultiDrawElements                             = nullptr;
PFNGLPOINTPARAMETERFPROC                                glPointParameterf                               = nullptr;
PFNGLPOINTPARAMETERFVPROC                               glPointParameterfv                              = nullptr;
PFNGLPOINTPARAMETERIPROC                                glPointParameteri                               = nullptr;
PFNGLPOINTPARAMETERIVPROC                               glPointParameteriv                              = nullptr;
#endif

/*  GL_VERSION_1_5  */ 
#if 1
PFNGLGENQUERIESPROC                                     glGenQueries                                    = nullptr;
PFNGLDELETEQUERIESPROC                                  glDeleteQueries                                 = nullptr;
PFNGLISQUERYPROC                                        glIsQuery                                       = nullptr;
PFNGLBEGINQUERYPROC                                     glBeginQuery                                    = nullptr;
PFNGLENDQUERYPROC                                       glEndQuery                                      = nullptr;
PFNGLGETQUERYIVPROC                                     glGetQueryiv                                    = nullptr;
PFNGLGETQUERYOBJECTIVPROC                               glGetQueryObjectiv                              = nullptr;
PFNGLGETQUERYOBJECTUIVPROC                              glGetQueryObjectuiv                             = nullptr;
PFNGLBINDBUFFERPROC                                     glBindBuffer                                    = nullptr;
PFNGLDELETEBUFFERSPROC                                  glDeleteBuffers                                 = nullptr;
PFNGLGENBUFFERSPROC                                     glGenBuffers                                    = nullptr;
PFNGLISBUFFERPROC                                       glIsBuffer                                      = nullptr;
PFNGLBUFFERDATAPROC                                     glBufferData                                    = nullptr;
PFNGLBUFFERSUBDATAPROC                                  glBufferSubData                                 = nullptr;
PFNGLGETBUFFERSUBDATAPROC                               glGetBufferSubData                              = nullptr;
PFNGLMAPBUFFERPROC                                      glMapBuffer                                     = nullptr;
PFNGLUNMAPBUFFERPROC                                    glUnmapBuffer                                   = nullptr;
PFNGLGETBUFFERPARAMETERIVPROC                           glGetBufferParameteriv                          = nullptr;
PFNGLGETBUFFERPOINTERVPROC                              glGetBufferPointerv                             = nullptr;
#endif

/*  GL_VERSION_2_0  */
#if 1
PFNGLBLENDEQUATIONSEPARATEPROC                          glBlendEquationSeparate                         = nullptr;
PFNGLDRAWBUFFERSPROC                                    glDrawBuffers                                   = nullptr;
PFNGLSTENCILOPSEPARATEPROC                              glStencilOpSeparate                             = nullptr;
PFNGLSTENCILFUNCSEPARATEPROC                            glStencilFuncSeparate                           = nullptr;
PFNGLSTENCILMASKSEPARATEPROC                            glStencilMaskSeparate                           = nullptr;
PFNGLATTACHSHADERPROC                                   glAttachShader                                  = nullptr;
PFNGLBINDATTRIBLOCATIONPROC                             glBindAttribLocation                            = nullptr;
PFNGLCOMPILESHADERPROC                                  glCompileShader                                 = nullptr;
PFNGLCREATEPROGRAMPROC                                  glCreateProgram                                 = nullptr;
PFNGLCREATESHADERPROC                                   glCreateShader                                  = nullptr;
PFNGLDELETEPROGRAMPROC                                  glDeleteProgram                                 = nullptr;
PFNGLDELETESHADERPROC                                   glDeleteShader                                  = nullptr;
PFNGLDETACHSHADERPROC                                   glDetachShader                                  = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC                       glDisableVertexAttribArray                      = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC                        glEnableVertexAttribArray                       = nullptr;
PFNGLGETACTIVEATTRIBPROC                                glGetActiveAttrib                               = nullptr;
PFNGLGETACTIVEUNIFORMPROC                               glGetActiveUniform                              = nullptr;
PFNGLGETATTACHEDSHADERSPROC                             glGetAttachedShaders                            = nullptr;
PFNGLGETATTRIBLOCATIONPROC                              glGetAttribLocation                             = nullptr;
PFNGLGETPROGRAMIVPROC                                   glGetProgramiv                                  = nullptr;
PFNGLGETPROGRAMINFOLOGPROC                              glGetProgramInfoLog                             = nullptr;
PFNGLGETSHADERIVPROC                                    glGetShaderiv                                   = nullptr;
PFNGLGETSHADERINFOLOGPROC                               glGetShaderInfoLog                              = nullptr;
PFNGLGETSHADERSOURCEPROC                                glGetShaderSource                               = nullptr;
PFNGLGETUNIFORMLOCATIONPROC                             glGetUniformLocation                            = nullptr;
PFNGLGETUNIFORMFVPROC                                   glGetUniformfv                                  = nullptr;
PFNGLGETUNIFORMIVPROC                                   glGetUniformiv                                  = nullptr;
PFNGLGETVERTEXATTRIBDVPROC                              glGetVertexAttribdv                             = nullptr;
PFNGLGETVERTEXATTRIBFVPROC                              glGetVertexAttribfv                             = nullptr;
PFNGLGETVERTEXATTRIBIVPROC                              glGetVertexAttribiv                             = nullptr;
PFNGLGETVERTEXATTRIBPOINTERVPROC                        glGetVertexAttribPointerv                       = nullptr;
PFNGLISPROGRAMPROC                                      glIsProgram                                     = nullptr;
PFNGLISSHADERPROC                                       glIsShader                                      = nullptr;
PFNGLLINKPROGRAMPROC                                    glLinkProgram                                   = nullptr;
PFNGLSHADERSOURCEPROC                                   glShaderSource                                  = nullptr;
PFNGLUSEPROGRAMPROC                                     glUseProgram                                    = nullptr;
PFNGLUNIFORM1FPROC                                      glUniform1f                                     = nullptr;
PFNGLUNIFORM2FPROC                                      glUniform2f                                     = nullptr;
PFNGLUNIFORM3FPROC                                      glUniform3f                                     = nullptr;
PFNGLUNIFORM4FPROC                                      glUniform4f                                     = nullptr;
PFNGLUNIFORM1IPROC                                      glUniform1i                                     = nullptr;
PFNGLUNIFORM2IPROC                                      glUniform2i                                     = nullptr;
PFNGLUNIFORM3IPROC                                      glUniform3i                                     = nullptr;
PFNGLUNIFORM4IPROC                                      glUniform4i                                     = nullptr;
PFNGLUNIFORM1FVPROC                                     glUniform1fv                                    = nullptr;
PFNGLUNIFORM2FVPROC                                     glUniform2fv                                    = nullptr;
PFNGLUNIFORM3FVPROC                                     glUniform3fv                                    = nullptr;
PFNGLUNIFORM4FVPROC                                     glUniform4fv                                    = nullptr;
PFNGLUNIFORM1IVPROC                                     glUniform1iv                                    = nullptr;
PFNGLUNIFORM2IVPROC                                     glUniform2iv                                    = nullptr;
PFNGLUNIFORM3IVPROC                                     glUniform3iv                                    = nullptr;
PFNGLUNIFORM4IVPROC                                     glUniform4iv                                    = nullptr;
PFNGLUNIFORMMATRIX2FVPROC                               glUniformMatrix2fv                              = nullptr;
PFNGLUNIFORMMATRIX3FVPROC                               glUniformMatrix3fv                              = nullptr;
PFNGLUNIFORMMATRIX4FVPROC                               glUniformMatrix4fv                              = nullptr;
PFNGLVALIDATEPROGRAMPROC                                glValidateProgram                               = nullptr;
PFNGLVERTEXATTRIB1DPROC                                 glVertexAttrib1d                                = nullptr;
PFNGLVERTEXATTRIB1DVPROC                                glVertexAttrib1dv                               = nullptr;
PFNGLVERTEXATTRIB1FPROC                                 glVertexAttrib1f                                = nullptr;
PFNGLVERTEXATTRIB1FVPROC                                glVertexAttrib1fv                               = nullptr;
PFNGLVERTEXATTRIB1SPROC                                 glVertexAttrib1s                                = nullptr;
PFNGLVERTEXATTRIB1SVPROC                                glVertexAttrib1sv                               = nullptr;
PFNGLVERTEXATTRIB2DPROC                                 glVertexAttrib2d                                = nullptr;
PFNGLVERTEXATTRIB2DVPROC                                glVertexAttrib2dv                               = nullptr;
PFNGLVERTEXATTRIB2FPROC                                 glVertexAttrib2f                                = nullptr;
PFNGLVERTEXATTRIB2FVPROC                                glVertexAttrib2fv                               = nullptr;
PFNGLVERTEXATTRIB2SPROC                                 glVertexAttrib2s                                = nullptr;
PFNGLVERTEXATTRIB2SVPROC                                glVertexAttrib2sv                               = nullptr;
PFNGLVERTEXATTRIB3DPROC                                 glVertexAttrib3d                                = nullptr;
PFNGLVERTEXATTRIB3DVPROC                                glVertexAttrib3dv                               = nullptr;
PFNGLVERTEXATTRIB3FPROC                                 glVertexAttrib3f                                = nullptr;
PFNGLVERTEXATTRIB3FVPROC                                glVertexAttrib3fv                               = nullptr;
PFNGLVERTEXATTRIB3SPROC                                 glVertexAttrib3s                                = nullptr;
PFNGLVERTEXATTRIB3SVPROC                                glVertexAttrib3sv                               = nullptr;
PFNGLVERTEXATTRIB4NBVPROC                               glVertexAttrib4Nbv                              = nullptr;
PFNGLVERTEXATTRIB4NIVPROC                               glVertexAttrib4Niv                              = nullptr;
PFNGLVERTEXATTRIB4NSVPROC                               glVertexAttrib4Nsv                              = nullptr;
PFNGLVERTEXATTRIB4NUBPROC                               glVertexAttrib4Nub                              = nullptr;
PFNGLVERTEXATTRIB4NUBVPROC                              glVertexAttrib4Nubv                             = nullptr;
PFNGLVERTEXATTRIB4NUIVPROC                              glVertexAttrib4Nuiv                             = nullptr;
PFNGLVERTEXATTRIB4NUSVPROC                              glVertexAttrib4Nusv                             = nullptr;
PFNGLVERTEXATTRIB4BVPROC                                glVertexAttrib4bv                               = nullptr;
PFNGLVERTEXATTRIB4DPROC                                 glVertexAttrib4d                                = nullptr;
PFNGLVERTEXATTRIB4DVPROC                                glVertexAttrib4dv                               = nullptr;
PFNGLVERTEXATTRIB4FPROC                                 glVertexAttrib4f                                = nullptr;
PFNGLVERTEXATTRIB4FVPROC                                glVertexAttrib4fv                               = nullptr;
PFNGLVERTEXATTRIB4IVPROC                                glVertexAttrib4iv                               = nullptr;
PFNGLVERTEXATTRIB4SPROC                                 glVertexAttrib4s                                = nullptr;
PFNGLVERTEXATTRIB4SVPROC                                glVertexAttrib4sv                               = nullptr;
PFNGLVERTEXATTRIB4UBVPROC                               glVertexAttrib4ubv                              = nullptr;
PFNGLVERTEXATTRIB4UIVPROC                               glVertexAttrib4uiv                              = nullptr;
PFNGLVERTEXATTRIB4USVPROC                               glVertexAttrib4usv                              = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC                            glVertexAttribPointer                           = nullptr;
#endif

/*  GL_VERSION_2_1  */ 
#if 1
PFNGLUNIFORMMATRIX2X3FVPROC                             glUniformMatrix2x3fv                            = nullptr;
PFNGLUNIFORMMATRIX3X2FVPROC                             glUniformMatrix3x2fv                            = nullptr;
PFNGLUNIFORMMATRIX2X4FVPROC                             glUniformMatrix2x4fv                            = nullptr;
PFNGLUNIFORMMATRIX4X2FVPROC                             glUniformMatrix4x2fv                            = nullptr;
PFNGLUNIFORMMATRIX3X4FVPROC                             glUniformMatrix3x4fv                            = nullptr;
PFNGLUNIFORMMATRIX4X3FVPROC                             glUniformMatrix4x3fv                            = nullptr;
#endif

/*  GL_VERSION_3_0  */
#if 1
PFNGLCOLORMASKIPROC                                     glColorMaski                                    = nullptr;
PFNGLGETBOOLEANI_VPROC                                  glGetBooleani_v                                 = nullptr;
PFNGLGETINTEGERI_VPROC                                  glGetIntegeri_v                                 = nullptr;
PFNGLENABLEIPROC                                        glEnablei                                       = nullptr;
PFNGLDISABLEIPROC                                       glDisablei                                      = nullptr;
PFNGLISENABLEDIPROC                                     glIsEnabledi                                    = nullptr;
PFNGLBEGINTRANSFORMFEEDBACKPROC                         glBeginTransformFeedback                        = nullptr;
PFNGLENDTRANSFORMFEEDBACKPROC                           glEndTransformFeedback                          = nullptr;
PFNGLBINDBUFFERRANGEPROC                                glBindBufferRange                               = nullptr;
PFNGLBINDBUFFERBASEPROC                                 glBindBufferBase                                = nullptr;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC                      glTransformFeedbackVaryings                     = nullptr;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC                    glGetTransformFeedbackVarying                   = nullptr;
PFNGLCLAMPCOLORPROC                                     glClampColor                                    = nullptr;
PFNGLBEGINCONDITIONALRENDERPROC                         glBeginConditionalRender                        = nullptr;
PFNGLENDCONDITIONALRENDERPROC                           glEndConditionalRender                          = nullptr;
PFNGLVERTEXATTRIBIPOINTERPROC                           glVertexAttribIPointer                          = nullptr;
PFNGLGETVERTEXATTRIBIIVPROC                             glGetVertexAttribIiv                            = nullptr;
PFNGLGETVERTEXATTRIBIUIVPROC                            glGetVertexAttribIuiv                           = nullptr;
PFNGLVERTEXATTRIBI1IPROC                                glVertexAttribI1i                               = nullptr;
PFNGLVERTEXATTRIBI2IPROC                                glVertexAttribI2i                               = nullptr;
PFNGLVERTEXATTRIBI3IPROC                                glVertexAttribI3i                               = nullptr;
PFNGLVERTEXATTRIBI4IPROC                                glVertexAttribI4i                               = nullptr;
PFNGLVERTEXATTRIBI1UIPROC                               glVertexAttribI1ui                              = nullptr;
PFNGLVERTEXATTRIBI2UIPROC                               glVertexAttribI2ui                              = nullptr;
PFNGLVERTEXATTRIBI3UIPROC                               glVertexAttribI3ui                              = nullptr;
PFNGLVERTEXATTRIBI4UIPROC                               glVertexAttribI4ui                              = nullptr;
PFNGLVERTEXATTRIBI1IVPROC                               glVertexAttribI1iv                              = nullptr;
PFNGLVERTEXATTRIBI2IVPROC                               glVertexAttribI2iv                              = nullptr;
PFNGLVERTEXATTRIBI3IVPROC                               glVertexAttribI3iv                              = nullptr;
PFNGLVERTEXATTRIBI4IVPROC                               glVertexAttribI4iv                              = nullptr;
PFNGLVERTEXATTRIBI1UIVPROC                              glVertexAttribI1uiv                             = nullptr;
PFNGLVERTEXATTRIBI2UIVPROC                              glVertexAttribI2uiv                             = nullptr;
PFNGLVERTEXATTRIBI3UIVPROC                              glVertexAttribI3uiv                             = nullptr;
PFNGLVERTEXATTRIBI4UIVPROC                              glVertexAttribI4uiv                             = nullptr;
PFNGLVERTEXATTRIBI4BVPROC                               glVertexAttribI4bv                              = nullptr;
PFNGLVERTEXATTRIBI4SVPROC                               glVertexAttribI4sv                              = nullptr;
PFNGLVERTEXATTRIBI4UBVPROC                              glVertexAttribI4ubv                             = nullptr;
PFNGLVERTEXATTRIBI4USVPROC                              glVertexAttribI4usv                             = nullptr;
PFNGLGETUNIFORMUIVPROC                                  glGetUniformuiv                                 = nullptr;
PFNGLBINDFRAGDATALOCATIONPROC                           glBindFragDataLocation                          = nullptr;
PFNGLGETFRAGDATALOCATIONPROC                            glGetFragDataLocation                           = nullptr;
PFNGLUNIFORM1UIPROC                                     glUniform1ui                                    = nullptr;
PFNGLUNIFORM2UIPROC                                     glUniform2ui                                    = nullptr;
PFNGLUNIFORM3UIPROC                                     glUniform3ui                                    = nullptr;
PFNGLUNIFORM4UIPROC                                     glUniform4ui                                    = nullptr;
PFNGLUNIFORM1UIVPROC                                    glUniform1uiv                                   = nullptr;
PFNGLUNIFORM2UIVPROC                                    glUniform2uiv                                   = nullptr;
PFNGLUNIFORM3UIVPROC                                    glUniform3uiv                                   = nullptr;
PFNGLUNIFORM4UIVPROC                                    glUniform4uiv                                   = nullptr;
PFNGLTEXPARAMETERIIVPROC                                glTexParameterIiv                               = nullptr;
PFNGLTEXPARAMETERIUIVPROC                               glTexParameterIuiv                              = nullptr;
PFNGLGETTEXPARAMETERIIVPROC                             glGetTexParameterIiv                            = nullptr;
PFNGLGETTEXPARAMETERIUIVPROC                            glGetTexParameterIuiv                           = nullptr;
PFNGLCLEARBUFFERIVPROC                                  glClearBufferiv                                 = nullptr;
PFNGLCLEARBUFFERUIVPROC                                 glClearBufferuiv                                = nullptr;
PFNGLCLEARBUFFERFVPROC                                  glClearBufferfv                                 = nullptr;
PFNGLCLEARBUFFERFIPROC                                  glClearBufferfi                                 = nullptr;
PFNGLGETSTRINGIPROC                                     glGetStringi                                    = nullptr;
#endif

/*  GL_VERSION_3_1  */ 
#if 1
PFNGLDRAWARRAYSINSTANCEDPROC                            glDrawArraysInstanced                           = nullptr;
PFNGLDRAWELEMENTSINSTANCEDPROC                          glDrawElementsInstanced                         = nullptr;
PFNGLTEXBUFFERPROC                                      glTexBuffer                                     = nullptr;
PFNGLPRIMITIVERESTARTINDEXPROC                          glPrimitiveRestartIndex                         = nullptr;
#endif

/*  GL_VERSION_3_2  */ 
#if 1
PFNGLGETINTEGER64I_VPROC                                glGetInteger64i_v                               = nullptr;
PFNGLGETBUFFERPARAMETERI64VPROC                         glGetBufferParameteri64v                        = nullptr;
PFNGLFRAMEBUFFERTEXTUREPROC                             glFramebufferTexture                            = nullptr;
#endif

/*  GL_ARB_framebuffer_object  */ 
#if 1
PFNGLISRENDERBUFFERPROC                                 glIsRenderbuffer                                = nullptr;
PFNGLBINDRENDERBUFFERPROC                               glBindRenderbuffer                              = nullptr;
PFNGLDELETERENDERBUFFERSPROC                            glDeleteRenderbuffers                           = nullptr;
PFNGLGENRENDERBUFFERSPROC                               glGenRenderbuffers                              = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC                            glRenderbufferStorage                           = nullptr;
PFNGLGETRENDERBUFFERPARAMETERIVPROC                     glGetRenderbufferParameteriv                    = nullptr;
PFNGLISFRAMEBUFFERPROC                                  glIsFramebuffer                                 = nullptr;
PFNGLBINDFRAMEBUFFERPROC                                glBindFramebuffer                               = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC                             glDeleteFramebuffers                            = nullptr;
PFNGLGENFRAMEBUFFERSPROC                                glGenFramebuffers                               = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC                         glCheckFramebufferStatus                        = nullptr;
PFNGLFRAMEBUFFERTEXTURE1DPROC                           glFramebufferTexture1D                          = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC                           glFramebufferTexture2D                          = nullptr;
PFNGLFRAMEBUFFERTEXTURE3DPROC                           glFramebufferTexture3D                          = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC                        glFramebufferRenderbuffer                       = nullptr;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC            glGetFramebufferAttachmentParameteriv           = nullptr;
PFNGLGENERATEMIPMAPPROC                                 glGenerateMipmap                                = nullptr;
PFNGLBLITFRAMEBUFFERPROC                                glBlitFramebuffer                               = nullptr;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC                 glRenderbufferStorageMultisample                = nullptr;
PFNGLFRAMEBUFFERTEXTURELAYERPROC                        glFramebufferTextureLayer                       = nullptr;
#endif

/*  GL_ARB_map_buffer_range  */ 
#if 1
PFNGLMAPBUFFERRANGEPROC                                 glMapBufferRange                                = nullptr;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC                         glFlushMappedBufferRange                        = nullptr;
#endif

/*  GL_ARB_vertex_array_object  */ 
#if 1
PFNGLBINDVERTEXARRAYPROC                                glBindVertexArray                               = nullptr;
PFNGLDELETEVERTEXARRAYSPROC                             glDeleteVertexArrays                            = nullptr;
PFNGLGENVERTEXARRAYSPROC                                glGenVertexArrays                               = nullptr;
PFNGLISVERTEXARRAYPROC                                  glIsVertexArray                                 = nullptr;
#endif

/*  GL_ARB_uniform_buffer_object  */
#if 1
PFNGLGETUNIFORMINDICESPROC                              glGetUniformIndices                             = nullptr;
PFNGLGETACTIVEUNIFORMSIVPROC                            glGetActiveUniformsiv                           = nullptr;
PFNGLGETACTIVEUNIFORMNAMEPROC                           glGetActiveUniformName                          = nullptr;
PFNGLGETUNIFORMBLOCKINDEXPROC                           glGetUniformBlockIndex                          = nullptr;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC                        glGetActiveUniformBlockiv                       = nullptr;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC                      glGetActiveUniformBlockName                     = nullptr;
PFNGLUNIFORMBLOCKBINDINGPROC                            glUniformBlockBinding                           = nullptr;
#endif

/*  GL_ARB_copy_buffer  */ 
PFNGLCOPYBUFFERSUBDATAPROC                              glCopyBufferSubData                             = nullptr;

/*  GL_ARB_draw_elements_base_vertex  */ 
#if 1
PFNGLDRAWELEMENTSBASEVERTEXPROC                         glDrawElementsBaseVertex                        = nullptr;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC                    glDrawRangeElementsBaseVertex                   = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC                glDrawElementsInstancedBaseVertex               = nullptr;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC                    glMultiDrawElementsBaseVertex                   = nullptr;
#endif

/*  GL_ARB_provoking_vertex  */ 
PFNGLPROVOKINGVERTEXPROC                                glProvokingVertex                               = nullptr;

/*  GL_ARB_sync  */
#if 1
PFNGLFENCESYNCPROC                                      glFenceSync                                     = nullptr;
PFNGLISSYNCPROC                                         glIsSync                                        = nullptr;
PFNGLDELETESYNCPROC                                     glDeleteSync                                    = nullptr;
PFNGLCLIENTWAITSYNCPROC                                 glClientWaitSync                                = nullptr;
PFNGLWAITSYNCPROC                                       glWaitSync                                      = nullptr;
PFNGLGETINTEGER64VPROC                                  glGetInteger64v                                 = nullptr;
PFNGLGETSYNCIVPROC                                      glGetSynciv                                     = nullptr;
#endif

/*  GL_ARB_texture_multisample  */ 
#if 1
PFNGLTEXIMAGE2DMULTISAMPLEPROC                          glTexImage2DMultisample                         = nullptr;
PFNGLTEXIMAGE3DMULTISAMPLEPROC                          glTexImage3DMultisample                         = nullptr;
PFNGLGETMULTISAMPLEFVPROC                               glGetMultisamplefv                              = nullptr;
PFNGLSAMPLEMASKIPROC                                    glSampleMaski                                   = nullptr;
#endif

/*  GL_ARB_sample_shading  */ 
PFNGLMINSAMPLESHADINGARBPROC                            glMinSampleShading                              = nullptr;

/*  GL_ARB_sampler_objects  */ 
#if 1
PFNGLGENSAMPLERSPROC                                    glGenSamplers                                   = nullptr;
PFNGLDELETESAMPLERSPROC                                 glDeleteSamplers                                = nullptr;
PFNGLISSAMPLERPROC                                      glIsSampler                                     = nullptr;
PFNGLBINDSAMPLERPROC                                    glBindSampler                                   = nullptr;
PFNGLSAMPLERPARAMETERIPROC                              glSamplerParameteri                             = nullptr;
PFNGLSAMPLERPARAMETERIVPROC                             glSamplerParameteriv                            = nullptr;
PFNGLSAMPLERPARAMETERFPROC                              glSamplerParameterf                             = nullptr;
PFNGLSAMPLERPARAMETERFVPROC                             glSamplerParameterfv                            = nullptr;
PFNGLSAMPLERPARAMETERIIVPROC                            glSamplerParameterIiv                           = nullptr;
PFNGLSAMPLERPARAMETERIUIVPROC                           glSamplerParameterIuiv                          = nullptr;
PFNGLGETSAMPLERPARAMETERIVPROC                          glGetSamplerParameteriv                         = nullptr;
PFNGLGETSAMPLERPARAMETERIIVPROC                         glGetSamplerParameterIiv                        = nullptr;
PFNGLGETSAMPLERPARAMETERFVPROC                          glGetSamplerParameterfv                         = nullptr;
PFNGLGETSAMPLERPARAMETERIUIVPROC                        glGetSamplerParameterIuiv                       = nullptr;
#endif

/*  GL_ARB_timer_query  */ 
#if 1
PFNGLQUERYCOUNTERPROC                                   glQueryCounter                                  = nullptr;
PFNGLGETQUERYOBJECTI64VPROC                             glGetQueryObjecti64v                            = nullptr;
PFNGLGETQUERYOBJECTUI64VPROC                            glGetQueryObjectui64v                           = nullptr;
#endif

/*  GL_ARB_tessellation_shader  */ 
PFNGLPATCHPARAMETERIPROC                                glPatchParameteri                               = nullptr;
PFNGLPATCHPARAMETERFVPROC                               glPatchParameterfv                              = nullptr;

/*  GL_ARB_get_program_binary  */ 
PFNGLGETPROGRAMBINARYPROC                               glGetProgramBinary                              = nullptr;
PFNGLPROGRAMBINARYPROC                                  glProgramBinary                                 = nullptr;
PFNGLPROGRAMPARAMETERIPROC                              glProgramParameteri                             = nullptr;

/*  GL_ARB_base_instance  */
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC                glDrawArraysInstancedBaseInstance               = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC              glDrawElementsInstancedBaseInstance             = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC    glDrawElementsInstancedBaseVertexBaseInstance   = nullptr;

/*  GL_ARB_transform_feedback2  */
#if 1
PFNGLBINDTRANSFORMFEEDBACKPROC                          glBindTransformFeedback                         = nullptr;
PFNGLDELETETRANSFORMFEEDBACKSPROC                       glDeleteTransformFeedbacks                      = nullptr;
PFNGLGENTRANSFORMFEEDBACKSPROC                          glGenTransformFeedbacks                         = nullptr;
PFNGLISTRANSFORMFEEDBACKPROC                            glIsTransformFeedback                           = nullptr;
PFNGLPAUSETRANSFORMFEEDBACKPROC                         glPauseTransformFeedback                        = nullptr;
PFNGLRESUMETRANSFORMFEEDBACKPROC                        glResumeTransformFeedback                       = nullptr;
PFNGLDRAWTRANSFORMFEEDBACKPROC                          glDrawTransformFeedback                         = nullptr;
#endif

/*  GL_ARB_transform_feedback3  */
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC                    glDrawTransformFeedbackStream                   = nullptr;
PFNGLBEGINQUERYINDEXEDPROC                              glBeginQueryIndexed                             = nullptr;
PFNGLENDQUERYINDEXEDPROC                                glEndQueryIndexed                               = nullptr;
PFNGLGETQUERYINDEXEDIVPROC                              glGetQueryIndexediv                             = nullptr;

/*  GL_ARB_transform_feedback_instanced  */
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC                 glDrawTransformFeedbackInstanced                = nullptr;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC           glDrawTransformFeedbackStreamInstanced          = nullptr;

/*  GL_VERSION_3_3  */ 
PFNGLVERTEXATTRIBDIVISORPROC                             glVertexAttribDivisor                           = nullptr;

/*  GL_ARB_texture_storage  */
PFNGLTEXSTORAGE1DPROC                                    glTexStorage1D = nullptr;
PFNGLTEXSTORAGE2DPROC                                    glTexStorage2D = nullptr;
PFNGLTEXSTORAGE3DPROC                                    glTexStorage3D = nullptr;

} }

#endif


namespace renderstack { namespace toolkit {

using namespace gl::detail;

void window::get_extensions()
{
    /*  print_all_extensions(window);  */ 

#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)
    gl::detail::glCullFace                                      = (PFNGLCULLFACEPROC                                    )get_proc_address("glCullFace");
    gl::detail::glFrontFace                                     = (PFNGLFRONTFACEPROC                                   )get_proc_address("glFrontFace");
    gl::detail::glHint                                          = (PFNGLHINTPROC                                        )get_proc_address("glHint");
    gl::detail::glLineWidth                                     = (PFNGLLINEWIDTHPROC                                   )get_proc_address("glLineWidth");
    gl::detail::glPointSize                                     = (PFNGLPOINTSIZEPROC                                   )get_proc_address("glPointSize");
    gl::detail::glPolygonMode                                   = (PFNGLPOLYGONMODEPROC                                 )get_proc_address("glPolygonMode");
    gl::detail::glScissor                                       = (PFNGLSCISSORPROC                                     )get_proc_address("glScissor");
    gl::detail::glTexParameterf                                 = (PFNGLTEXPARAMETERFPROC                               )get_proc_address("glTexParameterf");
    gl::detail::glTexParameterfv                                = (PFNGLTEXPARAMETERFVPROC                              )get_proc_address("glTexParameterfv");
    gl::detail::glTexParameteri                                 = (PFNGLTEXPARAMETERIPROC                               )get_proc_address("glTexParameteri");
    gl::detail::glTexParameteriv                                = (PFNGLTEXPARAMETERIVPROC                              )get_proc_address("glTexParameteriv");
    gl::detail::glTexImage1D                                    = (PFNGLTEXIMAGE1DPROC                                  )get_proc_address("glTexImage1D");
    gl::detail::glTexImage2D                                    = (PFNGLTEXIMAGE2DPROC                                  )get_proc_address("glTexImage2D");
    gl::detail::glDrawBuffer                                    = (PFNGLDRAWBUFFERPROC                                  )get_proc_address("glDrawBuffer");
    gl::detail::glClear                                         = (PFNGLCLEARPROC                                       )get_proc_address("glClear");
    gl::detail::glClearColor                                    = (PFNGLCLEARCOLORPROC                                  )get_proc_address("glClearColor");
    gl::detail::glClearStencil                                  = (PFNGLCLEARSTENCILPROC                                )get_proc_address("glClearStencil");
    gl::detail::glClearDepth                                    = (PFNGLCLEARDEPTHPROC                                  )get_proc_address("glClearDepth");
    gl::detail::glStencilMask                                   = (PFNGLSTENCILMASKPROC                                 )get_proc_address("glStencilMask");
    gl::detail::glColorMask                                     = (PFNGLCOLORMASKPROC                                   )get_proc_address("glColorMask");
    gl::detail::glDepthMask                                     = (PFNGLDEPTHMASKPROC                                   )get_proc_address("glDepthMask");
    gl::detail::glDisable                                       = (PFNGLDISABLEPROC                                     )get_proc_address("glDisable");
    gl::detail::glEnable                                        = (PFNGLENABLEPROC                                      )get_proc_address("glEnable");
    gl::detail::glFinish                                        = (PFNGLFINISHPROC                                      )get_proc_address("glFinish");
    gl::detail::glFlush                                         = (PFNGLFLUSHPROC                                       )get_proc_address("glFlush");
    gl::detail::glBlendFunc                                     = (PFNGLBLENDFUNCPROC                                   )get_proc_address("glBlendFunc");
    gl::detail::glLogicOp                                       = (PFNGLLOGICOPPROC                                     )get_proc_address("glLogicOp");
    gl::detail::glStencilFunc                                   = (PFNGLSTENCILFUNCPROC                                 )get_proc_address("glStencilFunc");
    gl::detail::glStencilOp                                     = (PFNGLSTENCILOPPROC                                   )get_proc_address("glStencilOp");
    gl::detail::glDepthFunc                                     = (PFNGLDEPTHFUNCPROC                                   )get_proc_address("glDepthFunc");
    gl::detail::glPixelStoref                                   = (PFNGLPIXELSTOREFPROC                                 )get_proc_address("glPixelStoref");
    gl::detail::glPixelStorei                                   = (PFNGLPIXELSTOREIPROC                                 )get_proc_address("glPixelStorei");
    gl::detail::glReadBuffer                                    = (PFNGLREADBUFFERPROC                                  )get_proc_address("glReadBuffer");
    gl::detail::glReadPixels                                    = (PFNGLREADPIXELSPROC                                  )get_proc_address("glReadPixels");
    gl::detail::glGetBooleanv                                   = (PFNGLGETBOOLEANVPROC                                 )get_proc_address("glGetBooleanv");
    gl::detail::glGetDoublev                                    = (PFNGLGETDOUBLEVPROC                                  )get_proc_address("glGetDoublev");
    gl::detail::glGetError                                      = (PFNGLGETERRORPROC                                    )get_proc_address("glGetError");
    gl::detail::glGetFloatv                                     = (PFNGLGETFLOATVPROC                                   )get_proc_address("glGetFloatv");
    gl::detail::glGetIntegerv                                   = (PFNGLGETINTEGERVPROC                                 )get_proc_address("glGetIntegerv");
    gl::detail::glGetString                                     = (PFNGLGETSTRINGPROC                                   )get_proc_address("glGetString");
    gl::detail::glGetTexImage                                   = (PFNGLGETTEXIMAGEPROC                                 )get_proc_address("glGetTexImage");
    gl::detail::glGetTexParameterfv                             = (PFNGLGETTEXPARAMETERFVPROC                           )get_proc_address("glGetTexParameterfv");
    gl::detail::glGetTexParameteriv                             = (PFNGLGETTEXPARAMETERIVPROC                           )get_proc_address("glGetTexParameteriv");
    gl::detail::glGetTexLevelParameterfv                        = (PFNGLGETTEXLEVELPARAMETERFVPROC                      )get_proc_address("glGetTexLevelParameterfv");
    gl::detail::glGetTexLevelParameteriv                        = (PFNGLGETTEXLEVELPARAMETERIVPROC                      )get_proc_address("glGetTexLevelParameteriv");
    gl::detail::glIsEnabled                                     = (PFNGLISENABLEDPROC                                   )get_proc_address("glIsEnabled");
    gl::detail::glDepthRange                                    = (PFNGLDEPTHRANGEPROC                                  )get_proc_address("glDepthRange");
    gl::detail::glViewport                                      = (PFNGLVIEWPORTPROC                                    )get_proc_address("glViewport");

    gl::detail::glDrawArrays                                    = (PFNGLDRAWARRAYSPROC                                  )get_proc_address("glDrawArrays");
    gl::detail::glDrawElements                                  = (PFNGLDRAWELEMENTSPROC                                )get_proc_address("glDrawElements");
    gl::detail::glGetPointerv                                   = (PFNGLGETPOINTERVPROC                                 )get_proc_address("glGetPointerv");
    gl::detail::glPolygonOffset                                 = (PFNGLPOLYGONOFFSETPROC                               )get_proc_address("glPolygonOffset");
    gl::detail::glCopyTexImage1D                                = (PFNGLCOPYTEXIMAGE1DPROC                              )get_proc_address("glCopyTexImage1D");
    gl::detail::glCopyTexImage2D                                = (PFNGLCOPYTEXIMAGE2DPROC                              )get_proc_address("glCopyTexImage2D");
    gl::detail::glCopyTexSubImage1D                             = (PFNGLCOPYTEXSUBIMAGE1DPROC                           )get_proc_address("glCopyTexSubImage1D");
    gl::detail::glCopyTexSubImage2D                             = (PFNGLCOPYTEXSUBIMAGE2DPROC                           )get_proc_address("glCopyTexSubImage2D");
    gl::detail::glTexSubImage1D                                 = (PFNGLTEXSUBIMAGE1DPROC                               )get_proc_address("glTexSubImage1D");
    gl::detail::glTexSubImage2D                                 = (PFNGLTEXSUBIMAGE2DPROC                               )get_proc_address("glTexSubImage2D");
    gl::detail::glBindTexture                                   = (PFNGLBINDTEXTUREPROC                                 )get_proc_address("glBindTexture");
    gl::detail::glDeleteTextures                                = (PFNGLDELETETEXTURESPROC                              )get_proc_address("glDeleteTextures");
    gl::detail::glGenTextures                                   = (PFNGLGENTEXTURESPROC                                 )get_proc_address("glGenTextures");
    gl::detail::glIsTexture                                     = (PFNGLISTEXTUREPROC                                   )get_proc_address("glIsTexture");
#endif

    //check_gl_error();
#if defined(EXT_FULL)
    gl::detail::glBlendColor                                    = (PFNGLBLENDCOLORPROC                                  )get_proc_address("glBlendColor");
    gl::detail::glBlendEquation                                 = (PFNGLBLENDEQUATIONPROC                               )get_proc_address("glBlendEquation");
    gl::detail::glDrawRangeElements                             = (PFNGLDRAWRANGEELEMENTSPROC                           )get_proc_address("glDrawRangeElements");
    gl::detail::glTexImage3D                                    = (PFNGLTEXIMAGE3DPROC                                  )get_proc_address("glTexImage3D");
    gl::detail::glTexSubImage3D                                 = (PFNGLTEXSUBIMAGE3DPROC                               )get_proc_address("glTexSubImage3D");
    gl::detail::glCopyTexSubImage3D                             = (PFNGLCOPYTEXSUBIMAGE3DPROC                           )get_proc_address("glCopyTexSubImage3D");
    gl::detail::glActiveTexture                                 = (PFNGLACTIVETEXTUREPROC                               )get_proc_address("glActiveTexture");
    gl::detail::glSampleCoverage                                = (PFNGLSAMPLECOVERAGEPROC                              )get_proc_address("glSampleCoverage");
    gl::detail::glCompressedTexImage3D                          = (PFNGLCOMPRESSEDTEXIMAGE3DPROC                        )get_proc_address("glCompressedTexImage3D");
    gl::detail::glCompressedTexImage2D                          = (PFNGLCOMPRESSEDTEXIMAGE2DPROC                        )get_proc_address("glCompressedTexImage2D");
    gl::detail::glCompressedTexImage1D                          = (PFNGLCOMPRESSEDTEXIMAGE1DPROC                        )get_proc_address("glCompressedTexImage1D");
    gl::detail::glCompressedTexSubImage3D                       = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC                     )get_proc_address("glCompressedTexSubImage3D");
    gl::detail::glCompressedTexSubImage2D                       = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC                     )get_proc_address("glCompressedTexSubImage2D");
    gl::detail::glCompressedTexSubImage1D                       = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC                     )get_proc_address("glCompressedTexSubImage1D");
    gl::detail::glGetCompressedTexImage                         = (PFNGLGETCOMPRESSEDTEXIMAGEPROC                       )get_proc_address("glGetCompressedTexImage");
    gl::detail::glBlendFuncSeparate                             = (PFNGLBLENDFUNCSEPARATEPROC                           )get_proc_address("glBlendFuncSeparate");
    gl::detail::glMultiDrawArrays                               = (PFNGLMULTIDRAWARRAYSPROC                             )get_proc_address("glMultiDrawArrays");
    gl::detail::glMultiDrawElements                             = (PFNGLMULTIDRAWELEMENTSPROC                           )get_proc_address("glMultiDrawElements");
    gl::detail::glPointParameterf                               = (PFNGLPOINTPARAMETERFPROC                             )get_proc_address("glPointParameterf");
    gl::detail::glPointParameterfv                              = (PFNGLPOINTPARAMETERFVPROC                            )get_proc_address("glPointParameterfv");
    gl::detail::glPointParameteri                               = (PFNGLPOINTPARAMETERIPROC                             )get_proc_address("glPointParameteri");
    gl::detail::glPointParameteriv                              = (PFNGLPOINTPARAMETERIVPROC                            )get_proc_address("glPointParameteriv");
    gl::detail::glGenQueries                                    = (PFNGLGENQUERIESPROC                                  )get_proc_address("glGenQueries");
    gl::detail::glDeleteQueries                                 = (PFNGLDELETEQUERIESPROC                               )get_proc_address("glDeleteQueries");
    gl::detail::glIsQuery                                       = (PFNGLISQUERYPROC                                     )get_proc_address("glIsQuery");
    gl::detail::glBeginQuery                                    = (PFNGLBEGINQUERYPROC                                  )get_proc_address("glBeginQuery");
    gl::detail::glEndQuery                                      = (PFNGLENDQUERYPROC                                    )get_proc_address("glEndQuery");
    gl::detail::glGetQueryiv                                    = (PFNGLGETQUERYIVPROC                                  )get_proc_address("glGetQueryiv");
    gl::detail::glGetQueryObjectiv                              = (PFNGLGETQUERYOBJECTIVPROC                            )get_proc_address("glGetQueryObjectiv");
    gl::detail::glGetQueryObjectuiv                             = (PFNGLGETQUERYOBJECTUIVPROC                           )get_proc_address("glGetQueryObjectuiv");
    gl::detail::glBindBuffer                                    = (PFNGLBINDBUFFERPROC                                  )get_proc_address("glBindBuffer");
    gl::detail::glDeleteBuffers                                 = (PFNGLDELETEBUFFERSPROC                               )get_proc_address("glDeleteBuffers");
    gl::detail::glGenBuffers                                    = (PFNGLGENBUFFERSPROC                                  )get_proc_address("glGenBuffers");
    gl::detail::glIsBuffer                                      = (PFNGLISBUFFERPROC                                    )get_proc_address("glIsBuffer");
    gl::detail::glBufferData                                    = (PFNGLBUFFERDATAPROC                                  )get_proc_address("glBufferData");
    gl::detail::glBufferSubData                                 = (PFNGLBUFFERSUBDATAPROC                               )get_proc_address("glBufferSubData");
    gl::detail::glGetBufferSubData                              = (PFNGLGETBUFFERSUBDATAPROC                            )get_proc_address("glGetBufferSubData");
    gl::detail::glMapBuffer                                     = (PFNGLMAPBUFFERPROC                                   )get_proc_address("glMapBuffer");
    gl::detail::glUnmapBuffer                                   = (PFNGLUNMAPBUFFERPROC                                 )get_proc_address("glUnmapBuffer");
    gl::detail::glGetBufferParameteriv                          = (PFNGLGETBUFFERPARAMETERIVPROC                        )get_proc_address("glGetBufferParameteriv");
    gl::detail::glGetBufferPointerv                             = (PFNGLGETBUFFERPOINTERVPROC                           )get_proc_address("glGetBufferPointerv");
    gl::detail::glBlendEquationSeparate                         = (PFNGLBLENDEQUATIONSEPARATEPROC                       )get_proc_address("glBlendEquationSeparate");
    gl::detail::glDrawBuffers                                   = (PFNGLDRAWBUFFERSPROC                                 )get_proc_address("glDrawBuffers");
    gl::detail::glStencilOpSeparate                             = (PFNGLSTENCILOPSEPARATEPROC                           )get_proc_address("glStencilOpSeparate");
    gl::detail::glStencilFuncSeparate                           = (PFNGLSTENCILFUNCSEPARATEPROC                         )get_proc_address("glStencilFuncSeparate");
    gl::detail::glStencilMaskSeparate                           = (PFNGLSTENCILMASKSEPARATEPROC                         )get_proc_address("glStencilMaskSeparate");
    gl::detail::glAttachShader                                  = (PFNGLATTACHSHADERPROC                                )get_proc_address("glAttachShader");
    gl::detail::glBindAttribLocation                            = (PFNGLBINDATTRIBLOCATIONPROC                          )get_proc_address("glBindAttribLocation");
    gl::detail::glCompileShader                                 = (PFNGLCOMPILESHADERPROC                               )get_proc_address("glCompileShader");
    gl::detail::glCreateProgram                                 = (PFNGLCREATEPROGRAMPROC                               )get_proc_address("glCreateProgram");
    gl::detail::glCreateShader                                  = (PFNGLCREATESHADERPROC                                )get_proc_address("glCreateShader");
    gl::detail::glDeleteProgram                                 = (PFNGLDELETEPROGRAMPROC                               )get_proc_address("glDeleteProgram");
    gl::detail::glDeleteShader                                  = (PFNGLDELETESHADERPROC                                )get_proc_address("glDeleteShader");
    gl::detail::glDetachShader                                  = (PFNGLDETACHSHADERPROC                                )get_proc_address("glDetachShader");
    gl::detail::glDisableVertexAttribArray                      = (PFNGLDISABLEVERTEXATTRIBARRAYPROC                    )get_proc_address("glDisableVertexAttribArray");
    gl::detail::glEnableVertexAttribArray                       = (PFNGLENABLEVERTEXATTRIBARRAYPROC                     )get_proc_address("glEnableVertexAttribArray");
    gl::detail::glGetActiveAttrib                               = (PFNGLGETACTIVEATTRIBPROC                             )get_proc_address("glGetActiveAttrib");
    gl::detail::glGetActiveUniform                              = (PFNGLGETACTIVEUNIFORMPROC                            )get_proc_address("glGetActiveUniform");
    gl::detail::glGetAttachedShaders                            = (PFNGLGETATTACHEDSHADERSPROC                          )get_proc_address("glGetAttachedShaders");
    gl::detail::glGetAttribLocation                             = (PFNGLGETATTRIBLOCATIONPROC                           )get_proc_address("glGetAttribLocation");
    gl::detail::glGetProgramiv                                  = (PFNGLGETPROGRAMIVPROC                                )get_proc_address("glGetProgramiv");
    gl::detail::glGetProgramInfoLog                             = (PFNGLGETPROGRAMINFOLOGPROC                           )get_proc_address("glGetProgramInfoLog");
    gl::detail::glGetShaderiv                                   = (PFNGLGETSHADERIVPROC                                 )get_proc_address("glGetShaderiv");
    gl::detail::glGetShaderInfoLog                              = (PFNGLGETSHADERINFOLOGPROC                            )get_proc_address("glGetShaderInfoLog");
    gl::detail::glGetShaderSource                               = (PFNGLGETSHADERSOURCEPROC                             )get_proc_address("glGetShaderSource");
    gl::detail::glGetUniformLocation                            = (PFNGLGETUNIFORMLOCATIONPROC                          )get_proc_address("glGetUniformLocation");
    gl::detail::glGetUniformfv                                  = (PFNGLGETUNIFORMFVPROC                                )get_proc_address("glGetUniformfv");
    gl::detail::glGetUniformiv                                  = (PFNGLGETUNIFORMIVPROC                                )get_proc_address("glGetUniformiv");
    gl::detail::glGetVertexAttribdv                             = (PFNGLGETVERTEXATTRIBDVPROC                           )get_proc_address("glGetVertexAttribdv");
    gl::detail::glGetVertexAttribfv                             = (PFNGLGETVERTEXATTRIBFVPROC                           )get_proc_address("glGetVertexAttribfv");
    gl::detail::glGetVertexAttribiv                             = (PFNGLGETVERTEXATTRIBIVPROC                           )get_proc_address("glGetVertexAttribiv");
    gl::detail::glGetVertexAttribPointerv                       = (PFNGLGETVERTEXATTRIBPOINTERVPROC                     )get_proc_address("glGetVertexAttribPointerv");
    gl::detail::glIsProgram                                     = (PFNGLISPROGRAMPROC                                   )get_proc_address("glIsProgram");
    gl::detail::glIsShader                                      = (PFNGLISSHADERPROC                                    )get_proc_address("glIsShader");
    gl::detail::glLinkProgram                                   = (PFNGLLINKPROGRAMPROC                                 )get_proc_address("glLinkProgram");
    gl::detail::glShaderSource                                  = (PFNGLSHADERSOURCEPROC                                )get_proc_address("glShaderSource");
    gl::detail::glUseProgram                                    = (PFNGLUSEPROGRAMPROC                                  )get_proc_address("glUseProgram");
    gl::detail::glUniform1f                                     = (PFNGLUNIFORM1FPROC                                   )get_proc_address("glUniform1f");
    gl::detail::glUniform2f                                     = (PFNGLUNIFORM2FPROC                                   )get_proc_address("glUniform2f");
    gl::detail::glUniform3f                                     = (PFNGLUNIFORM3FPROC                                   )get_proc_address("glUniform3f");
    gl::detail::glUniform4f                                     = (PFNGLUNIFORM4FPROC                                   )get_proc_address("glUniform4f");
    gl::detail::glUniform1i                                     = (PFNGLUNIFORM1IPROC                                   )get_proc_address("glUniform1i");
    gl::detail::glUniform2i                                     = (PFNGLUNIFORM2IPROC                                   )get_proc_address("glUniform2i");
    gl::detail::glUniform3i                                     = (PFNGLUNIFORM3IPROC                                   )get_proc_address("glUniform3i");
    gl::detail::glUniform4i                                     = (PFNGLUNIFORM4IPROC                                   )get_proc_address("glUniform4i");
    gl::detail::glUniform1fv                                    = (PFNGLUNIFORM1FVPROC                                  )get_proc_address("glUniform1fv");
    gl::detail::glUniform2fv                                    = (PFNGLUNIFORM2FVPROC                                  )get_proc_address("glUniform2fv");
    gl::detail::glUniform3fv                                    = (PFNGLUNIFORM3FVPROC                                  )get_proc_address("glUniform3fv");
    gl::detail::glUniform4fv                                    = (PFNGLUNIFORM4FVPROC                                  )get_proc_address("glUniform4fv");
    gl::detail::glUniform1iv                                    = (PFNGLUNIFORM1IVPROC                                  )get_proc_address("glUniform1iv");
    gl::detail::glUniform2iv                                    = (PFNGLUNIFORM2IVPROC                                  )get_proc_address("glUniform2iv");
    gl::detail::glUniform3iv                                    = (PFNGLUNIFORM3IVPROC                                  )get_proc_address("glUniform3iv");
    gl::detail::glUniform4iv                                    = (PFNGLUNIFORM4IVPROC                                  )get_proc_address("glUniform4iv");
    gl::detail::glUniformMatrix2fv                              = (PFNGLUNIFORMMATRIX2FVPROC                            )get_proc_address("glUniformMatrix2fv");
    gl::detail::glUniformMatrix3fv                              = (PFNGLUNIFORMMATRIX3FVPROC                            )get_proc_address("glUniformMatrix3fv");
    gl::detail::glUniformMatrix4fv                              = (PFNGLUNIFORMMATRIX4FVPROC                            )get_proc_address("glUniformMatrix4fv");
    gl::detail::glValidateProgram                               = (PFNGLVALIDATEPROGRAMPROC                             )get_proc_address("glValidateProgram");
    gl::detail::glVertexAttrib1d                                = (PFNGLVERTEXATTRIB1DPROC                              )get_proc_address("glVertexAttrib1d");
    gl::detail::glVertexAttrib1dv                               = (PFNGLVERTEXATTRIB1DVPROC                             )get_proc_address("glVertexAttrib1dv");
    gl::detail::glVertexAttrib1f                                = (PFNGLVERTEXATTRIB1FPROC                              )get_proc_address("glVertexAttrib1f");
    gl::detail::glVertexAttrib1fv                               = (PFNGLVERTEXATTRIB1FVPROC                             )get_proc_address("glVertexAttrib1fv");
    gl::detail::glVertexAttrib1s                                = (PFNGLVERTEXATTRIB1SPROC                              )get_proc_address("glVertexAttrib1s");
    gl::detail::glVertexAttrib1sv                               = (PFNGLVERTEXATTRIB1SVPROC                             )get_proc_address("glVertexAttrib1sv");
    gl::detail::glVertexAttrib2d                                = (PFNGLVERTEXATTRIB2DPROC                              )get_proc_address("glVertexAttrib2d");
    gl::detail::glVertexAttrib2dv                               = (PFNGLVERTEXATTRIB2DVPROC                             )get_proc_address("glVertexAttrib2dv");
    gl::detail::glVertexAttrib2f                                = (PFNGLVERTEXATTRIB2FPROC                              )get_proc_address("glVertexAttrib2f");
    gl::detail::glVertexAttrib2fv                               = (PFNGLVERTEXATTRIB2FVPROC                             )get_proc_address("glVertexAttrib2fv");
    gl::detail::glVertexAttrib2s                                = (PFNGLVERTEXATTRIB2SPROC                              )get_proc_address("glVertexAttrib2s");
    gl::detail::glVertexAttrib2sv                               = (PFNGLVERTEXATTRIB2SVPROC                             )get_proc_address("glVertexAttrib2sv");
    gl::detail::glVertexAttrib3d                                = (PFNGLVERTEXATTRIB3DPROC                              )get_proc_address("glVertexAttrib3d");
    gl::detail::glVertexAttrib3dv                               = (PFNGLVERTEXATTRIB3DVPROC                             )get_proc_address("glVertexAttrib3dv");
    gl::detail::glVertexAttrib3f                                = (PFNGLVERTEXATTRIB3FPROC                              )get_proc_address("glVertexAttrib3f");
    gl::detail::glVertexAttrib3fv                               = (PFNGLVERTEXATTRIB3FVPROC                             )get_proc_address("glVertexAttrib3fv");
    gl::detail::glVertexAttrib3s                                = (PFNGLVERTEXATTRIB3SPROC                              )get_proc_address("glVertexAttrib3s");
    gl::detail::glVertexAttrib3sv                               = (PFNGLVERTEXATTRIB3SVPROC                             )get_proc_address("glVertexAttrib3sv");
    gl::detail::glVertexAttrib4Nbv                              = (PFNGLVERTEXATTRIB4NBVPROC                            )get_proc_address("glVertexAttrib4Nbv");
    gl::detail::glVertexAttrib4Niv                              = (PFNGLVERTEXATTRIB4NIVPROC                            )get_proc_address("glVertexAttrib4Niv");
    gl::detail::glVertexAttrib4Nsv                              = (PFNGLVERTEXATTRIB4NSVPROC                            )get_proc_address("glVertexAttrib4Nsv");
    gl::detail::glVertexAttrib4Nub                              = (PFNGLVERTEXATTRIB4NUBPROC                            )get_proc_address("glVertexAttrib4Nub");
    gl::detail::glVertexAttrib4Nubv                             = (PFNGLVERTEXATTRIB4NUBVPROC                           )get_proc_address("glVertexAttrib4Nubv");
    gl::detail::glVertexAttrib4Nuiv                             = (PFNGLVERTEXATTRIB4NUIVPROC                           )get_proc_address("glVertexAttrib4Nuiv");
    gl::detail::glVertexAttrib4Nusv                             = (PFNGLVERTEXATTRIB4NUSVPROC                           )get_proc_address("glVertexAttrib4Nusv");
    gl::detail::glVertexAttrib4bv                               = (PFNGLVERTEXATTRIB4BVPROC                             )get_proc_address("glVertexAttrib4bv");
    gl::detail::glVertexAttrib4d                                = (PFNGLVERTEXATTRIB4DPROC                              )get_proc_address("glVertexAttrib4d");
    gl::detail::glVertexAttrib4dv                               = (PFNGLVERTEXATTRIB4DVPROC                             )get_proc_address("glVertexAttrib4dv");
    gl::detail::glVertexAttrib4f                                = (PFNGLVERTEXATTRIB4FPROC                              )get_proc_address("glVertexAttrib4f");
    gl::detail::glVertexAttrib4fv                               = (PFNGLVERTEXATTRIB4FVPROC                             )get_proc_address("glVertexAttrib4fv");
    gl::detail::glVertexAttrib4iv                               = (PFNGLVERTEXATTRIB4IVPROC                             )get_proc_address("glVertexAttrib4iv");
    gl::detail::glVertexAttrib4s                                = (PFNGLVERTEXATTRIB4SPROC                              )get_proc_address("glVertexAttrib4s");
    gl::detail::glVertexAttrib4sv                               = (PFNGLVERTEXATTRIB4SVPROC                             )get_proc_address("glVertexAttrib4sv");
    gl::detail::glVertexAttrib4ubv                              = (PFNGLVERTEXATTRIB4UBVPROC                            )get_proc_address("glVertexAttrib4ubv");
    gl::detail::glVertexAttrib4uiv                              = (PFNGLVERTEXATTRIB4UIVPROC                            )get_proc_address("glVertexAttrib4uiv");
    gl::detail::glVertexAttrib4usv                              = (PFNGLVERTEXATTRIB4USVPROC                            )get_proc_address("glVertexAttrib4usv");
    gl::detail::glVertexAttribPointer                           = (PFNGLVERTEXATTRIBPOINTERPROC                         )get_proc_address("glVertexAttribPointer");
    gl::detail::glUniformMatrix2x3fv                            = (PFNGLUNIFORMMATRIX2X3FVPROC                          )get_proc_address("glUniformMatrix2x3fv");
    gl::detail::glUniformMatrix3x2fv                            = (PFNGLUNIFORMMATRIX3X2FVPROC                          )get_proc_address("glUniformMatrix3x2fv");
    gl::detail::glUniformMatrix2x4fv                            = (PFNGLUNIFORMMATRIX2X4FVPROC                          )get_proc_address("glUniformMatrix2x4fv");
    gl::detail::glUniformMatrix4x2fv                            = (PFNGLUNIFORMMATRIX4X2FVPROC                          )get_proc_address("glUniformMatrix4x2fv");
    gl::detail::glUniformMatrix3x4fv                            = (PFNGLUNIFORMMATRIX3X4FVPROC                          )get_proc_address("glUniformMatrix3x4fv");
    gl::detail::glUniformMatrix4x3fv                            = (PFNGLUNIFORMMATRIX4X3FVPROC                          )get_proc_address("glUniformMatrix4x3fv");
    gl::detail::glColorMaski                                    = (PFNGLCOLORMASKIPROC                                  )get_proc_address("glColorMaski");
    gl::detail::glGetBooleani_v                                 = (PFNGLGETBOOLEANI_VPROC                               )get_proc_address("glGetBooleani_v");
    gl::detail::glGetIntegeri_v                                 = (PFNGLGETINTEGERI_VPROC                               )get_proc_address("glGetIntegeri_v");
    gl::detail::glEnablei                                       = (PFNGLENABLEIPROC                                     )get_proc_address("glEnablei");
    gl::detail::glDisablei                                      = (PFNGLDISABLEIPROC                                    )get_proc_address("glDisablei");
    gl::detail::glIsEnabledi                                    = (PFNGLISENABLEDIPROC                                  )get_proc_address("glIsEnabledi");
    gl::detail::glBeginTransformFeedback                        = (PFNGLBEGINTRANSFORMFEEDBACKPROC                      )get_proc_address("glBeginTransformFeedback");
    gl::detail::glEndTransformFeedback                          = (PFNGLENDTRANSFORMFEEDBACKPROC                        )get_proc_address("glEndTransformFeedback");
    gl::detail::glBindBufferRange                               = (PFNGLBINDBUFFERRANGEPROC                             )get_proc_address("glBindBufferRange");
    gl::detail::glBindBufferBase                                = (PFNGLBINDBUFFERBASEPROC                              )get_proc_address("glBindBufferBase");
    gl::detail::glTransformFeedbackVaryings                     = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC                   )get_proc_address("glTransformFeedbackVaryings");
    gl::detail::glGetTransformFeedbackVarying                   = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC                 )get_proc_address("glGetTransformFeedbackVarying");
    gl::detail::glClampColor                                    = (PFNGLCLAMPCOLORPROC                                  )get_proc_address("glClampColor");
    gl::detail::glBeginConditionalRender                        = (PFNGLBEGINCONDITIONALRENDERPROC                      )get_proc_address("glBeginConditionalRender");
    gl::detail::glEndConditionalRender                          = (PFNGLENDCONDITIONALRENDERPROC                        )get_proc_address("glEndConditionalRender");
    gl::detail::glVertexAttribIPointer                          = (PFNGLVERTEXATTRIBIPOINTERPROC                        )get_proc_address("glVertexAttribIPointer");
    gl::detail::glGetVertexAttribIiv                            = (PFNGLGETVERTEXATTRIBIIVPROC                          )get_proc_address("glGetVertexAttribIiv");
    gl::detail::glGetVertexAttribIuiv                           = (PFNGLGETVERTEXATTRIBIUIVPROC                         )get_proc_address("glGetVertexAttribIuiv");
    gl::detail::glVertexAttribI1i                               = (PFNGLVERTEXATTRIBI1IPROC                             )get_proc_address("glVertexAttribI1i");
    gl::detail::glVertexAttribI2i                               = (PFNGLVERTEXATTRIBI2IPROC                             )get_proc_address("glVertexAttribI2i");
    gl::detail::glVertexAttribI3i                               = (PFNGLVERTEXATTRIBI3IPROC                             )get_proc_address("glVertexAttribI3i");
    gl::detail::glVertexAttribI4i                               = (PFNGLVERTEXATTRIBI4IPROC                             )get_proc_address("glVertexAttribI4i");
    gl::detail::glVertexAttribI1ui                              = (PFNGLVERTEXATTRIBI1UIPROC                            )get_proc_address("glVertexAttribI1ui");
    gl::detail::glVertexAttribI2ui                              = (PFNGLVERTEXATTRIBI2UIPROC                            )get_proc_address("glVertexAttribI2ui");
    gl::detail::glVertexAttribI3ui                              = (PFNGLVERTEXATTRIBI3UIPROC                            )get_proc_address("glVertexAttribI3ui");
    gl::detail::glVertexAttribI4ui                              = (PFNGLVERTEXATTRIBI4UIPROC                            )get_proc_address("glVertexAttribI4ui");
    gl::detail::glVertexAttribI1iv                              = (PFNGLVERTEXATTRIBI1IVPROC                            )get_proc_address("glVertexAttribI1iv");
    gl::detail::glVertexAttribI2iv                              = (PFNGLVERTEXATTRIBI2IVPROC                            )get_proc_address("glVertexAttribI2iv");
    gl::detail::glVertexAttribI3iv                              = (PFNGLVERTEXATTRIBI3IVPROC                            )get_proc_address("glVertexAttribI3iv");
    gl::detail::glVertexAttribI4iv                              = (PFNGLVERTEXATTRIBI4IVPROC                            )get_proc_address("glVertexAttribI4iv");
    gl::detail::glVertexAttribI1uiv                             = (PFNGLVERTEXATTRIBI1UIVPROC                           )get_proc_address("glVertexAttribI1uiv");
    gl::detail::glVertexAttribI2uiv                             = (PFNGLVERTEXATTRIBI2UIVPROC                           )get_proc_address("glVertexAttribI2uiv");
    gl::detail::glVertexAttribI3uiv                             = (PFNGLVERTEXATTRIBI3UIVPROC                           )get_proc_address("glVertexAttribI3uiv");
    gl::detail::glVertexAttribI4uiv                             = (PFNGLVERTEXATTRIBI4UIVPROC                           )get_proc_address("glVertexAttribI4uiv");
    gl::detail::glVertexAttribI4bv                              = (PFNGLVERTEXATTRIBI4BVPROC                            )get_proc_address("glVertexAttribI4bv");
    gl::detail::glVertexAttribI4sv                              = (PFNGLVERTEXATTRIBI4SVPROC                            )get_proc_address("glVertexAttribI4sv");
    gl::detail::glVertexAttribI4ubv                             = (PFNGLVERTEXATTRIBI4UBVPROC                           )get_proc_address("glVertexAttribI4ubv");
    gl::detail::glVertexAttribI4usv                             = (PFNGLVERTEXATTRIBI4USVPROC                           )get_proc_address("glVertexAttribI4usv");
    gl::detail::glGetUniformuiv                                 = (PFNGLGETUNIFORMUIVPROC                               )get_proc_address("glGetUniformuiv");
    gl::detail::glBindFragDataLocation                          = (PFNGLBINDFRAGDATALOCATIONPROC                        )get_proc_address("glBindFragDataLocation");
    gl::detail::glGetFragDataLocation                           = (PFNGLGETFRAGDATALOCATIONPROC                         )get_proc_address("glGetFragDataLocation");
    gl::detail::glUniform1ui                                    = (PFNGLUNIFORM1UIPROC                                  )get_proc_address("glUniform1ui");
    gl::detail::glUniform2ui                                    = (PFNGLUNIFORM2UIPROC                                  )get_proc_address("glUniform2ui");
    gl::detail::glUniform3ui                                    = (PFNGLUNIFORM3UIPROC                                  )get_proc_address("glUniform3ui");
    gl::detail::glUniform4ui                                    = (PFNGLUNIFORM4UIPROC                                  )get_proc_address("glUniform4ui");
    gl::detail::glUniform1uiv                                   = (PFNGLUNIFORM1UIVPROC                                 )get_proc_address("glUniform1uiv");
    gl::detail::glUniform2uiv                                   = (PFNGLUNIFORM2UIVPROC                                 )get_proc_address("glUniform2uiv");
    gl::detail::glUniform3uiv                                   = (PFNGLUNIFORM3UIVPROC                                 )get_proc_address("glUniform3uiv");
    gl::detail::glUniform4uiv                                   = (PFNGLUNIFORM4UIVPROC                                 )get_proc_address("glUniform4uiv");
    gl::detail::glTexParameterIiv                               = (PFNGLTEXPARAMETERIIVPROC                             )get_proc_address("glTexParameterIiv");
    gl::detail::glTexParameterIuiv                              = (PFNGLTEXPARAMETERIUIVPROC                            )get_proc_address("glTexParameterIuiv");
    gl::detail::glGetTexParameterIiv                            = (PFNGLGETTEXPARAMETERIIVPROC                          )get_proc_address("glGetTexParameterIiv");
    gl::detail::glGetTexParameterIuiv                           = (PFNGLGETTEXPARAMETERIUIVPROC                         )get_proc_address("glGetTexParameterIuiv");
    gl::detail::glClearBufferiv                                 = (PFNGLCLEARBUFFERIVPROC                               )get_proc_address("glClearBufferiv");
    gl::detail::glClearBufferuiv                                = (PFNGLCLEARBUFFERUIVPROC                              )get_proc_address("glClearBufferuiv");
    gl::detail::glClearBufferfv                                 = (PFNGLCLEARBUFFERFVPROC                               )get_proc_address("glClearBufferfv");
    gl::detail::glClearBufferfi                                 = (PFNGLCLEARBUFFERFIPROC                               )get_proc_address("glClearBufferfi");
    gl::detail::glGetStringi                                    = (PFNGLGETSTRINGIPROC                                  )get_proc_address("glGetStringi");
    gl::detail::glDrawArraysInstanced                           = (PFNGLDRAWARRAYSINSTANCEDPROC                         )get_proc_address("glDrawArraysInstanced");
    gl::detail::glDrawElementsInstanced                         = (PFNGLDRAWELEMENTSINSTANCEDPROC                       )get_proc_address("glDrawElementsInstanced");
    gl::detail::glTexBuffer                                     = (PFNGLTEXBUFFERPROC                                   )get_proc_address("glTexBuffer");
    gl::detail::glPrimitiveRestartIndex                         = (PFNGLPRIMITIVERESTARTINDEXPROC                       )get_proc_address("glPrimitiveRestartIndex");
    gl::detail::glGetInteger64i_v                               = (PFNGLGETINTEGER64I_VPROC                             )get_proc_address("glGetInteger64i_v");
    gl::detail::glGetBufferParameteri64v                        = (PFNGLGETBUFFERPARAMETERI64VPROC                      )get_proc_address("glGetBufferParameteri64v");
    gl::detail::glFramebufferTexture                            = (PFNGLFRAMEBUFFERTEXTUREPROC                          )get_proc_address("glFramebufferTexture");
    gl::detail::glIsRenderbuffer                                = (PFNGLISRENDERBUFFERPROC                              )get_proc_address("glIsRenderbuffer");
    gl::detail::glBindRenderbuffer                              = (PFNGLBINDRENDERBUFFERPROC                            )get_proc_address("glBindRenderbuffer");
    gl::detail::glDeleteRenderbuffers                           = (PFNGLDELETERENDERBUFFERSPROC                         )get_proc_address("glDeleteRenderbuffers");
    gl::detail::glGenRenderbuffers                              = (PFNGLGENRENDERBUFFERSPROC                            )get_proc_address("glGenRenderbuffers");
    gl::detail::glRenderbufferStorage                           = (PFNGLRENDERBUFFERSTORAGEPROC                         )get_proc_address("glRenderbufferStorage");
    gl::detail::glGetRenderbufferParameteriv                    = (PFNGLGETRENDERBUFFERPARAMETERIVPROC                  )get_proc_address("glGetRenderbufferParameteriv");
    gl::detail::glIsFramebuffer                                 = (PFNGLISFRAMEBUFFERPROC                               )get_proc_address("glIsFramebuffer");
    gl::detail::glBindFramebuffer                               = (PFNGLBINDFRAMEBUFFERPROC                             )get_proc_address("glBindFramebuffer");
    gl::detail::glDeleteFramebuffers                            = (PFNGLDELETEFRAMEBUFFERSPROC                          )get_proc_address("glDeleteFramebuffers");
    gl::detail::glGenFramebuffers                               = (PFNGLGENFRAMEBUFFERSPROC                             )get_proc_address("glGenFramebuffers");
    gl::detail::glCheckFramebufferStatus                        = (PFNGLCHECKFRAMEBUFFERSTATUSPROC                      )get_proc_address("glCheckFramebufferStatus");
    gl::detail::glFramebufferTexture1D                          = (PFNGLFRAMEBUFFERTEXTURE1DPROC                        )get_proc_address("glFramebufferTexture1D");
    gl::detail::glFramebufferTexture2D                          = (PFNGLFRAMEBUFFERTEXTURE2DPROC                        )get_proc_address("glFramebufferTexture2D");
    gl::detail::glFramebufferTexture3D                          = (PFNGLFRAMEBUFFERTEXTURE3DPROC                        )get_proc_address("glFramebufferTexture3D");
    gl::detail::glFramebufferRenderbuffer                       = (PFNGLFRAMEBUFFERRENDERBUFFERPROC                     )get_proc_address("glFramebufferRenderbuffer");
    gl::detail::glGetFramebufferAttachmentParameteriv           = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC         )get_proc_address("glGetFramebufferAttachmentParameteriv");
    gl::detail::glGenerateMipmap                                = (PFNGLGENERATEMIPMAPPROC                              )get_proc_address("glGenerateMipmap");
    gl::detail::glBlitFramebuffer                               = (PFNGLBLITFRAMEBUFFERPROC                             )get_proc_address("glBlitFramebuffer");
    gl::detail::glRenderbufferStorageMultisample                = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC              )get_proc_address("glRenderbufferStorageMultisample");
    gl::detail::glFramebufferTextureLayer                       = (PFNGLFRAMEBUFFERTEXTURELAYERPROC                     )get_proc_address("glFramebufferTextureLayer");
    gl::detail::glMapBufferRange                                = (PFNGLMAPBUFFERRANGEPROC                              )get_proc_address("glMapBufferRange");
    gl::detail::glFlushMappedBufferRange                        = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC                      )get_proc_address("glFlushMappedBufferRange");
    gl::detail::glBindVertexArray                               = (PFNGLBINDVERTEXARRAYPROC                             )get_proc_address("glBindVertexArray");
    gl::detail::glDeleteVertexArrays                            = (PFNGLDELETEVERTEXARRAYSPROC                          )get_proc_address("glDeleteVertexArrays");
    gl::detail::glGenVertexArrays                               = (PFNGLGENVERTEXARRAYSPROC                             )get_proc_address("glGenVertexArrays");
    gl::detail::glIsVertexArray                                 = (PFNGLISVERTEXARRAYPROC                               )get_proc_address("glIsVertexArray");
    gl::detail::glGetUniformIndices                             = (PFNGLGETUNIFORMINDICESPROC                           )get_proc_address("glGetUniformIndices");
    gl::detail::glGetActiveUniformsiv                           = (PFNGLGETACTIVEUNIFORMSIVPROC                         )get_proc_address("glGetActiveUniformsiv");
    gl::detail::glGetActiveUniformName                          = (PFNGLGETACTIVEUNIFORMNAMEPROC                        )get_proc_address("glGetActiveUniformName");
    gl::detail::glGetUniformBlockIndex                          = (PFNGLGETUNIFORMBLOCKINDEXPROC                        )get_proc_address("glGetUniformBlockIndex");
    gl::detail::glGetActiveUniformBlockiv                       = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC                     )get_proc_address("glGetActiveUniformBlockiv");
    gl::detail::glGetActiveUniformBlockName                     = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC                   )get_proc_address("glGetActiveUniformBlockName");
    gl::detail::glUniformBlockBinding                           = (PFNGLUNIFORMBLOCKBINDINGPROC                         )get_proc_address("glUniformBlockBinding");
    gl::detail::glCopyBufferSubData                             = (PFNGLCOPYBUFFERSUBDATAPROC                           )get_proc_address("glCopyBufferSubData");
    gl::detail::glDrawElementsBaseVertex                        = (PFNGLDRAWELEMENTSBASEVERTEXPROC                      )get_proc_address("glDrawElementsBaseVertex");
    gl::detail::glDrawRangeElementsBaseVertex                   = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC                 )get_proc_address("glDrawRangeElementsBaseVertex");
    gl::detail::glDrawElementsInstancedBaseVertex               = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC             )get_proc_address("glDrawElementsInstancedBaseVertex");
    gl::detail::glMultiDrawElementsBaseVertex                   = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC                 )get_proc_address("glMultiDrawElementsBaseVertex");
    gl::detail::glProvokingVertex                               = (PFNGLPROVOKINGVERTEXPROC                             )get_proc_address("glProvokingVertex");
    gl::detail::glFenceSync                                     = (PFNGLFENCESYNCPROC                                   )get_proc_address("glFenceSync");
    gl::detail::glIsSync                                        = (PFNGLISSYNCPROC                                      )get_proc_address("glIsSync");
    gl::detail::glDeleteSync                                    = (PFNGLDELETESYNCPROC                                  )get_proc_address("glDeleteSync");
    gl::detail::glClientWaitSync                                = (PFNGLCLIENTWAITSYNCPROC                              )get_proc_address("glClientWaitSync");
    gl::detail::glWaitSync                                      = (PFNGLWAITSYNCPROC                                    )get_proc_address("glWaitSync");
    gl::detail::glGetInteger64v                                 = (PFNGLGETINTEGER64VPROC                               )get_proc_address("glGetInteger64v");
    gl::detail::glGetSynciv                                     = (PFNGLGETSYNCIVPROC                                   )get_proc_address("glGetSynciv");
    gl::detail::glTexImage2DMultisample                         = (PFNGLTEXIMAGE2DMULTISAMPLEPROC                       )get_proc_address("glTexImage2DMultisample");
    gl::detail::glTexImage3DMultisample                         = (PFNGLTEXIMAGE3DMULTISAMPLEPROC                       )get_proc_address("glTexImage3DMultisample");
    gl::detail::glGetMultisamplefv                              = (PFNGLGETMULTISAMPLEFVPROC                            )get_proc_address("glGetMultisamplefv");
    gl::detail::glSampleMaski                                   = (PFNGLSAMPLEMASKIPROC                                 )get_proc_address("glSampleMaski");
    gl::detail::glMinSampleShading                              = (PFNGLMINSAMPLESHADINGARBPROC                         )get_proc_address("glMinSampleShading");
    gl::detail::glGenSamplers                                   = (PFNGLGENSAMPLERSPROC                                 )get_proc_address("glGenSamplers");
    gl::detail::glDeleteSamplers                                = (PFNGLDELETESAMPLERSPROC                              )get_proc_address("glDeleteSamplers");
    gl::detail::glIsSampler                                     = (PFNGLISSAMPLERPROC                                   )get_proc_address("glIsSampler");
    gl::detail::glBindSampler                                   = (PFNGLBINDSAMPLERPROC                                 )get_proc_address("glBindSampler");
    gl::detail::glSamplerParameteri                             = (PFNGLSAMPLERPARAMETERIPROC                           )get_proc_address("glSamplerParameteri");
    gl::detail::glSamplerParameteriv                            = (PFNGLSAMPLERPARAMETERIVPROC                          )get_proc_address("glSamplerParameteriv");
    gl::detail::glSamplerParameterf                             = (PFNGLSAMPLERPARAMETERFPROC                           )get_proc_address("glSamplerParameterf");
    gl::detail::glSamplerParameterfv                            = (PFNGLSAMPLERPARAMETERFVPROC                          )get_proc_address("glSamplerParameterfv");
    gl::detail::glSamplerParameterIiv                           = (PFNGLSAMPLERPARAMETERIIVPROC                         )get_proc_address("glSamplerParameterIiv");
    gl::detail::glSamplerParameterIuiv                          = (PFNGLSAMPLERPARAMETERIUIVPROC                        )get_proc_address("glSamplerParameterIuiv");
    gl::detail::glGetSamplerParameteriv                         = (PFNGLGETSAMPLERPARAMETERIVPROC                       )get_proc_address("glGetSamplerParameteriv");
    gl::detail::glGetSamplerParameterIiv                        = (PFNGLGETSAMPLERPARAMETERIIVPROC                      )get_proc_address("glGetSamplerParameterIiv");
    gl::detail::glGetSamplerParameterfv                         = (PFNGLGETSAMPLERPARAMETERFVPROC                       )get_proc_address("glGetSamplerParameterfv");
    gl::detail::glGetSamplerParameterIuiv                       = (PFNGLGETSAMPLERPARAMETERIUIVPROC                     )get_proc_address("glGetSamplerParameterIuiv");
    gl::detail::glQueryCounter                                  = (PFNGLQUERYCOUNTERPROC                                )get_proc_address("glQueryCounter");
    gl::detail::glGetQueryObjecti64v                            = (PFNGLGETQUERYOBJECTI64VPROC                          )get_proc_address("glGetQueryObjecti64v");
    gl::detail::glGetQueryObjectui64v                           = (PFNGLGETQUERYOBJECTUI64VPROC                         )get_proc_address("glGetQueryObjectui64v");
    gl::detail::glPatchParameteri                               = (PFNGLPATCHPARAMETERIPROC                             )get_proc_address("glPatchParameteri");
    gl::detail::glPatchParameterfv                              = (PFNGLPATCHPARAMETERFVPROC                            )get_proc_address("glPatchParameterfv");
    gl::detail::glGetProgramBinary                              = (PFNGLGETPROGRAMBINARYPROC                            )get_proc_address("glGetProgramBinary");
    gl::detail::glProgramBinary                                 = (PFNGLPROGRAMBINARYPROC                               )get_proc_address("glProgramBinary");
    gl::detail::glProgramParameteri                             = (PFNGLPROGRAMPARAMETERIPROC                           )get_proc_address("glProgramParameteri");
    gl::detail::glDrawArraysInstancedBaseInstance               = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC             )get_proc_address("glDrawArraysInstancedBaseInstance");
    gl::detail::glDrawElementsInstancedBaseInstance             = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC           )get_proc_address("glDrawElementsInstancedBaseInstance");
    gl::detail::glDrawElementsInstancedBaseVertexBaseInstance   = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC )get_proc_address("glDrawElementsInstancedBaseVertexBaseInstance");
    gl::detail::glBindTransformFeedback                         = (PFNGLBINDTRANSFORMFEEDBACKPROC                       )get_proc_address("glBindTransformFeedback");
    gl::detail::glDeleteTransformFeedbacks                      = (PFNGLDELETETRANSFORMFEEDBACKSPROC                    )get_proc_address("glDeleteTransformFeedbacks");
    gl::detail::glGenTransformFeedbacks                         = (PFNGLGENTRANSFORMFEEDBACKSPROC                       )get_proc_address("glGenTransformFeedbacks");
    gl::detail::glIsTransformFeedback                           = (PFNGLISTRANSFORMFEEDBACKPROC                         )get_proc_address("glIsTransformFeedback");
    gl::detail::glPauseTransformFeedback                        = (PFNGLPAUSETRANSFORMFEEDBACKPROC                      )get_proc_address("glPauseTransformFeedback");
    gl::detail::glResumeTransformFeedback                       = (PFNGLRESUMETRANSFORMFEEDBACKPROC                     )get_proc_address("glResumeTransformFeedback");
    gl::detail::glDrawTransformFeedback                         = (PFNGLDRAWTRANSFORMFEEDBACKPROC                       )get_proc_address("glDrawTransformFeedback");
    gl::detail::glDrawTransformFeedbackStream                   = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC                 )get_proc_address("glDrawTransformFeedbackStream");
    gl::detail::glBeginQueryIndexed                             = (PFNGLBEGINQUERYINDEXEDPROC                           )get_proc_address("glBeginQueryIndexed");
    gl::detail::glEndQueryIndexed                               = (PFNGLENDQUERYINDEXEDPROC                             )get_proc_address("glEndQueryIndexed");
    gl::detail::glGetQueryIndexediv                             = (PFNGLGETQUERYINDEXEDIVPROC                           )get_proc_address("glGetQueryIndexediv");
    gl::detail::glDrawTransformFeedbackInstanced                = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC              )get_proc_address("glDrawTransformFeedbackInstanced");
    gl::detail::glDrawTransformFeedbackStreamInstanced          = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC        )get_proc_address("glDrawTransformFeedbackStreamInstanced");
    gl::detail::glVertexAttribDivisor                           = (PFNGLVERTEXATTRIBDIVISORPROC                         )get_proc_address("glVertexAttribDivisor");
    gl::detail::glTexStorage1D                                  = (PFNGLTEXSTORAGE1DPROC                                )get_proc_address("glTexStorage1D");
    gl::detail::glTexStorage2D                                  = (PFNGLTEXSTORAGE2DPROC                                )get_proc_address("glTexStorage2D");
    gl::detail::glTexStorage3D                                  = (PFNGLTEXSTORAGE3DPROC                                )get_proc_address("glTexStorage3D");

#endif

#if defined(EXT_SUBSET)
    gl::detail::glActiveTexture                                 = (PFNGLACTIVETEXTUREPROC                               )get_proc_address("glActiveTexture");
    gl::detail::glBindBuffer                                    = (PFNGLBINDBUFFERPROC                                  )get_proc_address("glBindBuffer");
    gl::detail::glDeleteBuffers                                 = (PFNGLDELETEBUFFERSPROC                               )get_proc_address("glDeleteBuffers");
    gl::detail::glGenBuffers                                    = (PFNGLGENBUFFERSPROC                                  )get_proc_address("glGenBuffers");
    gl::detail::glIsBuffer                                      = (PFNGLISBUFFERPROC                                    )get_proc_address("glIsBuffer");
    gl::detail::glBufferData                                    = (PFNGLBUFFERDATAPROC                                  )get_proc_address("glBufferData");
    gl::detail::glBufferSubData                                 = (PFNGLBUFFERSUBDATAPROC                               )get_proc_address("glBufferSubData");
    gl::detail::glGetBufferSubData                              = (PFNGLGETBUFFERSUBDATAPROC                            )get_proc_address("glGetBufferSubData");
    gl::detail::glMapBuffer                                     = (PFNGLMAPBUFFERPROC                                   )get_proc_address("glMapBuffer");
    gl::detail::glUnmapBuffer                                   = (PFNGLUNMAPBUFFERPROC                                 )get_proc_address("glUnmapBuffer");
    gl::detail::glGetBufferParameteriv                          = (PFNGLGETBUFFERPARAMETERIVPROC                        )get_proc_address("glGetBufferParameteriv");
    gl::detail::glGetBufferPointerv                             = (PFNGLGETBUFFERPOINTERVPROC                           )get_proc_address("glGetBufferPointerv");
    gl::detail::glAttachShader                                  = (PFNGLATTACHSHADERPROC                                )get_proc_address("glAttachShader");
    gl::detail::glBindAttribLocation                            = (PFNGLBINDATTRIBLOCATIONPROC                          )get_proc_address("glBindAttribLocation");
    gl::detail::glCompileShader                                 = (PFNGLCOMPILESHADERPROC                               )get_proc_address("glCompileShader");
    gl::detail::glCreateProgram                                 = (PFNGLCREATEPROGRAMPROC                               )get_proc_address("glCreateProgram");
    gl::detail::glCreateShader                                  = (PFNGLCREATESHADERPROC                                )get_proc_address("glCreateShader");
    gl::detail::glDeleteProgram                                 = (PFNGLDELETEPROGRAMPROC                               )get_proc_address("glDeleteProgram");
    gl::detail::glDeleteShader                                  = (PFNGLDELETESHADERPROC                                )get_proc_address("glDeleteShader");
    gl::detail::glDetachShader                                  = (PFNGLDETACHSHADERPROC                                )get_proc_address("glDetachShader");
    gl::detail::glDisableVertexAttribArray                      = (PFNGLDISABLEVERTEXATTRIBARRAYPROC                    )get_proc_address("glDisableVertexAttribArray");
    gl::detail::glEnableVertexAttribArray                       = (PFNGLENABLEVERTEXATTRIBARRAYPROC                     )get_proc_address("glEnableVertexAttribArray");
    gl::detail::glGetActiveAttrib                               = (PFNGLGETACTIVEATTRIBPROC                             )get_proc_address("glGetActiveAttrib");
    gl::detail::glGetActiveUniform                              = (PFNGLGETACTIVEUNIFORMPROC                            )get_proc_address("glGetActiveUniform");
    gl::detail::glGetAttachedShaders                            = (PFNGLGETATTACHEDSHADERSPROC                          )get_proc_address("glGetAttachedShaders");
    gl::detail::glGetAttribLocation                             = (PFNGLGETATTRIBLOCATIONPROC                           )get_proc_address("glGetAttribLocation");
    gl::detail::glGetProgramiv                                  = (PFNGLGETPROGRAMIVPROC                                )get_proc_address("glGetProgramiv");
    gl::detail::glGetProgramInfoLog                             = (PFNGLGETPROGRAMINFOLOGPROC                           )get_proc_address("glGetProgramInfoLog");
    gl::detail::glGetShaderiv                                   = (PFNGLGETSHADERIVPROC                                 )get_proc_address("glGetShaderiv");
    gl::detail::glGetShaderInfoLog                              = (PFNGLGETSHADERINFOLOGPROC                            )get_proc_address("glGetShaderInfoLog");
    gl::detail::glGetShaderSource                               = (PFNGLGETSHADERSOURCEPROC                             )get_proc_address("glGetShaderSource");
    gl::detail::glGetUniformLocation                            = (PFNGLGETUNIFORMLOCATIONPROC                          )get_proc_address("glGetUniformLocation");
    gl::detail::glGetUniformfv                                  = (PFNGLGETUNIFORMFVPROC                                )get_proc_address("glGetUniformfv");
    gl::detail::glGetUniformiv                                  = (PFNGLGETUNIFORMIVPROC                                )get_proc_address("glGetUniformiv");
    gl::detail::glGetVertexAttribdv                             = (PFNGLGETVERTEXATTRIBDVPROC                           )get_proc_address("glGetVertexAttribdv");
    gl::detail::glGetVertexAttribfv                             = (PFNGLGETVERTEXATTRIBFVPROC                           )get_proc_address("glGetVertexAttribfv");
    gl::detail::glGetVertexAttribiv                             = (PFNGLGETVERTEXATTRIBIVPROC                           )get_proc_address("glGetVertexAttribiv");
    gl::detail::glGetVertexAttribPointerv                       = (PFNGLGETVERTEXATTRIBPOINTERVPROC                     )get_proc_address("glGetVertexAttribPointerv");
    gl::detail::glIsProgram                                     = (PFNGLISPROGRAMPROC                                   )get_proc_address("glIsProgram");
    gl::detail::glIsShader                                      = (PFNGLISSHADERPROC                                    )get_proc_address("glIsShader");
    gl::detail::glLinkProgram                                   = (PFNGLLINKPROGRAMPROC                                 )get_proc_address("glLinkProgram");
    gl::detail::glShaderSource                                  = (PFNGLSHADERSOURCEPROC                                )get_proc_address("glShaderSource");
    gl::detail::glUseProgram                                    = (PFNGLUSEPROGRAMPROC                                  )get_proc_address("glUseProgram");
    gl::detail::glUniform1f                                     = (PFNGLUNIFORM1FPROC                                   )get_proc_address("glUniform1f");
    gl::detail::glUniform2f                                     = (PFNGLUNIFORM2FPROC                                   )get_proc_address("glUniform2f");
    gl::detail::glUniform3f                                     = (PFNGLUNIFORM3FPROC                                   )get_proc_address("glUniform3f");
    gl::detail::glUniform4f                                     = (PFNGLUNIFORM4FPROC                                   )get_proc_address("glUniform4f");
    gl::detail::glUniform1i                                     = (PFNGLUNIFORM1IPROC                                   )get_proc_address("glUniform1i");
    gl::detail::glUniform2i                                     = (PFNGLUNIFORM2IPROC                                   )get_proc_address("glUniform2i");
    gl::detail::glUniform3i                                     = (PFNGLUNIFORM3IPROC                                   )get_proc_address("glUniform3i");
    gl::detail::glUniform4i                                     = (PFNGLUNIFORM4IPROC                                   )get_proc_address("glUniform4i");
    gl::detail::glUniform1fv                                    = (PFNGLUNIFORM1FVPROC                                  )get_proc_address("glUniform1fv");
    gl::detail::glUniform2fv                                    = (PFNGLUNIFORM2FVPROC                                  )get_proc_address("glUniform2fv");
    gl::detail::glUniform3fv                                    = (PFNGLUNIFORM3FVPROC                                  )get_proc_address("glUniform3fv");
    gl::detail::glUniform4fv                                    = (PFNGLUNIFORM4FVPROC                                  )get_proc_address("glUniform4fv");
    gl::detail::glUniform1iv                                    = (PFNGLUNIFORM1IVPROC                                  )get_proc_address("glUniform1iv");
    gl::detail::glUniform2iv                                    = (PFNGLUNIFORM2IVPROC                                  )get_proc_address("glUniform2iv");
    gl::detail::glUniform3iv                                    = (PFNGLUNIFORM3IVPROC                                  )get_proc_address("glUniform3iv");
    gl::detail::glUniform4iv                                    = (PFNGLUNIFORM4IVPROC                                  )get_proc_address("glUniform4iv");
    gl::detail::glUniformMatrix2fv                              = (PFNGLUNIFORMMATRIX2FVPROC                            )get_proc_address("glUniformMatrix2fv");
    gl::detail::glUniformMatrix3fv                              = (PFNGLUNIFORMMATRIX3FVPROC                            )get_proc_address("glUniformMatrix3fv");
    gl::detail::glUniformMatrix4fv                              = (PFNGLUNIFORMMATRIX4FVPROC                            )get_proc_address("glUniformMatrix4fv");
    gl::detail::glValidateProgram                               = (PFNGLVALIDATEPROGRAMPROC                             )get_proc_address("glValidateProgram");
    gl::detail::glVertexAttrib1d                                = (PFNGLVERTEXATTRIB1DPROC                              )get_proc_address("glVertexAttrib1d");
    gl::detail::glVertexAttrib1dv                               = (PFNGLVERTEXATTRIB1DVPROC                             )get_proc_address("glVertexAttrib1dv");
    gl::detail::glVertexAttrib1f                                = (PFNGLVERTEXATTRIB1FPROC                              )get_proc_address("glVertexAttrib1f");
    gl::detail::glVertexAttrib1fv                               = (PFNGLVERTEXATTRIB1FVPROC                             )get_proc_address("glVertexAttrib1fv");
    gl::detail::glVertexAttrib1s                                = (PFNGLVERTEXATTRIB1SPROC                              )get_proc_address("glVertexAttrib1s");
    gl::detail::glVertexAttrib1sv                               = (PFNGLVERTEXATTRIB1SVPROC                             )get_proc_address("glVertexAttrib1sv");
    gl::detail::glVertexAttrib2d                                = (PFNGLVERTEXATTRIB2DPROC                              )get_proc_address("glVertexAttrib2d");
    gl::detail::glVertexAttrib2dv                               = (PFNGLVERTEXATTRIB2DVPROC                             )get_proc_address("glVertexAttrib2dv");
    gl::detail::glVertexAttrib2f                                = (PFNGLVERTEXATTRIB2FPROC                              )get_proc_address("glVertexAttrib2f");
    gl::detail::glVertexAttrib2fv                               = (PFNGLVERTEXATTRIB2FVPROC                             )get_proc_address("glVertexAttrib2fv");
    gl::detail::glVertexAttrib2s                                = (PFNGLVERTEXATTRIB2SPROC                              )get_proc_address("glVertexAttrib2s");
    gl::detail::glVertexAttrib2sv                               = (PFNGLVERTEXATTRIB2SVPROC                             )get_proc_address("glVertexAttrib2sv");
    gl::detail::glVertexAttrib3d                                = (PFNGLVERTEXATTRIB3DPROC                              )get_proc_address("glVertexAttrib3d");
    gl::detail::glVertexAttrib3dv                               = (PFNGLVERTEXATTRIB3DVPROC                             )get_proc_address("glVertexAttrib3dv");
    gl::detail::glVertexAttrib3f                                = (PFNGLVERTEXATTRIB3FPROC                              )get_proc_address("glVertexAttrib3f");
    gl::detail::glVertexAttrib3fv                               = (PFNGLVERTEXATTRIB3FVPROC                             )get_proc_address("glVertexAttrib3fv");
    gl::detail::glVertexAttrib3s                                = (PFNGLVERTEXATTRIB3SPROC                              )get_proc_address("glVertexAttrib3s");
    gl::detail::glVertexAttrib3sv                               = (PFNGLVERTEXATTRIB3SVPROC                             )get_proc_address("glVertexAttrib3sv");
    gl::detail::glVertexAttrib4Nbv                              = (PFNGLVERTEXATTRIB4NBVPROC                            )get_proc_address("glVertexAttrib4Nbv");
    gl::detail::glVertexAttrib4Niv                              = (PFNGLVERTEXATTRIB4NIVPROC                            )get_proc_address("glVertexAttrib4Niv");
    gl::detail::glVertexAttrib4Nsv                              = (PFNGLVERTEXATTRIB4NSVPROC                            )get_proc_address("glVertexAttrib4Nsv");
    gl::detail::glVertexAttrib4Nub                              = (PFNGLVERTEXATTRIB4NUBPROC                            )get_proc_address("glVertexAttrib4Nub");
    gl::detail::glVertexAttrib4Nubv                             = (PFNGLVERTEXATTRIB4NUBVPROC                           )get_proc_address("glVertexAttrib4Nubv");
    gl::detail::glVertexAttrib4Nuiv                             = (PFNGLVERTEXATTRIB4NUIVPROC                           )get_proc_address("glVertexAttrib4Nuiv");
    gl::detail::glVertexAttrib4Nusv                             = (PFNGLVERTEXATTRIB4NUSVPROC                           )get_proc_address("glVertexAttrib4Nusv");
    gl::detail::glVertexAttrib4bv                               = (PFNGLVERTEXATTRIB4BVPROC                             )get_proc_address("glVertexAttrib4bv");
    gl::detail::glVertexAttrib4d                                = (PFNGLVERTEXATTRIB4DPROC                              )get_proc_address("glVertexAttrib4d");
    gl::detail::glVertexAttrib4dv                               = (PFNGLVERTEXATTRIB4DVPROC                             )get_proc_address("glVertexAttrib4dv");
    gl::detail::glVertexAttrib4f                                = (PFNGLVERTEXATTRIB4FPROC                              )get_proc_address("glVertexAttrib4f");
    gl::detail::glVertexAttrib4fv                               = (PFNGLVERTEXATTRIB4FVPROC                             )get_proc_address("glVertexAttrib4fv");
    gl::detail::glVertexAttrib4iv                               = (PFNGLVERTEXATTRIB4IVPROC                             )get_proc_address("glVertexAttrib4iv");
    gl::detail::glVertexAttrib4s                                = (PFNGLVERTEXATTRIB4SPROC                              )get_proc_address("glVertexAttrib4s");
    gl::detail::glVertexAttrib4sv                               = (PFNGLVERTEXATTRIB4SVPROC                             )get_proc_address("glVertexAttrib4sv");
    gl::detail::glVertexAttrib4ubv                              = (PFNGLVERTEXATTRIB4UBVPROC                            )get_proc_address("glVertexAttrib4ubv");
    gl::detail::glVertexAttrib4uiv                              = (PFNGLVERTEXATTRIB4UIVPROC                            )get_proc_address("glVertexAttrib4uiv");
    gl::detail::glVertexAttrib4usv                              = (PFNGLVERTEXATTRIB4USVPROC                            )get_proc_address("glVertexAttrib4usv");
    gl::detail::glVertexAttribPointer                           = (PFNGLVERTEXATTRIBPOINTERPROC                         )get_proc_address("glVertexAttribPointer");
    gl::detail::glUniformMatrix2x3fv                            = (PFNGLUNIFORMMATRIX2X3FVPROC                          )get_proc_address("glUniformMatrix2x3fv");
    gl::detail::glUniformMatrix3x2fv                            = (PFNGLUNIFORMMATRIX3X2FVPROC                          )get_proc_address("glUniformMatrix3x2fv");
    gl::detail::glUniformMatrix2x4fv                            = (PFNGLUNIFORMMATRIX2X4FVPROC                          )get_proc_address("glUniformMatrix2x4fv");
    gl::detail::glUniformMatrix4x2fv                            = (PFNGLUNIFORMMATRIX4X2FVPROC                          )get_proc_address("glUniformMatrix4x2fv");
    gl::detail::glUniformMatrix3x4fv                            = (PFNGLUNIFORMMATRIX3X4FVPROC                          )get_proc_address("glUniformMatrix3x4fv");
    gl::detail::glUniformMatrix4x3fv                            = (PFNGLUNIFORMMATRIX4X3FVPROC                          )get_proc_address("glUniformMatrix4x3fv");
    gl::detail::glGetStringi                                    = (PFNGLGETSTRINGIPROC                                  )get_proc_address("glGetStringi");
#endif
    //check_gl_error();
}

} }

#endif