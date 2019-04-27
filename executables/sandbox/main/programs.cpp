#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/fragment_outputs.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_toolkit/file.hpp"
#include "renderstack_toolkit/platform.hpp"

#include "main/application.hpp"
#include "main/log.hpp"
#include "main/programs.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <utility>

#define LOG_CATEGORY &log_programs

using namespace renderstack::toolkit;
using namespace renderstack::graphics;
using namespace std;

Programs::Programs()
    : service("Programs")
{
}

Programs::~Programs()
{
}

void Programs::connect(
    shared_ptr<renderstack::graphics::Renderer>       renderer_,
    shared_ptr<renderstack::graphics::Shader_monitor> shader_monitor_)
{
    m_renderer       = renderer_;
    m_shader_monitor = shader_monitor_;

    initialization_depends_on(renderer_);
    initialization_depends_on(shader_monitor_);
}

void Programs::initialize_service()
{
    assert(m_renderer);

    slog_trace("Programs::initialize_service()\n");

    fragment_outputs.add("out_id", gl::fragment_output_type::float_vec4, 0);
    fragment_outputs.add("out_color", gl::fragment_output_type::float_vec4, 0);

    //fragment_outputs.add("out_normal_tangent",  gl::fragment_output_type::float_vec4, 0);
    fragment_outputs.add("out_normal", gl::fragment_output_type::float_vec4, 0);
    fragment_outputs.add("out_tangent", gl::fragment_output_type::float_vec4, 1);
    fragment_outputs.add("out_albedo", gl::fragment_output_type::float_vec4, 2);
    fragment_outputs.add("out_material", gl::fragment_output_type::float_vec4, 3);
    fragment_outputs.add("out_emission", gl::fragment_output_type::float_vec4, 4);

    fragment_outputs.add("out_linear", gl::fragment_output_type::float_vec4, 0);

    attribute_mappings.add("a_position", vertex_attribute_usage::position, 0, 0);
    attribute_mappings.add("a_normal", vertex_attribute_usage::normal, 0, 1);
    attribute_mappings.add("a_normal_flat", vertex_attribute_usage::normal, 1, 2);
    attribute_mappings.add("a_normal_smooth", vertex_attribute_usage::normal, 2, 3);
    attribute_mappings.add("a_tangent", vertex_attribute_usage::tangent, 0, 4);
    attribute_mappings.add("a_color", vertex_attribute_usage::color, 0, 5);
    attribute_mappings.add("a_texcoord", vertex_attribute_usage::tex_coord, 0, 6);
    attribute_mappings.add("a_id", vertex_attribute_usage::id, 0, 7);
    attribute_mappings.add("a_position_texcoord",
                           static_cast<vertex_attribute_usage::value>(
                               vertex_attribute_usage::position | vertex_attribute_usage::tex_coord),
                           0,
                           0);

    //mappings->add("a_position_texcoord",   vertex_attribute_usage::position,   0, 0);
    /*mappings->add(
      "a_position_texcoord",
      vertex_attribute_usage::position_texcoord,
      0, 6);*/

    m_poll_shaders = false;
    m_poll_ticks   = 0;

    models_block_access.clip_from_model  = models_block.add_mat4("clip_from_model").access();
    models_block_access.world_from_model = models_block.add_mat4("world_from_model").access();
    models_block_access.view_from_model  = models_block.add_mat4("view_from_model").access();
    models_block_access.id_offset        = models_block.add_vec3("id_offset").access();
    models_block.seal();

    camera_block_access.world_from_view = camera_block.add_mat4("world_from_view").access();
    camera_block_access.world_from_clip = camera_block.add_mat4("world_from_clip").access();
    camera_block_access.clip_from_world = camera_block.add_mat4("clip_from_world").access();
    camera_block_access.viewport        = camera_block.add_vec4("viewport").access();
    camera_block_access.exposure        = camera_block.add_float("exposure").access();
    camera_block.seal();

    materials_block_access.color     = materials_block.add_vec4("color").access();
    materials_block_access.roughness = materials_block.add_float("roughness").access();
    materials_block_access.isotropy  = materials_block.add_float("isotropy").access();
    materials_block.seal();

    lights_block_access.position    = lights_block.add_vec3("position").access();
    lights_block_access.direction   = lights_block.add_vec3("direction").access();
    lights_block_access.radiance    = lights_block.add_vec3("radiance").access();
    lights_block_access.spot_cutoff = lights_block.add_float("spot_cutoff").access();
    lights_block.seal();

    // This is created as default block. It is easier to use old style glUniform
    // API since updating uniform buffers efficiently would be too much trouble
    // with little or no benefit at all.
    debug_block_access.clip_from_world   = debug_block.add_mat4("clip_from_world").access();
    debug_block_access.color             = debug_block.add_vec4("color").access();
    debug_block_access.line_width        = debug_block.add_vec4("line_width").access();
    debug_block_access.show_rt_transform = debug_block.add_vec4("show_rt_transform").access();

    m_show_rt_sampler.set_mag_filter(gl::texture_mag_filter::nearest);
    m_show_rt_sampler.set_min_filter(gl::texture_min_filter::linear);

    samplers.add("font_texture", gl::active_uniform_type::sampler_2d, &m_nearest_sampler).set_texture_unit_index(0);
    samplers.add("background_texture", gl::active_uniform_type::sampler_2d, &m_nearest_sampler).set_texture_unit_index(1);

    //samplers.add("normal_tangent_texture",   gl::active_uniform_type::sampler_2d, &m_nearest_sampler)->set_texture_unit_index(0);
    samplers.add("normal_texture", gl::active_uniform_type::sampler_2d, &m_nearest_sampler).set_texture_unit_index(0);
    samplers.add("tangent_texture", gl::active_uniform_type::sampler_2d, &m_nearest_sampler).set_texture_unit_index(1);
    samplers.add("albedo_texture", gl::active_uniform_type::sampler_2d, &m_nearest_sampler).set_texture_unit_index(2);
    samplers.add("material_texture", gl::active_uniform_type::sampler_2d, &m_nearest_sampler).set_texture_unit_index(3);
    //samplers.add("emission_texture",         gl::active_uniform_type::sampler_2d, &m_nearest_sampler)->set_texture_unit_index(4);

    samplers.add("depth_texture", gl::active_uniform_type::sampler_2d, &m_nearest_sampler).set_texture_unit_index(4);

    samplers.add("show_rt_texture", gl::active_uniform_type::sampler_2d, &m_show_rt_sampler).set_texture_unit_index(0);

    samplers.add("linear_texture", gl::active_uniform_type::sampler_2d, &m_show_rt_sampler).set_texture_unit_index(0);

    m_shader_path = "res/shaders/";

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    m_shader_versions.push_back(make_pair("4", 300));
#else
    if ((renderstack::graphics::configuration::shader_model_version >= 5) &&
        (renderstack::graphics::configuration::glsl_version >= 400))
    {
        m_shader_versions.push_back(make_pair("5", 400));
    }

    if ((renderstack::graphics::configuration::shader_model_version >= 4) &&
        (renderstack::graphics::configuration::glsl_version >= 150))
    {
        m_shader_versions.push_back(make_pair("4", 150));
    }

    m_shader_versions.push_back(make_pair("0", 120));
#endif

    try
    {
        font                    = make_program("font");
        basic                   = make_program("basic");
        gbuffer                 = make_program("gbuffer");
        light_spot              = make_program("light", "LIGHT_TYPE_SPOT");
        light_directional       = make_program("light", "LIGHT_TYPE_DIRECTIONAL");
        stencil                 = make_program("stencil");
        show_rt                 = make_program("show_rt");
        show_rt_spherical       = make_program("show_rt_spherical");
        textured                = make_program("textured");
        id                      = make_program("id");
        debug_font              = make_program("font", "USE_DEBUG_UNIFORMS");
        debug_line              = make_program("debug_line");
        debug_light             = make_program("debug_light");
        anisotropic_spot        = make_program("anisotropic", "LIGHT_TYPE_SPOT");
        anisotropic_directional = make_program("anisotropic", "LIGHT_TYPE_DIRECTIONAL");
        camera                  = make_program("camera");
    }
    catch (runtime_error const &e)
    {
        log_error("Programs::initialize_service() - aborting\n");
        throw e;
    }
}

