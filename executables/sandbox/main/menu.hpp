#ifndef menu_hpp
#define menu_hpp

#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_ui/action.hpp"

#include "main/screen.hpp"

#include <memory>

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
class Text_buffer;
class Layer;
} // namespace ui
namespace toolkit
{
class window;
}
} // namespace renderstack

class Game;
class Application;
class Programs;

class window;
class textures;

class Menu
    : public Screen,
      public renderstack::ui::Action_sink,
      public renderstack::toolkit::service
{
public:
    Menu();

    virtual ~Menu() = default;

    void connect(std::shared_ptr<renderstack::graphics::Renderer> renderer,
                 std::shared_ptr<renderstack::ui::Gui_renderer>   gui_renderer,
                 std::shared_ptr<Programs>                        programs,
                 std::shared_ptr<textures>                        textures,
                 std::shared_ptr<Game>                            game,
                 std::shared_ptr<Application>                     application);

    void disconnect();

    void initialize_service() override;

    void action(renderstack::ui::Action_source* source) override;

    void on_load();

    void update() override;

    void on_enter() override;

    void on_exit() override;

    void on_resize(int width, int height) override;

    void on_focus(bool has_focus) override;

    void on_key(int key, int scancode, int action, int mods) override;

    void on_mouse_moved(double s, double y) override;

    void on_mouse_button(int button, int action, int mods) override;

    void on_scroll(double x, double y) override;

    void on_3d_mouse(long tx, long ty, long tz, long rx, long ry, long rz, long period) override;

private:
    void resize(float w, float h);
    void render();

    std::shared_ptr<renderstack::graphics::Renderer> m_renderer;
    std::shared_ptr<renderstack::ui::Gui_renderer>   m_gui_renderer;
    std::shared_ptr<Programs>                        m_programs;
    std::shared_ptr<textures>                        m_textures;
    std::shared_ptr<Game>                            m_game;
    std::shared_ptr<Application>                     m_application;

    std::shared_ptr<renderstack::graphics::Buffer> m_uniform_buffer;
    std::shared_ptr<renderstack::ui::Font>         m_font;
    std::shared_ptr<renderstack::ui::Text_buffer>  m_text_buffer;
    std::shared_ptr<renderstack::mesh::mesh>       m_mesh;
    std::shared_ptr<renderstack::ui::Layer>        m_root_layer;
    std::shared_ptr<renderstack::ui::Button>       m_sandbox_button;
    std::shared_ptr<renderstack::ui::Button>       m_quit;
    std::shared_ptr<renderstack::ui::Menulist>     m_menulist;

    bool m_resize;
};

#endif
