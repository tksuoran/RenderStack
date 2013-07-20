#ifndef game_hpp
#define game_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_ui/action.hpp"

#include "main/screen.hpp"
#include "main/programs.hpp"
#include "renderers/deferred_renderer.hpp"
#include "renderers/forward_renderer.hpp"
#include "renderers/id_renderer.hpp"
#include "renderers/debug_renderer.hpp"
#include "scene/model.hpp"
#include "util/frame_controller.hpp"

#include <glm/glm.hpp>
#include <unordered_set>
#include <vector>
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

class game
:  public screen
,  public renderstack::ui::action_sink
,  public renderstack::toolkit::service
{
public:
   game();
   /*virtual*/ ~game();

   void connect(
      std::shared_ptr<renderstack::graphics::renderer>  renderer,
      std::shared_ptr<renderstack::ui::gui_renderer>    gui_renderer,
      std::shared_ptr<programs>                         programs_,
      std::shared_ptr<textures>                         textures_,
      std::shared_ptr<debug_renderer>                   debug_renderer_,
      std::shared_ptr<forward_renderer>                 forward_renderer_,
      std::shared_ptr<deferred_renderer>                deferred_renderer_,
      std::shared_ptr<id_renderer>                      id_renderer_,
      std::shared_ptr<menu>                             menu_,
      std::shared_ptr<application>                      application_
   );
   void disconnect();
   /*virtual*/ void initialize_service();

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

   void render       ();
   void render_ui    ();
   void render_meshes();

   void update_fixed_steps    ();
   void update_fixed_step     ();
   void update_once_per_frame ();

   renderstack::mesh::geometry_mesh_format_info m_format_info;
   renderstack::mesh::geometry_mesh_buffer_info m_buffer_info;
   void reset_build_info();
   std::shared_ptr<model> make_model(
      std::shared_ptr<renderstack::scene::frame> parent,
      std::shared_ptr<renderstack::geometry::geometry> geometry,
      glm::vec3 position = glm::vec3(0.0f));

private: /* services */
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;
   std::shared_ptr<renderstack::ui::gui_renderer>     m_gui_renderer;
   std::shared_ptr<programs>                          m_programs;
   std::shared_ptr<textures>                          m_textures;
   std::shared_ptr<debug_renderer>                    m_debug_renderer;
   std::shared_ptr<forward_renderer>                  m_forward_renderer;
   std::shared_ptr<deferred_renderer>                 m_deferred_renderer;
   std::shared_ptr<id_renderer>                       m_id_renderer;
   std::shared_ptr<menu>                              m_menu;
   std::shared_ptr<application>                       m_application;

private: /* self owned parts */
   std::shared_ptr<class group>                                   m_models;
   std::shared_ptr<renderstack::scene::frame>                     m_manipulator_frame;
   std::shared_ptr<class group>                                   m_manipulator;
   std::shared_ptr<renderstack::ui::font>                         m_font;
   std::shared_ptr<renderstack::ui::text_buffer>                  m_text_buffer;
   std::shared_ptr<renderstack::ui::layer>                        m_root_layer;
   std::shared_ptr<renderstack::ui::button>                       m_menu_button;
   std::shared_ptr<renderstack::ui::slider>                       m_slider;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_text_uniform_buffer_range;

   std::vector<std::string>                        m_debug_lines;
   renderstack::graphics::render_states            m_font_render_states;
   controls                                        m_controls;
   glm::mat4                                       m_projection;

   double                                          m_update_time;
   double                                          m_frame_dt;
   double                                          m_min_frame_dt;
   double                                          m_max_frame_dt;
   double                                          m_simulation_time;

   // are we between on_enter() and on_exit()?
   bool m_screen_active;
   bool m_mouse_down;
};

#endif
