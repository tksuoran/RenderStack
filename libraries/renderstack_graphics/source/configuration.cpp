#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/log.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <list>
#include <stdexcept>

#define LOG_CATEGORY &log_configuration

namespace renderstack
{
namespace graphics
{

using namespace renderstack::toolkit;
using namespace std;

bool configuration::use_gl1 = false;
bool configuration::intel   = false;

int configuration::gl_version;
int configuration::glsl_version;
int configuration::shader_model_version;

bool configuration::core_profile                 = false;
bool configuration::compatibility_profile        = false;
bool configuration::forward_compatible           = false;
bool configuration::use_binary_shaders           = false;
bool configuration::use_integer_polygon_ids      = false;
bool configuration::use_vertex_array_object      = true;
bool configuration::must_use_vertex_array_object = false;

configuration::can_use_t configuration::can_use;

static const char *desc_debug_source(GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API: return "api";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "window system";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "shader compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "third party";
        case GL_DEBUG_SOURCE_APPLICATION: return "application";
        case GL_DEBUG_SOURCE_OTHER: return "other";
        default: return "?";
    }
}

static const char *desc_debug_type(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR: return "error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "deprecated behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "undefined behavior";
        case GL_DEBUG_TYPE_PORTABILITY: return "portability";
        case GL_DEBUG_TYPE_PERFORMANCE: return "performance";
        case GL_DEBUG_TYPE_OTHER: return "other";
        case GL_DEBUG_TYPE_MARKER: return "marker";
        case GL_DEBUG_TYPE_PUSH_GROUP: return "push group";
        case GL_DEBUG_TYPE_POP_GROUP: return "pop group";
        default: return "?";
    }
}

static const char *desc_debug_severity(GLenum severity)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: return "high";
        case GL_DEBUG_SEVERITY_MEDIUM: return "medium";
        case GL_DEBUG_SEVERITY_LOW: return "low";
        default: return "?";
    }
}

static void APIENTRY opengl_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei /*length*/,
    const GLchar *message,
    const void * /*userParam*/
)
{
    // Ignores
#if 1 // nvidia?
    if (id == 0x00020071)
        return; // memory usage
    if (id == 0x00020084)
        return; // Texture state usage warning: Texture 0 is base level inconsistent. Check texture size.
    if (id == 0x00020061)
        return; // Framebuffer detailed info: The driver allocated storage for renderbuffer 1.
    if (id == 0x00020004)
        return; // Usage warning: Generic vertex attribute array ... uses a pointer with a small value (...). Is this intended to be used as an offset into a buffer object?
    if (id == 0x00020072)
        return; // Buffer performance warning: Buffer object ... (bound to ..., usage hint is GL_STATIC_DRAW) is being copied/moved from VIDEO memory to HOST memory.
    if (id == 0x00020074)
        return; // Buffer usage warning: Analysis of buffer object ... (bound to ...) usage indicates that the GPU is the primary producer and consumer of data for this buffer object.  The usage hint s upplied with this buffer object, GL_STATIC_DRAW, is inconsistent with this usage pattern.  Try using GL_STREAM_COPY_ARB, GL_STATIC_COPY_ARB, or GL_DYNAMIC_COPY_ARB instead.
    if (id == 0x00020071)
        return; // Buffer detailed info: Buffer object 3 (bound to GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB (0), and GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
#endif
#if 0 // intel
   if (id == 0x00000008) return; // API_ID_REDUNDANT_FBO performance warning has been generated. Redundant state change in glBindFramebuffer API call, FBO 0, "", already bound.
#endif
    if (id == 1)
        return;

    log_info(
        "GL: source: %s type: %s id: 0x%08x severity: %s : %s",
        desc_debug_source(source),
        desc_debug_type(type),
        id,
        desc_debug_severity(severity),
        (message != nullptr ? message : ""));
#if defined(WIN32)
    DebugBreak();
#endif
}

