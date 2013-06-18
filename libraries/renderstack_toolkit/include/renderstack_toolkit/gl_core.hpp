#ifndef gl_core_hpp_renderstack_toolkit
#define gl_core_hpp_renderstack_toolkit

#include "platform.hpp"
#include "renderstack_toolkit/gl.hpp"

namespace gl {

void        set_error_checking(bool enable);
void        check_error (void);
const char *enum_string (GLenum e);

#if defined(RENDERSTACK_GL_API_OPENGL_WITH_LEGACY) || defined(RENDERSTACK_GL_API_OPENGL_ES_1)
// Fog - vertex shaders
void fog_f (GLenum pname, GLfloat param);
void fog_fv (GLenum pname, const GLfloat *params);
void fog_i (GLenum pname, GLint param);
void fog_iv (GLenum pname, const GLint *params);

// Texenv
void get_tex_env_fv(GLenum target, GLenum pname, GLfloat *params);
void get_tex_env_iv(GLenum target, GLenum pname, GLint *params);
void tex_env_f(GLenum target, GLenum pname, GLfloat param);
void tex_env_fv(GLenum target, GLenum pname, const GLfloat *params);
void tex_env_i(GLenum target, GLenum pname, GLint param);
void tex_env_iv(GLenum target, GLenum pname, const GLint *params);

// Texgen
#if defined(RENDERSTACK_GL_API_OPENGL) 
void tex_gen_d (GLenum coord, GLenum pname, GLdouble param);
void tex_gen_dv (GLenum coord, GLenum pname, const GLdouble *params);
#endif
void tex_gen_f (GLenum coord, GLenum pname, GLfloat param);
void tex_gen_fv (GLenum coord, GLenum pname, const GLfloat *params);
void tex_gen_i (GLenum coord, GLenum pname, GLint param);
void tex_gen_iv (GLenum coord, GLenum pname, const GLint *params);

// Lights
void light_model_f (GLenum pname, GLfloat param);
void light_model_fv (GLenum pname, const GLfloat *params);
void light_model_i (GLenum pname, GLint param);
void light_model_iv (GLenum pname, const GLint *params);
void light_f (GLenum light, GLenum pname, GLfloat param);
void light_fv (GLenum light, GLenum pname, const GLfloat *params);
void light_i (GLenum light, GLenum pname, GLint param);
void light_iv (GLenum light, GLenum pname, const GLint *params);

// Matrices
void load_identity (void);
#if defined(RENDERSTACK_GL_API_OPENGL) 
void load_matrix_d (const GLdouble *m);
#endif
void load_matrix_f (const GLfloat *m);
void matrix_mode (GLenum mode);
#if defined(RENDERSTACK_GL_API_OPENGL) 
void mult_matrix_d (const GLdouble *m);
#endif
void mult_matrix_f (const GLfloat *m);

// Material
void material_f (GLenum face, GLenum pname, GLfloat param);
void material_fv (GLenum face, GLenum pname, const GLfloat *params);
void material_i (GLenum face, GLenum pname, GLint param);
void material_iv (GLenum face, GLenum pname, const GLint *params);

void shade_model (GLenum mode);

void enable_client_state(GLenum array);
void disable_client_state(GLenum array);
void vertex_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void normal_pointer(GLenum type, GLsizei stride, const GLvoid *pointer);
void color_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void texcoord_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

/*  GL_VERSION_1_3  */ 
void client_active_texture(GLenum texture);
#endif

/*  GL_H  */ 
void array_element(GLint i);
void bind_texture(GLenum target, GLuint texture);
void blend_func(GLenum sfactor, GLenum dfactor);
void clear(GLbitfield mask);
void clear_color(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void clear_stencil(GLint s);
void color_mask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void copy_tex_image_2d(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void copy_tex_sub_image_2d(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void cull_face(GLenum mode);
void delete_textures(GLsizei n, const GLuint *textures);
void depth_func(GLenum func);
void depth_mask(GLboolean flag);
void disable(GLenum cap);
void draw_arrays(GLenum mode, GLint first, GLsizei count);
void draw_elements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void enable(GLenum cap);
void finish(void);
void flush(void);
void front_face(GLenum mode);
void gen_textures(GLsizei n, GLuint *textures);
void get_boolean_v(GLenum pname, GLboolean *params);
GLenum get_error(void);
void get_float_v(GLenum pname, GLfloat *params);
void get_integer_v(GLenum pname, GLint *params);
const GLubyte * get_string(GLenum name);
void get_tex_parameter_fv(GLenum target, GLenum pname, GLfloat *params);
void get_tex_parameter_iv(GLenum target, GLenum pname, GLint *params);
void hint(GLenum target, GLenum mode);
GLboolean is_enabled(GLenum cap);
GLboolean is_texture(GLuint texture);
void line_width(GLfloat width);
void pixel_store_i(GLenum pname, GLint param);
void polygon_offset(GLfloat factor, GLfloat units);
void read_pixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
void stencil_func(GLenum func, GLint ref, GLuint mask);
void stencil_mask(GLuint mask);
void stencil_op(GLenum fail, GLenum zfail, GLenum zpass);
void tex_image_2d(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void tex_parameter_f(GLenum target, GLenum pname, GLfloat param);
void tex_parameter_fv(GLenum target, GLenum pname, const GLfloat *params);
void tex_parameter_i(GLenum target, GLenum pname, GLint param);
void tex_parameter_iv(GLenum target, GLenum pname, const GLint *params);
void tex_sub_image_2d(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void viewport(GLint x, GLint y, GLsizei width, GLsizei height);

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void read_buffer(GLenum mode); // es 3
#endif

#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void depth_range(GLclampf zNear, GLclampf zFar);
#else
void clear_depth(GLclampd depth);
void copy_tex_image_1d(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
void copy_tex_sub_image_1d(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void depth_range(GLclampd zNear, GLclampd zFar);
void draw_buffer(GLenum mode);
void get_double_v(GLenum pname, GLdouble *params);
void get_pointer_v(GLenum pname, GLvoid* *params);
void get_tex_image(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void get_tex_level_parameter_fv(GLenum target, GLint level, GLenum pname, GLfloat *params);
void get_tex_level_parameter_iv(GLenum target, GLint level, GLenum pname, GLint *params);
void logic_op(GLenum opcode);
void pixel_store_f(GLenum pname, GLfloat param);
void point_size(GLfloat size);
void polygon_mode(GLenum face, GLenum mode);
void tex_image_1d(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void tex_sub_image_1d(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
#endif

/*  GL_VERSION_1_2  */ 
void blend_color(GLclampf a, GLclampf b, GLclampf c, GLclampf d);
void blend_equation(GLenum a);

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void draw_range_elements(GLenum a, GLuint b, GLuint c, GLsizei d, GLenum e, const GLvoid *f);
void tex_image_3d(GLenum a, GLint b, GLint c, GLsizei d, GLsizei e, GLsizei f, GLint g, GLenum h, GLenum i, const GLvoid *j);
void tex_sub_image_3d(GLenum a, GLint b, GLint c, GLint d, GLint e, GLsizei f, GLsizei g, GLsizei h, GLenum i, GLenum j, const GLvoid *k);
void copy_tex_sub_image_3d(GLenum a, GLint b, GLint c, GLint d, GLint e, GLint f, GLint g, GLsizei h, GLsizei i);
#endif

/*  GL_VERSION_1_3  */ 
void active_texture(GLenum a);
void sample_coverage(GLclampf a, GLboolean b);
void compressed_tex_image_2d(GLenum a, GLint b, GLenum c, GLsizei d, GLsizei e, GLint f, GLsizei g, const GLvoid *h);
void compressed_tex_sub_image_2d(GLenum a, GLint b, GLint c, GLint d, GLsizei e, GLsizei f, GLenum g, GLsizei h, const GLvoid *i);

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void compressed_tex_image_3d(GLenum a, GLint b, GLenum c, GLsizei d, GLsizei e, GLsizei f, GLint g, GLsizei h, const GLvoid *i);
void compressed_tex_sub_image_3d(GLenum a, GLint b, GLint c, GLint d, GLint e, GLsizei f, GLsizei g, GLsizei h, GLenum i, GLsizei j, const GLvoid *k);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void compressed_tex_image_1d(GLenum a, GLint b, GLenum c, GLsizei d, GLint e, GLsizei f, const GLvoid *g);
void compressed_tex_sub_image_1d(GLenum a, GLint b, GLint c, GLsizei d, GLenum e, GLsizei f, const GLvoid *g);
void get_compressed_tex_image(GLenum a, GLint b, GLvoid *c);
#endif


/*  GL_VERSION_1_4  */ 
void blend_func_separate(GLenum a, GLenum b, GLenum c, GLenum d);

#if defined(RENDERSTACK_GL_API_OPENGL)
void multi_draw_arrays(GLenum a, GLint *b, GLsizei *c, GLsizei d);
void multi_draw_elements(GLenum a, const GLsizei *b, GLenum c, const GLvoid* *d, GLsizei e);
void point_parameter_f(GLenum a, GLfloat b);
void point_parameter_fv(GLenum a, const GLfloat *b);
void point_parameter_i(GLenum a, GLint b);
void point_parameter_iv(GLenum a, const GLint *b);
#endif

/*  GL_VERSION_1_5  */ 
void bind_buffer(GLenum a, GLuint b);
void delete_buffers(GLsizei a, const GLuint *b);
void gen_buffers(GLsizei a, GLuint *b);
GLboolean is_buffer(GLuint a);
void buffer_data(GLenum a, GLsizeiptr b, const GLvoid *c, GLenum d);
void buffer_sub_data(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void get_buffer_parameter_iv(GLenum a, GLenum b, GLint *c);

// GL_OES_mapbuffer
GLvoid* map_buffer_oes(GLenum target, GLenum access);
void get_buffer_pointer_v_oes(GLenum target, GLenum pname, GLvoid **params);
GLboolean unmap_buffer_oes(GLenum target);

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void gen_queries(GLsizei a, GLuint *b);
void delete_queries(GLsizei a, const GLuint *b);
GLboolean is_query(GLuint a);
void begin_query(GLenum a, GLuint b);
void end_query(GLenum a);
void get_query_iv(GLenum a, GLenum b, GLint *c);
void get_query_object_uiv(GLuint a, GLenum b, GLuint *c);
GLvoid* map_buffer(GLenum target, GLenum access);
GLboolean unmap_buffer(GLenum target);
void get_buffer_pointer_v(GLenum a, GLenum b, GLvoid* *c);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void get_buffer_sub_data(GLenum a, GLintptr b, GLsizeiptr c, GLvoid *d);
void get_query_object_iv(GLuint a, GLenum b, GLint *c);
#endif

/*  GL_VERSION_2_0  */ 
void blend_equation_separate(GLenum a, GLenum b);
void stencil_op_separate(GLenum a, GLenum b, GLenum c, GLenum d);
void stencil_func_Separate(GLenum a, GLenum b, GLint c, GLuint d);
void stencil_mask_separate(GLenum a, GLuint b);
void attach_shader(GLuint a, GLuint b);
void bind_attrib_location(GLuint a, GLuint b, const GLchar * c);
void compile_shader(GLuint a);
GLuint create_program(void);
GLuint create_shader(GLenum a);
void delete_program(GLuint a);
void delete_shader(GLuint a);
void detach_shader(GLuint a, GLuint b);
void disable_vertex_attrib_array(GLuint a);
void enable_vertex_attrib_array(GLuint a);
void get_active_attrib(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLint *e, GLenum *f, GLchar *g);
void get_active_uniform(GLuint a, GLuint b , GLsizei c, GLsizei *d, GLint *e, GLenum *f, GLchar *g);
void get_attached_shaders(GLuint a, GLsizei b, GLsizei *c, GLuint *d);
GLint get_attrib_location(GLuint a, const GLchar *b);
void get_program_iv(GLuint a, GLenum b, GLint *c);
void get_program_info_log(GLuint a, GLsizei b, GLsizei *c, GLchar *d);
void get_shader_iv(GLuint shader, GLenum pname, GLint *params);
void get_shader_info_log(GLuint shader, GLsizei max_length, GLsizei *length, GLchar *info_log);
void get_shader_source(GLuint a, GLsizei b, GLsizei *c, GLchar *d);
GLint get_uniform_location(GLuint a, const GLchar *b);
void get_uniform_fv(GLuint a, GLint b, GLfloat *c);
void get_uniform_iv(GLuint a, GLint b, GLint *c);
void get_vertex_attrib_fv(GLuint a, GLenum b, GLfloat *c);
void get_vertex_attrib_iv(GLuint a, GLenum b, GLint *c);
void get_vertex_attrib_pointer_v(GLuint a, GLenum b, GLvoid* *c);
GLboolean is_program(GLuint a);
GLboolean is_shader(GLuint a);
void link_program(GLuint a);
void shader_source(GLuint a, GLsizei b, const GLchar* *c, const GLint *d);
void use_program(GLuint a);
void uniform_1f(GLint location, GLfloat b);
void uniform_2f(GLint location, GLfloat b, GLfloat c);
void uniform_3f(GLint location, GLfloat b, GLfloat c, GLfloat d);
void uniform_4f(GLint location, GLfloat b, GLfloat c, GLfloat d, GLfloat e);
void uniform_1i(GLint location, GLint b);
void uniform_2i(GLint location, GLint b, GLint c);
void uniform_3i(GLint location, GLint b, GLint c, GLint d);
void uniform_4i(GLint location, GLint b, GLint c, GLint d, GLint e);
void uniform_1fv(GLint location, GLsizei b, const GLfloat *c);
void uniform_2fv(GLint location, GLsizei b, const GLfloat *c);
void uniform_3fv(GLint location, GLsizei b, const GLfloat *c);
void uniform_4fv(GLint location, GLsizei b, const GLfloat *c);
void uniform_1iv(GLint location, GLsizei b, const GLint *c);
void uniform_2iv(GLint location, GLsizei b, const GLint *c);
void uniform_3iv(GLint location, GLsizei b, const GLint *c);
void uniform_4iv(GLint location, GLsizei b, const GLint *c);
void uniform_matrix_2fv(GLint location, GLsizei b, GLboolean c, const GLfloat *d);
void uniform_matrix_3fv(GLint location, GLsizei b, GLboolean c, const GLfloat *d);
void uniform_matrix_4fv(GLint location, GLsizei b, GLboolean c, const GLfloat *d);
void validate_program(GLuint a);
void vertex_attrib_2f(GLuint a, GLfloat b, GLfloat c);
void vertex_attrib_2fv(GLuint a, const GLfloat *b);
void vertex_attrib_3f(GLuint a, GLfloat b, GLfloat c, GLfloat d);
void vertex_attrib_3fv(GLuint a, const GLfloat *b);
void vertex_attrib_4f(GLuint a, GLfloat b, GLfloat c, GLfloat d, GLfloat e);
void vertex_attrib_4fv(GLuint a, const GLfloat *b);
void vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

#if defined(RENDERSTACK_GL_API_OPENGL)
void draw_buffers(GLsizei a, const GLenum * b);
void get_vertex_attrib_dv(GLuint a, GLenum b, GLdouble *c);
void vertex_attrib_1d(GLuint a, GLdouble b);
void vertex_attrib_1dv(GLuint a, const GLdouble *b);
void vertex_attrib_1f(GLuint a, GLfloat b);
void vertex_attrib_1fv(GLuint a, const GLfloat *b);
void vertex_attrib_1s(GLuint a, GLshort b);
void vertex_attrib_1sv(GLuint a, const GLshort *b);
void vertex_attrib_2d(GLuint a, GLdouble b, GLdouble c);
void vertex_attrib_2dv(GLuint a, const GLdouble *b);
void vertex_attrib_2s(GLuint a, GLshort b, GLshort c);
void vertex_attrib_2sv(GLuint a, const GLshort *b);
void vertex_attrib_3d(GLuint a, GLdouble b, GLdouble c, GLdouble d);
void vertex_attrib_3dv(GLuint a, const GLdouble *b);
void vertex_attrib_3s(GLuint a, GLshort b, GLshort c, GLshort d);
void vertex_attrib_3sv(GLuint a, const GLshort *b);
void vertex_attrib_4Nbv(GLuint a, const GLbyte *b);
void vertex_attrib_4Niv(GLuint a, const GLint *b);
void vertex_attrib_4Nsv(GLuint a, const GLshort *b);
void vertex_attrib_4Nub(GLuint a, GLubyte b, GLubyte c, GLubyte d, GLubyte e);
void vertex_attrib_4Nubv(GLuint a, const GLubyte *b);
void vertex_attrib_4Nuiv(GLuint a, const GLuint *b);
void vertex_attrib_4Nusv(GLuint a, const GLushort *b);
void vertex_attrib_4bv(GLuint a, const GLbyte *b);
void vertex_attrib_4d(GLuint a, GLdouble b, GLdouble c, GLdouble d, GLdouble e);
void vertex_attrib_4dv(GLuint a, const GLdouble *b);
void vertex_attrib_4iv(GLuint a, const GLint *b);
void vertex_attrib_4s(GLuint a, GLshort b, GLshort c, GLshort d, GLshort e);
void vertex_attrib_4sv(GLuint a, const GLshort *b);
void vertex_attrib_4ubv(GLuint a, const GLubyte *b);
void vertex_attrib_4uiv(GLuint a, const GLuint *b);
void vertex_attrib_4usv(GLuint a, const GLushort *b);
#endif

/*  GL_VERSION_2_1  */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void uniform_matrix_2x3fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d);
void uniform_matrix_3x2fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d);
void niform_matrix_2x4fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d);
void uniform_matrix_4x2fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d);
void uniform_matrix_3x4fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d);
void uniform_matrix_4x3fv(GLint a, GLsizei b, GLboolean c, const GLfloat *d);
#endif

/*  GL_VERSION_3_0  */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void get_integer_i_v(GLenum a, GLuint b, GLint *c);
void begin_transform_feedback(GLenum a);
void end_transform_feedback(void);
void bind_buffer_range(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void bind_buffer_base(GLenum a, GLuint b, GLuint c);
void transform_feedback_varyings(GLuint a, GLsizei b, const GLchar* *c, GLenum d);
void get_transform_feedback_varying(GLuint a, GLuint b, GLsizei c, GLsizei *d, GLsizei *e, GLenum *f, GLchar *g);
void vertex_attrib_i_pointer(GLuint a, GLint b, GLenum c, GLsizei d, const GLvoid *e);
void get_vertex_attrib_iiv(GLuint a, GLenum b, GLint *c);
void get_vertex_attrib_iuiv(GLuint a, GLenum b, GLuint *c);
void vertex_attrib_i4i(GLuint a, GLint b, GLint c, GLint d, GLint e); 
void vertex_attrib_i4iv(GLuint a, const GLint *b);
void vertex_attrib_i4ui(GLuint a, GLuint b, GLuint c, GLuint d, GLuint e);
void vertex_attrib_i4uiv(GLuint a, const GLuint *b);
void get_uniform_uiv(GLuint a, GLint b, GLuint *c);
GLint get_frag_data_location(GLuint a, const GLchar *b);
void uniform_1ui(GLint a, GLuint b);
void uniform_2ui(GLint a, GLuint b, GLuint c);
void uniform_3ui(GLint a, GLuint b, GLuint c, GLuint d);
void uniform_4ui(GLint a, GLuint b, GLuint c, GLuint d, GLuint e);
void uniform_1uiv(GLint a, GLsizei b, const GLuint *c);
void uniform_2uiv(GLint a, GLsizei b, const GLuint *c);
void uniform_3uiv(GLint a, GLsizei b, const GLuint *c);
void uniform_4uiv(GLint a, GLsizei b, const GLuint *c);
void clear_buffer_iv(GLenum a, GLint b, const GLint *c);
void clear_buffer_uiv(GLenum a, GLint b, const GLuint *c);
void clear_buffer_fv(GLenum a, GLint b, const GLfloat *c);
void clear_buffer_fi(GLenum a, GLint b, GLfloat c, GLint d);
const GLubyte * get_string_i(GLenum a, GLuint b);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void color_mask_i(GLuint a, GLboolean b, GLboolean c, GLboolean d, GLboolean e);
void get_boolean_i_v(GLenum a, GLuint b, GLboolean *c);
void enable_i(GLenum a, GLuint b);
void disable_i(GLenum a, GLuint b);
GLboolean is_enabled_i(GLenum a, GLuint b);
void clamp_color(GLenum a, GLenum b);
void begin_conditional_render(GLuint a, GLenum b);
void end_conditional_render(void);
void vertex_attrib_i1i(GLuint a, GLint b);
void vertex_attrib_i2i(GLuint a, GLint b, GLint c);
void vertex_attrib_i3i(GLuint a, GLint b, GLint c, GLint d);
void vertex_attrib_i1ui(GLuint a, GLuint b);
void vertex_attrib_i2ui(GLuint a, GLuint b, GLuint c);
void vertex_attrib_i3ui(GLuint a, GLuint b, GLuint c, GLuint d);
void vertex_attrib_i1iv(GLuint a, const GLint *b);
void vertex_attrib_i2iv(GLuint a, const GLint *b);
void vertex_attrib_i3iv(GLuint a, const GLint *b);
void vertex_attrib_i1uiv(GLuint a, const GLuint *b);
void vertex_attrib_i2uiv(GLuint a, const GLuint *b);
void vertex_attrib_i3uiv(GLuint a, const GLuint *b);
void vertex_attrib_i4bv(GLuint a, const GLbyte *b);
void vertex_attrib_i4sv(GLuint a, const GLshort *b);
void vertex_attrib_i4ubv(GLuint a, const GLubyte *b);
void vertex_attrib_i4usv(GLuint a, const GLushort *b);
void bind_frag_data_location(GLuint a, GLuint b, const GLchar *c);
void tex_parameter_iiv(GLenum a, GLenum b, const GLint *c);
void tex_parameter_iuiv(GLenum a, GLenum b, const GLuint *c);
void get_tex_parameter_iiv(GLenum a, GLenum b, GLint *c);
void get_tex_parameter_iuiv(GLenum a, GLenum b, GLuint *c);
#endif

/*  GL_VERSION_3_1  */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void draw_arrays_instanced(GLenum a, GLint b, GLsizei c, GLsizei d);
void draw_elements_instanced(GLenum a, GLsizei b, GLenum c, const GLvoid *d, GLsizei e);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void tex_buffer(GLenum target, GLenum internalformat, GLuint buffer);
void primitive_restart_index(GLuint a);
#endif

/*  GL_VERSION_3_2  */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void get_integer_64i_v(GLenum a, GLuint b, GLint64 *c);
void get_buffer_parameter_i64v(GLenum a, GLenum b, GLint64 *c);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void program_parameter_i(GLuint a, GLenum b, GLint c);
void framebuffer_texture(GLenum a, GLenum b, GLuint c, GLint d);
void framebuffer_texture_face(GLenum a, GLenum b, GLuint c, GLint d, GLenum e);
#endif

/*  3.0  */ 
/*  ARB_framebuffer_object  */ 
GLboolean is_renderbuffer(GLuint a);
void bind_renderbuffer(GLenum a, GLuint b);
void delete_renderbuffers(GLsizei a, const GLuint *b);
void gen_renderbuffers(GLsizei a, GLuint *b);
void renderbuffer_storage(GLenum a, GLenum b, GLsizei c, GLsizei d);
void get_renderbuffer_parameter_iv(GLenum a, GLenum b, GLint *c);
GLboolean is_framebuffer(GLuint a);
void bind_framebuffer(GLenum a, GLuint b);
void delete_framebuffers(GLsizei a, const GLuint *b);
void gen_framebuffers(GLsizei, GLuint *);
GLenum check_framebuffer_status(GLenum a);
void framebuffer_texture_2d(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void framebuffer_texture_3d(GLenum a, GLenum b, GLenum c, GLuint d, GLint e, GLint f);
void framebuffer_renderbuffer(GLenum a, GLenum b, GLenum c, GLuint d);
void get_framebuffer_attachment_parameter_iv(GLenum a, GLenum b, GLenum c, GLint *d);
void generate_mipmap(GLenum a);

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void blit_framebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
void renderbuffer_storage_multisample(GLenum target, GLsizei samples, GLenum internal_format, GLsizei width, GLsizei height);
void framebuffer_texture_layer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void framebuffer_texture_1d(GLenum a, GLenum b, GLenum c, GLuint d, GLint e);
#endif

/*  ARB_map_buffer_range  */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
GLvoid* map_buffer_range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void flush_mapped_buffer_range(GLenum target, GLintptr offset, GLsizeiptr length);
#endif
/*  ARB_vertex_array_object  */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void bind_vertex_array(GLuint array_);
void delete_vertex_arrays(GLsizei n, const GLuint *arrays);
void gen_vertex_arrays(GLsizei a, GLuint *b);
GLboolean is_vertex_array(GLuint a);
#endif

/*  3.1  */ 
/*  ARB_copy_buffer  */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void copy_buffer_sub_data(GLenum a, GLenum b, GLintptr c, GLintptr d, GLsizeiptr e);
#endif

/*  ARB_uniform_buffer_object  */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void get_uniform_indices(GLuint program, GLsizei uniformCount, const GLchar* *uniformNames, GLuint *uniformIndices);
void get_active_uniforms_iv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
void get_active_uniform_name(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
GLuint get_uniform_block_index(GLuint program, const GLchar *uniformBlockName);
void get_active_uniform_block_iv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
void get_active_uniform_block_name(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
void uniform_block_binding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
#endif
/*  3.2  */ 
/*  ARB_draw_elements_base_vertex  */ 
#if defined(RENDERSTACK_GL_API_OPENGL)
void draw_elements_base_vertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
void draw_range_elements_base_vertex(GLenum a, GLuint b, GLuint c, GLsizei d, GLenum e, const GLvoid *f, GLint g);
void draw_elements_instanced_base_vertex(GLenum a, GLsizei b, GLenum c, const GLvoid *d, GLsizei e, GLint f);
void multi_draw_elements_base_vertex(GLenum a, const GLsizei *b, GLenum c, const GLvoid* *d, GLsizei e, const GLint *f);
#endif


/*  ARB_provoking_vertex  */ 
#if defined(RENDERSTACK_GL_API_OPENGL)
void provoking_vertex(GLenum a);
#endif

/*  ARB_sync  */ 

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
GLsync fence_sync(GLenum condition, GLbitfield flags);
GLboolean is_sync(GLsync a);
void delete_sync(GLsync a);
GLenum client_wait_sync(GLsync a, GLbitfield b, GLuint64 c);
void wait_sync(GLsync a, GLbitfield b, GLuint64 c);
void get_integer_64v(GLenum a, GLint64 *b);
void get_sync_iv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
#endif
/*  ARB_texture_multisample  */ 
#if defined(RENDERSTACK_GL_API_OPENGL)
void tex_image_2d_multisample(GLenum a, GLsizei b, GLint c, GLsizei d, GLsizei e, GLboolean f);
void tex_image_3d_multisample(GLenum a, GLsizei b, GLint c, GLsizei d, GLsizei e, GLsizei f, GLboolean g);
void get_multisample_fv(GLenum a, GLuint b, GLfloat *c);
void sample_mask_i(GLuint a, GLbitfield b);
#endif

/*  GL_ARB_sample_shading  */ 
#if defined(RENDERSTACK_GL_API_OPENGL)
void min_sample_shading(GLclampf value);
#endif

/*  GL_ARB_sampler_objects  */ 
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void gen_samplers(GLsizei count, GLuint *samplers);
void delete_samplers(GLsizei count, const GLuint *samplers);
GLboolean is_sampler(GLuint sampler);
void bind_sampler(GLuint unit, GLuint sampler);
void sampler_parameter_i(GLuint sampler, GLenum pname, GLint param);
void sampler_parameter_iv(GLuint sampler, GLenum pname, const GLint *param);
void sampler_parameter_f(GLuint sampler, GLenum pname, GLfloat param);
void sampler_parameter_fv(GLuint sampler, GLenum pname, const GLfloat *param);
void get_sampler_parameter_iv(GLuint sampler, GLenum pname, GLint *params);
void get_sampler_parameter_fv(GLuint sampler, GLenum pname, GLfloat *params);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void sampler_parameter_i_iv(GLuint sampler, GLenum pname, const GLint *param);
void sampler_parameter_i_uiv(GLuint sampler, GLenum pname, const GLuint *param);
void get_sampler_parameter_i_iv(GLuint sampler, GLenum pname, GLint *params);
void get_sampler_parameter_i_uiv(GLuint sampler, GLenum pname, GLuint *params);
#endif

/*  GL_ARB_timer_query  */ 
#if defined(RENDERSTACK_GL_API_OPENGL)
void query_counter(GLuint id, GLenum target);
void get_query_object_i64v(GLuint id, GLenum pname, GLint64 *params);
void get_query_object_ui64v(GLuint id, GLenum pname, GLuint64 *params);
#endif

/*  GL_ARB_tessellation_shader  */
#if defined(RENDERSTACK_GL_API_OPENGL)
void patch_parameter_i(GLenum pname, GLint value);
void patch_parameter_fv(GLenum pname, const GLfloat *values);
#endif

/*  GL_ARB_get_program_binary  */

// OpenGL ES 3.0
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void get_program_binary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
void program_binary(GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length);
void program_parameter_i(GLuint program, GLenum pname, GLint value);
#endif

/*  GL_ARB_base_instance  */
#if defined(RENDERSTACK_GL_API_OPENGL)
void draw_arrays_instanced_base_instance(GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
void draw_elements_instanced_base_instance(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLuint baseinstance);
void draw_elements_instanced_base_vertex_base_instance(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);
#endif

/*  GL_ARB_transform_feedback2  */
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void bind_transform_feedback(GLenum target, GLuint id);
void delete_transform_feedbacks(GLsizei n, const GLuint *ids);
void gen_transform_feedbacks(GLsizei n, GLuint *ids);
GLboolean is_transform_feedback(GLuint id);
void pause_transform_feedback(void);
void resume_transform_feedback(void);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL)
void draw_transform_feedback(GLenum mode, GLuint id);
#endif

/*  GL_ARB_transform_feedback3  */
#if defined(RENDERSTACK_GL_API_OPENGL)
void draw_transform_feedback_stream(GLenum mode, GLuint id, GLuint stream);
void begin_query_indexed(GLenum target, GLuint index, GLuint id);
void end_query_indexed(GLenum target, GLuint index);
void get_query_indexed_iv(GLenum target, GLuint index, GLenum pname, GLint *params);
#endif

/*  GL_ARB_transform_feedback_instanced  */
#if defined(RENDERSTACK_GL_API_OPENGL)
void draw_transform_feedback_instanced(GLenum mode, GLuint id, GLsizei primcount);
void draw_transform_feedback_stream_instanced(GLenum mode, GLuint id, GLuint stream, GLsizei primcount);
#endif

/*  GL_VERSION_3_3 */
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void vertex_attrib_divisor(GLuint index, GLuint divisor);
#endif

/*  GL_ARB_texture_storage  */
#if defined(RENDERSTACK_GL_API_OPENGL)
void tex_storage_1d(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
void tex_storage_2d(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
void tex_storage_3d(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
#endif

} /* namespace gl */

#endif
