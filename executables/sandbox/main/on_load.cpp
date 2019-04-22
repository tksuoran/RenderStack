#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/enable_shared_from_this.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_ui/gui_renderer.hpp"

#include "main/application.hpp"
#include "main/game.hpp"
#include "main/menu.hpp"
#include "renderers/debug_renderer.hpp"
#include "renderers/quad_renderer.hpp"
#include "renderers/forward_renderer.hpp"
#include "renderers/deferred_renderer.hpp"
#include "renderers/light_debug_renderer.hpp"
#include "renderers/id_renderer.hpp"
#include "renderers/light_mesh.hpp"

#include <sstream>

using namespace gl;
using namespace std;
using namespace renderstack::toolkit;
using namespace renderstack::graphics;


bool application::create_gl_window()
{
#if defined(RENDERSTACK_GL_API_OPENGL)
   int versions[] = {46, 45, 44, 43, 42, 41, 40, 33, 32, 31, 30, 21, 20, 15, 14, 13, 12, 11, 10, 0 };
   int *v = versions;

   bool ok = false;
   int major = 0;
   int minor = 0;

   for (ok = false; *v && !ok; ++v)
   {
      int version = *v;
      major = version / 10;
      minor = version % 10;

      stringstream ss;
      ss << "RenderStack Sandbox (OpenGL " << major << "." << minor << ")";

      // Youtube 720p = 1280 x 720
      // Youtube 480p = 854 x 480
      ok = open(1280, 720, ss.str().c_str(), major, minor);
   }

   if (!ok)
      return false;

#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   open(512, 512, "OpenGL ES 3", 3, 0);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2)
   open(512, 512, "OpenGL ES 2", 2, 0)
#endif

   configuration::initialize();

   return true;
}

bool application::initialize_services()
{
   auto application_       = shared_from_this();

   auto renderer           = make_shared<renderstack::graphics::renderer>();
   auto gui_renderer       = make_shared<renderstack::ui::gui_renderer>();

   shared_ptr<programs>             programs_;
   shared_ptr<textures>             textures_;
   shared_ptr<debug_renderer>       debug_renderer_;
   shared_ptr<quad_renderer>        quad_renderer_;
   shared_ptr<forward_renderer>     forward_renderer_;
   shared_ptr<deferred_renderer>    deferred_renderer_;
   shared_ptr<light_debug_renderer> l_d_renderer_;
   shared_ptr<id_renderer>          id_renderer_;
   shared_ptr<light_mesh>           light_mesh_;
   shared_ptr<scene_manager>        scene_manager_;
   shared_ptr<menu>                 menu_;
   programs_          = make_shared<programs>();
   textures_          = make_shared<textures>();
   // 
   debug_renderer_    = make_shared<debug_renderer>();
   quad_renderer_     = make_shared<quad_renderer>();
   forward_renderer_  = make_shared<forward_renderer>();
   deferred_renderer_ = make_shared<deferred_renderer>();
   l_d_renderer_      = make_shared<light_debug_renderer>();
   id_renderer_       = make_shared<id_renderer>();
   light_mesh_        = make_shared<light_mesh>();

   auto shader_monitor_    = make_shared<shader_monitor>();

   scene_manager_     = make_shared<scene_manager>();

   auto game_         = smart_ptr_builder::create_shared_ptr<renderstack::ui::action_sink>(new game());
   menu_              = smart_ptr_builder::create_shared_ptr<renderstack::ui::action_sink>(new menu());

   m_services.add(renderer);
   m_services.add(gui_renderer);

   m_services.add(programs_);
   m_services.add(textures_);

   m_services.add(debug_renderer_);
   m_services.add(quad_renderer_);
   m_services.add(forward_renderer_);
   m_services.add(deferred_renderer_);
   m_services.add(l_d_renderer_);
   m_services.add(id_renderer_);
   m_services.add(light_mesh_);

   m_services.add(shader_monitor_);

   m_services.add(scene_manager_);

   m_services.add(game_);
   m_services.add(menu_);

   m_services.add(application_);

   if (gui_renderer)       gui_renderer->connect(renderer, shader_monitor_);

   if (programs_)          programs_->connect(renderer, shader_monitor_);
   if (textures_)          textures_->connect(renderer);

   if (debug_renderer_)    debug_renderer_->connect(renderer, gui_renderer, programs_, application_);
   if (quad_renderer_)     quad_renderer_->connect(renderer);
   if (forward_renderer_)  forward_renderer_->connect(renderer, debug_renderer_, programs_);
   if (deferred_renderer_) deferred_renderer_->connect(renderer, programs_, quad_renderer_, light_mesh_);
   if (l_d_renderer_)      l_d_renderer_->connect(renderer, programs_, light_mesh_);
   if (id_renderer_)       id_renderer_->connect(renderer, programs_);
   if (light_mesh_)        light_mesh_->connect(renderer, programs_);

   if (scene_manager_)     scene_manager_->connect(programs_, renderer);

   if (game_)
      game_->connect(
         renderer,
         shader_monitor_,
         gui_renderer,
         programs_,
         textures_,
         debug_renderer_,
         forward_renderer_,
         deferred_renderer_,
         l_d_renderer_,
         id_renderer_,
         menu_,
         application_,
         scene_manager_
      );

   if (menu_)
      menu_->connect(renderer, gui_renderer, programs_, textures_, game_, application_);

   if (application_)
      application_->connect(game_, menu_);

   try
   {
      m_services.initialize_services();
   }
   catch (runtime_error const &e)
   {
      throw e;
   }
   catch (...)
   {
      return false;
   }

   gl::viewport(0, 0, width(), height());
   gl::clear_color(0.0f, 0.0f, 0.0f, 0.0f);
   gl::clear(clear_buffer_mask::color_buffer_bit | clear_buffer_mask::depth_buffer_bit);

   return true;
}

bool application::on_load()
{
   try
   {
      if (!create_gl_window())
         return false;

      if (!initialize_services())
         return false;

      return true;
   }
   catch (runtime_error const &e)
   {
      throw e;
   }
   catch (...)
   {
      return false;
   }
}

bool application::on_exit()
{
   if (m_screen)
   {
      m_screen->on_exit();
      m_screen.reset();
   }

   if (m_game)
      m_game->disconnect();

   if (m_menu)
      m_menu->disconnect();

   m_screen.reset();
   m_last_screen.reset();
   m_game.reset();
   m_menu.reset();

   m_services.cleanup_services();

   return true;
}