configuration::can_use_t::can_use_t()
{
    vertex_buffer              = false; // 150 map buffer etc.
    gl_version_300             = false; // 300 -----------------------------------
    gpu_shader4                = false; // 300 integer attributes
    conditional_render         = false; // 300
    map_buffer_range           = false; // 300
    bind_buffer_range          = false; // 300
    texture_float              = false; // 300
    color_buffer_float         = false; // 300
    depth_buffer_float         = false; // 300
    packed_float               = false; // 300
    texture_shared_exponent    = false; // 300
    framebuffer_object         = false; // 300
    half_float                 = false; // 300
    half_float_pixel           = false; // 300
    framebuffer_multisample    = false; // 300
    framebuffer_blit           = false; // 300
    texture_integer            = false; // 300
    texture_array              = false; // 300
    texture_r                  = false; // 300
    texture_rg                 = false; // 300
    packed_depth_stencil       = false; // 300
    draw_buffers2              = false; // 300 Per-color-attachment blend enables and color writemasks
    texture_compression_rgtc   = false; // 300
    vertex_array_object        = false; // 300
    integer_framebuffer_format = false; // 300
    bind_frag_data_location    = false; // 300 - alias for shader_model_version >= 4
    integer_attributes         = false; // 300
    instanced_arrays           = false; // 300
    gl_version_310             = false; // 310 -----------------------------------
    draw_instanced             = false; // 310
    texture_buffer_object      = false; // 310
    uniform_buffer_object      = false; // 310
    gl_version_320             = false; // 320 -----------------------------------
    seamless_cube_map          = false; // 320
    draw_elements_base_vertex  = false; // 320
    geometry_shaders           = false; // 320
    gl_version_330             = false; // 330 -----------------------------------
    sampler_object             = false; // 330
    timer_query                = false; // 330
    gl_version_400             = false; // 400 -----------------------------------
    gpu_shader5                = false; // 400
    transform_feedback         = false; // 400
    tesselation_shaders        = false; // 400
    gl_version_410             = false; // 410 -----------------------------------
    binary_shaders             = false; // 410

    frame_terminator = false; //
    string_marker    = false;

    pixel_buffer_object = false; //  \todo

    map_buffer_oes          = false;
    discard_framebuffer_oes = false;
    invalidate_framebuffer  = false;
    tex_storage             = false;

    debug_output            = false;
    amd_performance_monitor = false;
}
bool         configuration::throw_program_exceptions           = true;
unsigned int configuration::default_vao                        = 0;
int          configuration::max_vertex_attribs                 = 0;
int          configuration::max_texture_size                   = 64;
int          configuration::max_3d_texture_size                = 0;
int          configuration::max_cube_map_texture_size          = 0;
int          configuration::max_texture_buffer_size            = 0;
int          configuration::max_texture_units                  = 0; /* fixed function */
int          configuration::max_texture_image_units            = 0; /* shaders */
int          configuration::max_combined_texture_image_units   = 0;
int          configuration::max_uniform_block_size             = 0;
int          configuration::max_uniform_buffer_bindings        = 8;
int          configuration::max_vertex_uniform_blocks          = 0;
int          configuration::max_fragment_uniform_blocks        = 0;
int          configuration::max_geometry_uniform_blocks        = 0;
int          configuration::max_tess_control_uniform_blocks    = 0;
int          configuration::max_tess_evaluation_uniform_blocks = 0;
unsigned int configuration::uniform_buffer_offset_alignment    = 1024;

