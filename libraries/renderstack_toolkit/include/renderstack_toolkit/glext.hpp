#ifndef glext_hpp_renderstack_toolkit
#define glext_hpp_renderstack_toolkit

#if defined(_WIN32)
#   define _CRT_SECURE_NO_WARNINGS
#   define WIN32_LEAN_AND_MEAN
#   define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#   define NOVIRTUALKEYCODES // VK_*
#   define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#   define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#   define NOSYSMETRICS      // SM_*
#   define NOMENUS           // MF_*
#   define NOICONS           // IDI_*
#   define NOKEYSTATES       // MK_*
#   define NOSYSCOMMANDS     // SC_*
#   define NORASTEROPS       // Binary and Tertiary raster ops
#   define NOSHOWWINDOW      // SW_*
#   define OEMRESOURCE       // OEM Resource values
#   define NOATOM            // Atom Manager routines
#   define NOCLIPBOARD       // Clipboard routines
#   define NOCOLOR           // Screen colors
#   define NOCTLMGR          // Control and Dialog routines
#   define NODRAWTEXT        // DrawText() and DT_*
#   define NOGDI             // All GDI defines and routines
#   define NOKERNEL          // All KERNEL defines and routines
#   define NOUSER            // All USER defines and routines
#   define NONLS             // All NLS defines and routines
#   define NOMB              // MB_* and MessageBox()
#   define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#   define NOMETAFILE        // typedef METAFILEPICT
#   define NOMINMAX          // Macros min(a,b) and max(a,b)
#   define NOMSG             // typedef MSG and associated routines
#   define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#   define NOSCROLL          // SB_* and scrolling routines
#   define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#   define NOSOUND           // Sound driver routines
#   define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#   define NOWH              // SetWindowsHook and WH_*
#   define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#   define NOCOMM            // COMM driver routines
#   define NOKANJI           // Kanji support stuff.
#   define NOHELP            // Help engine interface.
#   define NOPROFILER        // Profiler interface.
#   define NODEFERWINDOWPOS  // DeferWindowPos routines
#   define NOMCX             // Modem Configuration Extensions
#   include <windows.h>
#endif

#include <GL/glcorearb.h>
#include <GL/glext.h>

#if defined(RENDERSTACK_USE_GLFW)
# include <GLFW/glfw3.h>
#endif

