#include "renderstack_toolkit/platform.hpp"
#include "main/application.hpp"
#include "main/game.hpp"
#include "main/menu.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/enable_shared_from_this.hpp"
#include "renderstack_ui/gui_renderer.hpp"

using namespace gl;
using namespace std;
using namespace renderstack::toolkit;
using namespace renderstack::graphics;

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
   m_programs.reset();
   m_textures.reset();
   m_game.reset();
   m_menu.reset();
   m_gui_renderer.reset();
   m_renderer.reset();

   return true;
}
bool application::on_load()
{

#if defined(RENDERSTACK_GL_API_OPENGL)
   open(512, 512, "OpenGL", 3, 2);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   open(512, 512, "OpenGL ES 3", 3, 0);
#endif
#if defined(RENDERSTACK_GL_API_OPENGL_ES_2)
   open(512, 512, "OpenGL ES 2", 2, 0)
#endif

   configuration::initialize();

   m_renderer = make_shared<renderstack::graphics::renderer>();
   m_gui_renderer = make_shared<renderstack::ui::gui_renderer>(m_renderer);

#if 0
   {
      execute_tests();
      return false;
   }
#else
   {
      m_game = smart_ptr_builder::create_shared_ptr<renderstack::ui::action_sink>(new game());
      m_menu = smart_ptr_builder::create_shared_ptr<renderstack::ui::action_sink>(new menu());
      m_textures = make_shared<textures>();
      m_programs = make_shared<programs>();

      if (m_game)
         m_game->connect(m_renderer, m_gui_renderer, shared_from_this(), m_menu, m_programs, m_textures);

      if (m_menu)
         m_menu->connect(m_renderer, m_gui_renderer, shared_from_this(), m_game, m_programs, m_textures);

      if (m_textures)
         m_textures->on_load(*m_renderer);

      if (m_programs)
         m_programs->prepare_gl_resources();
      
      if (m_game)
         m_game->on_load();

      if (m_menu)
         m_menu->on_load();

      setup_gl_state();

#  if 1
      if (m_menu)
         set_screen(m_menu);
#  else
      if (m_game)
         set_screen(m_game);
#  endif

      m_last_screen.reset();
      return true;
   }
#endif

}

void application::setup_gl_state()
{
   gl::viewport(0, 0, width(), height());

   gl::clear_color(0.0f, 0.0f, 0.0f, 0.0f);
   gl::clear(clear_buffer_mask::color_buffer_bit | clear_buffer_mask::depth_buffer_bit);

   gl::enable(gl::enable_cap::depth_test);
   gl::enable(gl::enable_cap::cull_face);
   gl::enable(gl::enable_cap::scissor_test);
}
