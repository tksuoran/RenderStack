#include "renderers/debug_renderer.hpp"
#include "main/log.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/viewport.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include <cstdarg>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>

using namespace renderstack::graphics;
using namespace renderstack::scene;
using namespace renderstack::ui;
using namespace gl;
using namespace std;

using glm::mat4;
using glm::vec3;
using glm::vec4;

Debug_renderer::Debug_renderer()
    : service("Debug_renderer")
    , m_frame_duration_graph_size(500)
{
}

Debug_renderer::~Debug_renderer()
{
}

void Debug_renderer::connect(shared_ptr<Renderer>     renderer,
                             shared_ptr<Gui_renderer> gui_renderer,
                             shared_ptr<Programs>     programs,
                             shared_ptr<Application>  application)
{
    base_connect(renderer, programs, nullptr);

    m_gui_renderer = gui_renderer;
    m_application  = application;

    initialization_depends_on(renderer);
    initialization_depends_on(gui_renderer);
    initialization_depends_on(programs);
}

void Debug_renderer::initialize_service()
{
    base_initialize_service();

    auto &gr = *(m_gui_renderer.get());
    auto &r = renderer();

#if defined(RENDERSTACK_USE_FREETYPE)
    m_font = make_shared<Font>(r, "res/fonts/Ubuntu-R.ttf", 10);

    auto font = m_font.get();
    assert(font != nullptr);
    m_text_buffer = make_shared<Text_buffer>(gr, *font);

    m_font_render_states.blend.enabled = true;
    m_font_render_states.blend.rgb.equation_mode = gl::blend_equation_mode::func_add;
    m_font_render_states.blend.rgb.source_factor = gl::blending_factor_src::one;
    m_font_render_states.blend.rgb.destination_factor = gl::blending_factor_dest::one_minus_src_alpha;
#endif

    m_render_states.depth.enabled = false;
    m_render_states.face_cull.enabled = true;

    auto &vf = m_vertex_format;
    vf.make_attribute(Vertex_attribute::Usage::position,
                      gl::vertex_attrib_pointer_type::float_,
                      gl::vertex_attrib_pointer_type::float_,
                      0,
                      3);
    vf.make_attribute(Vertex_attribute::Usage::color,
                      gl::vertex_attrib_pointer_type::float_, // TODO unsigned byte would be sufficient
                      gl::vertex_attrib_pointer_type::float_,
                      0,
                      4);

    m_capacity_lines = 1000;
    m_vertex_offset  = 0;
    m_index_offset   = 0;

    m_vertex_buffer = make_shared<Buffer>(Buffer::Target::array_buffer,
                                          m_capacity_lines * 2,
                                          vf.stride(),
                                          gl::buffer_usage_hint::stream_draw);
    m_vertex_buffer->allocate_storage(r);

    m_index_buffer = make_shared<Buffer>(Buffer::Target::element_array_buffer,
                                         m_capacity_lines * 2,
                                         sizeof(GLushort),
                                         gl::buffer_usage_hint::stream_draw);
    m_index_buffer->allocate_storage(r);

    programs()->attribute_mappings.add_to_vertex_stream(m_vertex_stream, m_vertex_buffer.get(), m_vertex_format);

    auto va = m_vertex_stream.vertex_array();
    r.set_vertex_array(va);
    r.setup_attribute_pointers(m_vertex_stream, 0);
    va->set_index_buffer(m_index_buffer.get());

    r.reset_vertex_array();
}

void Debug_renderer::clear_text_lines()
{
    m_debug_lines.clear();
    m_debug_print_ats.clear();
}

void Debug_renderer::record_frame_duration(float frame_duration)
{
    if (m_frame_durations.size() == m_frame_duration_graph_size)
    {
        m_frame_durations.pop_front();
    }

    m_frame_durations.push_back(frame_duration);
}

void Debug_renderer::printf(int x, int y, const char *format, ...)
{
    char    buffer[1000]; // TODO
    va_list args;
    va_start(args, format);
    ::memset(buffer, 0, 1000);
    vsnprintf(buffer, 999, format, args);
    va_end(args);

    print_at p;
    p.x    = x;
    p.y    = y;
    p.text = buffer;
    m_debug_print_ats.push_back(p);
}

