#ifndef application_hpp
#define application_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_toolkit/services.hpp"
#include "main/screen.hpp"
#include "main/game.hpp"
#include "main/menu.hpp"
#include "main/programs.hpp"
#include "main/textures.hpp"
#include <vector>
#include <string>
#include <memory>

namespace renderstack { namespace renderer { class renderer; } }
namespace renderstack { namespace ui { class gui_renderer; } }

class application
:  public renderstack::toolkit::window
,  public renderstack::toolkit::service
,  public std::enable_shared_from_this<application>
{
private:
   renderstack::toolkit::services   m_services;

   std::shared_ptr<screen>          m_screen;
   std::shared_ptr<screen>          m_last_screen;
   std::shared_ptr<game>            m_game;
   std::shared_ptr<menu>            m_menu;

public:
   application();
   /*virtual*/ ~application();
   void connect(std::shared_ptr<game> game_, std::shared_ptr<menu> menu_);

   /*virtual*/ void initialize_service();

   void set_screen(std::shared_ptr<screen> screen);
   void reset_screen();

   void execute_tests();

   void reset();

   bool on_load         ();
   bool on_exit         ();
   void on_resize       (int width, int height);
   void update          ();

   void on_key          (int key, int scancode, int action, int mods);
   void on_mouse_moved  (double x, double y);
   void on_mouse_button (int button, int action, int mods);
   void on_scroll       (double x, double y);

   void setup_programs  ();

private:
   bool create_gl_window();
   bool initialize_services();
};

#endif