namespace gl { namespace detail {

#if defined(RENDERSTACK_DLOAD_ALL_GL_SYMBOLS)

/*  GL_VERSION_1_0  */ 
extern PFNGLCULLFACEPROC                                    glCullFace              ;
extern PFNGLFRONTFACEPROC                                   glFrontFace             ;
extern PFNGLHINTPROC                                        glHint                  ;
extern PFNGLLINEWIDTHPROC                                   glLineWidth             ;
extern PFNGLPOINTSIZEPROC                                   glPointSize             ;
extern PFNGLPOLYGONMODEPROC                                 glPolygonMode           ;
extern PFNGLSCISSORPROC                                     glScissor               ;
extern PFNGLTEXPARAMETERFPROC                               glTexParameterf         ;
extern PFNGLTEXPARAMETERFVPROC                              glTexParameterfv        ;
extern PFNGLTEXPARAMETERIPROC                               glTexParameteri         ;
extern PFNGLTEXPARAMETERIVPROC                              glTexParameteriv        ;
extern PFNGLTEXIMAGE1DPROC                                  glTexImage1D            ;
extern PFNGLTEXIMAGE2DPROC                                  glTexImage2D            ;
extern PFNGLDRAWBUFFERPROC                                  glDrawBuffer            ;
extern PFNGLCLEARPROC                                       glClear                 ;
extern PFNGLCLEARCOLORPROC                                  glClearColor            ;
extern PFNGLCLEARSTENCILPROC                                glClearStencil          ;
extern PFNGLCLEARDEPTHPROC                                  glClearDepth            ;
extern PFNGLSTENCILMASKPROC                                 glStencilMask           ;
extern PFNGLCOLORMASKPROC                                   glColorMask             ;
extern PFNGLDEPTHMASKPROC                                   glDepthMask             ;
extern PFNGLDISABLEPROC                                     glDisable               ;
extern PFNGLENABLEPROC                                      glEnable                ;
extern PFNGLFINISHPROC                                      glFinish                ;
extern PFNGLFLUSHPROC                                       glFlush                 ;
extern PFNGLBLENDFUNCPROC                                   glBlendFunc             ;
extern PFNGLLOGICOPPROC                                     glLogicOp               ;
extern PFNGLSTENCILFUNCPROC                                 glStencilFunc           ;
extern PFNGLSTENCILOPPROC                                   glStencilOp             ;
extern PFNGLDEPTHFUNCPROC                                   glDepthFunc             ;
extern PFNGLPIXELSTOREFPROC                                 glPixelStoref           ;
extern PFNGLPIXELSTOREIPROC                                 glPixelStorei           ;
extern PFNGLREADBUFFERPROC                                  glReadBuffer            ;
extern PFNGLREADPIXELSPROC                                  glReadPixels            ;
extern PFNGLGETBOOLEANVPROC                                 glGetBooleanv           ;
extern PFNGLGETDOUBLEVPROC                                  glGetDoublev            ;
extern PFNGLGETERRORPROC                                    glGetError              ;
extern PFNGLGETFLOATVPROC                                   glGetFloatv             ;
extern PFNGLGETINTEGERVPROC                                 glGetIntegerv           ;
extern PFNGLGETSTRINGPROC                                   glGetString             ;
extern PFNGLGETTEXIMAGEPROC                                 glGetTexImage           ;
extern PFNGLGETTEXPARAMETERFVPROC                           glGetTexParameterfv     ;
extern PFNGLGETTEXPARAMETERIVPROC                           glGetTexParameteriv     ;
extern PFNGLGETTEXLEVELPARAMETERFVPROC                      glGetTexLevelParameterfv; 
extern PFNGLGETTEXLEVELPARAMETERIVPROC                      glGetTexLevelParameteriv;
extern PFNGLISENABLEDPROC                                   glIsEnabled             ;
extern PFNGLDEPTHRANGEPROC                                  glDepthRange            ;
extern PFNGLVIEWPORTPROC                                    glViewport              ;

/*  GL_VERSION_1_1  */ 
extern PFNGLDRAWARRAYSPROC                                  glDrawArrays            ;
extern PFNGLDRAWELEMENTSPROC                                glDrawElements          ;
extern PFNGLGETPOINTERVPROC                                 glGetPointerv           ;
extern PFNGLPOLYGONOFFSETPROC                               glPolygonOffset         ;
extern PFNGLCOPYTEXIMAGE1DPROC                              glCopyTexImage1D        ;
extern PFNGLCOPYTEXIMAGE2DPROC                              glCopyTexImage2D        ;
extern PFNGLCOPYTEXSUBIMAGE1DPROC                           glCopyTexSubImage1D     ;
extern PFNGLCOPYTEXSUBIMAGE2DPROC                           glCopyTexSubImage2D     ;
extern PFNGLTEXSUBIMAGE1DPROC                               glTexSubImage1D         ;
extern PFNGLTEXSUBIMAGE2DPROC                               glTexSubImage2D         ;
extern PFNGLBINDTEXTUREPROC                                 glBindTexture           ;
extern PFNGLDELETETEXTURESPROC                              glDeleteTextures        ;
extern PFNGLGENTEXTURESPROC                                 glGenTextures           ;
extern PFNGLISTEXTUREPROC                                   glIsTexture             ;
#endif

/*  GL_VERSION_1_2  */ 
extern PFNGLBLENDCOLORPROC                                  glBlendColor                ;
extern PFNGLBLENDEQUATIONPROC                               glBlendEquation             ;
extern PFNGLDRAWRANGEELEMENTSPROC                           glDrawRangeElements         ;
extern PFNGLTEXIMAGE3DPROC                                  glTexImage3D                ;
extern PFNGLTEXSUBIMAGE3DPROC                               glTexSubImage3D             ;
extern PFNGLCOPYTEXSUBIMAGE3DPROC                           glCopyTexSubImage3D         ;

/*  GL_VERSION_1_3  */ 
extern PFNGLACTIVETEXTUREPROC                               glActiveTexture           ;
extern PFNGLSAMPLECOVERAGEPROC                              glSampleCoverage          ;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC                        glCompressedTexImage3D    ;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC                        glCompressedTexImage2D    ;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC                        glCompressedTexImage1D    ;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC                     glCompressedTexSubImage3D ;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC                     glCompressedTexSubImage2D ;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC                     glCompressedTexSubImage1D ;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC                       glGetCompressedTexImage   ;

/*  GL_VERSION_1_4  */ 
extern PFNGLBLENDFUNCSEPARATEPROC                           glBlendFuncSeparate       ;
extern PFNGLMULTIDRAWARRAYSPROC                             glMultiDrawArrays         ;
extern PFNGLMULTIDRAWELEMENTSPROC                           glMultiDrawElements       ;
extern PFNGLPOINTPARAMETERFPROC                             glPointParameterf         ;
extern PFNGLPOINTPARAMETERFVPROC                            glPointParameterfv        ;
extern PFNGLPOINTPARAMETERIPROC                             glPointParameteri         ;
extern PFNGLPOINTPARAMETERIVPROC                            glPointParameteriv        ;

/*  GL_VERSION_1_5  */ 
extern PFNGLGENQUERIESPROC                                  glGenQueries              ;
extern PFNGLDELETEQUERIESPROC                               glDeleteQueries           ;
extern PFNGLISQUERYPROC                                     glIsQuery                 ;
extern PFNGLBEGINQUERYPROC                                  glBeginQuery              ;
extern PFNGLENDQUERYPROC                                    glEndQuery                ;
extern PFNGLGETQUERYIVPROC                                  glGetQueryiv              ;
extern PFNGLGETQUERYOBJECTIVPROC                            glGetQueryObjectiv        ;
extern PFNGLGETQUERYOBJECTUIVPROC                           glGetQueryObjectuiv       ;
extern PFNGLBINDBUFFERPROC                                  glBindBuffer              ;
extern PFNGLDELETEBUFFERSPROC                               glDeleteBuffers           ;
extern PFNGLGENBUFFERSPROC                                  glGenBuffers              ;
extern PFNGLISBUFFERPROC                                    glIsBuffer                ;
extern PFNGLBUFFERDATAPROC                                  glBufferData              ;
extern PFNGLBUFFERSUBDATAPROC                               glBufferSubData           ;
extern PFNGLGETBUFFERSUBDATAPROC                            glGetBufferSubData        ;
extern PFNGLMAPBUFFERPROC                                   glMapBuffer               ;
extern PFNGLUNMAPBUFFERPROC                                 glUnmapBuffer             ;
extern PFNGLGETBUFFERPARAMETERIVPROC                        glGetBufferParameteriv    ;
extern PFNGLGETBUFFERPOINTERVPROC                           glGetBufferPointerv       ;

/*  GL_VERSION_2_0  */ 
extern PFNGLBLENDEQUATIONSEPARATEPROC                       glBlendEquationSeparate   ;
extern PFNGLDRAWBUFFERSPROC                                 glDrawBuffers             ;
extern PFNGLSTENCILOPSEPARATEPROC                           glStencilOpSeparate       ;
extern PFNGLSTENCILFUNCSEPARATEPROC                         glStencilFuncSeparate     ;
extern PFNGLSTENCILMASKSEPARATEPROC                         glStencilMaskSeparate     ;
extern PFNGLATTACHSHADERPROC                                glAttachShader            ;
extern PFNGLBINDATTRIBLOCATIONPROC                          glBindAttribLocation      ;
extern PFNGLCOMPILESHADERPROC                               glCompileShader           ;
extern PFNGLCREATEPROGRAMPROC                               glCreateProgram           ;
extern PFNGLCREATESHADERPROC                                glCreateShader            ;
extern PFNGLDELETEPROGRAMPROC                               glDeleteProgram           ;
extern PFNGLDELETESHADERPROC                                glDeleteShader            ;
extern PFNGLDETACHSHADERPROC                                glDetachShader            ;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC                    glDisableVertexAttribArray; 
extern PFNGLENABLEVERTEXATTRIBARRAYPROC                     glEnableVertexAttribArray ;
extern PFNGLGETACTIVEATTRIBPROC                             glGetActiveAttrib         ;
extern PFNGLGETACTIVEUNIFORMPROC                            glGetActiveUniform        ;
extern PFNGLGETATTACHEDSHADERSPROC                          glGetAttachedShaders      ;
extern PFNGLGETATTRIBLOCATIONPROC                           glGetAttribLocation       ;
extern PFNGLGETPROGRAMIVPROC                                glGetProgramiv            ;
extern PFNGLGETPROGRAMINFOLOGPROC                           glGetProgramInfoLog       ;
extern PFNGLGETSHADERIVPROC                                 glGetShaderiv             ;
extern PFNGLGETSHADERINFOLOGPROC                            glGetShaderInfoLog        ;
extern PFNGLGETSHADERSOURCEPROC                             glGetShaderSource         ;
extern PFNGLGETUNIFORMLOCATIONPROC                          glGetUniformLocation      ;
extern PFNGLGETUNIFORMFVPROC                                glGetUniformfv            ;
extern PFNGLGETUNIFORMIVPROC                                glGetUniformiv            ;
extern PFNGLGETVERTEXATTRIBDVPROC                           glGetVertexAttribdv       ;
extern PFNGLGETVERTEXATTRIBFVPROC                           glGetVertexAttribfv       ;
extern PFNGLGETVERTEXATTRIBIVPROC                           glGetVertexAttribiv       ;
extern PFNGLGETVERTEXATTRIBPOINTERVPROC                     glGetVertexAttribPointerv ;
extern PFNGLISPROGRAMPROC                                   glIsProgram               ;
extern PFNGLISSHADERPROC                                    glIsShader                ;
extern PFNGLLINKPROGRAMPROC                                 glLinkProgram             ;
extern PFNGLSHADERSOURCEPROC                                glShaderSource            ;
extern PFNGLUSEPROGRAMPROC                                  glUseProgram              ;
extern PFNGLUNIFORM1FPROC                                   glUniform1f               ;
extern PFNGLUNIFORM2FPROC                                   glUniform2f               ;
extern PFNGLUNIFORM3FPROC                                   glUniform3f               ;
extern PFNGLUNIFORM4FPROC                                   glUniform4f               ;
extern PFNGLUNIFORM1IPROC                                   glUniform1i               ;
extern PFNGLUNIFORM2IPROC                                   glUniform2i               ;
extern PFNGLUNIFORM3IPROC                                   glUniform3i               ;
extern PFNGLUNIFORM4IPROC                                   glUniform4i               ;
extern PFNGLUNIFORM1FVPROC                                  glUniform1fv              ;
extern PFNGLUNIFORM2FVPROC                                  glUniform2fv              ;
extern PFNGLUNIFORM3FVPROC                                  glUniform3fv              ;
extern PFNGLUNIFORM4FVPROC                                  glUniform4fv              ;
extern PFNGLUNIFORM1IVPROC                                  glUniform1iv              ;
extern PFNGLUNIFORM2IVPROC                                  glUniform2iv              ;
extern PFNGLUNIFORM3IVPROC                                  glUniform3iv              ;
extern PFNGLUNIFORM4IVPROC                                  glUniform4iv              ;
extern PFNGLUNIFORMMATRIX2FVPROC                            glUniformMatrix2fv        ;
extern PFNGLUNIFORMMATRIX3FVPROC                            glUniformMatrix3fv        ;
extern PFNGLUNIFORMMATRIX4FVPROC                            glUniformMatrix4fv        ;
extern PFNGLVALIDATEPROGRAMPROC                             glValidateProgram         ;
extern PFNGLVERTEXATTRIB1DPROC                              glVertexAttrib1d          ;
extern PFNGLVERTEXATTRIB1DVPROC                             glVertexAttrib1dv         ;
extern PFNGLVERTEXATTRIB1FPROC                              glVertexAttrib1f          ;
extern PFNGLVERTEXATTRIB1FVPROC                             glVertexAttrib1fv         ;
extern PFNGLVERTEXATTRIB1SPROC                              glVertexAttrib1s          ;
extern PFNGLVERTEXATTRIB1SVPROC                             glVertexAttrib1sv         ;
extern PFNGLVERTEXATTRIB2DPROC                              glVertexAttrib2d          ;
extern PFNGLVERTEXATTRIB2DVPROC                             glVertexAttrib2dv         ;
extern PFNGLVERTEXATTRIB2FPROC                              glVertexAttrib2f          ;
extern PFNGLVERTEXATTRIB2FVPROC                             glVertexAttrib2fv         ;
extern PFNGLVERTEXATTRIB2SPROC                              glVertexAttrib2s          ;
extern PFNGLVERTEXATTRIB2SVPROC                             glVertexAttrib2sv         ;
extern PFNGLVERTEXATTRIB3DPROC                              glVertexAttrib3d          ;
extern PFNGLVERTEXATTRIB3DVPROC                             glVertexAttrib3dv         ;
extern PFNGLVERTEXATTRIB3FPROC                              glVertexAttrib3f          ;
extern PFNGLVERTEXATTRIB3FVPROC                             glVertexAttrib3fv         ;
extern PFNGLVERTEXATTRIB3SPROC                              glVertexAttrib3s          ;
extern PFNGLVERTEXATTRIB3SVPROC                             glVertexAttrib3sv         ;
extern PFNGLVERTEXATTRIB4NBVPROC                            glVertexAttrib4Nbv        ;
extern PFNGLVERTEXATTRIB4NIVPROC                            glVertexAttrib4Niv        ;
extern PFNGLVERTEXATTRIB4NSVPROC                            glVertexAttrib4Nsv        ;
extern PFNGLVERTEXATTRIB4NUBPROC                            glVertexAttrib4Nub        ;
extern PFNGLVERTEXATTRIB4NUBVPROC                           glVertexAttrib4Nubv       ;
extern PFNGLVERTEXATTRIB4NUIVPROC                           glVertexAttrib4Nuiv       ;
extern PFNGLVERTEXATTRIB4NUSVPROC                           glVertexAttrib4Nusv       ;
extern PFNGLVERTEXATTRIB4BVPROC                             glVertexAttrib4bv         ;
extern PFNGLVERTEXATTRIB4DPROC                              glVertexAttrib4d          ;
extern PFNGLVERTEXATTRIB4DVPROC                             glVertexAttrib4dv         ;
extern PFNGLVERTEXATTRIB4FPROC                              glVertexAttrib4f          ;
extern PFNGLVERTEXATTRIB4FVPROC                             glVertexAttrib4fv         ;
extern PFNGLVERTEXATTRIB4IVPROC                             glVertexAttrib4iv         ;
extern PFNGLVERTEXATTRIB4SPROC                              glVertexAttrib4s          ;
extern PFNGLVERTEXATTRIB4SVPROC                             glVertexAttrib4sv         ;
extern PFNGLVERTEXATTRIB4UBVPROC                            glVertexAttrib4ubv        ;
extern PFNGLVERTEXATTRIB4UIVPROC                            glVertexAttrib4uiv        ;
extern PFNGLVERTEXATTRIB4USVPROC                            glVertexAttrib4usv        ;
extern PFNGLVERTEXATTRIBPOINTERPROC                         glVertexAttribPointer     ;

/*  GL_VERSION_2_1  */ 
extern PFNGLUNIFORMMATRIX2X3FVPROC                          glUniformMatrix2x3fv      ;
extern PFNGLUNIFORMMATRIX3X2FVPROC                          glUniformMatrix3x2fv      ;
extern PFNGLUNIFORMMATRIX2X4FVPROC                          glUniformMatrix2x4fv      ;
extern PFNGLUNIFORMMATRIX4X2FVPROC                          glUniformMatrix4x2fv      ;
extern PFNGLUNIFORMMATRIX3X4FVPROC                          glUniformMatrix3x4fv      ;
extern PFNGLUNIFORMMATRIX4X3FVPROC                          glUniformMatrix4x3fv      ;

/*  GL_VERSION_3_0  */ 
extern PFNGLCOLORMASKIPROC                                  glColorMaski          ;
extern PFNGLGETBOOLEANI_VPROC                               glGetBooleani_v       ;
extern PFNGLGETINTEGERI_VPROC                               glGetIntegeri_v       ;
extern PFNGLENABLEIPROC                                     glEnablei             ;
extern PFNGLDISABLEIPROC                                    glDisablei            ;
extern PFNGLISENABLEDIPROC                                  glIsEnabledi                 ;
extern PFNGLBEGINTRANSFORMFEEDBACKPROC                      glBeginTransformFeedback     ;
extern PFNGLENDTRANSFORMFEEDBACKPROC                        glEndTransformFeedback       ;
extern PFNGLBINDBUFFERRANGEPROC                             glBindBufferRange            ;
extern PFNGLBINDBUFFERBASEPROC                              glBindBufferBase             ;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC                   glTransformFeedbackVaryings  ;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC                 glGetTransformFeedbackVarying; 
extern PFNGLCLAMPCOLORPROC                                  glClampColor                 ;
extern PFNGLBEGINCONDITIONALRENDERPROC                      glBeginConditionalRender     ;
extern PFNGLENDCONDITIONALRENDERPROC                        glEndConditionalRender       ;
extern PFNGLVERTEXATTRIBIPOINTERPROC                        glVertexAttribIPointer       ;
extern PFNGLGETVERTEXATTRIBIIVPROC                          glGetVertexAttribIiv         ;
extern PFNGLGETVERTEXATTRIBIUIVPROC                         glGetVertexAttribIuiv        ;
extern PFNGLVERTEXATTRIBI1IPROC                             glVertexAttribI1i            ;
extern PFNGLVERTEXATTRIBI2IPROC                             glVertexAttribI2i            ;
extern PFNGLVERTEXATTRIBI3IPROC                             glVertexAttribI3i            ;
extern PFNGLVERTEXATTRIBI4IPROC                             glVertexAttribI4i            ;
extern PFNGLVERTEXATTRIBI1UIPROC                            glVertexAttribI1ui           ;
extern PFNGLVERTEXATTRIBI2UIPROC                            glVertexAttribI2ui           ;
extern PFNGLVERTEXATTRIBI3UIPROC                            glVertexAttribI3ui           ;
extern PFNGLVERTEXATTRIBI4UIPROC                            glVertexAttribI4ui           ;
extern PFNGLVERTEXATTRIBI1IVPROC                            glVertexAttribI1iv           ;
extern PFNGLVERTEXATTRIBI2IVPROC                            glVertexAttribI2iv           ;
extern PFNGLVERTEXATTRIBI3IVPROC                            glVertexAttribI3iv           ;
extern PFNGLVERTEXATTRIBI4IVPROC                            glVertexAttribI4iv           ;
extern PFNGLVERTEXATTRIBI1UIVPROC                           glVertexAttribI1uiv          ;
extern PFNGLVERTEXATTRIBI2UIVPROC                           glVertexAttribI2uiv          ;
extern PFNGLVERTEXATTRIBI3UIVPROC                           glVertexAttribI3uiv          ;
extern PFNGLVERTEXATTRIBI4UIVPROC                           glVertexAttribI4uiv          ;
extern PFNGLVERTEXATTRIBI4BVPROC                            glVertexAttribI4bv           ;
extern PFNGLVERTEXATTRIBI4SVPROC                            glVertexAttribI4sv           ;
extern PFNGLVERTEXATTRIBI4UBVPROC                           glVertexAttribI4ubv          ;
extern PFNGLVERTEXATTRIBI4USVPROC                           glVertexAttribI4usv          ;
extern PFNGLGETUNIFORMUIVPROC                               glGetUniformuiv              ;
extern PFNGLBINDFRAGDATALOCATIONPROC                        glBindFragDataLocation       ;
extern PFNGLGETFRAGDATALOCATIONPROC                         glGetFragDataLocation        ;
extern PFNGLUNIFORM1UIPROC                                  glUniform1ui                 ;
extern PFNGLUNIFORM2UIPROC                                  glUniform2ui                 ;
extern PFNGLUNIFORM3UIPROC                                  glUniform3ui                 ;
extern PFNGLUNIFORM4UIPROC                                  glUniform4ui                 ;
extern PFNGLUNIFORM1UIVPROC                                 glUniform1uiv                ;
extern PFNGLUNIFORM2UIVPROC                                 glUniform2uiv                ;
extern PFNGLUNIFORM3UIVPROC                                 glUniform3uiv                ;
extern PFNGLUNIFORM4UIVPROC                                 glUniform4uiv                ;
extern PFNGLTEXPARAMETERIIVPROC                             glTexParameterIiv            ;
extern PFNGLTEXPARAMETERIUIVPROC                            glTexParameterIuiv           ;
extern PFNGLGETTEXPARAMETERIIVPROC                          glGetTexParameterIiv         ;
extern PFNGLGETTEXPARAMETERIUIVPROC                         glGetTexParameterIuiv        ;
extern PFNGLCLEARBUFFERIVPROC                               glClearBufferiv              ;
extern PFNGLCLEARBUFFERUIVPROC                              glClearBufferuiv             ;
extern PFNGLCLEARBUFFERFVPROC                               glClearBufferfv              ;
extern PFNGLCLEARBUFFERFIPROC                               glClearBufferfi              ;
extern PFNGLGETSTRINGIPROC                                  glGetStringi                 ;

/*  GL_VERSION_3_1  */ 
extern PFNGLDRAWARRAYSINSTANCEDPROC                         glDrawArraysInstanced        ;
extern PFNGLDRAWELEMENTSINSTANCEDPROC                       glDrawElementsInstanced      ;
extern PFNGLTEXBUFFERPROC                                   glTexBuffer                  ;
extern PFNGLPRIMITIVERESTARTINDEXPROC                       glPrimitiveRestartIndex      ;

/*  GL_VERSION_3_2  */ 
extern PFNGLGETINTEGER64I_VPROC                             glGetInteger64i_v            ;
extern PFNGLGETBUFFERPARAMETERI64VPROC                      glGetBufferParameteri64v     ;
extern PFNGLFRAMEBUFFERTEXTUREPROC                          glFramebufferTexture         ;

/*  GL_ARB_framebuffer_object  */ 
extern PFNGLISRENDERBUFFERPROC                              glIsRenderbuffer                      ;
extern PFNGLBINDRENDERBUFFERPROC                            glBindRenderbuffer                    ;
extern PFNGLDELETERENDERBUFFERSPROC                         glDeleteRenderbuffers                 ;
extern PFNGLGENRENDERBUFFERSPROC                            glGenRenderbuffers                    ;
extern PFNGLRENDERBUFFERSTORAGEPROC                         glRenderbufferStorage                 ;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC                  glGetRenderbufferParameteriv          ;
extern PFNGLISFRAMEBUFFERPROC                               glIsFramebuffer                       ;
extern PFNGLBINDFRAMEBUFFERPROC                             glBindFramebuffer                     ;
extern PFNGLDELETEFRAMEBUFFERSPROC                          glDeleteFramebuffers                  ;
extern PFNGLGENFRAMEBUFFERSPROC                             glGenFramebuffers                     ;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC                      glCheckFramebufferStatus              ;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC                        glFramebufferTexture1D                ;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC                        glFramebufferTexture2D                ;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC                        glFramebufferTexture3D                ;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC                     glFramebufferRenderbuffer             ;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC         glGetFramebufferAttachmentParameteriv ;
extern PFNGLGENERATEMIPMAPPROC                              glGenerateMipmap                      ;
extern PFNGLBLITFRAMEBUFFERPROC                             glBlitFramebuffer                     ;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC              glRenderbufferStorageMultisample      ;
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC                     glFramebufferTextureLayer             ;

/*  GL_ARB_map_buffer_range  */ 
extern PFNGLMAPBUFFERRANGEPROC                              glMapBufferRange                      ;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC                      glFlushMappedBufferRange              ;

/*  GL_ARB_vertex_array_object  */ 
extern PFNGLBINDVERTEXARRAYPROC                             glBindVertexArray                     ;
extern PFNGLDELETEVERTEXARRAYSPROC                          glDeleteVertexArrays                  ;
extern PFNGLGENVERTEXARRAYSPROC                             glGenVertexArrays                     ;
extern PFNGLISVERTEXARRAYPROC                               glIsVertexArray                       ;

/*  GL_ARB_uniform_buffer_object  */
extern PFNGLGETUNIFORMINDICESPROC                           glGetUniformIndices                   ;
extern PFNGLGETACTIVEUNIFORMSIVPROC                         glGetActiveUniformsiv                 ;
extern PFNGLGETACTIVEUNIFORMNAMEPROC                        glGetActiveUniformName                ;
extern PFNGLGETUNIFORMBLOCKINDEXPROC                        glGetUniformBlockIndex                ;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC                     glGetActiveUniformBlockiv             ;
extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC                   glGetActiveUniformBlockName           ;
extern PFNGLUNIFORMBLOCKBINDINGPROC                         glUniformBlockBinding                 ;

/*  GL_ARB_copy_buffer  */ 
extern PFNGLCOPYBUFFERSUBDATAPROC                           glCopyBufferSubData                   ;

/*  GL_ARB_draw_elements_base_vertex  */ 
extern PFNGLDRAWELEMENTSBASEVERTEXPROC                      glDrawElementsBaseVertex              ;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC                 glDrawRangeElementsBaseVertex         ;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC             glDrawElementsInstancedBaseVertex     ;
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC                 glMultiDrawElementsBaseVertex         ;

/*  GL_ARB_provoking_vertex  */ 
extern PFNGLPROVOKINGVERTEXPROC                             glProvokingVertex                     ;

/*  GL_ARB_sync  */ 
extern PFNGLFENCESYNCPROC                                   glFenceSync                           ;
extern PFNGLISSYNCPROC                                      glIsSync                              ;
extern PFNGLDELETESYNCPROC                                  glDeleteSync                          ;
extern PFNGLCLIENTWAITSYNCPROC                              glClientWaitSync                      ;
extern PFNGLWAITSYNCPROC                                    glWaitSync                            ;
extern PFNGLGETINTEGER64VPROC                               glGetInteger64v                       ;
extern PFNGLGETSYNCIVPROC                                   glGetSynciv                           ;

/*  GL_ARB_texture_multisample  */ 
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC                       glTexImage2DMultisample;
extern PFNGLTEXIMAGE3DMULTISAMPLEPROC                       glTexImage3DMultisample;
extern PFNGLGETMULTISAMPLEFVPROC                            glGetMultisamplefv     ;
extern PFNGLSAMPLEMASKIPROC                                 glSampleMaski          ;

/*  GL_ARB_sample_shading  */ 
extern PFNGLMINSAMPLESHADINGARBPROC                         glMinSampleShading;

/*  GL_ARB_sampler_objects  */ 
extern PFNGLGENSAMPLERSPROC                                 glGenSamplers            ;
extern PFNGLDELETESAMPLERSPROC                              glDeleteSamplers         ;
extern PFNGLISSAMPLERPROC                                   glIsSampler              ;
extern PFNGLBINDSAMPLERPROC                                 glBindSampler            ;
extern PFNGLSAMPLERPARAMETERIPROC                           glSamplerParameteri      ;
extern PFNGLSAMPLERPARAMETERIVPROC                          glSamplerParameteriv     ;
extern PFNGLSAMPLERPARAMETERFPROC                           glSamplerParameterf      ;
extern PFNGLSAMPLERPARAMETERFVPROC                          glSamplerParameterfv     ;
extern PFNGLSAMPLERPARAMETERIIVPROC                         glSamplerParameterIiv    ;
extern PFNGLSAMPLERPARAMETERIUIVPROC                        glSamplerParameterIuiv   ;
extern PFNGLGETSAMPLERPARAMETERIVPROC                       glGetSamplerParameteriv  ;
extern PFNGLGETSAMPLERPARAMETERIIVPROC                      glGetSamplerParameterIiv ;
extern PFNGLGETSAMPLERPARAMETERFVPROC                       glGetSamplerParameterfv  ;
extern PFNGLGETSAMPLERPARAMETERIUIVPROC                     glGetSamplerParameterIuiv;

/*  GL_ARB_timer_query  */ 
extern PFNGLQUERYCOUNTERPROC                                glQueryCounter          ;
extern PFNGLGETQUERYOBJECTI64VPROC                          glGetQueryObjecti64v    ;
extern PFNGLGETQUERYOBJECTUI64VPROC                         glGetQueryObjectui64v   ;

/*  GL_ARB_tessellation_shader  */ 
extern PFNGLPATCHPARAMETERIPROC                             glPatchParameteri       ;
extern PFNGLPATCHPARAMETERFVPROC                            glPatchParameterfv      ;

/*  GL_ARB_get_program_binary  */ 
extern PFNGLGETPROGRAMBINARYPROC                            glGetProgramBinary      ;
extern PFNGLPROGRAMBINARYPROC                               glProgramBinary         ;
extern PFNGLPROGRAMPARAMETERIPROC                           glProgramParameteri     ;

/*  GL_ARB_base_instance  */
extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC             glDrawArraysInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC           glDrawElementsInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;

/*  GL_ARB_transform_feedback2  */
extern PFNGLBINDTRANSFORMFEEDBACKPROC                       glBindTransformFeedback               ;
extern PFNGLDELETETRANSFORMFEEDBACKSPROC                    glDeleteTransformFeedbacks            ;
extern PFNGLGENTRANSFORMFEEDBACKSPROC                       glGenTransformFeedbacks               ;
extern PFNGLISTRANSFORMFEEDBACKPROC                         glIsTransformFeedback                 ;
extern PFNGLPAUSETRANSFORMFEEDBACKPROC                      glPauseTransformFeedback              ;
extern PFNGLRESUMETRANSFORMFEEDBACKPROC                     glResumeTransformFeedback             ;
extern PFNGLDRAWTRANSFORMFEEDBACKPROC                       glDrawTransformFeedback               ;

/*  GL_ARB_transform_feedback3  */
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC                 glDrawTransformFeedbackStream         ;
extern PFNGLBEGINQUERYINDEXEDPROC                           glBeginQueryIndexed                   ;
extern PFNGLENDQUERYINDEXEDPROC                             glEndQueryIndexed                     ;
extern PFNGLGETQUERYINDEXEDIVPROC                           glGetQueryIndexediv                   ;

/*  GL_ARB_transform_feedback_instanced  */
extern PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC              glDrawTransformFeedbackInstanced      ;
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC        glDrawTransformFeedbackStreamInstanced;

/*  GL_VERSION_3_3  */ 
extern PFNGLVERTEXATTRIBDIVISORPROC                         glVertexAttribDivisor                 ;

/*  GL_ARB_texture_storage  */
extern PFNGLTEXSTORAGE1DPROC                                glTexStorage1D;
extern PFNGLTEXSTORAGE2DPROC                                glTexStorage2D;
extern PFNGLTEXSTORAGE3DPROC                                glTexStorage3D;

/*  GL_ARB_debug_output  */
extern PFNGLDEBUGMESSAGECONTROLARBPROC                      glDebugMessageControlARB ;
extern PFNGLDEBUGMESSAGEINSERTARBPROC                       glDebugMessageInsertARB  ;
extern PFNGLDEBUGMESSAGECALLBACKARBPROC                     glDebugMessageCallbackARB;
extern PFNGLGETDEBUGMESSAGELOGARBPROC                       glGetDebugMessageLogARB  ;

/*  GL_AMD_performance_monitor  */
extern PFNGLGETPERFMONITORGROUPSAMDPROC                     glGetPerfMonitorGroupsAMD       ;
extern PFNGLGETPERFMONITORCOUNTERSAMDPROC                   glGetPerfMonitorCountersAMD     ;
extern PFNGLGETPERFMONITORGROUPSTRINGAMDPROC                glGetPerfMonitorGroupStringAMD  ;
extern PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC              glGetPerfMonitorCounterStringAMD;
extern PFNGLGETPERFMONITORCOUNTERINFOAMDPROC                glGetPerfMonitorCounterInfoAMD  ;
extern PFNGLGENPERFMONITORSAMDPROC                          glGenPerfMonitorsAMD            ;
extern PFNGLDELETEPERFMONITORSAMDPROC                       glDeletePerfMonitorsAMD         ;
extern PFNGLSELECTPERFMONITORCOUNTERSAMDPROC                glSelectPerfMonitorCountersAMD  ;
extern PFNGLBEGINPERFMONITORAMDPROC                         glBeginPerfMonitorAMD           ;
extern PFNGLENDPERFMONITORAMDPROC                           glEndPerfMonitorAMD             ;
extern PFNGLGETPERFMONITORCOUNTERDATAAMDPROC                glGetPerfMonitorCounterDataAMD  ;

} }

#endif
