#ifndef game_hpp
#define game_hpp

#include "renderstack_scene/viewport.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_ui/action.hpp"
#include "renderstack_ui/choice.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/menulist.hpp"
#include "renderstack_ui/slider.hpp"

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
class Uniform_buffer;
class Uniform_buffer_range;
} // namespace graphics
namespace ui
{
class Button;
class Font;
class Gui_renderer;
class Slider;
class Text_buffer;
} // namespace ui
namespace mesh
{
class Geometry_mesh;
}
namespace scene
{
class Camera;
}
namespace toolkit
{
class window;
}
} // namespace renderstack

class vmap;
class Menu;
class textures;
class Application;

struct controls
{
    bool             mouse_locked{true};
    bool             ignore_mouse{false};
    bool             left_shift{false};
    bool             right_shift{false};
    double           mouse_x{0.0};
    double           mouse_y{0.0};
    glm::vec3        home{glm::vec3(0.0)};
    frame_controller camera_controller;

    void reset();

    void update_fixed_step();
};

class Game
    : public Screen
    , public renderstack::ui::Action_sink
    , public renderstack::toolkit::service
{
public:
    Game();

    virtual ~Game() = default;

    void connect(
        std::shared_ptr<renderstack::graphics::Renderer>       renderer,
        std::shared_ptr<renderstack::graphics::Shader_monitor> shader_monitor,
        std::shared_ptr<renderstack::ui::Gui_renderer>         gui_renderer,
        std::shared_ptr<Programs>                              programs,
        std::shared_ptr<textures>                              textures,
        std::shared_ptr<Debug_renderer>                        debug_renderer,
        std::shared_ptr<Forward_renderer>                      forward_renderer,
        std::shared_ptr<Deferred_renderer>                     deferred_renderer,
        std::shared_ptr<Light_debug_renderer>                  light_debug_renderer,
        std::shared_ptr<Id_renderer>                           id_renderer,
        std::shared_ptr<Menu>                                  menu,
        std::shared_ptr<Application>                           application,
        std::shared_ptr<Scene_manager>                         scene_manager);

    void disconnect();

    void initialize_service() override;

    void action(renderstack::ui::Action_source* source) override;

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

    // services
    std::shared_ptr<renderstack::graphics::Renderer>       m_renderer;
    std::shared_ptr<renderstack::graphics::Shader_monitor> m_shader_monitor;
    std::shared_ptr<renderstack::ui::Gui_renderer>         m_gui_renderer;
    std::shared_ptr<Programs>                              m_programs;
    std::shared_ptr<textures>                              m_textures;
    std::shared_ptr<Debug_renderer>                        m_debug_renderer;
    std::shared_ptr<Forward_renderer>                      m_forward_renderer;
    std::shared_ptr<Deferred_renderer>                     m_deferred_renderer;
    std::shared_ptr<Light_debug_renderer>                  m_light_debug_renderer;
    std::shared_ptr<Id_renderer>                           m_id_renderer;
    std::shared_ptr<Menu>                                  m_menu;
    std::shared_ptr<Application>                           m_application;
    std::shared_ptr<Scene_manager>                         m_scene_manager;

private:
    // self owned parts
    std::shared_ptr<renderstack::scene::Frame>           m_manipulator_frame;
    std::shared_ptr<std::vector<std::shared_ptr<Model>>> m_manipulator_models;

    std::unique_ptr<renderstack::ui::Layer>              m_root_layer;
    std::unique_ptr<renderstack::ui::Button>             m_menu_button;
    std::unique_ptr<renderstack::ui::Slider>             m_slider;
    std::unique_ptr<renderstack::ui::Menulist>           m_menulist;
    std::unique_ptr<renderstack::ui::Choice>             m_choice;

    controls                     m_controls;
    renderstack::scene::Viewport m_viewport;

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
