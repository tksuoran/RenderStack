#error not in use
#if defined(_WIN32)
#    define _CRT_SECURE_NO_WARNINGS
#    define WIN32_LEAN_AND_MEAN
#    define VC_EXTRALEAN
#    define STRICT
#    define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#    define NOVIRTUALKEYCODES // VK_*
#    define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#    define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#    define NOSYSMETRICS      // SM_*
#    define NOMENUS           // MF_*
#    define NOICONS           // IDI_*
#    define NOKEYSTATES       // MK_*
#    define NOSYSCOMMANDS     // SC_*
#    define NORASTEROPS       // Binary and Tertiary raster ops
#    define NOSHOWWINDOW      // SW_*
#    define OEMRESOURCE       // OEM Resource values
#    define NOATOM            // Atom Manager routines
#    define NOCLIPBOARD       // Clipboard routines
#    define NOCOLOR           // Screen colors
#    define NOCTLMGR          // Control and Dialog routines
#    define NODRAWTEXT        // DrawText() and DT_*
#    define NOGDI             // All GDI defines and routines
#    define NOKERNEL          // All KERNEL defines and routines
#    define NOUSER            // All USER defines and routines
#    define NONLS             // All NLS defines and routines
#    define NOMB              // MB_* and MessageBox()
#    define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#    define NOMETAFILE        // typedef METAFILEPICT
#    define NOMINMAX          // Macros min(a,b) and max(a,b)
#    define NOMSG             // typedef MSG and associated routines
#    define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#    define NOSCROLL          // SB_* and scrolling routines
#    define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#    define NOSOUND           // Sound driver routines
#    define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#    define NOWH              // SetWindowsHook and WH_*
#    define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#    define NOCOMM            // COMM driver routines
#    define NOKANJI           // Kanji support stuff.
#    define NOHELP            // Help engine interface.
#    define NOPROFILER        // Profiler interface.
#    define NODEFERWINDOWPOS  // DeferWindowPos routines
#    define NOMCX             // Modem Configuration Extensions
#    include <windows.h>
#endif

#include "renderstack_toolkit/dynamic_opengl_es2.hpp"

