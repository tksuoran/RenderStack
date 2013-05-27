#ifndef application_hpp
#define application_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/window.hpp"
#include "main/screen.hpp"
#include "main/game.hpp"
#include "main/menu.hpp"
#include "main/programs.hpp"
#include "main/textures.hpp"
#include <vector>
#include <string>
#include <memory>

extern log_category log_game;
extern log_category log_menu;
extern log_category log_programs;
extern log_category log_textures;

namespace renderstack { namespace renderer { class renderer; } }
namespace renderstack { namespace ui { class gui_renderer; } }


class application
:  public renderstack::toolkit::window
,  public std::enable_shared_from_this<application>
{
private:
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<renderstack::ui::gui_renderer>     m_gui_renderer;

   std::shared_ptr<screen>    m_screen;
   std::shared_ptr<screen>    m_last_screen;
   std::shared_ptr<programs>  m_programs;
   std::shared_ptr<textures>  m_textures;
   std::shared_ptr<game>      m_game;
   std::shared_ptr<menu>      m_menu;
   bool                       m_test_mode;

public:
   application();
   virtual ~application();

   void set_screen(std::shared_ptr<screen> screen);
   void reset_screen();

   void execute_tests();

   void reset();

   bool on_load        ();
   bool on_exit        ();
   void on_resize      (int width, int height);
   void update         ();

   void on_key_down     (int key);
   void on_key_up       (int key);
   void on_mouse_moved  (double x, double y);
   void on_mouse_button (int button, int value);
   void on_scroll       (double x, double y);

   void setup_programs ();
   void setup_gl_state ();
};

#endif
