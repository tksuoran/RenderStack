#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_ui/button.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/menulist.hpp"
#include "renderstack_ui/text_buffer.hpp"

#include "main/application.hpp"
#include "main/game.hpp"
#include "main/log.hpp"
#include "main/menu.hpp"
#include "main/programs.hpp"
#include "main/textures.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define LOG_CATEGORY &log_menu

using namespace gl;
using namespace glm;
using namespace std;
using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace renderstack::ui;
using namespace renderstack::toolkit;

//#define DEBUG_FONT
#define RENDER_TEXT 1
#define RENDER_BACKGROUND 1
#define RENDER_GUI 1
//#define DEBUG_FONT 1

Menu::Menu()
    : service("Menu"), m_resize(true)
{
}

void Menu::connect(shared_ptr<Renderer>     renderer,
                   shared_ptr<Gui_renderer> gui_renderer,
                   shared_ptr<Programs>     programs,
                   shared_ptr<textures>     textures,
                   shared_ptr<Game>         Game,
                   shared_ptr<Application>  application)
{
    m_renderer     = renderer;
    m_gui_renderer = gui_renderer;
    m_programs     = programs;
    m_textures     = textures;
    m_game         = Game;
    m_application  = application;

    initialization_depends_on(renderer);
    initialization_depends_on(gui_renderer);
    initialization_depends_on(programs);
    initialization_depends_on(textures);
}

void Menu::disconnect()
{
    slog_trace("Menu::disconnect()");

    m_renderer.reset();
    m_gui_renderer.reset();
    m_application.reset();
    m_game.reset();
    m_programs.reset();
    m_textures.reset();
}

void Menu::initialize_service()
{
    assert(m_renderer);
    assert(m_gui_renderer);
    assert(m_programs);
    assert(m_textures);

    slog_trace("Menu::on_load()");

    auto &gr = *(m_gui_renderer.get());
    auto &r = *m_renderer;

    m_font        = std::make_shared<Font>(r, "res/fonts/Ubuntu-R.ttf", 68, 8.0f);
    m_text_buffer = std::make_shared<Text_buffer>(gr, *(m_font.get()));

    // Background
    m_mesh = std::make_shared<mesh>();
    m_mesh->allocate_vertex_buffer(gr.vertex_buffer(), 4);
    m_mesh->allocate_index_buffer(gr.index_buffer(), 6);

    // Write indices for one quad 
    {
        gr.set_index_buffer();
        unsigned short *start = static_cast<unsigned short *>(
            m_mesh->index_buffer()->map(
                r,
                m_mesh->first_index(),
                m_mesh->index_count(),
                (gl::buffer_access_mask::value)(
                    gl::buffer_access_mask::map_write_bit |
                    gl::buffer_access_mask::map_invalidate_range_bit)));
        unsigned short base_vertex =
            configuration::can_use.draw_elements_base_vertex
                ? 0
                : static_cast<unsigned short>(m_mesh->first_vertex());
        unsigned short *ptr = start;
        *ptr++              = 0 + base_vertex;
        *ptr++              = 2 + base_vertex;
        *ptr++              = 3 + base_vertex;
        *ptr++              = 0 + base_vertex;
        *ptr++              = 1 + base_vertex;
        *ptr++              = 2 + base_vertex;
        m_mesh->index_buffer()->unmap(r);
    }

    // GUI
    auto &bs = gr.button_style();
    auto &ms = gr.menulist_style();

    {
        float w = static_cast<float>(m_application->width());
        float h = static_cast<float>(m_application->height());
        Rectangle size(w, h);

        m_root_layer = std::make_shared<Layer>(gr, size);
        m_root_layer->name = "m_root_layer";
    }

    m_menulist = std::make_shared<Menulist>(gr, ms, Area::Orientation::vertical);
    m_menulist->offset_free_size_relative = vec2(0.50f, 0.25f);
    m_menulist->offset_self_size_relative = vec2(-0.50f, -0.50f);
    m_menulist->set_child_layout_style(Area::Flow_mode::extend_horizontal);

    m_quit = std::make_shared<Button>(gr, "Quit", bs);
    m_quit->set_sink(this);
    m_menulist->add(m_quit.get());

    if (m_game)
    {
        m_sandbox_button = std::make_shared<Button>(gr, "Sandbox", bs);
        m_sandbox_button->set_sink(this);
        m_menulist->add(m_sandbox_button.get());
    }

    m_root_layer->add(m_menulist.get());

    if (configuration::can_use.uniform_buffer_object)
    {
        intptr_t size = 0;
        size += m_programs->models_block.size_bytes();
        size += m_programs->materials_block.size_bytes();
        size += m_programs->camera_block.size_bytes();

        m_uniform_buffer = make_shared<Buffer>(Buffer::Target::uniform_buffer, size, 1, gl::buffer_usage_hint::stream_draw);
        m_uniform_buffer->allocate_storage(r);
    }
}

void Menu::action(Action_source* source)
{
    if (source == dynamic_cast<Action_source*>(m_sandbox_button.get()))
    {
        if (m_game)
        {
            m_application->set_screen(m_game);
        }
    }

    if (source == dynamic_cast<Action_source*>(m_quit.get()))
    {
        m_application->close();
    }
}