void Debug_renderer::printf(const char *format, ...)
{
    char              buffer[1000]; // TODO
    string::size_type line_begin;
    string::size_type len;
    string::size_type pos;
    va_list           args;
    va_start(args, format);
    ::memset(buffer, 0, 1000);
    vsnprintf(buffer, 999, format, args);
    va_end(args);

    line_begin = 0;
    len        = 0;
    for (pos = 0; (buffer[pos] != 0) && (pos < 1000); ++pos)
    {
        if (buffer[pos] == '\n')
        {
            if (len > 0)
            {
                string line(&buffer[line_begin], len);
                m_debug_lines.push_back(line);
            }
            line_begin = pos + 1;
            len        = 0;
            continue;
        }
        ++len;
    }

    if (len > 0)
    {
        m_debug_lines.push_back(string(&buffer[line_begin], len));
    }
}

void Debug_renderer::render_text_lines(Viewport vp)
{
#if defined(RENDERSTACK_USE_FREETYPE)
    float w = static_cast<float>(vp.width);
    float h = static_cast<float>(vp.height);

    m_text_buffer->begin_print();
    for (size_t i = 0; i < m_debug_lines.size(); ++i)
    {
        m_text_buffer->print(m_debug_lines[i], 0, static_cast<int>(h - (i + 1) * m_font->line_height()));
    }

    for (size_t i = 0; i < m_debug_print_ats.size(); ++i)
    {
        m_text_buffer->print(m_debug_print_ats[i].text, m_debug_print_ats[i].x, m_debug_print_ats[i].y);
    }

    int chars_printed = m_text_buffer->end_print();
    if (chars_printed == 0)
    {
        return;
    }

    auto &r = renderer();
    {
        auto &t = r.track();
        t.execute(&m_font_render_states);
    }

    auto p = programs()->debug_font.get();
    r.set_program(p);

    gl::viewport(0, 0, (GLsizei)w, (GLsizei)h);
    mat4 ortho = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
    gl::scissor(0, 0, (GLsizei)w, (GLsizei)h);

    vec4 white(1.0f, 1.0f, 1.0f, 0.66f); // gamma in 4th component

    begin_edit();
    set_ortho(vp);
    end_edit();

    int clip_from_world = p->uniform_at(programs()->debug_block_access.clip_from_world);
    //int color           = p->uniform_at(programs()->debug_block_access.color);

    gl::uniform_matrix_4fv(clip_from_world, 1, GL_FALSE, value_ptr(ortho));
    //gl::uniform_4fv(color, 1, value_ptr(white));

    //int texture_ui = p->uniform("font_texture")->index();
    //gl::uniform_1i(texture_ui, 0);

    {
        auto t = m_text_buffer->font().texture();
        t->set_min_filter(texture_min_filter::nearest);
        t->set_mag_filter(texture_mag_filter::nearest);
        (void)r.set_texture(0, t);
        t->apply(r, 0);
    }

    m_text_buffer->render();
#endif
}

void Debug_renderer::set_camera(Camera *camera)
{
    m_camera = camera;
}

void Debug_renderer::set_model(mat4 world_from_model)
{
    assert(m_in_edit);

    set_clip_from_model(m_camera->clip_from_world.matrix() * world_from_model);
}

void Debug_renderer::set_clip_from_model(mat4 clip_from_model)
{
    assert(m_in_edit);

    if (m_index_offset > m_current_draw.first_index)
    {
        m_current_draw.count = m_index_offset - m_current_draw.first_index;
        m_draws.push_back(m_current_draw);
        ++m_model_index;
    }

    //m_current_draw.clip_from_model = clip_from_model;
    m_current_draw.first_index = m_index_offset;

    ::memcpy(m_models_start +
                 m_model_index * programs()->models_block.size_bytes() +
                 programs()->models_block_access.clip_from_model,
             value_ptr(clip_from_model), 16 * sizeof(float));
}