static vector<string> split(string text, char separator)
{
    vector<string> result;
    size_t         length     = text.size();
    size_t         span_start = numeric_limits<size_t>::max();
    for (size_t i = 0; i < length; ++i)
    {
        char c = text[i];
        if (c == separator)
        {
            if (span_start != numeric_limits<size_t>::max())
            {
                size_t span_length = i - span_start;
                if (span_length > 0)
                {
                    string span = text.substr(span_start, span_length);
                    result.push_back(span);
                }
                span_start = numeric_limits<size_t>::max();
            }
        }
        else
        {
            if (span_start == numeric_limits<size_t>::max())
            {
                span_start = i;
            }
        }
    }
    if (span_start != numeric_limits<size_t>::max())
    {
        if (length > span_start)
        {
            size_t span_length = length - span_start;
            string span        = text.substr(span_start, span_length);
            result.push_back(span);
        }
    }
    return result;
}

static string digits_only(string s)
{
    size_t size = s.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (::isdigit(s[i]) == 0)
        {
            if (i == 0)
                return "";
            else
                return s.substr(0, i);
        }
    }
    return s;
}

static bool contains(vector<string> const &collection, const std::string &key)
{
    auto i = find(collection.begin(), collection.end(), key);
    return i != collection.end();
}

static string get_string(int e)
{
    const GLubyte *gl_str = gl::get_string(static_cast<GLenum>(e));
    const char *   c_str  = reinterpret_cast<const char *>(gl_str);
    return (c_str != nullptr) ? string(c_str) : string();
}

void configuration::check(vector<string> const &extensions, bool &var, const std::string &name,
                          int gl_min_ver, int gles_min_ver, const std::string &gl_ext)
{
#if defined(RENDERSTACK_GL_API_OPENGL)
    (void)gles_min_ver;
    if (gl_version >= gl_min_ver || (gl_ext.length() > 1 && contains(extensions, gl_ext)))
    {
        var = true;
        log_info(name.c_str());
    }
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    (void)gl_min_ver;
    if (gl_version >= gles_min_ver || (gl_ext.length() > 1 && contains(extensions, gl_ext)))
    {
        var = true;
        log_info(name.c_str());
    }
#endif
}

void configuration::check(
    vector<string> const &extensions, bool &var, const std::string &name,
    int gl_min_ver, int gles_min_ver, const std::string &gl_ext, const std::string &gl_ext2)
{
#if defined(RENDERSTACK_GL_API_OPENGL)
    (void)gles_min_ver;
    if (gl_version >= gl_min_ver ||
        (gl_ext.length() > 1 && contains(extensions, gl_ext)) ||
        (gl_ext2.length() > 1 && contains(extensions, gl_ext2)))
    {
        var = true;
        log_info(name.c_str());
    }
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    (void)gl_min_ver;
    if (gl_version >= gles_min_ver ||
        (gl_ext.length() > 1 && contains(extensions, gl_ext)) ||
        (gl_ext2.length() > 1 && contains(extensions, gl_ext2)))
    {
        var = true;
        log_info(name.c_str());
    }
#endif
}

void configuration::check(
    vector<string> const &extensions, bool &var, const std::string &name,
    int gl_min_ver, int gles_min_ver, const std::string &gl_ext, const std::string &gl_ext2, const std::string &gl_ext3)
{
#if defined(RENDERSTACK_GL_API_OPENGL)
    (void)gles_min_ver;
    if (gl_version >= gl_min_ver ||
        (gl_ext.length() > 1 && contains(extensions, gl_ext)) ||
        (gl_ext2.length() > 1 && contains(extensions, gl_ext2)) ||
        (gl_ext3.length() > 1 && contains(extensions, gl_ext3)))
    {
        var = true;
        log_info(name.c_str());
    }
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    (void)gl_min_ver;
    if (gl_version >= gles_min_ver ||
        (gl_ext.length() > 1 && contains(extensions, gl_ext)) ||
        (gl_ext2.length() > 1 && contains(extensions, gl_ext2)) ||
        (gl_ext3.length() > 1 && contains(extensions, gl_ext3)))
    {
        var = true;
        log_info(name.c_str());
    }
#endif
}

