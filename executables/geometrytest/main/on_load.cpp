#include "renderstack_toolkit/platform.hpp"
#include "main/application.hpp"
#include "main/game.hpp"
#include "main/menu.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/enable_shared_from_this.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_renderer/context.hpp"

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

   renderstack::ui::context::deinitialize();
   renderstack::renderer::context::deinitialize();
   renderstack::graphics::context::deinitialize();

   return true;
}
bool application::on_load()
{
   configuration::initialize();

   renderstack::graphics::context::make_current(make_shared<renderstack::graphics::context>());

   renderstack::renderer::context::make_current(make_shared<renderstack::renderer::context>());
   renderstack::ui::context::make_current(make_shared<renderstack::ui::context>());

   if (m_test_mode)
   {
      execute_tests();
      return false;
   }
   else
   {
      m_game = smart_ptr_builder::create_shared_ptr<renderstack::ui::action_sink>(new game());
      m_menu = smart_ptr_builder::create_shared_ptr<renderstack::ui::action_sink>(new menu());
      m_textures = make_shared<textures>();
      m_programs = make_shared<programs>();

      if (m_game)
         m_game->connect(shared_from_this(), m_menu, m_programs, m_textures);

      if (m_menu)
         m_menu->connect(shared_from_this(), m_game, m_programs, m_textures);

      if (m_textures)
         m_textures->on_load();

      if (m_programs)
         m_programs->prepare_gl_resources();
      
      if (m_game)
         m_game->on_load();

      if (m_menu)
         m_menu->on_load();

      setup_gl_state();

      //if (m_game)
      //   set_screen(m_game);
      if (m_menu)
         set_screen(m_menu);

      m_last_screen.reset();
      return true;
   }
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
