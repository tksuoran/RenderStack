#ifndef renderstack_toolkit__gles_ext_hpp
#define renderstack_toolkit__gles_ext_hpp

#include "platform.hpp"
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2)
# include "GLES2/gl2.h"
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
# include "GLES3/gl3.h"
#endif
#include "GLES2/gl2ext.h"

namespace gl { namespace detail {

/*  GL_OES_get_program_binary */
extern PFNGLGETPROGRAMBINARYOESPROC                     glGetProgramBinaryOES               ;
extern PFNGLPROGRAMBINARYOESPROC                        glProgramBinaryOES                  ;

/*  GL_OES_mapbuffer  */
extern PFNGLMAPBUFFEROESPROC                            glMapBufferOES                      ;
extern PFNGLUNMAPBUFFEROESPROC                          glUnmapBufferOES                    ;
extern PFNGLGETBUFFERPOINTERVOESPROC                    glGetBufferPointervOES              ;

/*  GL_OES_texture_3D  */
extern PFNGLTEXIMAGE3DOESPROC                           glTexImage3DOES                     ;
extern PFNGLTEXSUBIMAGE3DOESPROC                        glTexSubImage3DOES                  ;
extern PFNGLCOPYTEXSUBIMAGE3DOESPROC                    glCopyTexSubImage3DOES              ;
extern PFNGLCOMPRESSEDTEXIMAGE3DOESPROC                 glCompressedTexImage3DOES           ;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC              glCompressedTexSubImage3DOES        ;
extern PFNGLFRAMEBUFFERTEXTURE3DOES                     glFramebufferTexture3DOES           ;

/* GL_OES_vertex_array_object */
extern PFNGLBINDVERTEXARRAYOESPROC                      glBindVertexArrayOES                ;
extern PFNGLDELETEVERTEXARRAYSOESPROC                   glDeleteVertexArraysOES             ;
extern PFNGLGENVERTEXARRAYSOESPROC                      glGenVertexArraysOES                ;
extern PFNGLISVERTEXARRAYOESPROC                        glIsVertexArrayOES                  ;

/* GL_EXT_discard_framebuffer */
extern PFNGLDISCARDFRAMEBUFFEREXTPROC                   glDiscardFramebufferEXT             ;

/* GL_EXT_multisampled_render_to_texture */
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC       glRenderbufferStorageMultisampleEXT     ;
extern PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC      glFramebufferTexture2DMultisampleEXT    ;

/* GL_EXT_multi_draw_arrays  */
extern PFNGLMULTIDRAWARRAYSEXTPROC                      glMultiDrawArraysEXT                ;
extern PFNGLMULTIDRAWELEMENTSEXTPROC                    glMultiDrawElementsEXT              ;

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

} }

#endif