namespace gl
{

void        check_error(void);
const char *enum_string(GLenum e);

#if defined(SUPPORT_LEGACY_OPENGL)
void enable_client_state(GLenum array);
void disable_client_state(GLenum array);
void vertex_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void normal_pointer(GLenum type, GLsizei stride, const GLvoid *pointer);
void color_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void texcoord_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
/*  GL_VERSION_1_3  */
void _client_active_texture(GLenum texture);
#endif

void           bind_texture(GLenum target, GLuint texture);
void           blend_func(GLenum sfactor, GLenum dfactor);
void           clear(GLbitfield mask);
void           clear_color(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void           clear_depth(GLclampf depth);
void           clear_stencil(GLint s);
void           color_mask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void           copy_tex_image_2d(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void           copy_tex_sub_image_2d(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void           cull_face(GLenum mode);
void           delete_textures(GLsizei n, const GLuint *textures);
void           depth_func(GLenum func);
void           depth_mask(GLboolean flag);
void           depth_range(GLclampf zNear, GLclampf zFar);
void           disable(GLenum cap);
void           draw_arrays(GLenum mode, GLint first, GLsizei count);
void           draw_elements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void           enable(GLenum cap);
void           finish(void);
void           flush(void);
void           front_face(GLenum mode);
void           gen_textures(GLsizei n, GLuint *textures);
void           get_boolean_v(GLenum pname, GLboolean *params);
GLenum         get_error(void);
void           get_float_v(GLenum pname, GLfloat *params);
void           get_integer_v(GLenum pname, GLint *params);
const GLubyte *get_string(GLenum name);
void           get_tex_parameter_fv(GLenum target, GLenum pname, GLfloat *params);
void           get_tex_parameter_iv(GLenum target, GLenum pname, GLint *params);
void           hint(GLenum target, GLenum mode);
GLboolean      is_enabled(GLenum cap);
GLboolean      is_texture(GLuint texture);
void           line_width(GLfloat width);
void           pixel_store_i(GLenum pname, GLint param);
void           polygon_offset(GLfloat factor, GLfloat units);
void           read_pixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void           scissor(GLint x, GLint y, GLsizei width, GLsizei height);
void           stencil_func(GLenum func, GLint ref, GLuint mask);
void           stencil_mask(GLuint mask);
void           stencil_op(GLenum fail, GLenum zfail, GLenum zpass);
void           tex_image_2d(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void           tex_parameter_f(GLenum target, GLenum pname, GLfloat param);
void           tex_parameter_fv(GLenum target, GLenum pname, const GLfloat *params);
void           tex_parameter_i(GLenum target, GLenum pname, GLint param);
void           tex_parameter_iv(GLenum target, GLenum pname, const GLint *params);
void           tex_sub_image_2d(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void           viewport(GLint x, GLint y, GLsizei width, GLsizei height);
/*  GL_VERSION_1_2  */
void blend_color(GLclampf a, GLclampf b, GLclampf c, GLclampf d);
void blend_equation(GLenum a);
/*  GL_VERSION_1_3  */
void active_texture(GLenum a);
void sample_coverage(GLclampf a, GLboolean b);
void compressed_tex_image_2d(GLenum a, GLint b, GLenum c, GLsizei d, GLsizei e, GLint f, GLsizei g, const GLvoid *h);
void compressed_tex_sub_image_2d(GLenum a, GLint b, GLint c, GLint d, GLsizei e, GLsizei f, GLenum g, GLsizei h, const GLvoid *i);
/*  GL_VERSION_1_4  */
void      blend_func_separate(GLenum a, GLenum b, GLenum c, GLenum d);
void      bind_buffer(GLenum a, GLuint b);
void      delete_buffers(GLsizei a, const GLuint *b);
void      gen_buffers(GLsizei a, GLuint *b);
GLboolean is_buffer(GLuint a);
void      buffer_data(GLenum a, GLsizeiptr b, const GLvoid *c, GLenum d);
void      buffer_sub_data(GLenum a, GLintptr b, GLsizeiptr c, const GLvoid *d);

/* GL_EXT_occlusion_query_boolean */
void      gen_queries(GLsizei a, GLuint *b);
void      delete_queries(GLsizei a, const GLuint *b);
GLboolean is_query(GLuint a);
void      begin_query(GLenum a, GLuint b);
void      end_query(GLenum a);
void      get_query_iv(GLenum a, GLenum b, GLint *c);
void      get_query_object_iv(GLuint a, GLenum b, GLint *c);
void      get_query_object_uiv(GLuint a, GLenum b, GLuint *c);

GLvoid *  map_buffer(GLenum target, GLenum access);
GLboolean unmap_buffer(GLenum);
void      get_buffer_pointer_v(GLenum a, GLenum b, GLvoid **c);

/*  GL_VERSION_2_0  */
void      blend_equation_separate(GLenum a, GLenum b);
void      stencil_op_separate(GLenum a, GLenum b, GLenum c, GLenum d);
void      stencil_func_Separate(GLenum a, GLenum b, GLint c, GLuint d);
void      stencil_mask_separate(GLenum a, GLuint b);
void      attach_shader(GLuint a, GLuint b);
void      bind_attrib_location(GLuint a, GLuint b, const GLchar *c);
void      compile_shader(GLuint a);
GLuint    create_program(void);
GLuint    create_shader(GLenum a);
void      delete_program(GLuint a);
void      delete_shader(GLuint a);
void      detach_shader(GLuint a, GLuint b);
void      disable_vertex_attrib_array(GLuint a);
void      enable_vertex_attrib_array(GLuint a);
void      get_active_attrib(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLint *e, GLenum *f, GLchar *g);
void      get_active_uniform(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLint *e, GLenum *f, GLchar *g);
void      get_attached_shaders(GLuint a, GLsizei b, GLsizei *c, GLuint *d);
GLint     get_attrib_location(GLuint a, const GLchar *b);
void      get_program_iv(GLuint a, GLenum b, GLint *c);
void      get_program_info_log(GLuint a, GLsizei b, GLsizei *c, GLchar *d);
void      get_shader_iv(GLuint shader, GLenum pname, GLint *params);
void      get_shader_info_log(GLuint shader, GLsizei max_length, GLsizei *length, GLchar *info_log);
void      get_shader_source(GLuint a, GLsizei b, GLsizei *c, GLchar *d);
GLint     get_uniform_location(GLuint a, const GLchar *b);
void      get_uniform_fv(GLuint a, GLint b, GLfloat *c);
void      get_uniform_iv(GLuint a, GLint b, GLint *c);
void      get_vertex_attrib_fv(GLuint a, GLenum b, GLfloat *c);
void      get_vertex_attrib_iv(GLuint a, GLenum b, GLint *c);
void      get_vertex_attrib_pointer_v(GLuint a, GLenum b, GLvoid **c);
GLboolean is_program(GLuint a);
GLboolean is_shader(GLuint a);
void      link_program(GLuint a);
void      shader_source(GLuint a, GLsizei b, const GLchar **c, const GLint *d);
void      use_program(GLuint a);
void      uniform_1f(GLint a, GLfloat b);
void      uniform_2f(GLint a, GLfloat b, GLfloat c);
void      uniform_3f(GLint a, GLfloat b, GLfloat c, GLfloat d);
void      uniform_4f(GLint a, GLfloat b, GLfloat c, GLfloat d, GLfloat e);
void      uniform_1i(GLint a, GLint b);
void      uniform_2i(GLint a, GLint b, GLint c);
void      uniform_3i(GLint a, GLint b, GLint c, GLint d);
void      uniform_4i(GLint a, GLint b, GLint c, GLint d, GLint e);
void      uniform_1fv(GLint a, GLsizei b, const GLfloat *c);
void      uniform_2fv(GLint a, GLsizei b, const GLfloat *c);
void      uniform_3fv(GLint a, GLsizei b, const GLfloat *c);
void      uniform_4fv(GLint a, GLsizei b, const GLfloat *c);
void      uniform_1iv(GLint a, GLsizei b, const GLint *c);
void      uniform_2iv(GLint a, GLsizei b, const GLint *c);
void      uniform_3iv(GLint a, GLsizei b, const GLint *c);
void      uniform_4iv(GLint a, GLsizei b, const GLint *c);
void      uniform_matrix_2fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d);
void      uniform_matrix_3fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d);
void      uniform_matrix_4fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d);
void      validate_program(GLuint a);
void      vertex_attrib_1f(GLuint a, GLfloat b);
void      vertex_attrib_1fv(GLuint a, const GLfloat *b);
void      vertex_attrib_2f(GLuint a, GLfloat b, GLfloat c);
void      vertex_attrib_2fv(GLuint a, const GLfloat *b);
void      vertex_attrib_3f(GLuint a, GLfloat b, GLfloat c, GLfloat d);
void      vertex_attrib_3fv(GLuint a, const GLfloat *b);
void      vertex_attrib_4f(GLuint a, GLfloat b, GLfloat c, GLfloat d, GLfloat e);
void      vertex_attrib_4fv(GLuint a, const GLfloat *b);
void      vertex_attrib_pointer(GLuint a, GLint b, GLenum c, GLboolean d, GLsizei e, const GLvoid *f);

/*  3.0  */
/*  ARB_framebuffer_object  */
GLboolean is_renderbuffer(GLuint a);
void      bind_renderbuffer(GLenum a, GLuint b);
void      delete_renderbuffers(GLsizei a, const GLuint *b);
void      gen_renderbuffers(GLsizei a, GLuint *b);
void      renderbuffer_storage(GLenum a, GLenum b, GLsizei c, GLsizei d);
void      get_renderbuffer_parameter_iv(GLenum a, GLenum b, GLint *c);
GLboolean is_framebuffer(GLuint a);
void      bind_framebuffer(GLenum a, GLuint b);
void      delete_framebuffers(GLsizei a, const GLuint *b);
void      gen_framebuffers(GLsizei, GLuint *);
GLenum    check_framebuffer_status(GLenum a);
void      framebuffer_texture_2d(GLenum a, GLenum b, GLenum c, GLuint d, GLint e);
void      framebuffer_renderbuffer(GLenum a, GLenum b, GLenum c, GLuint d);
void      get_framebuffer_attachment_parameter_iv(GLenum a, GLenum b, GLenum c, GLint *d);
void      generate_mipmap(GLenum a);

/* GL_OES_vertex_array_object */ /*  ARB_vertex_array_object  */
void      bind_vertex_array(GLuint a);
void      delete_vertex_arrays(GLsizei a, const GLuint *b);
void      gen_vertex_arrays(GLsizei a, GLuint *b);
GLboolean is_vertex_array(GLuint a);

} /* namespace gl */
