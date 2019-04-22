#include "main/game.hpp"
#include "main/application.hpp"
#include "main/log.hpp"
#include "main/menu.hpp"
#include "main/programs.hpp"
#include "main/textures.hpp"
#include "parsers/xml_polyhedron.hpp"
#include "renderstack_geometry/operation/catmull_clark.hpp"
#include "renderstack_geometry/operation/clone.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_mesh/build_info.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_ui/button.hpp"
#include "renderstack_ui/choice.hpp"
#include "renderstack_ui/color_picker.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/menulist.hpp"
#include "renderstack_ui/push_button.hpp"
#include "renderstack_ui/slider.hpp"
#include <cassert>

#if defined(RENDERSTACK_USE_GLFW)
#    include <GLFW/glfw3.h>
#endif
#if defined(RENDERSTACK_USE_GLWT)
#    include <GLWT/glwt.h>
#endif

// #define SHARED_BUFFERS 1

#define LOG_CATEGORY &log_game

#define USE_GUI 1

using namespace renderstack::graphics;
using namespace renderstack::toolkit;
using namespace renderstack::mesh;
using namespace renderstack::ui;
using namespace std;
using namespace gl;
using namespace glm;

game::game()
    : service("game"), m_update_time(0.0), m_frame_dt(0.0), m_min_frame_dt(0.0), m_max_frame_dt(0.0), m_simulation_time(0.0), m_paused(true), m_forward(false), m_deferred(true), m_debug_lights(true), m_max_lights(2), m_screen_active(false), m_mouse_down(false)
{
}

/*virtual*/ game::~game()
{
}

void game::connect(
    shared_ptr<renderstack::graphics::renderer>       renderer,
    shared_ptr<renderstack::graphics::shader_monitor> shader_monitor,
    shared_ptr<renderstack::ui::gui_renderer>         gui_renderer,
    shared_ptr<programs>                              programs_,
    shared_ptr<textures>                              textures_,
    shared_ptr<debug_renderer>                        debug_renderer_,
    shared_ptr<forward_renderer>                      forward_renderer_,
    shared_ptr<deferred_renderer>                     deferred_renderer_,
    shared_ptr<light_debug_renderer>                  light_debug_renderer_,
    shared_ptr<id_renderer>                           id_renderer_,
    shared_ptr<menu>                                  menu_,
    shared_ptr<application>                           application_,
    shared_ptr<scene_manager>                         scene_manager_)
{
    m_renderer             = renderer;
    m_shader_monitor       = shader_monitor;
    m_gui_renderer         = gui_renderer;
    m_application          = application_;
    m_menu                 = menu_;
    m_programs             = programs_;
    m_textures             = textures_;
    m_deferred_renderer    = deferred_renderer_;
    m_forward_renderer     = forward_renderer_;
    m_light_debug_renderer = light_debug_renderer_;
    m_id_renderer          = id_renderer_;
    m_debug_renderer       = debug_renderer_;
    m_scene_manager        = scene_manager_;

    initialization_depends_on(renderer);
    initialization_depends_on(gui_renderer);
    initialization_depends_on(programs_);
    initialization_depends_on(textures_);
    initialization_depends_on(scene_manager_);
}

void game::disconnect()
{
    slog_trace("game::disconnect()\n");

    m_application.reset();
    m_menu.reset();
    m_programs.reset();
    m_textures.reset();
}

void game::initialize_service()
{
    assert(m_renderer);
    assert(m_gui_renderer);
    //assert(m_programs);
    //assert(m_textures);

    slog_trace("game::on_load()\n");

    if (m_scene_manager)
    {
        auto camera = m_scene_manager->camera();
        m_controls.camera_controller.set_frame(camera->frame());
    }
    m_controls.home = vec3(0.0f, 1.7f, 10.0f);

    reset();

#if USE_GUI
    setup_gui();
#endif
}

void game::reset()
{
    slog_trace("game::reset()\n");

    m_controls.reset();

    m_frame_dt        = 0.0;
    m_update_time     = m_application->time();
    m_simulation_time = m_update_time;
}

