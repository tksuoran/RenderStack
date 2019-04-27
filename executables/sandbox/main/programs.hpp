#ifndef programs_hpp
#define programs_hpp

#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/fragment_outputs.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <memory>

namespace renderstack
{
namespace graphics
{
class Shader_monitor;
class Program;
class Uniform_buffer;
class Uniform_buffer_range;
class Vertex_stream_mappings;
} // namespace graphics
} // namespace renderstack

struct models_block_i
{
    size_t clip_from_model;  // mat4
    size_t world_from_model; // mat4
    size_t view_from_model;  // mat4
    size_t id_offset;        // vec3
};

struct camera_block_i
{
    size_t world_from_view; // mat4
    size_t world_from_clip; // mat4
    size_t clip_from_world; // mat4
    size_t viewport;        // vec4
    size_t exposure;
};

struct lights_block_i
{
    size_t position;
    size_t direction;
    size_t radiance;
    size_t spot_cutoff;
};

struct materials_block_i
{
    size_t color;     // vec4
    size_t roughness; // float aka 1.0 - smoothness
    size_t isotropy;  // float aka 1.0 - anisotropy
    size_t metalness; // float
                      // size_t translucency;        /* float */
                      // size_t specular_coverage;   /* float */
                      // size_t wet_roughness;       /* float */
};

struct debug_block_i
{
    size_t clip_from_world;   // mat4
    size_t color;             // vec4
    size_t line_width;        // vec2
    size_t show_rt_transform; // mat4
};

struct ubr_pos
{
    ubr_pos()
        : models(0), lights(0), materials(0), camera(0)
    {
    }
    size_t models;
    size_t lights;
    size_t materials;
    size_t camera;
};

struct ubr_ptr
{
    unsigned char *models;
    unsigned char *lights;
    unsigned char *materials;
    unsigned char *camera;
    unsigned char *debug;
};

class Programs : public renderstack::toolkit::service
{
public:
    Programs();

    virtual ~Programs();

    void connect(std::shared_ptr<renderstack::graphics::Renderer>       renderer,
                 std::shared_ptr<renderstack::graphics::Shader_monitor> shader_monitor);

    void initialize_service() override;

private:
    std::unique_ptr<renderstack::graphics::Program> make_program(const std::string &name, std::vector<std::string> const &defines);

    std::unique_ptr<renderstack::graphics::Program> make_program(const std::string &name, const std::string &define);

    std::unique_ptr<renderstack::graphics::Program> make_program(const std::string &name);

public:
    models_block_i    models_block_access;
    lights_block_i    lights_block_access;
    materials_block_i materials_block_access;
    camera_block_i    camera_block_access;
    debug_block_i     debug_block_access;

    renderstack::graphics::Uniform_block models_block{0, "models"};
    renderstack::graphics::Uniform_block camera_block{1, "camera"};
    renderstack::graphics::Uniform_block materials_block{2, "materials"};
    renderstack::graphics::Uniform_block lights_block{3, "lights"};
    renderstack::graphics::Uniform_block debug_block{"debug"};

    renderstack::graphics::Vertex_attribute_mappings attribute_mappings;
    renderstack::graphics::Fragment_outputs          fragment_outputs;

    renderstack::graphics::Sampler m_nearest_sampler;
    renderstack::graphics::Sampler m_show_rt_sampler;
    renderstack::graphics::Samplers                  samplers;
    std::unique_ptr<renderstack::graphics::Program>  font;
    std::unique_ptr<renderstack::graphics::Program>  basic;
    std::unique_ptr<renderstack::graphics::Program>  anisotropic_spot;
    std::unique_ptr<renderstack::graphics::Program>  anisotropic_directional;
    std::unique_ptr<renderstack::graphics::Program>  debug_line;
    std::unique_ptr<renderstack::graphics::Program>  debug_light;
    std::unique_ptr<renderstack::graphics::Program>  debug_font;
    std::unique_ptr<renderstack::graphics::Program>  textured;
    std::unique_ptr<renderstack::graphics::Program>  gbuffer;
    std::unique_ptr<renderstack::graphics::Program>  light_spot;
    std::unique_ptr<renderstack::graphics::Program>  light_directional;
    std::unique_ptr<renderstack::graphics::Program>  stencil;
    std::unique_ptr<renderstack::graphics::Program>  show_rt;
    std::unique_ptr<renderstack::graphics::Program>  show_rt_spherical;
    std::unique_ptr<renderstack::graphics::Program>  id;
    std::unique_ptr<renderstack::graphics::Program>  camera;

public:
    void update_fixed_step();

private:
    std::shared_ptr<renderstack::graphics::Renderer>       m_renderer;
    std::shared_ptr<renderstack::graphics::Shader_monitor> m_shader_monitor;

    bool   m_poll_shaders;
    size_t m_poll_ticks;

    std::vector<std::pair<std::string, int>> m_shader_versions;
    std::string                              m_shader_path;
};

#endif
