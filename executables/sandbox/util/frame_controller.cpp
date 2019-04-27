#include "util/frame_controller.hpp"
#include "renderstack_scene/frame.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "util/controller.hpp"
#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>

using namespace renderstack::toolkit;
using namespace glm;

void frame_controller::set_position(vec3 position)
{
    m_position = position;
    update();
}

void frame_controller::set_elevation(float value)
{
    m_elevation = value;
    update();
}

void frame_controller::set_heading(float value)
{
    m_heading = value;
    create_rotation(m_heading, vec3_unit_y, m_heading_matrix);
    update();
}

vec3 frame_controller::position() const
{
    return m_position;
}

float frame_controller::elevation() const
{
    return m_elevation;
}

float frame_controller::heading() const
{
    return m_heading;
}


frame_controller::frame_controller()
{
    clear();
    rotate_x.set_damp(0.700f);
    rotate_y.set_damp(0.700f);
    rotate_z.set_damp(0.700f);
    rotate_x.set_max_delta(0.02f);
    rotate_y.set_max_delta(0.02f);
    rotate_z.set_max_delta(0.02f);
    translate_x.set_damp(0.92f);
    translate_y.set_damp(0.92f);
    translate_z.set_damp(0.92f);
    translate_x.set_max_delta(0.004f);
    translate_y.set_max_delta(0.004f);
    translate_z.set_max_delta(0.004f);
    speed_modifier.set_max_value(3.0f);
    speed_modifier.set_damp(0.92f);
    speed_modifier.set_max_delta(0.5f);

    update();
}

void frame_controller::set_frame(renderstack::scene::Frame *value)
{
    m_frame = value;

    if (!m_frame)
    {
        return;
    }

    //vec3 right  = m_frame->world_from_local().matrix * vec3_unit_x;
    //vec3 up     = m_frame->world_from_local().matrix * vec3_unit_y;
    vec3 back(m_frame->world_from_local.matrix() * vec4(vec3_unit_z, 0.0f));

    // Decompose frame transformation matrix to position, theta and phi
    m_position = vec3(m_frame->world_from_local.matrix() * vec4(0.0f, 0.0f, 0.0f, 0.0f));
    cartesian_to_spherical(vec3(m_frame->world_from_local.matrix() * vec4(vec3_unit_z, 1.0f)),
                           m_heading, m_elevation);

    create_rotation(m_heading, vec3_unit_y, m_heading_matrix);

    update();
}

renderstack::scene::Frame *frame_controller::frame()
{
    return m_frame;
}

void frame_controller::clear()
{
    translate_x.clear();
    translate_y.clear();
    translate_z.clear();
    rotate_x.clear();
    rotate_y.clear();
    rotate_z.clear();
}

void frame_controller::update()
{
    mat4 elevation_matrix;
    create_rotation(m_elevation, vec3_unit_x, elevation_matrix);
    m_rotation_matrix = m_heading_matrix * elevation_matrix;

    mat4 parent_from_local = m_rotation_matrix;
    //mat4 parent_to_local = transpose(local_to_parent);

    // HACK
    if (m_position.y < 0.03f)
    {
        m_position.y = 0.03f;
    }

    // Put translation to column 3
    parent_from_local[3] = vec4(m_position, 1.0f);

    // Put inverse translation to column 3
    /*parentToLocal._03 = parentToLocal._00 * -positionInParent.X + parentToLocal._01 * -positionInParent.Y + parentToLocal._02 * - positionInParent.Z;
   parentToLocal._13 = parentToLocal._10 * -positionInParent.X + parentToLocal._11 * -positionInParent.Y + parentToLocal._12 * - positionInParent.Z;
   parentToLocal._23 = parentToLocal._20 * -positionInParent.X + parentToLocal._21 * -positionInParent.Y + parentToLocal._22 * - positionInParent.Z;
   parentToLocal._33 = 1.0f;
   */
    //m_local_from_parent = inverse(m_parent_from_local);
    if (m_frame)
    {
        m_frame->parent_from_local.set(parent_from_local);
    }

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
    translate_x.update();
    translate_y.update();
    translate_z.update();
    rotate_x.update();
    rotate_y.update();
    rotate_z.update();
    speed_modifier.update();

    float speed = 0.8f + speed_modifier.current_value();

    if (translate_x.current_value() != 0.0f)
    {
        m_position += right() * translate_x.current_value() * speed;
    }

    if (translate_y.current_value() != 0.0f)
    {
        m_position += up() * translate_y.current_value() * speed;
    }

    if (translate_z.current_value() != 0.0f)
    {
        m_position += back() * translate_z.current_value() * speed;
    }

    if ((rotate_x.current_value() != 0.0f) ||
        (rotate_y.current_value() != 0.0f))
    {
        m_heading += rotate_y.current_value();
        m_elevation += rotate_x.current_value();
        mat4 elevation_matrix;
        create_rotation(m_elevation, vec3_unit_x, elevation_matrix);
        create_rotation(m_heading, vec3_unit_y, m_heading_matrix);
        m_rotation_matrix = m_heading_matrix * elevation_matrix;
    }

    update();
#if 0
   m_parent_from_local = m_rotation_matrix;
   //Matrix4 parentToLocal;

   //Matrix4.Transpose(localToParent, out parentToLocal);

   // HACK
   if (m_position.y < 0.03f)
      m_position.y = 0.03f;

   /*  Put translation to column 3  */ 
   m_parent_from_local[3] = vec4(m_position, 1.0f);

#    if 0
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
#    endif

   m_local_from_parent = inverse(m_parent_from_local);
#endif
}
