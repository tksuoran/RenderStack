#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/slider.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include "renderstack_scene/camera.hpp"

#include "main/game.hpp"
#include "main/application.hpp"
#include "main/log.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>

#define LOG_CATEGORY &log_game

#define EXTRA_DEBUG 1

using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace renderstack::ui;
using namespace renderstack::toolkit;
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;


void game::render_meshes()
{
   slog_trace("game::render_meshes()");

#if 1
   // ID render pass
   shared_ptr<model> hover_model;
   {
      double dx;
      double dy;

      // int iw = m_application->width();
      int ih = m_application->height();

      m_application->get_mouse_pos(dx, dy);
#if 1
      int mouse_x = static_cast<int>(dx);
      int mouse_y = static_cast<int>(ih - 1 - dy);

      int x = mouse_x;
      int y = mouse_y;
#else
      int center_x = iw / 2;
      int center_y = ih / 2;

      int x = center_x;
      int y = center_y;
#endif

      m_id_renderer->clear();
      m_id_renderer->render_pass(
         m_models,
         m_camera,
         m_application->time(),
         x,
         y
      );
      // uint32_t id = 0xffffffffu;
      // float depth = 1.0f;
      // bool got = m_id_renderer->get(x, y, id, depth);
      hover_model = m_id_renderer->get(x, y);
      if (hover_model)
         m_manipulator_frame->set_parent(hover_model->frame());
   }
#endif

#if 0 // Deferred renderer
   m_deferred_renderer->geometry_pass(m_models, m_camera);
   m_deferred_renderer->light_pass(m_camera);
   m_deferred_renderer->show_rt();
#endif

#if 1 // Forward renderer
   glClearColor(0.05f, 0.1f, 0.15f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   m_forward_renderer->render_pass(m_models, m_camera);
   //m_forward_renderer->render_pass(m_manipulator, m_camera);
#endif

#if 1
   if (hover_model)
   {
      m_debug_renderer->set_camera(m_camera);
      m_debug_renderer->begin_edit();
      m_debug_renderer->set_color(vec4(0.0f, 1.0f, 0.0f, 1.0f));
      //for (auto i = m_models.cbegin(); i != m_models.cend(); ++i)
      {
         //auto model = *i;
         auto model = hover_model;
         auto geometry_mesh = model->geometry_mesh();
         mat4 world_from_model = model->frame()->world_from_local().matrix();

         m_debug_renderer->set_model(world_from_model);
         m_debug_renderer->add_box(geometry_mesh->min(), geometry_mesh->max());
      }
      m_debug_renderer->end_edit();
      m_debug_renderer->render();
   }

#endif

   //r.track.reset();

#if 0 // just blit one rt to screen
   int iw = m_application->width();
   int ih = m_application->height();

   gl::bind_framebuffer(gl::framebuffer_target::read_framebuffer, m_fbo);
   gl::read_buffer(GL_COLOR_ATTACHMENT1);

   gl::bind_framebuffer(gl::framebuffer_target::draw_framebuffer, 0);

   glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   gl::blit_framebuffer(
      0, 0, iw, ih,
      0, 0, iw / 6, ih / 6,
      GL_COLOR_BUFFER_BIT,
      GL_NEAREST);
#endif

   // restore default framebuffer
   {
      gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
      gl::bind_framebuffer(GL_READ_FRAMEBUFFER, 0);

#if defined(RENDERSTACK_GL_API_OPENGL)
      gl::draw_buffer(GL_BACK);
#endif

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
      GLenum draw_buffer = GL_BACK;
      gl::draw_buffers(1, &draw_buffer);
#endif
   }
}
void game::render()
{
   slog_trace("game::render()");

   int iw = m_application->width();
   int ih = m_application->height();

   gl::disable(gl::enable_cap::scissor_test); // TODO render state for this

   gl::viewport(0, 0, iw, ih);

   gl::clear_color(0.4f, 0.2f, 0.0f, 1.0f);
   gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   render_meshes();

   {
      gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
      gl::bind_framebuffer(GL_READ_FRAMEBUFFER, 0);

#if defined(RENDERSTACK_GL_API_OPENGL)
      gl::draw_buffer(GL_BACK);
#endif

#if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
      GLenum draw_buffer = GL_BACK;
      gl::draw_buffers(1, &draw_buffer);
#endif
   }

#if 1
   if (m_debug_renderer)
   {
      auto gr = m_gui_renderer;
      gr->prepare();
      gr->on_resize(iw, ih);

      m_debug_renderer->render_text_lines(m_viewport);
   }
#endif

   if ((m_controls.mouse_locked == false) && m_root_layer)
   {
      ui_context c;
      double x;
      double y;
      m_application->get_mouse_pos(x, y);
      c.mouse.x = static_cast<float>(x);
      c.mouse.y = static_cast<float>(ih - 1 - y);
      c.mouse_buttons[0] = (m_application->get_mouse_button(0) != 0);
      c.mouse_buttons[1] = (m_application->get_mouse_button(1) != 0);
      c.mouse_buttons[2] = (m_application->get_mouse_button(2) != 0);
      auto gr = m_gui_renderer;
      gr->prepare();
      gr->on_resize(iw, ih);
      
      m_root_layer->draw(c);
   }

   m_application->swap_buffers();
}

