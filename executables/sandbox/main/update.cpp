#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/light.hpp"

#include "main/game.hpp"
#include "main/programs.hpp"
#include "main/application.hpp"

using namespace renderstack;
using namespace renderstack::toolkit;
using namespace std;
using namespace glm;

void game::update()
{
   //if (!m_paused)
   {
      m_frame_dt = -m_update_time;
      double new_time = m_application->time();
      if (new_time < m_update_time)
         printf("new time %f < prev time %f\n", new_time, m_update_time);
      m_update_time = new_time;
      m_frame_dt += m_update_time;
      if (m_frame_dt < m_min_frame_dt)
         m_min_frame_dt = m_frame_dt;

      if (m_frame_dt > m_max_frame_dt)
         m_max_frame_dt = m_frame_dt;
   }

   m_debug_renderer->clear_text_lines();
   m_debug_renderer->record_frame_duration(static_cast<float>(m_frame_dt));

   //if (!m_paused)
   {
      update_fixed_steps();
   }

   update_once_per_frame();
   render();
}

void game::update_fixed_steps()
{
   double dt = 1.0 / 120.0;
   int updates = 0;

   while (m_simulation_time < m_update_time)
   {
      update_fixed_step();
      m_simulation_time += dt;
      ++updates;

#if 0 // TODO
      if (updates > 100000)
         throw runtime_error("oh no!");
#endif
   }
}

void game::update_fixed_step()
{
   m_programs->update_fixed_step();
   m_controls.update_fixed_step();
}

void controls::update_fixed_step()
{
   camera_controller.update_fixed_step();
}

void game::update_once_per_frame()
{
   static int counter = 0;

   if (m_shader_monitor)
   {
      ++counter;
      if (counter > 20)
      {
         m_shader_monitor->poll();
         counter = 0;
      }
   }

   m_controls.camera_controller.update();

   m_scene_manager->camera()->update(m_viewport);

#if 1
   if (!m_paused)
   {
      auto &lights = m_scene_manager->lights();
      int n_lights = lights->size();
      int light_index = 0;
      for (auto i = lights->cbegin(); i != lights->cend(); ++i)
      {
         auto l = *i;

         float rel = static_cast<float>(light_index) / static_cast<float>(n_lights);
         float t = 1.8f * static_cast<float>(m_simulation_time) + rel * pi<float>() * 2.0f;
         mat4 m;

         light_index++;

         vec3 eye = vec3(
            l->frame()->world_from_local().matrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f)
         );
         eye.y = 13.0f + 6.0f * std::sin(t * 1.1f);
         vec3 center = eye + vec3(
             3.0f * std::sin(t * 0.9f),
             0.0f,
             1.0f * std::cos(t * 0.7f)
         );
         center.y = 0.0f;

         create_look_at(
            eye,
            center,
            vec3(0.0f, 0.0f, 1.0f), // up
            m
         );
         l->frame()->parent_from_local().set(m);
         l->frame()->update_hierarchical_no_cache();
      }
   }
#endif

#if 0
   // local = view
   // parent = world

   mat4 const &view_from_world = m_controls.camera_controller.local_from_parent();
   m_controls.clip_from_world = m_camera->clip_from_view().matrix() * view_from_world;

   m_controls.world_from_clip = glm::inverse(
      m_controls.clip_from_world
   );
#endif

   //m_controls.world_from_clip = m_camera->clip_from_world().inverse_matrix();

}
