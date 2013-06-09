#include "renderstack_toolkit/platform.hpp"
#include "main/game.hpp"
#include "main/application.hpp"
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
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/slider.hpp"
#include "renderstack_ui/text_buffer.hpp"
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

void game::render_ui()
{
   slog_trace("game::render_ui()");

   assert(m_programs);
   assert(m_text_buffer);

   float w = (float)m_application->width();
   float h = (float)m_application->height();

   m_text_buffer->begin_print();
   for (size_t i = 0; i < m_debug_lines.size(); ++i)
      m_text_buffer->print(m_debug_lines[i], 0.0f, h - (i + 1) * m_font->line_height());

   if (m_controls.mouse_locked)
      m_text_buffer->print("Mouse locked", 0.0f, 0.0f);

   int chars_printed = m_text_buffer->end_print();

   if (chars_printed > 0)
   {
      //auto gr = *m_gui_renderer;
      auto &r = *m_renderer;
      auto &t = r.track();
      t.execute(&m_font_render_states);

      auto p = m_programs->font;
      r.set_program(p);

      gl::viewport(0, 0, (GLsizei)w, (GLsizei)h);
      mat4 ortho = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
      gl::scissor(0, 0, (GLsizei)w, (GLsizei)h);

      glm::vec4 white(1.0f, 1.0f, 1.0f, 0.66f); // gamma in 4th component
      if (m_programs->use_uniform_buffers())
      {
         assert(m_text_uniform_buffer_range);

         uniform_offsets &o = m_programs->uniform_offsets;
         r.set_uniform_buffer_range(
            m_programs->block->binding_point(),
            m_text_uniform_buffer_range);
         unsigned char *start = m_text_uniform_buffer_range->begin_edit(r);
         ::memcpy(&start[o.clip_from_model], value_ptr(ortho), 16 * sizeof(float));
         ::memcpy(&start[o.color],           value_ptr(white), 4 * sizeof(float));
         m_text_uniform_buffer_range->end_edit(r);
      }
      else
      {
         int model_to_clip_ui = p->uniform_at(m_programs->uniform_keys.clip_from_model);
         int color_ui         = p->uniform_at(m_programs->uniform_keys.color);

         gl::uniform_matrix_4fv(model_to_clip_ui, 1, GL_FALSE, value_ptr(ortho));
         gl::uniform_4fv(color_ui, 1, value_ptr(white));
      }

      //int texture_ui = p->uniform("font_texture")->index();
      //gl::uniform_1i(texture_ui, 0);

      {
         auto t = m_text_buffer->font()->texture();
         t->set_min_filter(texture_min_filter::nearest);
         t->set_mag_filter(texture_mag_filter::nearest);
         (void)r.set_texture(0, t);
         t->apply(r, 0);
      }

      m_text_buffer->render();
   }

}

void game::render_meshes()
{
   slog_trace("game::render_meshes()");

#if 1
   m_deferred_renderer->geometry_pass(
      m_models,
      m_controls.clip_from_world,
      m_controls.camera_controller.local_from_parent()
   );
   m_deferred_renderer->light_pass(
      m_controls.camera_controller.parent_from_local()
   );

   m_deferred_renderer->show_rt();
#else
   m_forward_renderer->render_pass(
      m_models,
      m_controls.clip_from_world,
      m_controls.camera_controller.local_from_parent()
   );
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

   glClearColor(0.4f, 0.2f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   render_meshes();

#if 1
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

#if 0
   if (m_text_buffer)
   {
      auto gr = m_gui_renderer;
      gr->prepare();
      gr->on_resize(iw, ih);

      render_ui();
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
#endif

   m_application->swap_buffers();
}

