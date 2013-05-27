#ifndef menu_hpp
#define menu_hpp

#include "renderstack_toolkit/platform.hpp"
#include "main/screen.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_ui/action.hpp"
#include "renderstack_ui/action.hpp"
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

class menu : public screen, public renderstack::ui::action_sink
{
public:
   menu();
   virtual ~menu();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<renderstack::ui::gui_renderer>     gui_renderer,
      std::shared_ptr<application>  screen,
      std::shared_ptr<class game>   game,
      std::shared_ptr<programs>     programs,
      std::shared_ptr<textures>     textures
   );
   void disconnect();

   void action          (std::weak_ptr<renderstack::ui::action_source> source);

   void on_load         ();
   void on_enter        ();
   void on_exit         ();
   void on_resize       (int width, int height);

   void update          ();
   void on_key_down     (int key);
   void on_key_up       (int key);
   void on_mouse_moved  (double s, double y);
   void on_mouse_button (int button, int value);
   void on_scroll       (double x, double y);

private:
   void render();

private:
   std::shared_ptr<application>                                   m_application;
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<renderstack::ui::gui_renderer>                 m_gui_renderer;
   std::shared_ptr<class game>                                    m_game;
   std::shared_ptr<programs>                                      m_programs;
   std::shared_ptr<textures>                                      m_textures;
   std::shared_ptr<renderstack::graphics::buffer>                 m_uniform_buffer;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_uniform_buffer_range;
   std::shared_ptr<renderstack::ui::font>                         m_font;
   std::shared_ptr<renderstack::ui::text_buffer>                  m_text_buffer;
   std::shared_ptr<renderstack::mesh::mesh>                       m_mesh;
   std::shared_ptr<renderstack::ui::layer>                        m_root_layer;
   std::shared_ptr<renderstack::ui::button>                       m_map_editor;
   std::shared_ptr<renderstack::ui::button>                       m_quit;
};

#endif