void Debug_renderer::set_ortho(Viewport vp)
{
    mat4 ortho = glm::ortho(static_cast<float>(vp.x), static_cast<float>(vp.width),
                            static_cast<float>(vp.y), static_cast<float>(vp.height));

    set_clip_from_model(ortho);
}

void Debug_renderer::add_frame_duration_graph(Viewport vp)
{
    begin_edit();

    set_ortho(vp);

    float scale                = 1000.0f * 4.0f; // 1 ms = 10 pixels
    float first_frame_duration = m_frame_durations.front();
    vec3  previous(0.0f, scale * first_frame_duration, 0.0f);
    float frame = 0.0f;

    /// debug cross
    // set_color(vec4(0.0f, 0.0f, 1.0f, 1.0f));
    // add_line(vec3(0.0f, 0.0f, 0.0f),
    //          vec3(vp.width, vp.height, 0.0f));
    // add_line(vec3(vp.width, 0.0f, 0.0f),
    //          vec3(0.0f, vp.height, 0.0f));

    // 32 ms
    float left       = 0.0f;
    float right      = static_cast<float>(m_frame_duration_graph_size);
    float sixty_fps  = scale / 60.0f;
    float thirty_fps = scale / 30.0f;

    set_color(vec4(0.0f, 1.0f, 0.0f, 1.0f));
    add_line(vec3(left, sixty_fps, 0.0f),
             vec3(right, sixty_fps, 0.0f));

    set_color(vec4(0.0f, 0.0f, 1.0f, 1.0f));
    add_line(vec3(left, thirty_fps, 0.0f),
             vec3(right, thirty_fps, 0.0f));

    set_color(vec4(1.0f, 0.5f, 0.0f, 1.0f));
    for (auto frame_duration : m_frame_durations)
    {
        vec3 current(frame, scale * frame_duration, 0.0f);
        if (frame > 0.0f)
        {
            add_line(previous, current);
        }

        previous = current;
        frame += 1.0f;
    }
    end_edit();
}

void Debug_renderer::render()
{
    if (m_index_offset > m_current_draw.first_index)
    {
        m_current_draw.count = m_index_offset - m_current_draw.first_index;
        m_draws.push_back(m_current_draw);
    }

    auto &r = renderer();
    auto &t = r.track();
    auto  p = programs()->debug_line.get();
    assert(p != nullptr);

    t.execute(&m_render_states);
    r.set_program(p);
    r.use_vertex_stream(m_vertex_stream);

    size_t model_index = 0;
    for (auto &draw : m_draws)
    {
        if (p->use_uniform_buffers())
        {
            bind_model(model_index);
        }
        else
        {
            throw runtime_error("not implemented");
            /*gl::uniform_matrix_4fv(
            p->uniform_at(programs()->model_block_access.clip_from_model),
            1,
            GL_FALSE,
            value_ptr(draw.clip_from_model)
            );*/
        }

        gl::draw_elements(gl::begin_mode::lines,
                          draw.count,
                          gl::draw_elements_type::unsigned_short,
                          reinterpret_cast<GLvoid *>(draw.first_index * 2));
        ++model_index;
    }

    m_draws.clear();
}

void Debug_renderer::set_color(vec4 color)
{
    m_color = color;
}

void Debug_renderer::begin_edit_vbo()
{
    m_vertex_offset = 0;
    renderer().set_vertex_array(m_vertex_stream.vertex_array());
    renderer().set_buffer(Buffer::Target::array_buffer, m_vertex_buffer.get());
    m_vertex_ptr = reinterpret_cast<float *>(
        m_vertex_buffer->map(renderer(),
                             0,
                             m_capacity_lines * 2,
                             static_cast<gl::buffer_access_mask::value>(
                                 gl::buffer_access_mask::map_flush_explicit_bit |
                                 gl::buffer_access_mask::map_invalidate_range_bit |
                                 gl::buffer_access_mask::map_write_bit)));

    m_vertex_ptr_start = m_vertex_ptr;
}