void game::setup_gui()
{
#if !USE_GUI
    slog_trace("game::setup_gui() - USE_GUI not defined, skipping\n");
    return;
#else
    slog_trace("game::setup_gui()\n");
    assert(m_application);

    auto gr = m_gui_renderer;
    auto bs = gr->button_style();
    auto cs = gr->choice_style();
    auto ms = gr->menulist_style();
    auto ps = gr->colorpicker_style();
    auto ss = gr->slider_style();

    float     w = (float)m_application->width();
    float     h = (float)m_application->height();
    rectangle size(w, h);

    m_root_layer = smart_ptr_builder::create_shared_ptr<area>(new layer(gr, size));
    m_root_layer->set_name("m_root_layer");

    auto d = smart_ptr_builder::create_shared_ptr<area>(new menulist(gr, ms, orientation::vertical));
    d->set_offset_free_size_relative(vec2(1.0f, 1.0f));
    d->set_offset_self_size_relative(vec2(-1.0f, -1.0f));
    d->set_child_layout_style(area_layout_style::extend_horizontal);

    weak_ptr<action_sink> as = action_sink::shared_from_this();
    m_menu_button            = smart_ptr_builder::create_shared_ptr<action_source, area>(
        new button(gr, "Back to Menu", bs));
    m_menu_button->set_sink(as);
    d->add(m_menu_button);

    shared_ptr<renderstack::ui::choice> c = renderstack::toolkit::smart_ptr_builder::create_shared_ptr<
        renderstack::ui::action_source,
        renderstack::ui::choice,
        renderstack::ui::area>(
        new renderstack::ui::choice(gr, cs, bs, orientation::horizontal));
    c->add_choice_item("Foo", true);

    c->add_choice_item("Bar");
    d->add(c);

    //auto p = new color_picker(ps);
    //d->add(p);

    m_slider = smart_ptr_builder::create_shared_ptr<action_source, area>(
        new slider(gr, ss, "Slider", 0.0f, 80.0f));
    d->add(m_slider);

    m_root_layer->add(d);
#endif
}

void game::on_resize(int width, int height)
{
    slog_trace("game::on_resize(width = %d, height = %d)\n", width, height);

    if (!m_screen_active)
    {
        return;
    }

    m_viewport.set_x(0);
    m_viewport.set_y(0);
    m_viewport.set_width(width);
    m_viewport.set_height(height);

#if USE_GUI

    auto gr = m_gui_renderer;
    gr->on_resize(width, height);

    if (m_root_layer)
    {
        gr->prepare();
        float w = (float)width;  // (float)m_window->width();
        float h = (float)height; // (float)m_window->height();
        m_root_layer->set_layer_size(w, h);
        m_root_layer->update();
    }
#endif

    if (m_light_debug_renderer)
    {
        m_light_debug_renderer->base_resize(width, height);
    }

    if (m_deferred_renderer)
    {
        m_deferred_renderer->resize(width, height);
    }

    if (m_debug_renderer)
    {
        m_debug_renderer->base_resize(width, height);
    }

    if (m_forward_renderer)
    {
        m_forward_renderer->base_resize(width, height);
    }
}

void game::action(weak_ptr<action_source> source)
{
#if !USE_GUI
    slog_trace("game::action() - USE_GUI not defined, skipping\n");
    return;
#else
    slog_trace("game::action()\n");

    auto s = source.lock();
    if (s == m_menu_button)
    {
        if (m_menu)
        {
            m_application->set_screen(m_menu);
        }
        else
        {
            throw runtime_error("m_menu does not exist");
        }
    }

    /*if (s == m_slider)
   {
      m_slider->relative_value();
   }*/
#endif
}

void game::on_enter()
{
    slog_trace("game::on_enter()\n");

    assert(m_application);

    m_screen_active = true;
    on_resize(m_application->width(), m_application->height());

    lock_mouse(true);
}

void game::on_exit()
{
    gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
    m_screen_active = false;
    m_application->capture_mouse(false);
}

