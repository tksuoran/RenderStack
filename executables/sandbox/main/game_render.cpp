#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/layer.hpp"
#include "renderstack_ui/slider.hpp"
#include "renderstack_ui/text_buffer.hpp"

#include "main/application.hpp"
#include "main/game.hpp"
#include "main/log.hpp"

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>

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

void Game::render_meshes()
{
    slog_trace("Game::render_meshes()\n");

    if (m_debug_renderer)
    {
        m_debug_renderer->printf("%d lights %s%s%s",
                                 m_max_lights,
                                 m_forward ? "forward " : "",
                                 m_deferred ? "deferred " : "",
                                 m_deferred && m_deferred_renderer->use_stencil() ? "stencil optimization" : "");
    }

    if (true && m_deferred && m_deferred_renderer)
    {
        m_deferred_renderer->set_max_lights(m_max_lights);
        m_deferred_renderer->geometry_pass(m_scene_manager->materials(), m_scene_manager->models(), m_scene_manager->camera());
        m_deferred_renderer->light_pass(m_scene_manager->lights(), m_scene_manager->camera());
    }

    if (true && m_forward && m_forward_renderer)
    {
        m_forward_renderer->set_max_lights(m_max_lights);
        m_forward_renderer->render_pass(m_scene_manager->materials(),
                                        m_scene_manager->models(),
                                        m_scene_manager->lights(),
                                        m_scene_manager->camera());
    }

    if (true && m_debug_lights && m_light_debug_renderer)
    {
        m_light_debug_renderer->set_max_lights(m_max_lights);
        m_light_debug_renderer->light_pass(m_scene_manager->lights(),
                                           m_scene_manager->camera());
    }

    if (true && m_debug_renderer)
    {
        m_debug_renderer->add_frame_duration_graph(m_viewport);
        m_debug_renderer->render();
        //r.track.reset();
    }
}

void Game::render()
{
    slog_trace("Game::render()\n");

    int iw = m_application->width();
    int ih = m_application->height();

    // gl::disable(gl::enable_cap::scissor_test); // TODO render state for this
    //
    // gl::viewport(0, 0, iw, ih);
    //
    // gl::clear_color(0.4f, 0.2f, 0.0f, 1.0f);
    // gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

    if (true && m_debug_renderer)
    {
        auto gr = m_gui_renderer;
        gr->prepare();
        gr->on_resize(iw, ih);

        m_debug_renderer->printf("%s", gl::get_string(GL_VENDOR));
        m_debug_renderer->printf("%s", gl::get_string(GL_RENDERER));
        m_debug_renderer->printf("%s", gl::get_string(GL_VERSION));
        m_debug_renderer->printf("%s", gl::get_string(GL_SHADING_LANGUAGE_VERSION));

        m_debug_renderer->render_text_lines(m_viewport);
    }

    if ((m_controls.mouse_locked == false) && m_root_layer)
    {
        ui_context c;
        double     x;
        double     y;
        m_application->get_mouse_pos(x, y);
        c.mouse.x          = static_cast<float>(x);
        c.mouse.y          = static_cast<float>(ih - 1 - y);
        c.mouse_buttons[0] = (m_application->get_mouse_button(0) != 0);
        c.mouse_buttons[1] = (m_application->get_mouse_button(1) != 0);
        c.mouse_buttons[2] = (m_application->get_mouse_button(2) != 0);
        auto gr            = m_gui_renderer;
        gr->prepare();
        gr->on_resize(iw, ih);

        m_root_layer->draw(c);
    }

    //log_error("\n\n\n - - - - -\n\n\n");
    m_application->swap_buffers();
}

// ID render pass
#if 0
   shared_ptr<Model> hover_model;
   {
      double dx;
      double dy;

      // int iw = m_application->width();
      int ih = m_application->height();

      m_application->get_mouse_pos(dx, dy);
#    if 1
      int mouse_x = static_cast<int>(dx);
      int mouse_y = static_cast<int>(ih - 1 - dy);

      int x = mouse_x;
      int y = mouse_y;
#    else
      int center_x = iw / 2;
      int center_y = ih / 2;

      int x = center_x;
      int y = center_y;
#    endif

      m_id_renderer->clear();
      m_id_renderer->render_pass(
         m_scene_manager->models(),
         m_scene_manager->camera(),
         m_application->time(),
         x,
         y
      );
      uint32_t id = 0xffffffffu;
      float depth = 1.0f;
      bool got = m_id_renderer->get(x, y, id, depth);
      hover_model = m_id_renderer->get(x, y);
      if (got)
      {
         
         vec3 pos = unproject(
            m_scene_manager->camera()->clip_from_world().inverse_matrix(),
            static_cast<float>(x),
            static_cast<float>(y),
            depth,
            0.0f,
            0.0f,
            static_cast<float>(m_application->width()),
            static_cast<float>(m_application->height())
         );
         //m_debug_renderer->printf(x, y, "Hover X: % 7.2f Y: % 7.2f Z: % 7.2f", pos.x, pos.y, pos.z);
         if (hover_model)
            m_debug_renderer->printf(x, y, hover_model->name().c_str());
      }
      if (m_manipulator_frame && hover_model)
         m_manipulator_frame->set_parent(hover_model->frame());
   }
#endif

#if 0
   if (hover_model && m_debug_renderer)
   {
      m_debug_renderer->set_camera(m_scene_manager->camera());
      m_debug_renderer->begin_edit();
      m_debug_renderer->set_color(vec4(0.0f, 1.0f, 0.0f, 1.0f));
      //for (auto i = m_models.cbegin(); i != m_models.cend(); ++i)
      {
         //auto model = *i;
         auto model = hover_model;
         auto Geometry_mesh = model->Geometry_mesh();
         mat4 world_from_model = model->frame()->world_from_local().matrix();

         m_debug_renderer->set_model(world_from_model);
         m_debug_renderer->add_box(Geometry_mesh->min(), Geometry_mesh->max());
      }
      m_debug_renderer->end_edit();
      m_debug_renderer->render();
   }

#endif
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
//    {
//       gl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
//       gl::bind_framebuffer(GL_READ_FRAMEBUFFER, 0);
//
// #if defined(RENDERSTACK_GL_API_OPENGL)
//       gl::draw_buffer(GL_BACK);
// #endif
//
// #if defined(RENDERSTACK_GL_API_OPENGL_ES_3)
//       GLenum draw_buffer = GL_BACK;
//       gl::draw_buffers(1, &draw_buffer);
// #endif
//    }