void Debug_renderer::begin_edit_ibo()
{
    m_index_offset = 0;
    renderer().set_vertex_array(m_vertex_stream.vertex_array());
    m_index_ptr = reinterpret_cast<uint16_t *>(
        m_index_buffer->map(renderer(),
                            0,
                            m_capacity_lines * 2,
                            static_cast<gl::buffer_access_mask::value>(
                                gl::buffer_access_mask::map_flush_explicit_bit |
                                gl::buffer_access_mask::map_invalidate_range_bit |
                                gl::buffer_access_mask::map_write_bit)));

    m_index_ptr_start = m_index_ptr;
}

void Debug_renderer::begin_edit()
{
    begin_edit_vbo();
    begin_edit_ibo();
    m_in_edit = true;

    m_current_draw.first_index = 0;
    m_current_draw.count       = 0;
    m_model_index              = 0;

    if (configuration::can_use.uniform_buffer_object)
    {
        m_models_start = Base_renderer::begin_edit(uniform_buffer_usage::models, 0);
    }
}

void Debug_renderer::end_edit()
{
    auto &r = renderer();
    m_vertex_buffer->flush_and_unmap(r, m_vertex_offset);
    m_index_buffer->flush_and_unmap(r, m_index_offset);

    if (configuration::can_use.uniform_buffer_object)
    {
        Base_renderer::end_edit(uniform_buffer_usage::models);
        m_models_start = nullptr;
    }

    m_in_edit    = false;
    m_vertex_ptr = nullptr;
    m_index_ptr  = nullptr;
}

uint16_t Debug_renderer::add_point(vec3 p)
{
    *m_vertex_ptr++ = p.x;
    *m_vertex_ptr++ = p.y;
    *m_vertex_ptr++ = p.z;
    *m_vertex_ptr++ = m_color.x;
    *m_vertex_ptr++ = m_color.y;
    *m_vertex_ptr++ = m_color.z;
    *m_vertex_ptr++ = m_color.w;
    return m_vertex_offset++;
}

void Debug_renderer::add_line(vec3 start, vec3 end)
{
    m_index_ptr[m_index_offset++] = add_point(start);
    m_index_ptr[m_index_offset++] = add_point(end);
}

void Debug_renderer::add_box(vec3 min_, vec3 max_)
{
    uint16_t a = add_point(vec3(min_.x, min_.y, min_.z)); //    h      g
    uint16_t b = add_point(vec3(max_.x, min_.y, min_.z)); //   e      f
    uint16_t c = add_point(vec3(max_.x, min_.y, max_.z)); //
    uint16_t d = add_point(vec3(min_.x, min_.y, max_.z)); //    d      c
    uint16_t e = add_point(vec3(min_.x, max_.y, min_.z)); //   a      b
    uint16_t f = add_point(vec3(max_.x, max_.y, min_.z));
    uint16_t g = add_point(vec3(max_.x, max_.y, max_.z));
    uint16_t h = add_point(vec3(min_.x, max_.y, max_.z));

    m_index_ptr[m_index_offset++] = a; // bottom abcd
    m_index_ptr[m_index_offset++] = b;
    m_index_ptr[m_index_offset++] = b;
    m_index_ptr[m_index_offset++] = c;
    m_index_ptr[m_index_offset++] = c;
    m_index_ptr[m_index_offset++] = d;
    m_index_ptr[m_index_offset++] = d;
    m_index_ptr[m_index_offset++] = a;

    m_index_ptr[m_index_offset++] = e; // top efgh
    m_index_ptr[m_index_offset++] = f;
    m_index_ptr[m_index_offset++] = f;
    m_index_ptr[m_index_offset++] = g;
    m_index_ptr[m_index_offset++] = g;
    m_index_ptr[m_index_offset++] = h;
    m_index_ptr[m_index_offset++] = h;
    m_index_ptr[m_index_offset++] = e;

    m_index_ptr[m_index_offset++] = a; // connect bottom and top
    m_index_ptr[m_index_offset++] = e;
    m_index_ptr[m_index_offset++] = b;
    m_index_ptr[m_index_offset++] = f;
    m_index_ptr[m_index_offset++] = c;
    m_index_ptr[m_index_offset++] = g;
    m_index_ptr[m_index_offset++] = d;
    m_index_ptr[m_index_offset++] = h;
}