void Menu::on_resize(int width, int height)
{
    static_cast<void>(width);
    static_cast<void>(height);
    m_resize = true;
    slog_trace("Menu::on_resize()");
}

void Menu::resize(float w, float h)
{
    auto &gr = *(m_gui_renderer.get());
    auto &r  = *(m_renderer.get());

    // Background
    {
        // Write corner vertices for one quad
        gr.set_vertex_buffer();
        float *ptr = static_cast<float *>(
            m_mesh->vertex_buffer()->map(
                r,
                m_mesh->first_vertex(),
                m_mesh->vertex_count(),
                (gl::buffer_access_mask::value)(
                    gl::buffer_access_mask::map_write_bit |
                    gl::buffer_access_mask::map_invalidate_range_bit)));

#if defined(DEBUG_FONT)
        float max_x = static_cast<float>(m_font->texture()->width());
        float max_y = static_cast<float>(m_font->texture()->height());
#else
        float max_x = w;
        float max_y = h;
#endif

        *ptr++ = 0.0f;
        *ptr++ = 0.0f;
        *ptr++ = 0.0f;
        *ptr++ = 0.0f;
        *ptr++ = max_x;
        *ptr++ = 0.0f;
        *ptr++ = 1.0f;
        *ptr++ = 0.0f;
        *ptr++ = max_x;
        *ptr++ = max_y;
        *ptr++ = 1.0f;
        *ptr++ = 1.0f;
        *ptr++ = 0.0f;
        *ptr++ = max_y;
        *ptr++ = 0.0f;
        *ptr++ = 1.0f;

        m_mesh->vertex_buffer()->unmap(r);
    }

    if (m_text_buffer)
    {
        string title = "Hello, World!";
        //string title = "The quick brown fox jumps over the lazy dog. 1234567890";
        //string title = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG. 1234567890";
        //string title = "Volumetric Elements";
        float x = floor(w / 2.0f);
        float y = ceil(3.0f * h / 4.0f);

        m_text_buffer->begin_print();
        m_text_buffer->print_center(title, x, y);
        m_text_buffer->end_print();
    }

    m_resize = false;
}

void Menu::on_focus(bool has_focus)
{
    (void)has_focus;
}

void Menu::update()
{
    slog_trace("Menu::update()");

    m_programs->update_fixed_step();
    render();
}

void Menu::on_key(int key, int scancode, int action, int mods)
{
    (void)key;
    (void)scancode;
    (void)action;
    (void)mods;
}

void Menu::on_mouse_moved(double x, double y)
{
    (void)x;
    (void)y;
}

void Menu::on_mouse_button(int button, int action, int mods)
{
    (void)button;
    (void)action;
    (void)mods;
}

void Menu::on_scroll(double x, double y)
{
    (void)x;
    (void)y;
}

void Menu::on_3d_mouse(long tx, long ty, long tz, long rx, long ry, long rz, long period)
{
    (void)tx;
    (void)ty;
    (void)tz;
    (void)rx;
    (void)ry;
    (void)rz;
    (void)period;
}

void Menu::on_enter()
{
    slog_trace("Menu::on_enter()");

    m_gui_renderer->prepare(); // disable cull face, depth test & mask, blend
    gl::clear_color(0.3f, 0.1f, 0.2f, 0.0f);
    gl::disable(gl::enable_cap::scissor_test); // TODO stencil_state
    on_resize(m_application->width(), m_application->height());
}

void Menu::on_exit()
{
    slog_trace("Menu::on_exit()");
}

