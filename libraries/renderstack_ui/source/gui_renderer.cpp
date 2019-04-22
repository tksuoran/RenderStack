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

using namespace std;
using namespace glm;
using namespace renderstack::graphics;
using namespace renderstack::toolkit;

gui_renderer::gui_renderer()
    : service("renderstack::ui::gui_renderer"), m_start(nullptr)
{
}

/*virtual*/ gui_renderer::~gui_renderer()
{
}

void gui_renderer::connect(
    shared_ptr<renderstack::graphics::renderer>       renderer_,
    shared_ptr<renderstack::graphics::shader_monitor> shader_monitor_)
{
    m_renderer       = renderer_;
    m_shader_monitor = shader_monitor_;

    initialization_depends_on(renderer_);
    initialization_depends_on(shader_monitor_);
}

shared_ptr<program> gui_renderer::load_program(string const &name, string const &path)
{
    bool any_found = false;
    for (auto i = m_shader_versions.cbegin(); i != m_shader_versions.cend(); ++i)
    {
        string vs_path = m_shader_path + path + ".vs" + i->first + ".txt";
        string fs_path = m_shader_path + path + ".fs" + i->first + ".txt";

        if (!exists(vs_path) || !exists(fs_path))
            continue;

        any_found = true;
        auto p    = make_shared<program>(name, i->second, m_samplers, m_vertex_attribute_mappings, m_fragment_outputs);
        p->add(m_uniform_block);
        p->load_vs(vs_path);
        p->load_fs(fs_path);
        p->link();
        m_uniform_block->map_program(p);

        if (m_shader_monitor)
        {
            m_shader_monitor->add(vs_path, p);
            m_shader_monitor->add(fs_path, p);
        }

        return p;
    }

    stringstream ss;
    ss << "gui_renderer::load_program(" << name << ") failed";
    if (!any_found)
    {
        ss << "\nNo source files found.\n";
        ss << "Did you run export-files build target?\n";
        ss << "Current working directory is: " << get_current_working_directory() << "\n";
        for (auto i = m_shader_versions.cbegin(); i != m_shader_versions.cend(); ++i)
        {
            string vs_path = m_shader_path + path + ".vs" + i->first + ".txt";
            string fs_path = m_shader_path + path + ".fs" + i->first + ".txt";

            ss << "\t" << vs_path << "\n";
            ss << "\t" << fs_path << "\n";
        }
    }

    log_error("%s", ss.str().c_str());
    throw runtime_error(ss.str());
}