void configuration::initialize()
{
    vector<string> extensions;

    string gl_vendor      = get_string(GL_VENDOR);
    string gl_renderer    = get_string(GL_RENDERER);
    string gl_version_str = get_string(GL_VERSION);
    //bool gles20 = false; // TODO

    log_info("GL Vendor:     %s\n", gl_vendor.c_str());
    log_info("GL Renderer:   %s\n", gl_renderer.c_str());
    log_info("GL Version:    %s\n", gl_version_str.c_str());

    {
        int c = gl_vendor.compare("Intel");
        if (c == 0)
            configuration::intel = true;
    }

    // TODO: "OpenGL ES "
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    string opengl_es = gl_version_str.substr(0, 10);
    if (opengl_es != "OpenGL ES ")
        throw runtime_error("Unexpected GL_VERSION (should have started with OpenGL ES ");
    gl_version_str = gl_version_str.substr(10);
#endif
    vector<string> versions = split(gl_version_str, '.');

    int major = (versions.size() > 0) ? ::atoi(digits_only(versions[0]).c_str()) : 0;
    int minor = (versions.size() > 1) ? ::atoi(digits_only(versions[1]).c_str()) : 0;

    gl_version = (major * 100) + (minor * 10);

    gl::get_integer_v(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    log_trace("max texture size: %d\n", max_texture_size);

    if (gl_version >= 200) // TODO extension?
    {
        gl::get_integer_v(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);
        log_trace("max vertex attribs: %d\n", max_vertex_attribs);
    }
    else
        max_vertex_attribs = 0;

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (gl_version >= 300)
    {
        //  GL 3 introduced a new way to access extension strings
        GLint num_extensions;

        gl::get_integer_v(GL_NUM_EXTENSIONS, &num_extensions);
        if (num_extensions > 0)
        {
            extensions.reserve(num_extensions);
            for (unsigned int i = 0; i < static_cast<unsigned int>(num_extensions); ++i)
            {
                const GLubyte *extension = gl::get_string_i(GL_EXTENSIONS, i);
                string         e         = string(reinterpret_cast<const char *>(extension));
                extensions.push_back(e);
            }
        }
    }
    else
#endif
    {
        string e   = get_string(GL_EXTENSIONS);
        extensions = split(e, ' ');
    }

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (gl_version >= 200 || contains(extensions, "GL_ARB_shading_language_100"))
    {
        try
        {
            string shading_language_version = get_string(GL_SHADING_LANGUAGE_VERSION);
            log_info("GLSL Version:  %s\n", shading_language_version.c_str());
#    if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
            string fallback_prefix = "OpenGL ES GLSL ";
            string expected_prefix = "OpenGL ES GLSL ES ";
            size_t prefix_len      = expected_prefix.size();
            string prefix          = shading_language_version.substr(0, prefix_len);
            if (prefix != expected_prefix)
            {
                expected_prefix = fallback_prefix;
                prefix_len      = expected_prefix.size();
                prefix          = shading_language_version.substr(0, prefix_len);
                log_info("Warning: Invalid OpenGL ES GLSL ES version prefix\n");
            }
            shading_language_version = shading_language_version.substr(prefix_len);
#    endif
            versions = split(shading_language_version, '.');

            major        = (versions.size() > 0) ? ::atoi(digits_only(versions[0]).c_str()) : 0;
            minor        = (versions.size() > 1) ? ::atoi(digits_only(versions[1]).c_str()) : 0;
            glsl_version = (major * 100) + minor;
#    if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
            if (glsl_version < 300)
            {
                log_info("Warning: Invalid OpenGL ES GLSL ES version - forcing 300\n");
                glsl_version = 300;
            }
#    endif
        }
        catch (...)
        {
            //  This should not happen; glsl_version is left to 0.
            //  Note: implicit value for glsl shader source version is 110 if not explicitly set
        }
    }
#endif

    log_trace("glVersion:   %d\n", gl_version);
    log_trace("glslVersion: %d\n", glsl_version);

#if 0
   if (gl_version < 300)
   {
      max_texture_units = 1;
      if (gl_version >= 121 || contains(extensions, "GL_ARB_multitexture"))
      {
         gl::get_integer_v(GL_MAX_TEXTURE_UNITS, &max_texture_units);
      }
   }
#endif

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
#    if !defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (gl_version >= 120 || contains(extensions, "GL_EXT_texture3D"))
#    endif
    {
        gl::get_integer_v(GL_MAX_3D_TEXTURE_SIZE, &max_3d_texture_size);
    }
#endif

#if defined(RENDERSTACK_GL_API_OPENGL)
    if (gl_version >= 130 || contains(extensions, "GL_ARB_texture_cube_map"))
        gl::get_integer_v(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &max_cube_map_texture_size);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    gl::get_integer_v(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &max_cube_map_texture_size);
#endif

#if defined(RENDERSTACK_GL_API_OPENGL)
    if (gl_version >= 200)
    {
        gl::get_integer_v(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units);
        gl::get_integer_v(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_combined_texture_image_units);
    }
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    gl::get_integer_v(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units);
    gl::get_integer_v(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_combined_texture_image_units);
#endif

    //  GL 3.0 introduced context flags
#if defined(RENDERSTACK_GL_API_OPENGL)
    {
        if (gl_version >= 300)
        {
            int context_flags;
            gl::get_integer_v(GL_CONTEXT_FLAGS, &context_flags);
            if ((context_flags & (int)(GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)) != 0)
            {
                forward_compatible = true;
                log_info("forwardCompatible\n");
            }
        }
    }
#endif

    //  GL 3.3 introduced context profile mask
#if defined(RENDERSTACK_GL_API_OPENGL)
    {
        if (gl_version >= 330)
        {
            int context_profile_mask;
            gl::get_integer_v(GL_CONTEXT_PROFILE_MASK, &context_profile_mask);
            if ((context_profile_mask & (int)(GL_CONTEXT_CORE_PROFILE_BIT)) != 0)
            {
                core_profile = true;
                log_info("core_profile\n");
            }
            if ((context_profile_mask & (int)(GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)) != 0)
            {
                compatibility_profile = true;
                log_info("compatibility_profile\n");
                //  This is for testing
                //  glslVersion = 120;
            }
        }
    }
#endif

    // Extension versions have not been tested

    check(extensions, can_use.map_buffer_oes, "map_buffer_oes", 999, 999, "GL_OES_mapbuffer");
    check(extensions, can_use.discard_framebuffer_oes, "discard_framebuffer_oes", 999, 999, "GL_EXT_discard_framebuffer");

    check(extensions, can_use.texture_3d, "texture_3d", 120, 300, "GL_EXT_texture3D",
          "GL_OES_texture_3D");
    check(extensions, can_use.vertex_buffer, "vertex_buffer", 150, 100, "GL_ARB_vertex_buffer_object");
    check(extensions, can_use.pixel_buffer_object, "pixel_buffer_object", 210, 300, "GL_ARB_pixel_buffer_object",
          "GL_NV_pixel_buffer_object");
    check(extensions, can_use.gpu_shader4, "gpu_shader4", 300, 300, "GL_EXT_gpu_shader4");
    check(extensions, can_use.map_buffer_range, "map_buffer_range", 300, 300, "GL_ARB_map_buffer_range",
          "GL_EXT_map_buffer_range");
    check(extensions, can_use.framebuffer_object, "framebuffer_object", 300, 200, "GL_ARB_framebuffer_object",
          "GL_OES_framebuffer_object");
    check(extensions, can_use.depth_buffer_float, "depth_buffer_float", 300, 300, "GL_NV_depth_buffer_float");
    check(extensions, can_use.texture_float, "texture_float", 300, 300, "GL_ARB_texture_float");
    check(extensions, can_use.packed_float, "packed_float", 300, 300, "GL_EXT_packed_float",
          "GL_NV_packed_float");
    check(extensions, can_use.texture_shared_exponent, "texture_shared_exponent", 300, 300, "GL_EXT_texture_shared_exponent");
    check(extensions, can_use.half_float, "half_float", 300, 300, "GL_NV_half_float");
    check(extensions, can_use.framebuffer_multisample, "framebuffer_multisample", 300, 300, "GL_EXT_framebuffer_multisample",
          "GL_NV_framebuffer_multisample",
          "GL_ANGLE_framebuffer_multisample");
    check(extensions, can_use.framebuffer_blit, "framebuffer_blit", 300, 300, "GL_EXT_framebuffer_blit",
          "GL_NV_framebuffer_blit",
          "GL_ANGLE_framebuffer_blit");
    check(extensions, can_use.texture_integer, "texture_integer", 300, 300, "GL_EXT_texture_integer");
    check(extensions, can_use.texture_array, "texture_array", 300, 300, "GL_EXT_texture_array",
          "GL_NV_texture_array");
    check(extensions, can_use.texture_r, "texture_r", 300, 300, "");
    check(extensions, can_use.texture_rg, "texture_rg", 300, 300, "");
    check(extensions, can_use.packed_depth_stencil, "packed_depth_stencil", 300, 300, "GL_EXT_packed_depth_stencil");
    check(extensions, can_use.vertex_array_object, "vertex_array_object", 300, 300, "GL_ARB_vertex_array_object");
    check(extensions, can_use.integer_framebuffer_format, "integer_framebuffer_format", 300, 300, "GL_EXT_gpu_shader4"); // GL_EXT_texture_integer too?
    check(extensions, can_use.color_buffer_float, "color_buffer_float", 300, 999, "GL_ARB_color_buffer_float",
          "GL_EXT_color_buffer_float");
    check(extensions, can_use.draw_instanced, "draw_instanced", 310, 300, "GL_ARB_draw_instanced",
          "GL_NV_draw_instanced");
    check(extensions, can_use.uniform_buffer_object, "uniform_buffer_object", 310, 300, "GL_ARB_uniform_buffer_object");
    check(extensions, can_use.seamless_cube_map, "seamless_cube_map", 320, 300, "GL_ARB_seamless_cube_map");
    check(extensions, can_use.sampler_object, "sampler_object", 330, 300, "ARB_sampler_objects");
    check(extensions, can_use.instanced_arrays, "instanced_arrays", 330, 300, "GL_ARB_instanced_arrays",
          "GL_NV_instanced_arrays",
          "GL_ANGLE_instanced_arrays");
    check(extensions, can_use.transform_feedback, "transform_feedback", 400, 300, "GL_ARB_transform_feedback3");
    check(extensions, can_use.binary_shaders, "binary_shaders", 410, 300, "GL_ARB_get_program_binary");
    check(extensions, can_use.tex_storage, "tex_storage", 420, 300, "GL_ARB_texture_storage");
    check(extensions, can_use.invalidate_framebuffer, "invalidate_framebuffer", 430, 999, "GL_ARB_invalidate_subdata");

#if defined(RENDERSTACK_GL_API_OPENGL)
    check(extensions, can_use.bind_frag_data_location, "bind_frag_data_location", 300, 999, "");
    check(extensions, can_use.conditional_render, "conditional_render", 300, 999, "GL_NV_conditional_render");
    check(extensions, can_use.half_float_pixel, "half_float_pixel", 300, 999, "GL_ARB_half_float_pixel");
    check(extensions, can_use.draw_buffers2, "draw_buffers2", 300, 999, "GL_EXT_draw_buffers2");
    check(extensions, can_use.texture_compression_rgtc, "texture_compression_rgtc", 300, 999, "GL_EXT_texture_compression_rgtc");
    check(extensions, can_use.texture_buffer_object, "texture_buffer_object", 310, 999, "GL_ARB_texture_buffer_object");
    check(extensions, can_use.draw_elements_base_vertex, "draw_elements_base_vertex", 320, 999, "GL_ARB_draw_elements_base_vertex");
    check(extensions, can_use.geometry_shaders, "geometry_shaders", 320, 999, "GL_ARB_geometry_shader4");
    check(extensions, can_use.timer_query, "timer_query", 330, 999, "GL_ARB_timer_query");
    check(extensions, can_use.gpu_shader5, "gpu_shader5", 400, 999, "GL_EXT_gpu_shader5");
    check(extensions, can_use.tesselation_shaders, "tesselation_shaders", 400, 999, "GL_ARB_tesselation_shader");

    check(extensions, can_use.debug_output, "debug_output", 430, 999, "GL_KHR_debug" /*"GL_ARB_debug_output"*/);
    check(extensions, can_use.amd_performance_monitor, "amd_performance_monitor", 999, 999, "GL_AMD_performance_monitor");
#endif

    can_use.bind_buffer_range = can_use.map_buffer_range;

    if (gl_vendor == "ARM Ltd.")
    {
        if (gl_renderer == "OpenGL ES Emulator Revision r2p0-00rel0")
        {
            can_use.map_buffer_range = false;
            log_info("Disabling map buffer range support for ARM ES 3.0 Emulator\n");
        }
    }
    if (gl_renderer == "Qualcomm OpenGL ES 3.0 Emulator")
    {
        // TODO Have they fixed it? Which version was it?
        can_use.transform_feedback = false;
        log_info("Disabling transform feedback support for Qualcomm ES 3.0 Emulator\n");
    }

#if defined(RENDERSTACK_GL_API_OPENGL)
    //  GL versions 3.0, 3.1 and 3.2 use forwardCompatible
    //  GL versions 3.3 and later have coreProfile
    //  Default VAO no longer exists in GL version 3.1 forward compatible contexts
    if (gl_version >= 310 && (core_profile || forward_compatible))
    {
        must_use_vertex_array_object = true;
        log_info("must_use_vertex_array_object\n");
    }
#endif

    if (contains(extensions, "GL_GREMEDY_frame_terminator"))
    {
        can_use.frame_terminator = true;
        log_info("can_use_frame_terminator\n");
    }
    if (contains(extensions, "GL_GREMEDY_string_marker"))
    {
        can_use.string_marker = true;
        log_info("can_use_string_marker\n");
    }

#if defined(RENDERSTACK_GL_API_OPENGL)
    if (can_use.texture_buffer_object)
    {
        // \todo check gl core version requirement
        // \todo enable support for EXT version (different entry points)
        gl::get_integer_v(GL_MAX_TEXTURE_BUFFER_SIZE, &max_texture_buffer_size);
        log_info("can_use_texture_buffer_object (%d)\n", max_texture_buffer_size);
        //gl::get_integer_v(GetPName.MaxTextureBufferSize, out MaxTextureBufferSize);
    }
#endif

    if (can_use.debug_output)
    {
        gl::debug_message_callback(opengl_callback, nullptr);
        gl::debug_message_control(
            GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            nullptr,
            GL_TRUE);
        gl::enable(GL_DEBUG_OUTPUT);
        gl::enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (can_use.uniform_buffer_object)
    {
        GLint alignment;
        gl::get_integer_v(GL_MAX_UNIFORM_BLOCK_SIZE, &max_uniform_block_size);
        gl::get_integer_v(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
        gl::get_integer_v(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max_uniform_buffer_bindings);
        gl::get_integer_v(GL_MAX_VERTEX_UNIFORM_BLOCKS, &max_vertex_uniform_blocks);
        gl::get_integer_v(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &max_fragment_uniform_blocks);
        uniform_buffer_offset_alignment = static_cast<GLuint>(alignment);
#    if defined(RENDERSTACK_GL_API_OPENGL)
        if (can_use.geometry_shaders)
            gl::get_integer_v(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &max_geometry_uniform_blocks);

        if (can_use.tesselation_shaders)
        {
            gl::get_integer_v(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS, &max_tess_control_uniform_blocks);
            gl::get_integer_v(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS, &max_tess_evaluation_uniform_blocks);
        }
#    endif
        log_info(
            "can_use_uniform_buffer_object "
            "(max block size = %d"
            ", offset alignment = %d"
            ", max bindings = %d"
            ", max vertex blocks = %d"
            ", max fragment blocks = %d"
            ")\n",
            max_uniform_block_size,
            uniform_buffer_offset_alignment,
            max_uniform_buffer_bindings,
            max_vertex_uniform_blocks,
            max_fragment_uniform_blocks);
    }
#endif

    //  Shader model detection could do a lot better job that this...
    shader_model_version = 0;
    if (can_use.gpu_shader4)
        shader_model_version = 4;

    if (can_use.gpu_shader5)
        shader_model_version = 5;

        // can_use.map_buffer_range      = false;
        // can_use.uniform_buffer_object = false;

#if 0 // Disable vertex array object
   if (!configuration::must_use_vertex_array_object)
   {
      can_use.vertex_array_object = false;
      gl::detail::glGenVertexArrays = nullptr;
      gl::detail::glBindVertexArray = nullptr;
      use_vertex_array_object = false;
   }
   else
   {
      log_info("Warning: Cannot disable vertex array object in Core profile");
   }
#endif

#if 0 // Disable draw elements base vertex
   can_use.draw_elements_base_vertex = false;
   gl::detail::glDrawElementsInstancedBaseVertex = nullptr;
#endif

#if 0 // Disable Map Buffer Range
   can_use.map_buffer_range               = false;
   gl::detail::glMapBufferRange           = nullptr;
   gl::detail::glFlushMappedBufferRange   = nullptr;
   gl::detail::glUnmapBuffer              = nullptr;
#endif

#if 0 // Disable uniform buffer
   can_use.uniform_buffer_object       = false;
   gl::detail::glUniformBlockBinding   = nullptr;
   gl::detail::glBindBufferRange       = nullptr;
   gl::detail::glBindBufferBase        = nullptr;
#endif

#if 0 // Disable transform feedback
   can_use.transform_feedback                = false;
   gl::detail::glTransformFeedbackVaryings   = nullptr;
   gl::detail::glGenTransformFeedbacks       = nullptr;
   gl::detail::glIsTransformFeedback         = nullptr;
   gl::detail::glDeleteTransformFeedbacks    = nullptr;
   gl::detail::glBeginTransformFeedback      = nullptr;
   gl::detail::glPauseTransformFeedback      = nullptr;
   gl::detail::glResumeTransformFeedback     = nullptr;
   gl::detail::glEndTransformFeedback        = nullptr;
#endif

#if 0
   shader_model_version = 0;

   use_binary_shaders                   = false;
   use_integer_polygon_ids              = false;
   use_vertex_array_object              = false;

   can_use.binary_shaders               = false;
   can_use.draw_elements_base_vertex    = false;
   can_use.texture_float                = false;
   can_use.framebuffer_object           = false;
   can_use.frame_terminator             = false;
   can_use.geometry_shaders             = false;
   can_use.draw_instanced               = false;
   can_use.integer_framebuffer_format   = false;
   can_use.map_buffer_range             = false;
   can_use.pixel_buffer_object          = false;





   can_use.sampler_object               = false;
   can_use.seamless_cube_map            = false;
   can_use.string_marker                = false;
   can_use.tesselation_shaders          = false;
   can_use.texture_array                = false;
   can_use.texture_buffer_object        = false;
   can_use.timer_query                  = false;
   can_use.transform_feedback           = false;
   can_use.uniform_buffer_object        = false;
   can_use.vertex_array_object          = false;
#endif

    extensions.clear();
    set_text_color(C_GRAY);
}

} // namespace graphics
} // namespace renderstack
