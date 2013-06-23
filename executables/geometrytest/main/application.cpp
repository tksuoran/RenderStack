#include "renderstack_toolkit/platform.hpp"
#include "main/application.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>

log_category log_game      (C_WHITE,     C_GRAY, LOG_WARN);
log_category log_menu      (C_WHITE,     C_GRAY, LOG_WARN);
log_category log_programs  (C_DARK_BLUE, C_GRAY, LOG_WARN);
log_category log_textures  (C_DARK_BLUE, C_GRAY, LOG_WARN);


using namespace gl;
using namespace glm;
using namespace std;
using namespace renderstack::graphics;
using namespace renderstack::ui;
using namespace renderstack::toolkit;


application::application()
{
}

application::~application()
{
   on_exit(); // TODO figure out how to get ~window call to on_exit() to call derived on_exit?
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


int main(int argc, char *argv[])
{
   console_init();

   init_memory_system();
   begin_memory_compare();

   (void)argc;
   (void)argv;
   shared_ptr<renderstack::toolkit::window> g_application;
   int return_value = EXIT_FAILURE;

#  if defined(NDEBUG)
   try
#  endif
   {
      g_application = make_shared<application>();
      if (g_application->on_load())
      {
         g_application->run();
         return_value = EXIT_SUCCESS;
      }
      else
      {
         return_value = EXIT_FAILURE;
      }

   }
#  if defined(NDEBUG)
   catch(...)
   {
      ;
   }
#  endif

   if (g_application)
      g_application->on_exit();

   g_application.reset();

   end_memory_compare();

   exit(return_value);
}
