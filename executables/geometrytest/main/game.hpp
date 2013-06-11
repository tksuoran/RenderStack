#ifndef game_hpp
#define game_hpp

#include "renderstack_toolkit/platform.hpp"
#include "main/screen.hpp"
#include "main/programs.hpp"
#include "main/deferred_renderer.hpp"
#include "main/forward_renderer.hpp"
#include "renderer/model.hpp"
#include "renderstack_ui/action.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "util/frame_controller.hpp"
#include <glm/glm.hpp>
#include <unordered_set>
#include <vector>
#include <memory>

extern log_category log_game;

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
      class layer;
      class slider;
      class text_buffer;
   }
   namespace mesh
   {
      class geometry_mesh;
   }
   namespace toolkit
   {
      class window;
   }
}

class vmap;
class menu;
class textures;
class application;


struct controls
{
   bool              mouse_locked;
   bool              ignore_mouse;
   bool              left_shift;
   bool              right_shift;
   double            mouse_x;
   double            mouse_y;
   glm::mat4         clip_from_world;
   glm::mat4         world_from_clip;
   glm::vec3         home;
   frame_controller  camera_controller;
   float             fov;
   float             near_;
   float             far_;
   float             aspect;

public:
   controls()
   :  mouse_locked(true)
   ,  ignore_mouse(false)
   ,  left_shift(false)
   ,  right_shift(false)
   {
   }
   void reset();
   void update_fixed_step();
};

class game : public screen, public renderstack::ui::action_sink
{
public:
   game();
   virtual ~game()
   {
   }

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer,
      std::shared_ptr<renderstack::ui::gui_renderer>     gui_renderer,
      std::shared_ptr<application>  application,
      std::shared_ptr<menu>         menu,
      std::shared_ptr<programs>     programs,
      std::shared_ptr<textures>     textures
   );
   void disconnect();

   void action(std::weak_ptr<renderstack::ui::action_source> source);

public:
   void on_load         ();
   void on_resize       (int width, int height);
   void on_enter        ();
   void on_exit         ();
   void update          ();
   void on_key          (int key, int scancode, int action, int mods);
   void on_mouse_moved  (double x, double y);
   void on_mouse_button (int button, int action, int mods);
   void on_scroll       (double x, double y);

private:
   void reset();
   void setup_gui();

   void shift(bool left, bool value);
   void lock_mouse(bool lock);
   void toggle_mouse_lock();

private: 
   void render       ();
   void render_ui    ();
   void render_meshes();

private: 
   void update_fixed_steps    ();
   void update_fixed_step     ();
   void update_once_per_frame ();

private:
   std::shared_ptr<application>                       m_application;

   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<renderstack::ui::gui_renderer>     m_gui_renderer;

   std::shared_ptr<menu>                              m_menu;
   std::shared_ptr<programs>                          m_programs;
   std::shared_ptr<textures>                          m_textures;

   std::shared_ptr<renderstack::ui::font>             m_font;
   std::shared_ptr<renderstack::ui::text_buffer>      m_text_buffer;

   std::shared_ptr<renderstack::ui::layer>            m_root_layer;
   std::shared_ptr<renderstack::ui::button>           m_menu_button;
   std::shared_ptr<renderstack::ui::slider>           m_slider;

   std::shared_ptr<renderstack::graphics::buffer>                 m_uniform_buffer;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_text_uniform_buffer_range;

   std::vector<std::string>                        m_debug_lines;
   double                                          m_update_time;
   double                                          m_frame_dt;
   double                                          m_min_frame_dt;
   double                                          m_max_frame_dt;
   double                                          m_simulation_time;

   renderstack::graphics::render_states            m_font_render_states;

   controls                                        m_controls;

   std::vector<std::shared_ptr<class model>>       m_models;
   std::shared_ptr<deferred_renderer>              m_deferred_renderer;
   std::shared_ptr<forward_renderer>               m_forward_renderer;
   glm::mat4                                       m_projection;

   // are we between on_enter() and on_exit()?
   bool m_screen_active;
};

#endif
