#ifndef application_hpp
#define application_hpp

#include "main/game.hpp"
#include "main/menu.hpp"
#include "main/programs.hpp"
#include "main/screen.hpp"
#include "main/textures.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/services.hpp"
#include "renderstack_toolkit/window.hpp"
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace renderer
{
class Renderer;
}
} // namespace renderstack
namespace renderstack
{
namespace ui
{
class Gui_renderer;
}
} // namespace renderstack

class Application
    : public renderstack::toolkit::window
    , public renderstack::toolkit::service
    , public std::enable_shared_from_this<Application>
{
public:
    Application();

    virtual ~Application();

    void connect(std::shared_ptr<Game> game, std::shared_ptr<Menu> menu);

    void initialize_service() override;

    void set_screen(std::shared_ptr<Screen> screen);

    void reset_screen();

    void execute_tests();

    void reset();

    bool on_load() override;

    bool on_exit() override;

    void on_resize(int width, int height) override;

    void on_focus(bool has_focus) override;

    void update() override;

    void on_key(int key, int scancode, int action, int mods) override;

    void on_mouse_moved(double x, double y) override;

    void on_mouse_button(int button, int action, int mods) override;

    void on_scroll(double x, double y) override;

    void on_3d_mouse(long tx, long ty, long tz, long rx, long ry, long rz, long period) override;

    void setup_programs();

private:
    bool create_gl_window();

    bool initialize_services();

    renderstack::toolkit::services m_services;

    std::shared_ptr<Screen> m_screen;
    std::shared_ptr<Screen> m_last_screen;
    std::shared_ptr<Game>   m_game;
    std::shared_ptr<Menu>   m_menu;
};

#endif
