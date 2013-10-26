#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_ui/font.hpp"

#include "main/application.hpp"
#include "main/log.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>


using namespace renderstack::graphics;
using namespace renderstack::ui;
using namespace renderstack::toolkit;
using namespace gl;
using namespace glm;
using namespace std;


application::application()
:  service("application")
{
}

application::~application()
{
   on_exit(); // TODO figure out how to get ~window call to on_exit() to call derived on_exit?
}

void application::connect(shared_ptr<game> game_, shared_ptr<menu> menu_)
{
   m_game = game_;
   m_menu = menu_;

   initialization_depends_on(game_);
   initialization_depends_on(menu_);
}

void application::initialize_service()
{
#  if 1
   if (m_menu)
      set_screen(m_menu);
#  else
   if (m_game)
      set_screen(m_game);
#  endif

   m_last_screen.reset();
}

void application::set_screen(shared_ptr<screen> screen)
{
   m_screen = screen;
}

void application::reset_screen()
{
   m_screen.reset();
}

void application::on_resize(int width, int height)
{
   m_screen->on_resize(width, height);
}

void application::on_focus(bool has_focus)
{
   m_screen->on_focus(has_focus);
}

void application::update()
{
   if (m_last_screen != m_screen)
   {
      if (m_last_screen)
         m_last_screen->on_exit();

      m_screen->on_enter();
      m_last_screen = m_screen;
   }

   m_screen->update();
}
void application::on_key(int key, int scancode, int action, int mods)
{
   m_screen->on_key(key, scancode, action, mods);
}
void application::on_mouse_moved(double x, double y)
{
   m_screen->on_mouse_moved(x, y);
}
void application::on_mouse_button(int button, int action, int mods)
{
   m_screen->on_mouse_button(button, action, mods);
}
void application::on_scroll(double x, double y)
{
   m_screen->on_scroll(x, y);
}
void application::on_3d_mouse(long tx, long ty, long tz, long rx, long ry, long rz, long period)
{
   m_screen->on_3d_mouse(tx, ty, tz, rx, ry, rz, period);
}

