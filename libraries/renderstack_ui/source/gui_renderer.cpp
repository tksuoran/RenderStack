#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/fragment_outputs.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_toolkit/file.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/log.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include "renderstack_ui/style.hpp"
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

#define LOG_CATEGORY &log_gui_renderer

namespace renderstack
{
namespace ui
{

using namespace glm;
using namespace renderstack::graphics;
using namespace renderstack::toolkit;

Gui_renderer::Gui_renderer()
    : service("renderstack::ui::Gui_renderer")
{
}

void Gui_renderer::connect(std::shared_ptr<Renderer> renderer, std::shared_ptr<Shader_monitor> shader_monitor)
{
    m_renderer = renderer;
    m_shader_monitor = shader_monitor;

    initialization_depends_on(renderer);
    initialization_depends_on(shader_monitor);
}

std::unique_ptr<Program> Gui_renderer::load_program(const std::string &name, const std::string &path)
{
    for (auto &i : m_shader_versions)
    {
        std::string vs_path = m_shader_path + path + ".vs" + i.first + ".txt";
        std::string fs_path = m_shader_path + path + ".fs" + i.first + ".txt";

        if (!exists(vs_path) || !exists(fs_path))
        {
            continue;
        }

        auto p = std::make_unique<Program>(name, i.second, &m_samplers, &m_vertex_attribute_mappings, &m_fragment_outputs);
        p->add(&m_uniform_block);
        p->load_vs(vs_path);
        p->load_fs(fs_path);
        p->link();
        m_uniform_block.map_program(*(p.get()));

        if (m_shader_monitor)
        {
            m_shader_monitor->add(vs_path, p.get());
            m_shader_monitor->add(fs_path, p.get());
        }

        return p;
    }

    std::stringstream ss;
    ss << "Gui_renderer::load_program(" << name << ") failed";

    ss << "\nNo source files found.\n";
    ss << "Did you run export-files build target?\n";
    ss << "Current working directory is: " << get_current_working_directory() << "\n";
    for (auto &i : m_shader_versions)
    {
        std::string vs_path = m_shader_path + path + ".vs" + i.first + ".txt";
        std::string fs_path = m_shader_path + path + ".fs" + i.first + ".txt";

        ss << "\t" << vs_path << "\n";
        ss << "\t" << fs_path << "\n";
    }

    log_error("%s", ss.str().c_str());
    throw std::runtime_error(ss.str());
}

void Gui_renderer::initialize_service()
{
    slog_trace("Gui_renderer::initialize_service()");

    m_blend_alpha.enabled                  = true;
    m_blend_alpha.rgb.equation_mode        = gl::blend_equation_mode::func_add;
    m_blend_alpha.rgb.source_factor        = gl::blending_factor_src::src_alpha;
    m_blend_alpha.rgb.destination_factor   = gl::blending_factor_dest::one_minus_src_alpha;
    m_blend_alpha.alpha.equation_mode      = gl::blend_equation_mode::func_add;
    m_blend_alpha.alpha.source_factor      = gl::blending_factor_src::src_alpha;
    m_blend_alpha.alpha.destination_factor = gl::blending_factor_dest::one_minus_src_alpha;

    m_blend_add.enabled                  = true;
    m_blend_add.rgb.equation_mode        = gl::blend_equation_mode::func_add;
    m_blend_add.rgb.source_factor        = gl::blending_factor_src::one;
    m_blend_add.rgb.destination_factor   = gl::blending_factor_dest::one;
    m_blend_add.alpha.equation_mode      = gl::blend_equation_mode::func_add;
    m_blend_add.alpha.source_factor      = gl::blending_factor_src::one;
    m_blend_add.alpha.destination_factor = gl::blending_factor_dest::one;

    m_vertex_attribute_mappings.add("a_position", Vertex_attribute::Usage::position, 0, 0);
    m_vertex_attribute_mappings.add("a_normal", Vertex_attribute::Usage::normal, 0, 1);
    m_vertex_attribute_mappings.add("a_normal_flat", Vertex_attribute::Usage::normal, 1, 2);
    m_vertex_attribute_mappings.add("a_normal_smooth", Vertex_attribute::Usage::normal, 2, 3);
    m_vertex_attribute_mappings.add("a_color", Vertex_attribute::Usage::color, 0, 4);
    m_vertex_attribute_mappings.add("a_texcoord", Vertex_attribute::Usage::tex_coord, 1, 5);
    m_vertex_attribute_mappings.add("a_position_texcoord",
                                     Vertex_attribute::Usage::position | Vertex_attribute::Usage::tex_coord,
                                     0,
                                     0);

    m_vertex_format.make_attribute(Vertex_attribute::Usage::position | Vertex_attribute::Usage::tex_coord,
                                   gl::vertex_attrib_pointer_type::float_,
                                   gl::vertex_attrib_pointer_type::float_,
                                   0,
                                   4,
                                   false);

    auto &r = *m_renderer;

    auto va     = m_vertex_stream.vertex_array();
    auto old_va = r.set_vertex_array(va);

    m_vertex_buffer = std::make_shared<Buffer>(Buffer::Target::array_buffer,
                                               128 * 1024,
                                               4 * sizeof(float),
                                               gl::buffer_usage_hint::dynamic_draw);
    m_vertex_buffer->allocate_storage(*m_renderer);

    m_index_buffer = std::make_shared<Buffer>(Buffer::Target::element_array_buffer,
                                              512 * 1024,
                                              sizeof(unsigned short),
                                              gl::buffer_usage_hint::dynamic_draw);
    m_index_buffer->allocate_storage(*m_renderer);

    m_vertex_attribute_mappings.add_to_vertex_stream(m_vertex_stream, m_vertex_buffer.get(), m_vertex_format);
    r.setup_attribute_pointers(m_vertex_stream, 0);
    va->set_index_buffer(m_index_buffer.get());

    m_fragment_outputs.add("out_color", gl::fragment_output_type::float_vec4, 0);

    // Gui_renderer does not use uniform blocks / buffers.
    // constructor without block index creates uniforms to the default block
    m_uniforms.clip_from_model = m_uniform_block.add_mat4("clip_from_model").access();
    m_uniforms.color_add       = m_uniform_block.add_vec4("color_add").access();
    m_uniforms.color_scale     = m_uniform_block.add_vec4("color_scale").access();
    m_uniforms.hsv_matrix      = m_uniform_block.add_mat4("hsv_matrix").access();
    m_uniforms.t               = m_uniform_block.add_float("t").access();
    m_uniform_block.seal();

    m_shader_path = "res/shaders/";

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    m_shader_versions.push_back(make_pair("4", 300));
#else
    if ((configuration::shader_model_version >= 5) &&
        (configuration::glsl_version >= 400))
    {
        m_shader_versions.emplace_back("5", 400);
    }

    if ((configuration::shader_model_version >= 4) &&
        (configuration::glsl_version >= 150))
    {
        m_shader_versions.emplace_back("4", 150);
    }

    m_shader_versions.emplace_back("0", 120);
#endif

    m_samplers.add("font_texture", gl::active_uniform_type::sampler_2d, &m_nearest_sampler).set_texture_unit_index(0);
    m_samplers.add("background_texture", gl::active_uniform_type::sampler_2d, &m_nearest_sampler).set_texture_unit_index(1);

    try
    {
        m_ninepatch_program = load_program("ninepatch", "gui");
        m_slider_program    = load_program("slider", "gui_slider");
        m_font_program      = load_program("Font", "gui_font");
        m_hsv_program       = load_program("hsv", "gui_hsv");
    }
    catch (const std::runtime_error &e)
    {
        log_error("Gui_renderer() shaders are broken: %s", e.what());
        throw;
    }
    catch (...)
    {
        log_error("shaders are broken");
        throw;
    }

    m_font = std::make_unique<Font>(r, "res/fonts/Ubuntu-R.ttf", 11, 1.0f);
    //m_font = make_shared<Font>(r, "res/fonts/SourceCodePro-Semibold.ttf", 8, 1.0f);
    //m_font = make_shared<font>(r, "res/fonts/SourceCodePro-Bold.ttf", 9, 1.0f);

    m_button_ninepatch_style   = std::make_unique<Ninepatch_style>(r, "res/images/button_released.png", m_ninepatch_program.get(), 1);
    m_menulist_ninepatch_style = std::make_unique<Ninepatch_style>(r, "res/images/shadow.png",          m_ninepatch_program.get(), 1);
    m_slider_ninepatch_style   = std::make_unique<Ninepatch_style>(r, "res/images/button_released.png", m_slider_program.get(), 1);

    vec2 zero(0.0f, 0.0f);
    vec2 button_padding(26.0f, 6.0f);
    vec2 menulist_padding(16.0f, 16.0f);
    vec2 inner_padding(6.0f, 6.0f);

    m_default_style      = std::make_unique<Style>(vec2(6.0f, 6.0f),
                                                   vec2(6.0f, 6.0f));
    m_null_padding_style = std::make_unique<Style>(vec2(0.0f, 0.0f),
                                                   vec2(0.0f, 0.0f));

    // With MSVC this needs _VARIADIC_MAX defined to 6 or more
    m_button_style      = std::make_unique<Style>(button_padding,
                                                  inner_padding,
                                                  m_font.get(),
                                                  m_button_ninepatch_style.get(),
                                                  m_font_program.get());
    m_slider_style      = std::make_unique<Style>(button_padding,
                                                  inner_padding,
                                                  m_font.get(),
                                                  m_slider_ninepatch_style.get(),
                                                  m_font_program.get());
    m_choice_style      = std::make_unique<Style>(zero,
                                                  zero,
                                                  m_font.get(),
                                                  m_button_ninepatch_style.get(),
                                                  m_font_program.get());
    m_menulist_style    = std::make_unique<Style>(menulist_padding,
                                                  inner_padding,
                                                  nullptr,
                                                  m_menulist_ninepatch_style.get(),
                                                  nullptr);
    m_colorpicker_style = std::make_unique<Style>(menulist_padding,
                                                  inner_padding,
                                                  nullptr,
                                                  m_menulist_ninepatch_style.get(),
                                                  m_hsv_program.get());

    if (configuration::use_vertex_array_object)
    {
        r.set_vertex_array(old_va);
    }
}

void Gui_renderer::blend_alpha()
{
    m_renderer->track().blend.execute(&m_blend_alpha);
}

void Gui_renderer::blend_add()
{
    m_renderer->track().blend.execute(&m_blend_add);
}

void Gui_renderer::blend_disable()
{
    m_renderer->track().blend.execute(&m_blend_disabled);
}

void Gui_renderer::set_vertex_buffer()
{
    slog_trace("Gui_renderer::set_vertex_buffer()");

    m_renderer->set_buffer(Buffer::Target::array_buffer, m_vertex_buffer.get());
}

void Gui_renderer::set_index_buffer()
{
    slog_trace("Gui_renderer::set_index_buffer()");

    // Make sure our VAO is bound, then make sure our IBO is bound
    auto va = m_vertex_stream.vertex_array();

    m_renderer->set_vertex_array(va);
    va->set_index_buffer(m_index_buffer.get());
}

void Gui_renderer::prepare()
{
    slog_trace("Gui_renderer::prepare()");

    // Default state
    auto &r = *(m_renderer.get());
    State_trackers &t = r.track();
    //m_renderer->track.execute(&m_gui_render_states);
    t.color_mask.execute(&m_gui_render_states.color_mask);
    t.blend.execute(&m_gui_render_states.blend);
    t.depth.execute(&m_gui_render_states.depth);
    t.face_cull.execute(&m_gui_render_states.face_cull);
    t.stencil.execute(&m_gui_render_states.stencil);

    blend_alpha();

    // We bind these for both render and edit uses

    m_renderer->use_vertex_stream(m_vertex_stream);
    m_renderer->set_buffer(Buffer::Target::array_buffer, m_vertex_buffer.get());
    m_renderer->vertex_array()->set_index_buffer(m_index_buffer.get());
}

void Gui_renderer::on_resize(int width, int height)
{
    m_ortho = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
}

void Gui_renderer::set_program(Program *value)
{
    slog_trace("Gui_renderer::set_program(%s)", value->name().c_str());

    assert(value);
    m_renderer->set_program(value);
    m_program = value;
}

void Gui_renderer::set_texture(unsigned int unit, Texture *texture)
{
    slog_trace("Gui_renderer::set_texture(unit = %u, texture = %u)", unit, texture ? texture->gl_name() : 0);

    (void)m_renderer->set_texture(unit, texture);
}

void Gui_renderer::set_transform(mat4 value)
{
    assert(m_program);
    gl::uniform_matrix_4fv(m_program->uniform_at(m_uniforms.clip_from_model),
                           1,
                           GL_FALSE,
                           value_ptr(value));
}

void Gui_renderer::set_color_add(vec4 value)
{
    assert(m_program);
    gl::uniform_4fv(m_program->uniform_at(m_uniforms.color_add), 1, value_ptr(value));
}

void Gui_renderer::set_color_scale(vec4 value)
{
    assert(m_program);
    gl::uniform_4fv(m_program->uniform_at(m_uniforms.color_scale), 1, value_ptr(value));
}

void Gui_renderer::set_hsv_matrix(mat4 value)
{
    assert(m_program);
    gl::uniform_matrix_4fv(m_program->uniform_at(m_uniforms.hsv_matrix),
                           1,
                           GL_FALSE,
                           value_ptr(value));
}

void Gui_renderer::set_t(float value)
{
    assert(m_program);
    gl::uniform_1f(m_program->uniform_at(m_uniforms.t), value);
}

void Gui_renderer::draw_elements_base_vertex(GLenum begin_mode, GLsizei count, GLenum index_type, const GLvoid *indices, GLint base_vertex)
{
    // TODO Can we do this in caller somewhere?
    auto va = m_vertex_stream.vertex_array();

    m_renderer->set_vertex_array(va);
    va->set_index_buffer(m_index_buffer.get());
    m_renderer->draw_elements_base_vertex(m_vertex_stream, begin_mode, count, index_type, indices, base_vertex);
}

} // namespace ui
} // namespace renderstack