void controls::reset()
{
    slog_trace("controls::reset()\n");

    camera_controller.clear();
    camera_controller.set_elevation(0.0f);
    camera_controller.set_heading(0.0f);
    camera_controller.set_position(home);
    mouse_locked = true;
}

void game::lock_mouse(bool lock)
{
    m_controls.mouse_locked = lock;

#if 0
   if (m_controls.mouse_locked)
   {
      int mouse_x = m_application->width() / 2;
      int mouse_y = m_application->height() / 2;
      //::glfwSetMousePos(mouse_x, mouse_y);
      //::glfwDisable(GLFW_MOUSE_CURSOR);
      //::glfwGetMousePos(&m_controls.mouse_x, &m_controls.mouse_y);

      m_application->set_mouse_pos(mouse_x, mouse_y);
      //m_application->set_input_mode(GLFW_CURSOR_MODE, GLFW_CURSOR_CAPTURED);
      m_application->capture_mouse(true);
      m_application->get_mouse_pos(m_controls.mouse_x, m_controls.mouse_y);

      //int dx = m_controls.mouse_x - mouse_x;
      //int dy = m_controls.mouse_y - mouse_y;

      //  Enable mouse tracking
      m_controls.ignore_mouse = true;
   }
   else
   {
      //m_application->set_input_mode(GLFW_CURSOR_MODE, GLFW_CURSOR_NORMAL);
      m_application->capture_mouse(false);
   }
#endif
}

void game::toggle_mouse_lock()
{
    assert(m_application);

    lock_mouse(!m_controls.mouse_locked);
}

void game::toggle_pause()
{
    m_paused = !m_paused;
}

void game::toggle_deferred()
{
    if (!m_deferred_renderer || !m_forward_renderer)
    {
        return;
    }

    m_forward  = !m_forward;
    m_deferred = !m_deferred;
}

void game::shift(bool left, bool value)
{
    if (left)
    {
        m_controls.left_shift = value;
    }
    else
    {
        m_controls.right_shift = value;
    }

    bool any_shift_pressed = m_controls.left_shift || m_controls.right_shift;
    m_controls.camera_controller.translate_y().set_less(any_shift_pressed);
}

void game::on_focus(bool has_focus)
{
    if (!has_focus)
    {
        m_controls.camera_controller.translate_z().set_more(false);
        m_controls.camera_controller.translate_z().set_less(false);
        m_controls.camera_controller.translate_x().set_more(false);
        m_controls.camera_controller.translate_x().set_less(false);
    }
    m_controls.camera_controller.translate_z().set_inhibit(!has_focus);
    m_controls.camera_controller.translate_x().set_inhibit(!has_focus);
}