void gui_renderer::initialize_service()
{
    slog_trace("gui_renderer::initialize_service()");

    m_blend_alpha.set_enabled(true);
    m_blend_alpha.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
    m_blend_alpha.rgb().set_source_factor(gl::blending_factor_src::src_alpha);
    m_blend_alpha.rgb().set_destination_factor(gl::blending_factor_dest::one_minus_src_alpha);
    m_blend_alpha.alpha().set_equation_mode(gl::blend_equation_mode::func_add);
    m_blend_alpha.alpha().set_source_factor(gl::blending_factor_src::src_alpha);
    m_blend_alpha.alpha().set_destination_factor(gl::blending_factor_dest::one_minus_src_alpha);

    m_blend_add.set_enabled(true);
    m_blend_add.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
    m_blend_add.rgb().set_source_factor(gl::blending_factor_src::one);
    m_blend_add.rgb().set_destination_factor(gl::blending_factor_dest::one);
    m_blend_add.alpha().set_equation_mode(gl::blend_equation_mode::func_add);
    m_blend_add.alpha().set_source_factor(gl::blending_factor_src::one);
    m_blend_add.alpha().set_destination_factor(gl::blending_factor_dest::one);

    m_vertex_attribute_mappings = make_shared<renderstack::graphics::vertex_attribute_mappings>();
    m_vertex_attribute_mappings->add("a_position", vertex_attribute_usage::position, 0, 0);
    m_vertex_attribute_mappings->add("a_normal", vertex_attribute_usage::normal, 0, 1);
    m_vertex_attribute_mappings->add("a_normal_flat", vertex_attribute_usage::normal, 1, 2);
    m_vertex_attribute_mappings->add("a_normal_smooth", vertex_attribute_usage::normal, 2, 3);
    m_vertex_attribute_mappings->add("a_color", vertex_attribute_usage::color, 0, 4);
    m_vertex_attribute_mappings->add("a_texcoord", vertex_attribute_usage::tex_coord, 1, 5);
    m_vertex_attribute_mappings->add("a_position_texcoord",
                                     static_cast<vertex_attribute_usage::value>(
                                         vertex_attribute_usage::position | vertex_attribute_usage::tex_coord),
                                     0,
                                     0);

    m_vertex_format = make_shared<renderstack::graphics::vertex_format>();
    m_vertex_format->make_attribute(
        static_cast<vertex_attribute_usage::value>(
            vertex_attribute_usage::position | vertex_attribute_usage::tex_coord),
        gl::vertex_attrib_pointer_type::float_,
        gl::vertex_attrib_pointer_type::float_,
        0,
        4,
        false);

    auto &r = *m_renderer;

    m_vertex_stream = make_shared<renderstack::graphics::vertex_stream>();
    auto va         = m_vertex_stream->vertex_array();
    auto old_va     = r.set_vertex_array(va);

    m_vertex_buffer = make_shared<renderstack::graphics::buffer>(
        buffer_target::array_buffer,
        128 * 1024,
        4 * sizeof(float),
        gl::buffer_usage_hint::dynamic_draw);
    m_vertex_buffer->allocate_storage(*m_renderer);

    m_index_buffer = make_shared<renderstack::graphics::buffer>(
        buffer_target::element_array_buffer,
        512 * 1024,
        sizeof(unsigned short),
        gl::buffer_usage_hint::dynamic_draw);
    m_index_buffer->allocate_storage(*m_renderer);

    m_vertex_attribute_mappings->add_to_vertex_stream(m_vertex_stream, m_vertex_buffer, m_vertex_format);
    r.setup_attribute_pointers(m_vertex_stream, 0);
    va->set_index_buffer(m_index_buffer);

    m_fragment_outputs = make_shared<fragment_outputs>();
    m_fragment_outputs->add("out_color", gl::fragment_output_type::float_vec4, 0);

    // gui_renderer does not use uniform blocks / buffers.
    // constructor without block index creates uniforms to the default block
    m_uniform_block            = make_shared<renderstack::graphics::uniform_block>("gui");
    m_uniforms.clip_from_model = m_uniform_block->add_mat4("clip_from_model")->access();
    m_uniforms.color_add       = m_uniform_block->add_vec4("color_add")->access();
    m_uniforms.color_scale     = m_uniform_block->add_vec4("color_scale")->access();
    m_uniforms.hsv_matrix      = m_uniform_block->add_mat4("hsv_matrix")->access();
    m_uniforms.t               = m_uniform_block->add_float("t")->access();
    m_uniform_block->seal();

    m_shader_path = "res/shaders/";

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
    m_shader_versions.push_back(make_pair("4", 300));
#else
    if (
        (renderstack::graphics::configuration::shader_model_version >= 5) &&
        (renderstack::graphics::configuration::glsl_version >= 400))
        m_shader_versions.push_back(make_pair("5", 400));

    if (
        (renderstack::graphics::configuration::shader_model_version >= 4) &&
        (renderstack::graphics::configuration::glsl_version >= 150))
        m_shader_versions.push_back(make_pair("4", 150));

    m_shader_versions.push_back(make_pair("0", 120));
#endif

    auto nearest_sampler = make_shared<sampler>();
    m_samplers           = make_shared<samplers>();
    m_samplers->add("font_texture", gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(0);
    m_samplers->add("background_texture", gl::active_uniform_type::sampler_2d, nearest_sampler)->set_texture_unit_index(1);

    try
    {
        m_ninepatch_program = load_program("ninepatch", "gui");
        m_slider_program    = load_program("slider", "gui_slider");
        m_font_program      = load_program("font", "gui_font");
        m_hsv_program       = load_program("hsv", "gui_hsv");
    }
    catch (runtime_error const &e)
    {
        log_error("gui_renderer() shaders are broken: %s", e.what());
        throw;
    }
    catch (...)
    {
        log_error("shaders are broken");
        throw;
    }

    m_font = make_shared<font>(r, "res/fonts/Ubuntu-R.ttf", 11, 1.0f);
    //m_font = make_shared<font>(r, "res/fonts/SourceCodePro-Semibold.ttf", 8, 1.0f);
    //m_font = make_shared<font>(r, "res/fonts/SourceCodePro-Bold.ttf", 9, 1.0f);

    m_button_ninepatch_style = make_shared<ninepatch_style>(
        r,
        "res/images/button_released.png",
        m_ninepatch_program,
        1 // texture unit
    );
    m_menulist_ninepatch_style = make_shared<ninepatch_style>(
        r,
        "res/images/shadow.png",
        m_ninepatch_program,
        1 // texture unit
    );
    m_slider_ninepatch_style = make_shared<ninepatch_style>(
        r,
        "res/images/button_released.png",
        m_slider_program,
        1 // texture unit
    );

    vec2 zero(0.0f, 0.0f);
    vec2 button_padding(26.0f, 6.0f);
    vec2 menulist_padding(16.0f, 16.0f);
    vec2 inner_padding(6.0f, 6.0f);

    m_default_style = make_shared<style>(
        vec2(6.0f, 6.0f),
        vec2(6.0f, 6.0f));
    m_null_padding_style = make_shared<style>(
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f));

    // With MSVC this needs _VARIADIC_MAX defined to 6 or more
    m_button_style = make_shared<renderstack::ui::style>(
        button_padding,
        inner_padding,
        m_font,
        m_button_ninepatch_style,
        m_font_program);
    m_slider_style = make_shared<style>(
        button_padding,
        inner_padding,
        m_font,
        m_slider_ninepatch_style,
        m_font_program);
    m_choice_style = make_shared<style>(
        zero,
        zero,
        m_font,
        m_button_ninepatch_style,
        m_font_program);
    m_menulist_style = make_shared<style>(
        menulist_padding,
        inner_padding,
        nullptr,
        m_menulist_ninepatch_style,
        nullptr);
    m_colorpicker_style = make_shared<style>(
        menulist_padding,
        inner_padding,
        nullptr,
        m_menulist_ninepatch_style,
        m_hsv_program);

    if (renderstack::graphics::configuration::use_vertex_array_object)
        r.set_vertex_array(old_va);
}

