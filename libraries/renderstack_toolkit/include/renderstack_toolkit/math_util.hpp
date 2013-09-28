#ifndef math_util_hpp_renderstack_toolkit
#define math_util_hpp_renderstack_toolkit

#include "renderstack_toolkit/platform.hpp"
#include <cstdint>
#include <glm/glm.hpp>

namespace renderstack { namespace toolkit {

inline int round(float num)
{
   return (int)((num > 0.0f) ? std::floor(num + 0.5f) : std::ceil(num - 0.5f));
}

inline int dround(double num)
{
   return (int)((num > 0.0) ? std::floor(num + 0.5) : std::ceil(num - 0.5));
}

inline uint32_t next_power_of_two(uint32_t x)
{
   x--;
   x |= x >> 1;  // handle  2 bit numbers
   x |= x >> 2;  // handle  4 bit numbers
   x |= x >> 4;  // handle  8 bit numbers
   x |= x >> 8;  // handle 16 bit numbers
   x |= x >> 16; // handle 32 bit numbers
   x++;

   return x;
}

inline uint32_t uint_from_vector3(glm::vec3 const &v)
{
   float rf = v.x * 255.0f;
   float gf = v.y * 255.0f;
   float bf = v.z * 255.0f;
   int   r  = round(rf) << 16;
   int   g  = round(gf) <<  8;
   int   b  = round(bf) <<  0;
   int   i  = r | g | b;

   return static_cast<uint32_t>(i);
}

inline glm::vec3 vec3_from_uint(uint32_t i)
{
   uint32_t r = (i >> 16) & 0xff;
   uint32_t g = (i >>  8) & 0xff;
   uint32_t b = (i >>  0) & 0xff;

   return glm::vec3(
      r / 255.0f,
      g / 255.0f,
      b / 255.0f
   );
}

const glm::vec3 vec3_unit_x(1.0f, 0.0f, 0.0f);
const glm::vec3 vec3_unit_y(0.0f, 1.0f, 0.0f);
const glm::vec3 vec3_unit_z(0.0f, 0.0f, 1.0f);

inline glm::vec3 max_axis(glm::vec3 v)
{
   if (std::abs(v.x) >= std::abs(v.y) && std::abs(v.x) >= std::abs(v.z))
      return vec3_unit_x;
   if (std::abs(v.y) >= std::abs(v.x) && std::abs(v.y) >= std::abs(v.z))
      return vec3_unit_y;
   return vec3_unit_z;
}
inline glm::vec3 min_axis(glm::vec3 v)
{
   if (std::abs(v.x) <= std::abs(v.y) && std::abs(v.x) <= std::abs(v.z))
      return vec3_unit_x;
   if (std::abs(v.y) <= std::abs(v.x) && std::abs(v.y) <= std::abs(v.z))
      return vec3_unit_y;
   return vec3_unit_z;
}

const glm::mat4 mat4_identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
const glm::mat4 mat4_swap_xy(
   0.0f, 1.0f, 0.0f, 0.0f,
   1.0f, 0.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 1.0f, 0.0f,
   0.0f, 0.0f, 0.0f, 1.0f);
const glm::mat4 mat4_rotate_xy_cw(
   0.0f, -1.0f,  0.0f,  0.0f,
   1.0f,  0.0f,  0.0f,  0.0f,
   0.0f,  0.0f,  1.0f,  0.0f,
   0.0f,  0.0f,  0.0f,  1.0f);
const glm::mat4 mat4_rotate_xz_cw(
   0.0f,  0.0f, -1.0f,  0.0f,
   0.0f,  1.0f,  0.0f,  0.0f,
   1.0f,  0.0f,  0.0f,  0.0f,
   0.0f,  0.0f,  0.0f,  1.0f);

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
extern void cartesian_to_spherical(glm::vec3 v, float &heading, float &elevation);
extern glm::vec3 spherical_to_cartesian(float theta, float phi);

} }

#endif
