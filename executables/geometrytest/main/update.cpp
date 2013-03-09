#include "renderstack_toolkit/platform.hpp"
#include "main/game.hpp"
#include "main/programs.hpp"
#include "main/application.hpp"
#include "renderstack_toolkit/window.hpp"
#include "renderstack_toolkit/math_util.hpp"

using namespace renderstack;
using namespace renderstack::toolkit;
using namespace glm;

void game::update()
{
   m_frame_dt = -m_update_time;
   m_update_time = m_application->time();
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

   mat4 const &camera_world_to_view = m_controls.camera_controller.parent_to_local();
   m_controls.view_projection = m_projection * camera_world_to_view;

   m_controls.inverse_view_projection = glm::inverse(
      m_controls.view_projection
   );

}
