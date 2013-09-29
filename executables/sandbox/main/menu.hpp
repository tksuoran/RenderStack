#ifndef menu_hpp
#define menu_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_ui/action.hpp"
#include "renderstack_ui/action.hpp"

#include "main/screen.hpp"

#include <memory>

namespace renderstack
{
   namespace graphics
   {
      class uniform_buffer;
      class uniform_buffer_range;
   }
   namespace ui
   {
      class button;
      class font;
      class gui_renderer;
      class text_buffer;
      class layer;
   }
   namespace toolkit
   {
      class window;
   }
}

class game;
class window;
class programs;
class textures;

class menu
:  public screen
,  public renderstack::ui::action_sink
,  public renderstack::toolkit::service
{
public:
   menu();   
   /*virtual*/ ~menu();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<renderstack::ui::gui_renderer>     gui_renderer,
      std::shared_ptr<programs>                          programs_,
      std::shared_ptr<textures>                          textures_,
      std::shared_ptr<class game>                        game_,
      std::shared_ptr<class application>                 application_
   );
   void disconnect();
   /*virtual*/ void initialize_service();

   void action          (std::weak_ptr<renderstack::ui::action_source> source);

   void on_load         ();
   void on_enter        ();
   void on_exit         ();
   void on_resize       (int width, int height);
   void on_focus        (bool has_focus);

   void update          ();
   void on_key          (int key, int scancode, int action, int mods);
   void on_mouse_moved  (double s, double y);
   void on_mouse_button (int button, int action, int mods);
   void on_scroll       (double x, double y);
   void on_3d_mouse     (long tx, long ty, long tz, long rx, long ry, long rz, long period);

private:
   void render();

private: /* services */
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<renderstack::ui::gui_renderer>                 m_gui_renderer;
   std::shared_ptr<programs>                                      m_programs;
   std::shared_ptr<textures>                                      m_textures;
   std::shared_ptr<class game>                                    m_game;
   std::shared_ptr<application>                                   m_application;

   std::shared_ptr<renderstack::ui::font>                         m_font;
   std::shared_ptr<renderstack::ui::text_buffer>                  m_text_buffer;
   std::shared_ptr<renderstack::mesh::mesh>                       m_mesh;
   std::shared_ptr<renderstack::ui::layer>                        m_root_layer;
   std::shared_ptr<renderstack::ui::button>                       m_sandbox_button;
   std::shared_ptr<renderstack::ui::button>                       m_quit;
};

#endif