void game::on_key(int key, int scancode, int action, int mods)
{
    bool pressed = action != 0;

    static_cast<void>(mods);
    static_cast<void>(scancode);

    if (pressed)
    {
        switch (key)
        {
            case RS_KEY_TAB:
            {
                toggle_pause();
                break;
            }

            case RS_KEY_ESCAPE:
            {
                toggle_mouse_lock();
                break;
            }

            case RS_KEY_F1:
            {
                m_min_frame_dt = 1.0;
                m_max_frame_dt = 0.0;
                break;
            }

            case RS_KEY_F2:
            {
                toggle_deferred();
                break;
            }

            case RS_KEY_F3:
            {
                if (m_light_debug_renderer)
                {
                    m_debug_lights = !m_debug_lights;
                }
                break;
            }

            case RS_KEY_F4:
            {
                --m_max_lights;
                break;
            }

            case RS_KEY_F5:
            {
                ++m_max_lights;
                break;
            }

            case RS_KEY_F6:
            {
                if (m_deferred_renderer)
                {
                    bool use_stencil = m_deferred_renderer->use_stencil();
                    use_stencil      = !use_stencil;
                    m_deferred_renderer->set_use_stencil(use_stencil);
                    m_deferred_renderer->resize(m_application->width(), m_application->height());
                }
                break;
            }

            case RS_KEY_F7:
            {
                if (m_deferred_renderer)
                {
                    int scale = m_deferred_renderer->scale();
                    --scale;
                    if (scale < 1)
                    {
                        scale = 1;
                    }
                    m_deferred_renderer->set_scale(scale);
                    m_deferred_renderer->resize(m_application->width(), m_application->height());
                }
                break;
            }

            case RS_KEY_F8:
            {
                if (m_deferred_renderer)
                {
                    int scale = m_deferred_renderer->scale();
                    ++scale;
                    m_deferred_renderer->set_scale(scale);
                    m_deferred_renderer->resize(m_application->width(), m_application->height());
                }
                break;
            }

            case RS_KEY_B:
            {
                /* m_controls.fov *= 1.1f; TODO */
                break;
            }

            case RS_KEY_N:
            {
                /* m_controls.fov /= 1.1f; TODO */
                break;
            }

            case RS_KEY_M:
            {
                reset();
                break;
            }
        }
    }

    switch (key)
    {
        case RS_KEY_SPACE:
        {
            m_controls.camera_controller.translate_y().set_more(pressed);
            break;
        }

        case RS_KEY_LEFT_SHIFT:
        {
            shift(true, pressed);
            break;
        }

        case RS_KEY_RIGHT_SHIFT:
        {
            shift(false, pressed);
            break;
        }
        case RS_KEY_W: m_controls.camera_controller.translate_z().set_less(pressed); break;
        case RS_KEY_S: m_controls.camera_controller.translate_z().set_more(pressed); break;
        case RS_KEY_D: m_controls.camera_controller.translate_x().set_more(pressed); break;
        case RS_KEY_A: m_controls.camera_controller.translate_x().set_less(pressed); break;
    }
}
void game::on_mouse_moved(double x, double y)
{
    if (!m_controls.mouse_locked)
    {
        m_controls.mouse_x = x;
        m_controls.mouse_y = y;
        return;
    }

    if (m_controls.ignore_mouse)
    {
        m_controls.mouse_x      = x;
        m_controls.mouse_y      = y;
        m_controls.ignore_mouse = false;
        return;
    }

    if (!m_mouse_down)
    {
        m_controls.mouse_x = x;
        m_controls.mouse_y = y;
        return;
    }

    double x_delta = m_controls.mouse_x - x;
    double y_delta = m_controls.mouse_y - y;

    if (x_delta != 0.0)
    {
        float value = static_cast<float>(x_delta / 8192.0);
        m_controls.camera_controller.rotate_y().adjust(-value);
        m_controls.mouse_x = x;
    }

    if (y_delta != 0.0)
    {
        float value = static_cast<float>(y_delta / 8192.0);
        m_controls.camera_controller.rotate_x().adjust(-value);
        m_controls.mouse_y = y;
    }
}

void game::on_3d_mouse(long tx, long ty, long tz, long rx, long ry, long rz, long period)
{
    double ts = (double)period / (65536.0 * 16.0);
    double rs = (double)period / (65536.0 * 256.0);
    if (tx)
    {
        m_controls.camera_controller.translate_x().adjust((double)tx * ts);
    }

    if (ty)
    {
        m_controls.camera_controller.translate_y().adjust((double)ty * ts);
    }

    if (tz)
    {
        m_controls.camera_controller.translate_z().adjust(-(double)tz * ts);
    }

    if (rx)
    {
        m_controls.camera_controller.rotate_x().adjust((double)rx * rs);
    }

    if (ry)
    {
        m_controls.camera_controller.rotate_y().adjust((double)ry * rs);
    }

    if (rz)
    {
        m_controls.camera_controller.rotate_z().adjust((double)rz * rs);
    }
}

void game::on_mouse_button(int button, int action, int mods)
{
    m_mouse_down = !!action;

    if (m_mouse_down)
    {
    }

    (void)button;
    (void)mods;
}

void game::on_scroll(double x, double y)
{
    (void)x;
    (void)y;
}
