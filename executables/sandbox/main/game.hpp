#ifndef game_hpp
#define game_hpp

#include "renderstack_scene/viewport.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_ui/action.hpp"

#include "main/programs.hpp"
#include "main/screen.hpp"
#include "renderers/debug_renderer.hpp"
#include "renderers/deferred_renderer.hpp"
#include "renderers/forward_renderer.hpp"
#include "renderers/id_renderer.hpp"
#include "renderers/light_debug_renderer.hpp"
#include "scene/model.hpp"
#include "scene/scene_manager.hpp"
#include "util/frame_controller.hpp"

#include <cstdlib>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace renderstack
{
namespace graphics
{
class uniform_buffer;
class uniform_buffer_range;
} // namespace graphics
namespace ui
{
class button;
class font;
class gui_renderer;
class layer;
class slider;
class text_buffer;
} // namespace ui
namespace mesh
{
class geometry_mesh;
}
namespace scene
{
class camera;
}
namespace toolkit
{
class window;
}
} // namespace renderstack

class vmap;
class menu;
class textures;
class application;

struct controls
{
    bool             mouse_locked;
    bool             ignore_mouse;
    bool             left_shift;
    bool             right_shift;
    double           mouse_x;
    double           mouse_y;
    glm::vec3        home;
    frame_controller camera_controller;

public:
    controls()
        : mouse_locked(true), ignore_mouse(false), left_shift(false), right_shift(false)
    {
    }
    void reset();
    void update_fixed_step();
};

class game
    : public screen,
      public renderstack::ui::action_sink,
      public renderstack::toolkit::service
{
public:
    game();
    /*virtual*/ ~game();

    void connect(
        std::shared_ptr<renderstack::graphics::renderer>       renderer,
        std::shared_ptr<renderstack::graphics::shader_monitor> shader_monitor_,
        std::shared_ptr<renderstack::ui::gui_renderer>         gui_renderer,
        std::shared_ptr<programs>                              programs_,
        std::shared_ptr<textures>                              textures_,
        std::shared_ptr<debug_renderer>                        debug_renderer_,
        std::shared_ptr<forward_renderer>                      forward_renderer_,
        std::shared_ptr<deferred_renderer>                     deferred_renderer_,
        std::shared_ptr<light_debug_renderer>                  light_debug_renderer_,
        std::shared_ptr<id_renderer>                           id_renderer_,
        std::shared_ptr<menu>                                  menu_,
        std::shared_ptr<application>                           application_,
        std::shared_ptr<scene_manager>                         scene_manager_);
    void disconnect();
    void initialize_service() override;

    void action(std::weak_ptr<renderstack::ui::action_source> source) override;

public:
    void on_load();
    void on_resize(int width, int height) override;
    void on_focus(bool has_focus) override;
    void on_enter() override;
    void on_exit() override;
    void update() override;
    void on_key(int key, int scancode, int action, int mods) override;
    void on_mouse_moved(double x, double y) override;
    void on_mouse_button(int button, int action, int mods) override;
    void on_scroll(double x, double y) override;
    void on_3d_mouse(long tx, long ty, long tz, long rx, long ry, long rz, long period) override;

private:
    void reset();
    void setup_gui();

    void shift(bool left, bool value);
    void lock_mouse(bool lock);
    void toggle_mouse_lock();
    void toggle_pause();
    void toggle_deferred();

    void render();
    void render_meshes();

    void update_fixed_steps();
    void update_fixed_step();
    void update_once_per_frame();

private:
    // services
    std::shared_ptr<renderstack::graphics::renderer>       m_renderer;
    std::shared_ptr<renderstack::graphics::shader_monitor> m_shader_monitor;
    std::shared_ptr<renderstack::ui::gui_renderer>         m_gui_renderer;
    std::shared_ptr<programs>                              m_programs;
    std::shared_ptr<textures>                              m_textures;
    std::shared_ptr<debug_renderer>                        m_debug_renderer;
    std::shared_ptr<forward_renderer>                      m_forward_renderer;
    std::shared_ptr<deferred_renderer>                     m_deferred_renderer;
    std::shared_ptr<light_debug_renderer>                  m_light_debug_renderer;
    std::shared_ptr<id_renderer>                           m_id_renderer;
    std::shared_ptr<menu>                                  m_menu;
    std::shared_ptr<application>                           m_application;
    std::shared_ptr<scene_manager>                         m_scene_manager;

private:
    // self owned parts
    std::shared_ptr<renderstack::scene::frame>           m_manipulator_frame;
    std::shared_ptr<std::vector<std::shared_ptr<model>>> m_manipulator_models;
    std::shared_ptr<renderstack::ui::layer>              m_root_layer;
    std::shared_ptr<renderstack::ui::button>             m_menu_button;
    std::shared_ptr<renderstack::ui::slider>             m_slider;

    controls                     m_controls;
    renderstack::scene::viewport m_viewport;

    double m_update_time;
    double m_frame_dt;
    double m_min_frame_dt;
    double m_max_frame_dt;
    double m_simulation_time;

    bool m_paused;
    bool m_forward;
    bool m_deferred;
    bool m_debug_lights;
    int  m_max_lights;

    // are we between on_enter() and on_exit()?
    bool m_screen_active;
    bool m_mouse_down;
};

#endif
