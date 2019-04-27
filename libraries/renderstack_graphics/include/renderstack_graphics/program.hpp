#ifndef program_hpp_renderstack_graphics
#define program_hpp_renderstack_graphics

#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/log.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace graphics
{

class Fragment_outputs;
class Renderer;
class Samplers;
class Shader_monitor;
class Uniform_block;

class Program : public std::enable_shared_from_this<Program>
{
public:
    Program(const std::string &       name,
            int                       glsl_version,
            Samplers                  *samplers,
            Vertex_attribute_mappings *vertex_attributes,
            Fragment_outputs          *fragment_output_mappings);

    const std::string &name() const
    {
        return m_name;
    }

    Samplers *samplers()
    {
        return m_samplers;
    }

    Vertex_attribute_mappings *vertex_attribute_mappings()
    {
        return m_vertex_attribute_mappings;
    }

    void dump_shaders() const;

    void map_uniform(size_t key, const std::string &name);

    int uniform_at(size_t index) const;

    bool use_uniform_buffers() const;

    void bind_attrib_location(int location, const std::string &name);

    void bind_frag_data_location(int location, const std::string &name);

    void define(const std::string &key, const std::string &value);

    int get_uniform_location(const char *name);

    void load_shader(gl::shader_type::value type, const std::string &path);

    void set_shader(gl::shader_type::value type, const std::string &source);

    Program &load_vs(const std::string &path);

    Program &load_tcs(const std::string &path);

    Program &load_tes(const std::string &path);

    Program &load_gs(const std::string &path);

    Program &load_fs(const std::string &path);

    Program &set_vs(const std::string &source);

    Program &set_tcs(const std::string &source);

    Program &set_tes(const std::string &source);

    Program &set_gs(const std::string &source);

    Program &set_fs(const std::string &source);

    Uniform &uniform(const std::string &name);

    void transform_feedback(std::vector<std::string> varyings, GLenum buffer_mode);

    void add(Uniform_block *uniform_block);

    void link();

    void reload(Shader_monitor &monitor);

    friend class Renderer;

private:
    unsigned int gl_name() const
    {
        return m_gl_name;
    }

private:
    struct loaded_shader
    {
        gl::shader_type::value type;
        unsigned int           shader;
        unsigned int           last_good_shader;
        std::string            path;
        std::string            source;
        std::string            compiled_src;
    };

    std::string                                         m_name;
    int                                                 m_glsl_version;
    unsigned int                                        m_gl_name{~0U};
    std::unordered_map<std::string, Uniform_block*>     m_uniform_blocks;
    std::unordered_map<std::string, Uniform>            m_uniforms;
    Samplers                                            *m_samplers{nullptr};
    Vertex_attribute_mappings                           *m_vertex_attribute_mappings{nullptr};
    Fragment_outputs                                    *m_fragment_outputs{nullptr};
    std::vector<loaded_shader>                          m_loaded_shaders;
    std::vector<std::pair<std::string, std::string>>    m_defines;
    std::vector<GLint>                                  m_uniform_map;

private:
    static std::string format(const std::string &source);

private:
    unsigned int make_shader(gl::shader_type::value type,
                             std::string            source_str,
                             std::string            &compiled_src);
};

} // namespace graphics
} // namespace renderstack

#endif
