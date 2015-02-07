#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include <glm/gtc/constants.hpp>
#include <algorithm>
#include <stdexcept>

namespace renderstack { namespace toolkit {

using namespace glm;
using namespace std;

vec3 unproject(
   mat4  const &inverse_model_view_projection,
   float win_x,
   float win_y,
   float win_z,
   float viewport_x,
   float viewport_y,
   float viewport_width,
   float viewport_height
)
{
   vec4 in;
   vec4 out;

   in.x = (win_x - viewport_x + 0.5f) / viewport_width  * 2.0f - 1.0f;
   in.y = (win_y - viewport_y + 0.5f) / viewport_height * 2.0f - 1.0f;
   in.z = 2.0f * win_z - 1.0f;
   in.w = 1.0f;

   //  Objects coordinates
   out = inverse_model_view_projection * in;
   if (out.w == 0.0f)
      throw runtime_error("w singularity");

   return vec3(out.x / out.w, out.y / out.w, out.z / out.w);
}
vec2 project_to_screen_space(
   mat4 const  &model_view_projection,
   vec3 const  &position_in_world,
   float       viewport_x,
   float       viewport_y,
   float       viewport_width,
   float       viewport_height,
   float       &depth
)
{
   vec4 clip = model_view_projection * vec4(position_in_world, 1.0f);

   float depth_rangenear_    = 0.0f;
   float depth_rangefar_     = 1.0f;
   float halffar__minusnear_ = 0.5f * (depth_rangefar_ - depth_rangenear_);
   float halfnear__plusfar_  = 0.5f * (depth_rangenear_ + depth_rangefar_);

   vec3 ndc(
      clip.x / clip.w,
      clip.y / clip.w,
      clip.z / clip.w
   );

   depth = halffar__minusnear_ * ndc.z + halfnear__plusfar_;

   vec2 window = vec2(
      (0.5f + (ndc.x * 0.5f)) * viewport_width  + viewport_x,
      (0.5f + (ndc.y * 0.5f)) * viewport_height + viewport_y
   );

   return window;
}

void create_frustum(float left, float right, float bottom, float top, float near, float far, mat4 &result)
{
   float x, y, a, b, c, d;

   //  TODO Do we need to do something about potential division by zero?
   x =  (2.0f  * near)   / (right - left);
   y =  (2.0f  * near)   / (top   - bottom);
   a =  (right + left)   / (right - left);
   b =  (top   + bottom) / (top   - bottom);
   c = -(far   + near)   / (far   - near);
   d = -(2.0f * far * near) / (far - near);

   result[0][0] = x; result[1][0] = 0; result[2][0] = a;     result[3][0] = 0;
   result[0][1] = 0; result[1][1] = y; result[2][1] = b;     result[3][1] = 0;
   result[0][2] = 0; result[1][2] = 0; result[2][2] = c;     result[3][2] = d;
   result[0][3] = 0; result[1][3] = 0; result[2][3] = -1.0f; result[3][3] = 0;
}
void create_frustum_simple(float width, float height, float near_, float far_, mat4 &result)
{
   float x;
   float y;
   float c;
   float d;
   float near2;
   float far_plusnear__;
   float far_minusnear_;
   float far_timesnear_;

   near2          = 2.0f * near_;
   far_plusnear__ = far_ + near_;
   far_minusnear_ = far_ - near_;
   far_timesnear_ = far_ * near_;

   x = near2 / width;
   y = near2 / height;
   c = -       far_plusnear__ / far_minusnear_;
   d = -2.0f * far_timesnear_ / far_minusnear_;

   result[0][0] = x; result[1][0] = 0; result[2][0] = 0;     result[3][0] = 0;
   result[0][1] = 0; result[1][1] = y; result[2][1] = 0;     result[3][1] = 0;
   result[0][2] = 0; result[1][2] = 0; result[2][2] = c;     result[3][2] = d;
   result[0][3] = 0; result[1][3] = 0; result[2][3] = -1.0f; result[3][3] = 0;
}
void create_perspective(float fov_x, float fov_y, float near_, float far_, mat4 &result)
{
   fov_y = (std::max)(fov_y, 0.01f);
   fov_y = (std::min)(fov_y, (float)(3.14159265 * 0.99));
   fov_x = (std::max)(fov_x, 0.01f);
   fov_x = (std::min)(fov_x, (float)(3.14159265 * 0.99));
   float tanXHalfAngle = (float)tan(fov_x * 0.5);
   float tanYHalfAngle = (float)tan(fov_y * 0.5);
   float width         = 2.0f * near_ * tanXHalfAngle;
   float height        = 2.0f * near_ * tanYHalfAngle;
   create_frustum_simple(width, height, near_, far_, result);
}
void create_perspective_vertical(float fov_y, float aspect_ratio, float near_, float far_, mat4 &result)
{
   fov_y = std::max(fov_y, 0.01f);
   fov_y = std::min(fov_y, (float)(3.14159265 * 0.99));
   float tanHalfAngle = (float)tan(fov_y * 0.5);
   float height       = 2.0f * near_ * tanHalfAngle;
   float width        = height * aspect_ratio;

   create_frustum_simple(width, height, near_, far_, result);
}
void create_perspective_horizontal(float fov_x, float aspect_ratio, float near_, float far_, mat4 &result)
{
   fov_x = std::max(fov_x, 0.001f);
   fov_x = std::min(fov_x, (float)(3.14159265 * 0.999));
   float tan_half_angle = (float)tan(fov_x * 0.5);
   float width          = 2.0f * near_ * tan_half_angle;
   float height         = width / aspect_ratio;

   create_frustum_simple(width, height, near_, far_, result);
}
/*  http://and-what-happened.blogspot.com/p/just-formulas.html  */
/*  The projection produced by this formula has x, y and z extents of -1:+1.  */
/*  The perspective control value p is not restricted to integer values.  */ 
/*  The view plane is defined by z.  */
/*  Objects on the view plane will have a homogeneous w value of 1.0 after the transform. */ 
void create_projection(
   float s,             //  Stereo-scopic 3D eye separation
   float p,             //  Perspective (0 == parallel, 1 == perspective)
   float n, float f,    //  Near and far z clip depths
   float w, float h,    //  Width and height of viewport (at depth vz)
   vec3 const &v,       //  Center of viewport
   vec3 const &e,       //  Center of projection (eye position)
   mat4 &result
)
{
   result[0][0] =  2.0f / w;
   result[0][1] =  0.0f;
   result[0][2] = (2.0f * (e.x - v.x) + s) / (w * (v.z - e.z));
   result[0][3] = (2.0f * ((v.x * e.z) - (e.x * v.z)) - s * v.z) / (w * (v.z - e.z));

   result[1][0] =  0.0f;
   result[1][1] =  2.0f / h;
   result[1][2] =  2.0f * (e.y - v.y) / (h * (v.z - e.z));
   result[1][3] =  2.0f * ((v.y * e.z) - (e.y * v.z)) / (h * (v.z - e.z));

   result[2][0] =  0.0f;
   result[2][1] =  0.0f;
   result[2][2] = (2.0f * (v.z * (1.0f - p) - e.z) + p * (f + n)) / ((f - n) * (v.z - e.z));
   result[2][3] = -((v.z * (1.0f - p) - e.z) * (f + n) + 2.0f * f * n * p) / ((f - n) * (v.z - e.z));

   result[3][0] =  0.0f;
   result[3][1] =  0.0f;
   result[3][2] = p / (v.z - e.z);
   result[3][3] = (v.z * (1.0f - p) - e.z) / (v.z - e.z);

   /*  Changes handedness  */
   result[0][2] = -result[0][2];
   result[1][2] = -result[1][2];
   result[2][2] = -result[2][2];
   result[3][2] = -result[3][2];
}
void create_orthographic(float left, float right, float bottom, float top, float near, float far, mat4 &result)
{
   float width  = right - left;
   float height = top - bottom;
   float depth  = far - near;
   float x     =  2.0f / width;
   float y     =  2.0f / height;
   float z     = -2.0f / depth;
   float xt    = -(right + left) / width;
   float yt    = -(top + bottom) / height;
   float zt    = -(far + near) / depth;

   result[0][0] = x;    result[1][0] = 0.0f; result[2][0] = 0.0f; result[3][0] = xt;
   result[0][1] = 0.0f; result[1][1] = y;    result[2][1] = 0.0f; result[3][1] = yt;
   result[0][2] = 0.0f; result[1][2] = 0.0f; result[2][2] = z;    result[3][2] = zt;
   result[0][3] = 0.0f; result[1][3] = 0.0f; result[2][3] = 0.0f; result[3][3] = 1.0f;
}
void create_orthographic_centered(float width, float height, float near_, float far_, mat4 &result)
{
   float depth = far_ - near_;
   float faddn = far_ + near_;
   float x     =  2.0f / width;
   float y     =  2.0f / height;
   float z     = -2.0f / depth;
   float zt    = -faddn / depth;

   result[0][0] = x;    result[1][0] = 0.0f; result[2][0] = 0.0f; result[3][0] = 0.0f;
   result[0][1] = 0.0f; result[1][1] = y;    result[2][1] = 0.0f; result[3][1] = 0.0f;
   result[0][2] = 0.0f; result[1][2] = 0.0f; result[2][2] = z;    result[3][2] = zt;
   result[0][3] = 0.0f; result[1][3] = 0.0f; result[2][3] = 0.0f; result[3][3] = 1.0f;
}
void create_translation(vec2 const &t, mat4 &result)
{
   result[0][0] = 1.0f; result[1][0] = 0.0f; result[2][0] = 0.0f; result[3][0] = t.x; 
   result[0][1] = 0.0f; result[1][1] = 1.0f; result[2][1] = 0.0f; result[3][1] = t.y; 
   result[0][2] = 0.0f; result[1][2] = 0.0f; result[2][2] = 1.0f; result[3][2] = 0.0f;
   result[0][3] = 0.0f; result[1][3] = 0.0f; result[2][3] = 0.0f; result[3][3] = 1.0f;
}
void create_translation(glm::vec3 const &t, glm::mat4 &result)
{
   result[0][0] = 1.0f; result[1][0] = 0.0f; result[2][0] = 0.0f; result[3][0] = t.x; 
   result[0][1] = 0.0f; result[1][1] = 1.0f; result[2][1] = 0.0f; result[3][1] = t.y; 
   result[0][2] = 0.0f; result[1][2] = 0.0f; result[2][2] = 1.0f; result[3][2] = t.z;
   result[0][3] = 0.0f; result[1][3] = 0.0f; result[2][3] = 0.0f; result[3][3] = 1.0f;
}
void create_translation(float x, float y, float z, mat4 &result)
{
   result[0][0] = 1.0f; result[1][0] = 0.0f; result[2][0] = 0.0f; result[3][0] = x; 
   result[0][1] = 0.0f; result[1][1] = 1.0f; result[2][1] = 0.0f; result[3][1] = y; 
   result[0][2] = 0.0f; result[1][2] = 0.0f; result[2][2] = 1.0f; result[3][2] = z;
   result[0][3] = 0.0f; result[1][3] = 0.0f; result[2][3] = 0.0f; result[3][3] = 1.0f;
}
void create_rotation(float angle_radians, vec3 const &axis, mat4 &result)
{
   float rsin = sin(angle_radians);
   float rcos = cos(angle_radians);

   float u = axis.x;
   float v = axis.y;
   float w = axis.z;

   /*  Set the first row  */
   result[0][0] =      rcos + u * u * (1 - rcos);
   result[1][0] = -w * rsin + v * u * (1 - rcos);
   result[2][0] =  v * rsin + w * u * (1 - rcos);
   result[3][0] =                              0;

   /*  Set the second row  */
   result[0][1] =  w * rsin + u * v * (1 - rcos);
   result[1][1] =      rcos + v * v * (1 - rcos);
   result[2][1] = -u * rsin + w * v * (1 - rcos);
   result[3][1] =                              0;

   /*  Set the third row  */
   result[0][2] = -v * rsin + u * w * (1 - rcos);
   result[1][2] =  u * rsin + v * w * (1 - rcos);
   result[2][2] =      rcos + w * w * (1 - rcos);
   result[3][2] =                              0;

   /*  Set the fourth row  */
   result[0][3] = 0.0f;
   result[1][3] = 0.0f;
   result[2][3] = 0.0f;
   result[3][3] = 1.0f;
}
void create_scale(float x, float y, float z, mat4 &result)
{
   result[0][0] = x;    result[1][0] = 0.0f; result[2][0] = 0.0f; result[3][0] = 0.0f; 
   result[0][1] = 0.0f; result[1][1] = y;    result[2][1] = 0.0f; result[3][1] = 0.0f; 
   result[0][2] = 0.0f; result[1][2] = 0.0f; result[2][2] = z;    result[3][2] = 0.0f;
   result[0][3] = 0.0f; result[1][3] = 0.0f; result[2][3] = 0.0f; result[3][3] = 1.0f;
}
void create_scale(float s, mat4 &result)
{
   result[0][0] = s;    result[1][0] = 0.0f; result[2][0] = 0.0f; result[3][0] = 0.0f; 
   result[0][1] = 0.0f; result[1][1] = s;    result[2][1] = 0.0f; result[3][1] = 0.0f; 
   result[0][2] = 0.0f; result[1][2] = 0.0f; result[2][2] = s;    result[3][2] = 0.0f;
   result[0][3] = 0.0f; result[1][3] = 0.0f; result[2][3] = 0.0f; result[3][3] = 1.0f;
}
void create_look_at(vec3 const &eye, vec3 const &center, vec3 up0, mat4 &result)
{
   if (eye == center)
   {
      result = mat4_identity;
      return;
   }

   vec3 back = normalize(eye - center);
   if (up0 == back)
   {
      up0 = min_axis(back);
   }

   vec3 right  = normalize(cross(up0, back));
   vec3 up     = cross(back, right);

   result[0] = vec4(right, 0.0f);
   result[1] = vec4(up,    0.0f);
   result[2] = vec4(back,  0.0f);
   result[3] = vec4(eye,   1.0f);
}


float degrees_to_radians(float degrees)
{
   return degrees * pi<float>() / 180.0f;
}

void hsv_to_rgb(float h, float s, float v, float &r, float &g, float &b)
{
   r = 0.0f;
   g = 0.0f;
   b = 0.0f;
   float f;
   float p;
   float q;
   float t;
   int   i;

   if (s == 0.0f)
   {
      r = v;
      g = v;
      b = v;
   }
   else
   {
      if (h == 360.0f)
         h = 0.0f;

      h = h / 60.0f;
      i = (int)(h);
      f = h - i;
      p = v * (1.0f -  s             );
      q = v * (1.0f - (s *         f ));
      t = v * (1.0f - (s * (1.0f - f)));

      switch (i)
      {
      case 0:
         r = v;
         g = t;
         b = p;
         break;
      case 1:
         r = q;
         g = v;
         b = p;
         break;
      case 2:
         r = p;
         g = v;
         b = t;
         break;
      case 3:
         r = p;
         g = q;
         b = v;
         break;
      case 4:
         r = t;
         g = p;
         b = v;
         break;
      case 5:
         r = v;
         g = p;
         b = q;
         break;
      default:
         r = 1.0f;
         g = 1.0f;
         b = 1.0f;
         break;
      }
   }
}

void rgb_to_hsv(float r, float g, float b, float &h, float &s, float &v)
{
   h = 0;
   s = 1.0f;
   v = 1.0f;
   float diff;
   float r_dist;
   float g_dist;
   float b_dist;
   float undefined = 0;

   float max;
   float min;

   if (r > g)
      max = r;
   else
      max = g;

   if (b > max)
      max = b;

   if (r < g)
      min = r;
   else
      min = g;

   if (b < min)
      min = b;

   diff = max - min;
   v   = max;

   if (max < 0.0001f)
      s = 0;
   else
      s = diff / max;

   if (s == 0)
      h = undefined;
   else
   {
      r_dist = (max - r) / diff;
      g_dist = (max - g) / diff;
      b_dist = (max - b) / diff;
      if (r == max)
         h = b_dist - g_dist;
      else
         if (g == max)
            h = 2.0f + r_dist - b_dist;
         else
            if(b == max)
               h = 4.0f + g_dist - r_dist;
      h = h * 60.0f;
      if (h < 0)
         h += 360.0f;
   }
}

float srgb_to_linear(float cs)
{
   float cl = 0.0f;

   cs = (std::max)(0.0f, cs);
   cs = (std::min)(cs, 1.0f);

   if (cs <= 0.04045f)
      cl = cs / 12.92f;
   else
      cl = std::pow(
         (cs + 0.055f) / 1.055f,
         2.4f
      );

   return cl;
}

float linear_rgb_to_srgb(float cl)
{
   if (cl > 1.0f)
      return 1.0f;
   else if(cl < 0.0)
      return 0.0f;
   else if(cl < 0.0031308f)
      return 12.92f * cl;
   else
      return 1.055f * std::pow(cl, 0.41666f) - 0.055f;
}

vec3 srgb_to_linear_rgb(vec3 const &srgb)
{
   return vec3(
      srgb_to_linear(srgb.x),
      srgb_to_linear(srgb.y),
      srgb_to_linear(srgb.z)
   );
}

vec3 linear_rgb_to_srgb(vec3 const linear_rgb)
{
   return vec3(
      linear_rgb_to_srgb(linear_rgb.x),
      linear_rgb_to_srgb(linear_rgb.y),
      linear_rgb_to_srgb(linear_rgb.z)
   );
}

void cartesian_to_spherical(vec3 v, float &theta, float &phi)
{
   theta = std::acos(v.z); // heading
   phi = std::atan2(v.y, v.x); // elevation;
}

vec3 spherical_to_cartesian(float theta, float phi)
{
   vec3 v;
   v.x = std::sin(theta) * std::cos(phi);
   v.y = std::sin(theta) * std::sin(phi);
   v.z = std::cos(theta);
   return v;
}

} }
