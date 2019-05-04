#ifndef transform_hpp_renderstack_scene
#define transform_hpp_renderstack_scene

#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>

namespace renderstack
{
namespace scene
{

class Transform
{
public:
    Transform() = default;

    Transform(const Transform  &t);

    explicit Transform(glm::mat4 m);

    Transform(glm::mat4 matrix, glm::mat4 inverse_matrix);

    glm::mat4 matrix() const
    {
        return m_matrix;
    }

    glm::mat4 inverse_matrix() const
    {
        return m_inverse_matrix;
    }

    void set_translation(glm::vec3 v);

    void fix_inverse();

    void set_translation(float x, float y, float z);

    void set_rotation(float angle_radians, glm::vec3 axis);

    void set_scale(float s);

    void set_scale(float x, float y, float z);

    void set_projection(float s,           // Stereo-scopic 3D eye separation
                        float p,           // Perspective (0 == parallel, 1 == perspective)
                        float n, float f,  // Near and far z clip depths
                        float w, float h,  // Width and height of viewport (at depth vz)
                        glm::vec3 v,       // Center of viewport
                        glm::vec3 e        // Center of projection (eye position)
    );

    void set_orthographic(float left, float right, float bottom, float top, float near_, float far_);

    void set_orthographic_centered(float width, float height, float near_, float far_);

    void set_frustum(float left, float right, float bottom, float top, float near_, float far_);

    void set_frustum_simple(float width, float height, float near_, float far_);

    void set_perspective(float fov_x, float fov_y, float near_, float far_);

    void set_perspective_vertical(float fov_y, float aspect_ratio, float near_, float far_);

    void set_perspective_horizontal(float fov_x, float aspect_ratio, float near_, float far_);

    void set(glm::mat4 matrix);

    void set(glm::mat4 matrix, glm::mat4 inverse_matrix);

    void catenate(glm::mat4 m);

private:
    glm::mat4 m_matrix{1.0f};
    glm::mat4 m_inverse_matrix{1.0f};
};

} // namespace scene
} // namespace renderstack

#endif
