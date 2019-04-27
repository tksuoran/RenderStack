#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_ui/font.hpp"

#include "main/application.hpp"
#include "main/log.hpp"

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>

using namespace renderstack::graphics;
using namespace renderstack::ui;
using namespace renderstack::toolkit;
using namespace gl;
using namespace glm;
using namespace std;

Application::Application()
    : service("Application")
{
}

Application::~Application()
{
    on_exit(); // TODO figure out how to get ~window call to on_exit() to call derived on_exit?
}

void Application::connect(shared_ptr<Game> game_, shared_ptr<Menu> menu_)
{
    m_game = game_;
    m_menu = menu_;

    initialization_depends_on(game_);
    initialization_depends_on(menu_);
}

void Application::initialize_service()
{
    if (m_menu)
    {
        set_screen(m_menu);
    }
    else if (m_game)
    {
        set_screen(m_game);
    }

    m_last_screen.reset();
}

void Application::set_screen(shared_ptr<Screen> screen)
{
    m_screen = screen;
}

void Application::reset_screen()
{
    m_screen.reset();
}

void Application::on_resize(int width, int height)
{
    m_screen->on_resize(width, height);
}

void Application::on_focus(bool has_focus)
{
    m_screen->on_focus(has_focus);
}

void Application::update()
{
    if (m_last_screen != m_screen)
    {
        if (m_last_screen)
        {
            m_last_screen->on_exit();
        }

        m_screen->on_enter();
        m_last_screen = m_screen;
    }

    m_screen->update();
}

void Application::on_key(int key, int scancode, int action, int mods)
{
    m_screen->on_key(key, scancode, action, mods);
}

void Application::on_mouse_moved(double x, double y)
{
    m_screen->on_mouse_moved(x, y);
}

void Application::on_mouse_button(int button, int action, int mods)
{
    m_screen->on_mouse_button(button, action, mods);
}

void Application::on_scroll(double x, double y)
{
    m_screen->on_scroll(x, y);
}

void Application::on_3d_mouse(long tx, long ty, long tz, long rx, long ry, long rz, long period)
{
    m_screen->on_3d_mouse(tx, ty, tz, rx, ry, rz, period);
}