unique_ptr<renderstack::graphics::Program> Programs::make_program(
    const std::string &name)
{
    vector<string> no_defines;
    return make_program(name, no_defines);
}

unique_ptr<renderstack::graphics::Program> Programs::make_program(
    const std::string &name,
    const std::string &define)
{
    vector<string> defines;
    defines.push_back(define);
    return make_program(name, defines);
}

unique_ptr<renderstack::graphics::Program> Programs::make_program(
    const std::string &   name,
    vector<string> const &defines)
{
    log_info("Programs::make_program(%s)\n", name.c_str());

    for (auto i = m_shader_versions.cbegin(); i != m_shader_versions.cend(); ++i)
    {
        int glsl_version = i->second;
        string vs_path = m_shader_path + name + ".vs" + i->first + ".txt";
        string fs_path = m_shader_path + name + ".fs" + i->first + ".txt";

        if (!exists(vs_path) || !exists(fs_path))
        {
            continue;
        }

        auto p = std::make_unique<Program>(name, glsl_version, &samplers, &attribute_mappings, &fragment_outputs);
        auto &pr = *(p.get());
        pr.add(&models_block);
        pr.add(&lights_block);
        pr.add(&materials_block);
        pr.add(&camera_block);
        pr.add(&debug_block);
        for (auto j : defines)
        {
            pr.define(j, "1");
        }
        pr.load_vs(vs_path);
        pr.load_fs(fs_path);
        pr.link();
        models_block.map_program(pr);
        lights_block.map_program(pr);
        materials_block.map_program(pr);
        camera_block.map_program(pr);
        debug_block.map_program(pr);

        if (m_shader_monitor)
        {
            m_shader_monitor->add(vs_path, p.get());
            m_shader_monitor->add(fs_path, p.get());
        }
        return p;
    }

    stringstream ss;
    ss << "Programs::make_program(" << name << ") failed";
    log_error("%s", ss.str().c_str());
    throw runtime_error(ss.str());
}

void Programs::update_fixed_step()
{
#if 0
   ++m_poll_ticks;
   if (m_poll_ticks == 30)
   {
      auto monitor = context::current()->shader_monitor();
      monitor.poll();
      m_poll_ticks = 0;
   }
#endif
}
