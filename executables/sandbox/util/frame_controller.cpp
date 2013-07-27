#include "renderstack_toolkit/platform.hpp"
#include "util/frame_controller.hpp"
#include "util/controller.hpp"
#include "renderstack_toolkit/math_util.hpp"
#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>

using namespace glm;
using namespace renderstack;

void frame_controller::set_position(vec3 const &position)
{
   m_position = position;
   update();
}

void frame_controller::set_elevation(float value)
{
   m_elevation = value;
}
void frame_controller::set_heading(float value)
{
   m_heading = value;
}
vec3 const &frame_controller::position() const { return m_position; }
float frame_controller::elevation() const { return m_elevation; }
float frame_controller::heading() const { return m_heading; }

controller &frame_controller::rotate_x      (){ return m_rotate_x; }
controller &frame_controller::rotate_y      (){ return m_rotate_y; }
controller &frame_controller::rotate_z      (){ return m_rotate_z; }
controller &frame_controller::translate_x   (){ return m_translate_x; }
controller &frame_controller::translate_y   (){ return m_translate_y; }
controller &frame_controller::translate_z   (){ return m_translate_z; }
controller &frame_controller::speed_modifier(){ return m_speed_modifier; }

frame_controller::frame_controller()
{
   clear();
   m_rotate_x.set_damp(0.700f);
   m_rotate_y.set_damp(0.700f);
   m_rotate_z.set_damp(0.700f);
   m_rotate_x.set_max_delta(0.02f);
   m_rotate_y.set_max_delta(0.02f);
   m_rotate_z.set_max_delta(0.02f);
   m_translate_x.set_damp(0.92f);
   m_translate_y.set_damp(0.92f);
   m_translate_z.set_damp(0.92f);
   m_translate_x.set_max_delta(0.004f);
   m_translate_y.set_max_delta(0.004f);
   m_translate_z.set_max_delta(0.004f);
   m_speed_modifier.set_max_value(3.0f);
   m_speed_modifier.set_damp(0.92f);
   m_speed_modifier.set_max_delta(0.5f);

   m_elevation = 0.0f;
   m_heading = 0.0f;
   update();
}

void frame_controller::clear()
{
   m_translate_x.clear();
   m_translate_y.clear();
   m_translate_z.clear();
   m_rotate_x.clear();
   m_rotate_y.clear();
   m_rotate_z.clear();
}

void frame_controller::update()
{
   mat4 elevation_matrix;
   create_rotation(m_elevation, vec3_unit_x, elevation_matrix);
   m_rotation_matrix = m_heading_matrix * elevation_matrix;

   m_parent_from_local = m_rotation_matrix;
   //mat4 parent_to_local = transpose(local_to_parent);

   // HACK
   if (m_position.y < 0.03f)
      m_position.y = 0.03f;

   /*  Put translation to column 3  */ 
   m_parent_from_local[3] = vec4(m_position, 1.0f);

   /*  Put inverse translation to column 3 */ 
   /*parentToLocal._03 = parentToLocal._00 * -positionInParent.X + parentToLocal._01 * -positionInParent.Y + parentToLocal._02 * - positionInParent.Z;
   parentToLocal._13 = parentToLocal._10 * -positionInParent.X + parentToLocal._11 * -positionInParent.Y + parentToLocal._12 * - positionInParent.Z;
   parentToLocal._23 = parentToLocal._20 * -positionInParent.X + parentToLocal._21 * -positionInParent.Y + parentToLocal._22 * - positionInParent.Z;
   parentToLocal._33 = 1.0f;
   */
   m_local_from_parent = inverse(m_parent_from_local);

   //Frame.LocalToParent.Set(localToParent, parentToLocal);
}

vec3 frame_controller::right() const
{
   return vec3(m_heading_matrix[0].x, m_heading_matrix[0].y, m_heading_matrix[0].z);
}

vec3 frame_controller::up() const
{
   return vec3(m_heading_matrix[1].x, m_heading_matrix[1].y, m_heading_matrix[1].z);
}

vec3 frame_controller::back() const
{
   return vec3(m_heading_matrix[2].x, m_heading_matrix[2].y, m_heading_matrix[2].z);
}

void frame_controller::update_fixed_step()
{
   m_translate_x.update();
   m_translate_y.update();
   m_translate_z.update();
   m_rotate_x.update();
   m_rotate_y.update();
   m_rotate_z.update();
   m_speed_modifier.update();

   float speed = 0.8f + m_speed_modifier.current_value();

   if (m_translate_x.current_value() != 0.0f)
      m_position += right() * m_translate_x.current_value() * speed;

   if (m_translate_y.current_value() != 0.0f)
      m_position += up() * m_translate_y.current_value() * speed;

   if (m_translate_z.current_value() != 0.0f)
      m_position += back() * m_translate_z.current_value() * speed;

   if (
      (m_rotate_x.current_value() != 0.0f) ||
      (m_rotate_y.current_value() != 0.0f) 
   )
   {
      m_heading += m_rotate_y.current_value();
      m_elevation += m_rotate_x.current_value();
      mat4 elevation_matrix;
      create_rotation(m_elevation, vec3_unit_x, elevation_matrix);
      create_rotation(m_heading, vec3_unit_y, m_heading_matrix);
      m_rotation_matrix = m_heading_matrix * elevation_matrix;
   }

   m_parent_from_local = m_rotation_matrix;
   //Matrix4 parentToLocal;

   //Matrix4.Transpose(localToParent, out parentToLocal);

   // HACK
   if (m_position.y < 0.03f)
      m_position.y = 0.03f;

   /*  Put translation to column 3  */ 
   m_parent_from_local[3] = vec4(m_position, 1.0f);

#if 0
   localToParent._03 = positionInParent.X;
   localToParent._13 = positionInParent.Y;
   localToParent._23 = positionInParent.Z;
   localToParent._33 = 1.0f;

   /*  Put inverse translation to column 3 */ 
   parentToLocal._03 = parentToLocal._00 * -positionInParent.X + parentToLocal._01 * -positionInParent.Y + parentToLocal._02 * - positionInParent.Z;
   parentToLocal._13 = parentToLocal._10 * -positionInParent.X + parentToLocal._11 * -positionInParent.Y + parentToLocal._12 * - positionInParent.Z;
   parentToLocal._23 = parentToLocal._20 * -positionInParent.X + parentToLocal._21 * -positionInParent.Y + parentToLocal._22 * - positionInParent.Z;
   parentToLocal._33 = 1.0f;

   Frame.LocalToParent.Set(localToParent, parentToLocal);
#endif

   m_local_from_parent = inverse(m_parent_from_local);
}


