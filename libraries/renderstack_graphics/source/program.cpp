#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/fragment_outputs.hpp"
#include "renderstack_graphics/log.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_toolkit/file.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <cstdarg>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <tuple>
#include <utility>

#define LOG_CATEGORY &log_program

// #define LOG_LINK 1

namespace renderstack
{
namespace graphics
{

using namespace std;
using namespace gl;
using namespace renderstack::toolkit;

string Program::format(const std::string &source)
{
    int         line = 1;
    const char *head = source.c_str();
    char        c;

    stringstream sb;
    sb << setw(3) << setfill(' ') << line << ": ";

    for (;;)
    {
        c = *head;
        ++head;
        if (c == '\r')
        {
            continue;
        }
        if (c == 0)
        {
            break;
        }

        if (c == '\n')
        {
            ++line;
            sb << '\n'
               << setw(3) << setfill(' ') << line << ": ";
            continue;
        }
        sb << c;
    }
    return sb.str();
}

static string string_replace_all(string s,
                                 string old,
                                 string new_)
{
    for (;;)
    {
        auto pos = s.find(old);
        if (pos == string::npos)
        {
            return s;
        }

        s.replace(pos, old.length(), new_);
    }
}

unsigned int Program::make_shader(shader_type::value type,
                                  std::string        source_str,
                                  std::string &      compiled_src)
{
    int delete_status;
    int compile_status;
    int shader_type;

    stringstream sb;
    if (m_glsl_version > 0)
    {
#if defined(RENDERSTACK_GL_API_OPENGL)
        sb << "#version " << m_glsl_version << "\n";
#endif
        // No GLSL versions in ES 2 .. ?
#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
        sb << "#version " << m_glsl_version << " es\n";
#endif
    }

    for (auto i = m_defines.cbegin(); i != m_defines.cend(); ++i)
    {
        sb << "#define " << i->first << " " << i->second << "\n";
    }

    // TODO Temporary
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    sb << "precision highp float;\n";
#endif

    if (type == shader_type::fragment_shader)
    {
        if (m_fragment_outputs)
        {
            sb << m_fragment_outputs->source(m_glsl_version);
        }
    }

    // TODO If we can not use uniform buffers, replace "block_name." with "block_name_"
    bool use_uniform_buffers =
        renderstack::graphics::configuration::can_use.uniform_buffer_object &&
        (m_glsl_version >= 140) &&
        (renderstack::graphics::configuration::shader_model_version >= 4);

    for (auto i = m_uniform_blocks.cbegin(); i != m_uniform_blocks.cend(); ++i)
    {
        auto block = i->second;
        sb << block->source(m_glsl_version);

        if (!use_uniform_buffers || block->default_block())
        {
            source_str = string_replace_all(source_str,
                                            block->name() + ".",
                                            block->name() + "_");
        }
    }

    if (m_samplers)
    {
        sb << m_samplers->str();
    }

    sb << source_str;

    unsigned int shader = gl::create_shader(type);
    if (shader == 0)
    {
        throw runtime_error("create_shader() failed\n");
    }

    {
        compiled_src            = sb.str();
        const char *   c_source = compiled_src.c_str();
        const GLchar **source   = &c_source;

        gl::shader_source(shader, 1, source, nullptr);
        gl::compile_shader(shader);
        gl::get_shader_iv(shader, gl::shader_parameter::compile_status, &compile_status);
        gl::get_shader_iv(shader, gl::shader_parameter::shader_type, &shader_type);
        gl::get_shader_iv(shader, gl::shader_parameter::delete_status, &delete_status);
        if (compile_status != GL_TRUE)
        {
            GLint length;
            gl::get_shader_iv(shader, gl::shader_parameter::info_log_length, &length);
            vector<char> log(length + 1, 0);
            gl::get_shader_info_log(shader, length, nullptr, &log[0]);
            string f_source = format(*source);
            log_error("Shader compilation failed: \n");
            log_error("%s", &log[0]);
            log_write(&log_glsl, false, LOG_ERROR, "\n%s\n", f_source.c_str());
            log_error("Shader compilation failed: \n");
            log_error("%s", &log[0]);
            cout << flush;
            cerr << flush;
            throw runtime_error("shader compilation failed");
        }
    }
    return shader;
}

Program::Program(const std::string &       name,
                 int                       glsl_version,
                 Samplers                  *samplers,
                 Vertex_attribute_mappings *vertex_attributes,
                 Fragment_outputs          *fragment_outputs)
    : m_name(name)
    , m_glsl_version(glsl_version)
    , m_samplers(samplers)
    , m_vertex_attribute_mappings(vertex_attributes)
    , m_fragment_outputs(fragment_outputs)
{
    m_gl_name = gl::create_program();

    assert(vertex_attributes);

#if 1 // TODO use layout qualifiers
    vertex_attributes->bind_attrib_locations(*this);
#endif

    assert(fragment_outputs);

    fragment_outputs->bind(*this);
}

bool Program::use_uniform_buffers() const
{
    bool u = renderstack::graphics::configuration::can_use.uniform_buffer_object &&
             (m_glsl_version >= 140) &&
             (renderstack::graphics::configuration::shader_model_version >= 4);
    return u;
}

void Program::bind_attrib_location(int location, const string &name)
{
    gl::bind_attrib_location(m_gl_name, location, name.c_str());
}

void Program::bind_frag_data_location(int location, const string &name)
{
    gl::bind_frag_data_location(m_gl_name, location, name.c_str());
}

void Program::define(const std::string &key, const std::string &value)
{
    m_defines.push_back(make_pair(key, value));
}
int Program::get_uniform_location(const char *name)
{
    return gl::get_uniform_location(m_gl_name, name);
}

void Program::dump_shaders() const
{
    log_trace("Shaders for %s:\n", m_name.c_str());
    for (auto resource : m_loaded_shaders)
    {
        string f_source = format(resource.compiled_src);

        //  detach old shader, load, compile and attach new
        log_trace("%s compiled from %s:\n", gl::enum_string(resource.type), resource.path.c_str());
        log_trace("%s\n", f_source.c_str());
    }
}

void Program::map_uniform(size_t key, const std::string &name)
{
    if (m_uniform_map.size() < (key + 1))
    {
        m_uniform_map.resize(key + 1);
    }

    int location       = get_uniform_location(name.c_str());
    m_uniform_map[key] = location;
}

int Program::uniform_at(size_t index) const
{
    if (index >= m_uniform_map.size())
    {
        return -1;
    }

    return m_uniform_map[index];
}

void Program::set_shader(shader_type::value type, const std::string &source)
{
    string compiled_src;
    GLuint shader = make_shader(type, source, compiled_src);

    gl::attach_shader(m_gl_name, shader);
}

void Program::load_shader(shader_type::value type, const std::string &path)
{
    string source = read(path);
    string compiled_src;
    GLuint shader = make_shader(type, "\n// Loaded from: " + path + "\n\n" + source, compiled_src);

    gl::attach_shader(m_gl_name, shader);

    loaded_shader resource;
    resource.type             = type;
    resource.path             = path;
    resource.shader           = shader;
    resource.last_good_shader = shader;
    resource.source           = source;
    resource.compiled_src     = compiled_src;

    m_loaded_shaders.push_back(resource);
}

void Program::reload(Shader_monitor &monitor)
{
    // TODO resource.source and resource.compiled_src get messedup if loading fails!

    try
    {
        //  Reload all shaders
        for (auto &resource : m_loaded_shaders)
        {
            //  detach old shader, load, compile and attach new
            cout << "reload detach old " << resource.shader << '\n';
            gl::detach_shader(m_gl_name, resource.shader);
            //  Mark current shader as 0 before compilation in order to
            //  know when to attach the shader back if compilation fails.
            resource.shader       = 0;
            std::string path      = resource.path;
            std::string load_path = monitor.most_recent_version(path);
            std::string source    = read(load_path);
            std::string compiled_src;
            resource.shader       = make_shader(resource.type, source, compiled_src);
            resource.compiled_src = compiled_src;
            cout << "reload attach new " << resource.shader << '\n';
            gl::attach_shader(m_gl_name, resource.shader);
        }

        link();

        // Since we are here, linking succeeded.
        // Update last known good shader
        for (auto &resource : m_loaded_shaders)
        {
            resource.last_good_shader = resource.shader;
            cout << "considering " << resource.shader << " last good\n";
        }
    }
    catch (...)
    {
        // New shaders did not work, revert to last known good
        try
        {
            for (auto &resource : m_loaded_shaders)
            {
                if (resource.shader != resource.last_good_shader)
                {
                    log_warn("fail, detaching and deleting %u and attaching %u\n",
                             resource.shader, resource.last_good_shader);

                    if (resource.shader != 0)
                    {
                        gl::detach_shader(m_gl_name, resource.shader);
                        gl::delete_shader(resource.shader);
                    }
                    gl::attach_shader(m_gl_name, resource.last_good_shader);

                    resource.shader = resource.last_good_shader;
                }
            }

            log_warn("fail, linking with last known good shaders\n");
            link();
            log_warn("restoring last known good shaders succeeded\n");
        }
        catch (...)
        {
            log_error("this should not happen\n");
        }
    }
}

Program &Program::load_vs(const std::string &path)
{
    load_shader(shader_type::vertex_shader, path);
    return *this;
}

Program &Program::load_tcs(const std::string &path)
{
#if defined(RENDERSTACK_GL_API_OPENGL)
    // It is important to use selected GLSL version, not configuration
    if (m_glsl_version >= 400)
    {
        load_shader(shader_type::tess_control_shader, path);
        return *this;
    }
    else
#else
    (void)path;
#endif
    {
        throw runtime_error("tesselation shaders are not supported / require GLSL version 4.00");
    }
}

Program &Program::load_tes(const std::string &path)
{
#if defined(RENDERSTACK_GL_API_OPENGL)
    // It is important to use selected GLSL version, not configuration
    if (m_glsl_version >= 400)
    {
        load_shader(shader_type::tess_evaluation_shader, path);
        return *this;
    }
    else
#else
    (void)path;
#endif
    {
        throw runtime_error("Tesselation shaders are not supported");
    }
}

Program &Program::load_gs(const std::string &path)
{
#if defined(RENDERSTACK_GL_API_OPENGL)
    // It is important to use selected GLSL version, not configuration
    if (m_glsl_version >= 150)
    {
        load_shader(shader_type::geometry_shader, path);
        return *this;
    }
    else
#else
    (void)path;
#endif
    {
        throw runtime_error("Geometry shaders are not supported");
    }
}

Program &Program::load_fs(const std::string &path)
{
    load_shader(shader_type::fragment_shader, path);
    return *this;
}

Program &Program::set_vs(const std::string &source)
{
    set_shader(shader_type::vertex_shader, source);
    return *this;
}

Program &Program::set_tcs(const std::string &source)
{
    set_shader(shader_type::tess_control_shader, source);
    return *this;
}

Program &Program::set_tes(const std::string &source)
{
    set_shader(shader_type::tess_evaluation_shader, source);
    return *this;
}

Program &Program::set_gs(const std::string &source)
{
    set_shader(shader_type::geometry_shader, source);
    return *this;
}

Program &Program::set_fs(const std::string &source)
{
    set_shader(shader_type::fragment_shader, source);
    return *this;
}

void Program::transform_feedback(vector<string> varyings, GLenum buffer_mode)
{
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (m_glsl_version >= 400 /*configuration::can_use.transform_feedback*/)
    {
        vector<char const *> c_array(varyings.size());
        for (size_t i = 0; i < varyings.size(); ++i)
        {
            c_array[i] = varyings[i].c_str();
        }

        gl::transform_feedback_varyings(m_gl_name, static_cast<GLsizei>(varyings.size()), &c_array[0], buffer_mode);
    }
    else
#endif
    {
        (void)varyings;
        (void)buffer_mode;
#if defined(RENDERSTACK_GL_API_OPENGL)
        throw runtime_error("transform feedback is not supported, requires GLSL version 4.00");
#else
        throw runtime_error("transform feedback is not supported");
#endif
    }
}

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
static char const *const attrib_type_str(GLenum type)
{
    switch (type)
    {
        case gl::active_attrib_type::int_: return "int_";
        case gl::active_attrib_type::unsigned_int: return "unsigned_int";
        case gl::active_attrib_type::float_: return "float_";
        case gl::active_attrib_type::double_: return "double_";
        case gl::active_attrib_type::float_vec2: return "float_vec2";
        case gl::active_attrib_type::float_vec3: return "float_vec3";
        case gl::active_attrib_type::float_vec4: return "float_vec4";
        case gl::active_attrib_type::int_vec2: return "int_vec2";
        case gl::active_attrib_type::int_vec3: return "int_vec3";
        case gl::active_attrib_type::int_vec4: return "int_vec4";
        case gl::active_attrib_type::float_mat2: return "float_mat2";
        case gl::active_attrib_type::float_mat3: return "float_mat3";
        case gl::active_attrib_type::float_mat4: return "float_mat4";
        case gl::active_attrib_type::unsigned_int_vec2: return "unsigned_int_vec2";
        case gl::active_attrib_type::unsigned_int_vec3: return "unsigned_int_vec3";
        case gl::active_attrib_type::unsigned_int_vec4: return "unsigned_int_vec4";
        case gl::active_attrib_type::double_mat2: return "double_mat2";
        case gl::active_attrib_type::double_mat3: return "double_mat3";
        case gl::active_attrib_type::double_mat4: return "double_mat4";
        case gl::active_attrib_type::double_mat2x3: return "double_mat2x3";
        case gl::active_attrib_type::double_mat2x4: return "double_mat2x4";
        case gl::active_attrib_type::double_mat3x2: return "double_mat3x2";
        case gl::active_attrib_type::double_mat3x4: return "double_mat3x4";
        case gl::active_attrib_type::double_mat4x2: return "double_mat4x2";
        case gl::active_attrib_type::double_mat4x3: return "double_mat4x3";
        case gl::active_attrib_type::double_vec2: return "double_vec2";
        case gl::active_attrib_type::double_vec3: return "double_vec3";
        case gl::active_attrib_type::double_vec4: return "double_vec4";
        default: return "?";
    };
}
#endif

void Program::add(Uniform_block *uniform_block)
{
    m_uniform_blocks[uniform_block->block_name()] = uniform_block;
}

void Program::link()
{
#if defined(LOG_LINK)
    cout << "Linking Program " << m_gl_name << ":\n";
    for (auto i = m_loaded_shaders.cbegin(); i != m_loaded_shaders.cend(); ++i)
    {
        cout << i->path << ":\n--------------------------------------------------------------------\n";
        string f_source = format(i->source);
        cout << f_source << "\n--------------------------------------------------------------------\n";
    }

    cout << "\n";
#endif

    gl::link_program(m_gl_name);
    int link_status                 = GL_FALSE;
    int validate_status             = GL_FALSE;
    int info_log_length             = 0;
    int attached_shaders            = 0;
    int active_attributes           = 0;
    int active_attribute_max_length = 128;
    int active_uniforms             = 0;
    int active_uniform_max_length   = 128;
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    int active_uniform_blocks                 = 0;
    int active_uniform_block_max_name_length  = 128;
    int transform_feedback_buffer_mode        = GL_INTERLEAVED_ATTRIBS;
    int transform_feedback_varyings           = 0;
    int transform_feedback_varying_max_length = 0;
#endif
    gl::get_program_iv(m_gl_name, gl::program_parameter::link_status, &link_status);
    gl::get_program_iv(m_gl_name, gl::program_parameter::validate_status, &validate_status);
    gl::get_program_iv(m_gl_name, gl::program_parameter::info_log_length, &info_log_length);
    gl::get_program_iv(m_gl_name, gl::program_parameter::attached_shaders, &attached_shaders);
    gl::get_program_iv(m_gl_name, gl::program_parameter::active_attributes, &active_attributes);
    gl::get_program_iv(m_gl_name, gl::program_parameter::active_attribute_max_length, &active_attribute_max_length);
    gl::get_program_iv(m_gl_name, gl::program_parameter::active_uniforms, &active_uniforms);
    gl::get_program_iv(m_gl_name, gl::program_parameter::active_uniform_max_length, &active_uniform_max_length);
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (configuration::can_use.uniform_buffer_object)
    {
        gl::get_program_iv(m_gl_name, gl::program_parameter::active_uniform_blocks, &active_uniform_blocks);
        gl::get_program_iv(m_gl_name, gl::program_parameter::active_uniform_block_max_name_length, &active_uniform_block_max_name_length);
    }
    if (configuration::can_use.transform_feedback)
    {
        gl::get_program_iv(m_gl_name, gl::program_parameter::transform_feedback_buffer_mode, &transform_feedback_buffer_mode);
        gl::get_program_iv(m_gl_name, gl::program_parameter::transform_feedback_varyings, &transform_feedback_varyings);
        gl::get_program_iv(m_gl_name, gl::program_parameter::transform_feedback_varying_max_length, &transform_feedback_varying_max_length);
    }
#endif
    if (link_status != GL_TRUE)
    {
        string log(info_log_length + 1, 0);
        gl::get_program_info_log(m_gl_name, info_log_length, nullptr, &log[0]);
        log_error("Program linking failed: \n");
        log_error(&log[0]);
        log_error("\n");
        for (auto i = m_loaded_shaders.begin(); i != m_loaded_shaders.end(); ++i)
        {
            auto &s = *i;

            string f_source = format(s.compiled_src);
            log_write(&log_glsl, false, LOG_ERROR, "\n%s\n", f_source.c_str());
        }
        log_error("Program linking failed: \n");
        log_error(&log[0]);
        log_error("\n");
        throw runtime_error("Program linking failed");
    }

    //  This is required by gl::uniform_1i() calls
    gl::use_program(m_gl_name);

    int buffer_size = active_uniform_max_length;
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    bool use_uniform_buffers =
        renderstack::graphics::configuration::can_use.uniform_buffer_object &&
        (m_glsl_version >= 140) &&
        (renderstack::graphics::configuration::shader_model_version >= 4);

    if (use_uniform_buffers && active_uniform_block_max_name_length > buffer_size)
    {
        buffer_size = active_uniform_block_max_name_length;
    }
#endif
    vector<char> buffer(buffer_size + 1);

    int uniform_count;

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (use_uniform_buffers)
    {
        if (active_uniform_blocks < 0)
        {
            assert(0);
            active_uniform_blocks = 0;
        }
        //m_uniform_block_names.clear();
        for (unsigned int i = 0; i < static_cast<unsigned int>(active_uniform_blocks); ++i)
        {
            GLsizei length;
            gl::get_active_uniform_block_name(m_gl_name, i, buffer_size, &length, &buffer[0]);
            gl::get_active_uniform_block_iv(m_gl_name,
                                            i,
                                            gl::active_uniform_block_parameter::uniform_block_active_uniforms,
                                            &uniform_count);

            // This should never happen
            if (length > buffer_size)
            {
                length = buffer_size;
            }

            buffer[length] = 0;

            string name(&buffer[0], static_cast<size_t>(length));

            auto fi = m_uniform_blocks.find(name);
            if (fi != m_uniform_blocks.end())
            {
                auto         block         = fi->second;
                unsigned int binding_point = block->binding_point();

                slog_trace("Program::link() uniform block: %s binding point: %u", name.c_str(), binding_point);
                gl::uniform_block_binding(m_gl_name, i, binding_point);
            }
            else
            {
                throw runtime_error("uniform block error");
            }
        }
    }
#endif

    uniform_count = active_uniforms;

    GLint  size;
    GLenum type;
    m_uniforms.clear();
    for (unsigned int i = 0; i < static_cast<unsigned int>(uniform_count); ++i)
    {
        GLsizei length;

        gl::get_active_uniform(m_gl_name,
                               i,
                               buffer_size,
                               &length,
                               &size,
                               &type,
                               &buffer[0]);

        //  This should never happen
        if (length > buffer_size)
        {
            length = buffer_size;
        }

        buffer[length] = 0;

        int index = gl::get_uniform_location(m_gl_name, &buffer[0]);

        if (index >= 0)
        {
            string name_str(&buffer[0], static_cast<size_t>(length));

            // cout << "name = " << &buffer[0] << " index = " << index << " name_str = " << name_str << "\n";

            m_uniforms.emplace(std::piecewise_construct, 
                               std::forward_as_tuple(name_str), 
                               std::forward_as_tuple(name_str,
                                                     index,
                                                     static_cast<size_t>(size),
                                                     static_cast<gl::active_uniform_type::value>(type)));

            if (m_samplers)
            {
                auto sampler_uniform = m_samplers->sampler(name_str);
                if (sampler_uniform)
                {
                    GLint texture_unit = sampler_uniform->texture_unit_index();
                    //printf("Program %s uniform %s bound to texture unit %d\n", m_name.c_str(), name_str.c_str(), texture_unit);
                    gl::uniform_1i(index, texture_unit);
                }
            }
        }
    }

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    if (configuration::can_use.transform_feedback)
    {
        if (transform_feedback_varyings < 0)
        {
            assert(0);
            transform_feedback_varyings = 0;
        }
        if (transform_feedback_varyings > 0)
        {
            if (transform_feedback_varying_max_length < 0)
            {
                assert(0);
            }
            string    buffer_(transform_feedback_varying_max_length + 1, 0);
            ::GLsizei length;
            ::GLsizei size2;
            ::GLenum  type2;
            for (unsigned int i = 0; i < static_cast<unsigned int>(transform_feedback_varyings); ++i)
            {
                gl::get_transform_feedback_varying(m_gl_name,
                                                   i,
                                                   transform_feedback_varying_max_length,
                                                   &length,
                                                   &size2,
                                                   &type2,
                                                   &buffer_[0]);

                if (size2 > 1)
                {
                    log_info("transform feedback varying %d %s %s[%d]\n", i, attrib_type_str(type2), &buffer_[0], size2);
                }
                else
                {
                    log_info("Transform feedback varying %d %s %s\n", i, attrib_type_str(type2), &buffer_[0]);
                }
            }
        }
    }
#endif
}

Uniform &Program::uniform(const std::string &name)
{
    return m_uniforms[name];
}

} // namespace graphics
} // namespace renderstack