void gui_renderer::blend_alpha()
{
    m_renderer->track().blend().execute(&m_blend_alpha);
}

void gui_renderer::blend_add()
{
    m_renderer->track().blend().execute(&m_blend_add);
}

void gui_renderer::blend_disable()
{
    m_renderer->track().blend().execute(&m_blend_disabled);
}

void gui_renderer::set_vertex_buffer()
{
    slog_trace("gui_renderer::set_vertex_buffer()");

    m_renderer->set_buffer(buffer_target::array_buffer, m_vertex_buffer);
}

void gui_renderer::set_index_buffer()
{
    slog_trace("gui_renderer::set_index_buffer()");

    // Make sure our VAO is bound, then make sure our IBO is bound
    auto va = m_vertex_stream->vertex_array();
    (void)m_renderer->set_vertex_array(va);
    va->set_index_buffer(m_index_buffer);
}

void gui_renderer::prepare()
{
    slog_trace("gui_renderer::prepare()");

    // Default state
    auto &          r = *m_renderer;
    state_trackers &t = r.track();
    //m_renderer->track.execute(&m_gui_render_states);
    t.color_mask().execute(&m_gui_render_states.color_mask);
    t.blend().execute(&m_gui_render_states.blend);
    t.depth().execute(&m_gui_render_states.depth);
    t.face_cull().execute(&m_gui_render_states.face_cull);
    t.stencil().execute(&m_gui_render_states.stencil);

    blend_alpha();

    // We bind these for both render and edit uses

    m_renderer->use_vertex_stream(m_vertex_stream);
    m_renderer->set_buffer(buffer_target::array_buffer, m_vertex_buffer);
    m_renderer->vertex_array()->set_index_buffer(m_index_buffer);
}
void gui_renderer::on_resize(int width, int height)
{
    m_ortho = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
}
void gui_renderer::set_program(shared_ptr<program> value)
{
    slog_trace("gui_renderer::set_program(%s)", value->name().c_str());

    assert(value);
    m_renderer->set_program(value);
    m_program = value;
}
void gui_renderer::set_texture(unsigned int unit, shared_ptr<class texture> texture)
{
    slog_trace("gui_renderer::set_texture(unit = %u, texture = %u)", unit, texture.get());

    (void)m_renderer->set_texture(unit, texture);
}
void gui_renderer::set_transform(mat4 const &value)
{
    assert(m_program);
    gl::uniform_matrix_4fv(
        m_program->uniform_at(m_uniforms.clip_from_model),
        1,
        GL_FALSE,
        value_ptr(value));
}
void gui_renderer::set_color_add(vec4 const &value)
{
    assert(m_program);
    gl::uniform_4fv(
        m_program->uniform_at(m_uniforms.color_add), 1, value_ptr(value));
}
void gui_renderer::set_color_scale(vec4 const &value)
{
    assert(m_program);
    gl::uniform_4fv(
        m_program->uniform_at(m_uniforms.color_scale), 1, value_ptr(value));
}
void gui_renderer::set_hsv_matrix(mat4 const &value)
{
    assert(m_program);
    gl::uniform_matrix_4fv(
        m_program->uniform_at(m_uniforms.hsv_matrix),
        1,
        GL_FALSE,
        value_ptr(value));
}
void gui_renderer::set_t(float value)
{
    assert(m_program);
    gl::uniform_1f(m_program->uniform_at(m_uniforms.t), value);
}

void gui_renderer::draw_elements_base_vertex(
    GLenum begin_mode, GLsizei count, GLenum index_type, const GLvoid *indices,
    GLint base_vertex)
{
    // TODO Can we do this in caller somewhere?
    auto va = m_vertex_stream->vertex_array();
    (void)m_renderer->set_vertex_array(va);
    (void)va->set_index_buffer(m_index_buffer);

    m_renderer->draw_elements_base_vertex(m_vertex_stream, begin_mode, count, index_type, indices, base_vertex);
}

} // namespace ui
} // namespace renderstack
