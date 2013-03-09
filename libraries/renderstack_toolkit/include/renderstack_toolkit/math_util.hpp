#ifndef renderstack_toolkit__math_util_hpp
#define renderstack_toolkit__math_util_hpp

#include "platform.hpp"
#include <glm/glm.hpp>

namespace renderstack {

inline int round(float num)
{
    return (int)((num > 0.0f) ? std::floor(num + 0.5f) : std::ceil(num - 0.5f));
}

inline unsigned int uint_from_vector3(glm::vec3 const &v)
{
    float  rf  = v.x * 255.0f;
    float  gf  = v.y * 255.0f;
    float  bf  = v.z * 255.0f;
    int    r   = round(rf) << 16;
    int    g   = round(gf) <<  8;
    int    b   = round(bf) <<  0;
    int    i   = r | g | b;

    return static_cast<unsigned int>(i);
}

inline glm::vec3 vec3_from_uint(unsigned int i)
{
    unsigned int r = (i >> 16) & 0xff;
    unsigned int g = (i >>  8) & 0xff;
    unsigned int b = (i >>  0) & 0xff;

    return glm::vec3(
        r / 255.0f,
        g / 255.0f,
        b / 255.0f
    );
}

const glm::vec3 vec3_unit_x(1.0f, 0.0f, 0.0f);
const glm::vec3 vec3_unit_y(0.0f, 1.0f, 0.0f);
const glm::vec3 vec3_unit_z(0.0f, 0.0f, 1.0f);
const glm::mat4 mat4_identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
const glm::mat4 mat4_swap_xy(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
const glm::mat4 mat4_rotate_xy_cw(0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

glm::vec3 unproject(
    glm::mat4 const &inverse_model_view_projection,
    float win_x, 
    float win_y, 
    float win_z, 
    float viewport_x,
    float viewport_y,
    float viewport_width,
    float viewport_height
);
glm::vec2 project_to_screen_space(
    glm::mat4 const  &model_view_projection,
    glm::vec3 const  &position_in_world, 
    float            viewport_x,
    float            viewport_y,
    float            viewport_width,
    float            viewport_height,
    float            &depth
);

extern glm::vec3 const& max_axis(glm::vec3 const &v);
extern glm::vec3 const& min_axis(glm::vec3 const &v);
extern void create_frustum(float left, float right, float bottom, float top, float near_, float far_, glm::mat4 &result);
extern void create_frustum_simple(float width, float height, float near_, float far_, glm::mat4 &result);
extern void create_perspective(float fov_x, float fov_y, float near_, float far_, glm::mat4 &result);
extern void create_perspective_vertical(float fov_y, float aspect_ratio, float near_, float far_, glm::mat4 &result);
extern void create_perspective_horizontal(float fov_x, float aspect_ratio, float near_, float far_, glm::mat4 &result);
extern void create_projection(float s, float p, float n, float f, float w, float h, glm::vec3 const &v, glm::vec3 const &e, glm::mat4 &result);
extern void create_orthographic(float left, float right, float bottom, float top, float near_, float far_, glm::mat4 &result);
extern void create_orthographic_centered(float width, float height, float near_, float far_, glm::mat4 &result);
extern void create_translation(glm::vec2 const &t, glm::mat4 &result);
extern void create_translation(glm::vec3 const &t, glm::mat4 &result);
extern void create_translation(float x, float y, float z, glm::mat4 &result);
extern void create_rotation(float angle_radians, glm::vec3 const &axis, glm::mat4 &result);
extern void create_scale(float x, float y, float z, glm::mat4 &result);
extern void create_scale(float s, glm::mat4 &result);
extern void create_look_at(glm::vec3 const& eye, glm::vec3 const& center, glm::vec3 up0, glm::mat4 &result);
extern float degrees_to_radians(float degrees);
extern void hsv_to_rgb(float h, float s, float v, float &r, float &g, float &b);
extern void rgb_to_hsv(float r, float g, float b, float &h, float &s, float &v);
extern float srgb_to_linear(float cs);
extern float linear_rgb_to_srgb(float cl);
extern glm::vec3 srgb_to_linear_rgb(glm::vec3 const &srgb);
extern glm::vec3 linear_rgb_to_srgb(glm::vec3 const linear_rgb);

}

#endif