void Menu::render()
{
    slog_trace("Menu::render()");

    assert(m_application);
    assert(m_programs);

    int   iw = m_application->width();
    int   ih = m_application->height();
    float w  = static_cast<float>(iw);
    float h  = static_cast<float>(ih);

    auto &gr = *(m_gui_renderer.get());
    auto &r  = *(m_renderer.get());

    if (m_resize)
    {
        gr.on_resize(iw, ih);
        resize(w, h);
        m_root_layer->set_layer_size(w, h);
        m_root_layer->update();
    }

    //  viewport
    gl::viewport(0, 0, (GLsizei)w, (GLsizei)h);

    mat4 ortho = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
    vec4 white(1.0f, 1.0f, 1.0f, 1.0f);

    gl::clear_color(0.5f, 0.0f, 0.0f, 1.0f);
    gl::clear(clear_buffer_mask::color_buffer_bit | clear_buffer_mask::depth_buffer_bit | clear_buffer_mask::depth_buffer_bit);

    if (configuration::can_use.uniform_buffer_object)
    {
        intptr_t models_offset    = 0;
        intptr_t materials_offset = m_programs->models_block.size_bytes();
        intptr_t camera_offset    = materials_offset + m_programs->materials_block.size_bytes();
        intptr_t size             = camera_offset + m_programs->camera_block.size_bytes();

        // Update and bind uniform buffer
        r.set_buffer(Buffer::Target::uniform_buffer, m_uniform_buffer.get());
        unsigned char *ptr = (unsigned char *)m_uniform_buffer->map(
            r,
            0,
            size,
            static_cast<gl::buffer_access_mask::value>(
                gl::buffer_access_mask::map_invalidate_buffer_bit |
                gl::buffer_access_mask::map_write_bit));

        // Update models
        memcpy(ptr + models_offset + m_programs->models_block_access.clip_from_model,
               value_ptr(ortho),
               16 * sizeof(float));

        // Update camera
        memcpy(ptr + camera_offset + m_programs->camera_block_access.clip_from_world,
               value_ptr(ortho),
               16 * sizeof(float));

        // Update materials
        memcpy(ptr + materials_offset + m_programs->materials_block_access.color,
               value_ptr(white),
               4 * sizeof(float));
        m_uniform_buffer->unmap(r);

        // bind models
        m_uniform_buffer->bind_range(m_programs->models_block.binding_point(),
                                     models_offset,
                                     m_programs->models_block.size_bytes());

        // bind materials
        m_uniform_buffer->bind_range(m_programs->materials_block.binding_point(),
                                     materials_offset,
                                     m_programs->materials_block.size_bytes());

        // bind camera
        m_uniform_buffer->bind_range(m_programs->camera_block.binding_point(),
                                     camera_offset,
                                     m_programs->materials_block.size_bytes());
    }

    //  Background
    if (m_textures)
    {
#if defined(DEBUG_FONT)
        auto t = m_font->texture();
        (void)m_renderer->set_texture(1, t);
        t->set_min_filter(texture_min_filter::nearest);
        t->set_mag_filter(texture_mag_filter::nearest);
        t->set_wrap(0, gl::texture_wrap_mode::clamp_to_edge);
        t->set_wrap(1, gl::texture_wrap_mode::clamp_to_edge);
        t->apply(*m_renderer, 1);
#else
        auto  t     = m_textures->background_texture;
        (void)r.set_texture(1, t.get());
        t->set_min_filter(texture_min_filter::linear);
        t->set_mag_filter(texture_mag_filter::linear);
        t->set_wrap(0, gl::texture_wrap_mode::clamp_to_edge);
        t->set_wrap(1, gl::texture_wrap_mode::clamp_to_edge);
        t->apply(r, 1);
#endif
        assert(m_mesh);

        gl::begin_mode::value         begin_mode    = gl::begin_mode::triangles;
        GLsizei                       count         = 6;
        gl::draw_elements_type::value index_type    = gl::draw_elements_type::unsigned_short;
        size_t                        first_index   = m_mesh->first_index();
        GLvoid *                      index_pointer = reinterpret_cast<GLvoid *>(first_index * sizeof(unsigned short));
        GLint                         base_vertex   = configuration::can_use.draw_elements_base_vertex
                                ? static_cast<GLint>(m_mesh->first_vertex())
                                : 0;

        auto p = m_programs->textured.get();
        assert(p);
        r.set_program(p);
        if (!configuration::can_use.uniform_buffer_object)
        {
            gl::uniform_matrix_4fv(p->uniform_at(m_programs->models_block_access.clip_from_model), 1, GL_FALSE, value_ptr(ortho));
            gl::uniform_4fv(p->uniform_at(m_programs->materials_block_access.color), 1, value_ptr(white));
        }

        gr.draw_elements_base_vertex(begin_mode, count, index_type, index_pointer, base_vertex);
    }

    // Label text
    if (m_text_buffer)
    {
        assert(m_font);

        m_gui_renderer->blend_alpha();

        auto t = m_font->texture();
        (void)r.set_texture(0, t);
        t->set_min_filter(texture_min_filter::nearest);
        t->set_mag_filter(texture_mag_filter::nearest);
        t->set_wrap(0, gl::texture_wrap_mode::clamp_to_edge);
        t->set_wrap(1, gl::texture_wrap_mode::clamp_to_edge);
        t->apply(r, 0);

        auto p = m_programs->font.get();
        assert(p != nullptr);
        r.set_program(p);
        if (!configuration::can_use.uniform_buffer_object)
        {
            gl::uniform_matrix_4fv(p->uniform_at(m_programs->models_block_access.clip_from_model), 1, GL_FALSE, value_ptr(ortho));
            gl::uniform_4fv(p->uniform_at(m_programs->materials_block_access.color), 1, value_ptr(white));
        }

        m_text_buffer->render();
        m_gui_renderer->blend_disable();
    }

    // GUI
    {
        ui_context c;
        double     x;
        double     y;
        m_application->get_mouse_pos(x, y);
        c.mouse.x          = static_cast<float>(x);
        c.mouse.y          = static_cast<float>(ih - 1 - y);
        c.mouse_buttons[0] = (m_application->get_mouse_button(0) != 0);
        c.mouse_buttons[1] = (m_application->get_mouse_button(1) != 0);
        c.mouse_buttons[2] = (m_application->get_mouse_button(2) != 0);
        m_gui_renderer->on_resize(iw, ih);
        m_gui_renderer->prepare();
        m_root_layer->draw(c);
    }

    m_application->swap_buffers();
}
