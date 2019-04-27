#ifndef programs_hpp
#define programs_hpp

#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <memory>

namespace renderstack
{
namespace graphics
{
class shader_monitor;
class sampler;
class samplers;
class program;
class uniform_block;
class uniform_buffer;
class uniform_buffer_range;
class vertex_stream_mappings;
} // namespace graphics
} // namespace renderstack

struct models_block_i
{
    std::size_t clip_from_model;  /* mat4 */
    std::size_t world_from_model; /* mat4 */
    std::size_t view_from_model;  /* mat4 */
    std::size_t id_offset;        /* vec3 */
};

struct camera_block_i
{
    std::size_t world_from_view; /* mat4 */
    std::size_t world_from_clip; /* mat4 */
    std::size_t clip_from_world; /* mat4 */
    std::size_t viewport;        /* vec4 */
    std::size_t exposure;
};

struct lights_block_i
{
    std::size_t position;
    std::size_t direction;
    std::size_t radiance;
    std::size_t spot_cutoff;
};

struct materials_block_i
{
    std::size_t color;     /* vec4 */
    std::size_t roughness; /* float aka 1.0 - smoothness */
    std::size_t isotropy;  /* float aka 1.0 - anisotropy */
    std::size_t metalness; /* float */
                           // std::size_t translucency;        /* float */
                           // std::size_t specular_coverage;   /* float */
                           // std::size_t wet_roughness;       /* float */
};

struct debug_block_i
{
    std::size_t clip_from_world;   /* mat4 */
    std::size_t color;             /* vec4 */
    std::size_t line_width;        /* vec2 */
    std::size_t show_rt_transform; /* mat4 */
};

struct ubr_pos
{
    ubr_pos()
        : models(0), lights(0), materials(0), camera(0)
    {
    }
    std::size_t models;
    std::size_t lights;
    std::size_t materials;
    std::size_t camera;
};

struct ubr_ptr
{
    unsigned char *models;
    unsigned char *lights;
    unsigned char *materials;
    unsigned char *camera;
    unsigned char *debug;
};

class programs : public renderstack::toolkit::service
{
public:
    programs();
    /*virtual*/ ~programs();
    void connect(
        std::shared_ptr<renderstack::graphics::renderer>       renderer_,
        std::shared_ptr<renderstack::graphics::shader_monitor> shader_monitor_);
    void initialize_service() override;

private:
    std::shared_ptr<renderstack::graphics::program> make_program(
        std::string const &             name,
        std::vector<std::string> const &defines);
    std::shared_ptr<renderstack::graphics::program> make_program(
        std::string const &name,
        std::string const &define);
    std::shared_ptr<renderstack::graphics::program> make_program(
        std::string const &name);

public:
    models_block_i    models_block_access;
    lights_block_i    lights_block_access;
    materials_block_i materials_block_access;
    camera_block_i    camera_block_access;
    debug_block_i     debug_block_access;

    std::shared_ptr<renderstack::graphics::uniform_block> models_block;
    std::shared_ptr<renderstack::graphics::uniform_block> lights_block;
    std::shared_ptr<renderstack::graphics::uniform_block> materials_block;
    std::shared_ptr<renderstack::graphics::uniform_block> camera_block;
    std::shared_ptr<renderstack::graphics::uniform_block> debug_block;

    std::shared_ptr<renderstack::graphics::samplers>                  samplers;
    std::shared_ptr<renderstack::graphics::vertex_attribute_mappings> attribute_mappings;
    std::shared_ptr<renderstack::graphics::fragment_outputs>          fragment_outputs;
    std::shared_ptr<renderstack::graphics::program>                   font;
    std::shared_ptr<renderstack::graphics::program>                   basic;
    std::shared_ptr<renderstack::graphics::program>                   anisotropic_spot;
    std::shared_ptr<renderstack::graphics::program>                   anisotropic_directional;
    std::shared_ptr<renderstack::graphics::program>                   debug_line;
    std::shared_ptr<renderstack::graphics::program>                   debug_light;
    std::shared_ptr<renderstack::graphics::program>                   debug_font;
    std::shared_ptr<renderstack::graphics::program>                   textured;
    std::shared_ptr<renderstack::graphics::program>                   gbuffer;
    std::shared_ptr<renderstack::graphics::program>                   light_spot;
    std::shared_ptr<renderstack::graphics::program>                   light_directional;
    std::shared_ptr<renderstack::graphics::program>                   stencil;
    std::shared_ptr<renderstack::graphics::program>                   show_rt;
    std::shared_ptr<renderstack::graphics::program>                   show_rt_spherical;
    std::shared_ptr<renderstack::graphics::program>                   id;
    std::shared_ptr<renderstack::graphics::program>                   camera;

public:
    void update_fixed_step();

private:
    std::shared_ptr<renderstack::graphics::renderer>       m_renderer;
    std::shared_ptr<renderstack::graphics::shader_monitor> m_shader_monitor;

    bool   m_poll_shaders;
    size_t m_poll_ticks;

    std::vector<std::pair<std::string, int>> m_shader_versions;
    std::string                              m_shader_path;
};

#endif
