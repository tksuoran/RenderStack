#include "renderstack_toolkit/platform.hpp"
#include "main/game.hpp"
#include "main/programs.hpp"
#include "main/application.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_toolkit/math_util.hpp"

using namespace renderstack;
using namespace renderstack::toolkit;
using namespace std;
using namespace glm;

void game::update()
{
   m_frame_dt = -m_update_time;
   double new_time = m_application->time();
   if (new_time < m_update_time)
      printf("new time %f < prev time %f\n", new_time, m_update_time);
   m_update_time = new_time;
   m_frame_dt += m_update_time;
   if (m_frame_dt < m_min_frame_dt) m_min_frame_dt = m_frame_dt;
   if (m_frame_dt > m_max_frame_dt) m_max_frame_dt = m_frame_dt;

   m_debug_lines.clear();

   update_fixed_steps();
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
      if (updates > 100000)
         throw runtime_error("oh no!");
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
   m_controls.camera_controller.update();

   m_controls.aspect = (float)m_application->width() / (float)m_application->height();

   create_perspective_vertical(
      m_controls.fov,
      m_controls.aspect,
      m_controls.near_,
      m_controls.far_,
      m_projection
   );

   // local = view
   // parent = world

   mat4 const &view_from_world = m_controls.camera_controller.local_from_parent();
   m_controls.clip_from_world = m_projection * view_from_world;

   m_controls.world_from_clip = glm::inverse(
      m_controls.clip_from_world
   );

}
