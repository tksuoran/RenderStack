#include "renderstack_scene/transform.hpp"

namespace renderstack
{
namespace scene
{

using namespace renderstack::toolkit;
using namespace glm;

transform::transform(transform const &t)
{
    m_matrix         = t.m_matrix;
    m_inverse_matrix = t.m_inverse_matrix;
}
transform::transform()
{
    m_inverse_matrix = m_matrix = mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}
transform::transform(mat4 const &m)
{
    m_matrix         = m;
    m_inverse_matrix = inverse(m);
}
transform::transform(mat4 const &matrix, mat4 const &inverse_matrix)
{
    m_matrix         = matrix;
    m_inverse_matrix = inverse_matrix;
}

void transform::set_translation(vec3 const &v)
{
    create_translation(v, m_matrix);
    create_translation(-v, m_inverse_matrix);
}
void transform::fix_inverse()
{
    m_inverse_matrix = inverse(m_matrix);
}
void transform::set_translation(float x, float y, float z)
{
    create_translation(x, y, z, m_matrix);
    create_translation(-x, -y, -z, m_inverse_matrix);
}
void transform::set_rotation(float angle_radians, vec3 const &axis)
{
    create_rotation(angle_radians, axis, m_matrix);
    create_rotation(-angle_radians, axis, m_inverse_matrix);
}
void transform::set_scale(float s)
{
    create_scale(s, m_matrix);
    create_scale(1.0f / s, m_inverse_matrix);
}
void transform::set_scale(float x, float y, float z)
{
    create_scale(x, y, z, m_matrix);
    create_scale(1.0f / x, 1.0f / y, 1.0f / z, m_inverse_matrix);
}

void transform::set_projection(
    float s,          //  Stereo-scopic 3D eye separation
    float p,          //  Perspective (0 == parallel, 1 == perspective)
    float n, float f, //  Near and far z clip depths
    float w, float h, //  Width and height of viewport (at depth vz)
    vec3 const &v,    //  Center of viewport
    vec3 const &e     //  Center of projection (eye position)
)
{
    create_projection(s, p, n, f, w, h, v, e, m_matrix);
    m_inverse_matrix = inverse(m_matrix);
}
void transform::set_orthographic(float left, float right, float bottom, float top, float near_, float far_)
{
    create_orthographic(left, right, bottom, top, near_, far_, m_matrix);
    m_inverse_matrix = inverse(m_matrix);
}
void transform::set_orthographic_centered(float width, float height, float near_, float far_)
{
    create_orthographic_centered(width, height, near_, far_, m_matrix);
    m_inverse_matrix = inverse(m_matrix);
}
void transform::set_frustum(float left, float right, float bottom, float top, float near_, float far_)
{
    create_frustum(left, right, bottom, top, near_, far_, m_matrix);
    m_inverse_matrix = inverse(m_matrix);
}
void transform::set_frustum_simple(float width, float height, float near_, float far_)
{
    create_frustum_simple(width, height, near_, far_, m_matrix);
    m_inverse_matrix = inverse(m_matrix);
}
void transform::set_perspective(float fov_x, float fov_y, float near_, float far_)
{
    create_perspective(fov_x, fov_y, near_, far_, m_matrix);
    m_inverse_matrix = inverse(m_matrix);
}
void transform::set_perspective_vertical(float fov_y, float aspect_ratio, float near_, float far_)
{
    create_perspective_vertical(fov_y, aspect_ratio, near_, far_, m_matrix);
    m_inverse_matrix = inverse(m_matrix);
}
void transform::set_perspective_horizontal(float fov_x, float aspect_ratio, float near_, float far_)
{
    create_perspective_horizontal(fov_x, aspect_ratio, near_, far_, m_matrix);
    m_inverse_matrix = inverse(m_matrix);
}

void transform::set(mat4 const &matrix)
{
    m_matrix         = matrix;
    m_inverse_matrix = inverse(matrix);
}
void transform::set(mat4 const &matrix, mat4 const &inverse_matrix)
{
    m_matrix         = matrix;
    m_inverse_matrix = inverse_matrix;
}
void transform::catenate(mat4 const &m)
{
    m_matrix         = m_matrix * m;
    m_inverse_matrix = inverse(m_matrix);
}

} // namespace scene
} // namespace renderstack
