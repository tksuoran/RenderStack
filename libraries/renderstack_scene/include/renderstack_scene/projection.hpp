#ifndef renderstack_scene__projection_hpp
#define renderstack_scene__projection_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_scene/stereo_parameters.hpp"
#include "renderstack_scene/transform.hpp"
#include <glm/gtx/constants.hpp>
#include <cstdint>

namespace renderstack { namespace scene {

class viewport
{
private:
   int m_border;
   int m_x;
   int m_y;
   int m_width;
   int m_height;

public:
   int   border      ()          { return m_border; }
   void  set_border  (int value) { m_border = value; }
   int   x           ()          { return m_x; }
   void  set_x       (int value) { m_x = value; }
   int   y           ()          { return m_y; }
   void  set_y       (int value) { m_y = value; }
   int   width       ()          { return m_width; }
   void  set_width   (int value) { m_width = value; }

   float aspect_ratio()
   {
      return m_height != 0.0f ? static_cast<float>(m_width) / static_cast<float>(m_height) : 1.0f;
   }
};

namespace projection_type
{
   enum value {
      other = 0,              //  Projection is done by shader in unusual way - hemispherical for example
      perspective_horizontal,
      perspective_vertical,
      perspective,            //  Uses both horizontal and vertical fov and ignores aspect ratio
      orthogonal_horizontal,
      orthogonal_vertical,
      orthogonal,             //  Uses both horizontal and vertical size and ignores aspect ratio, O-centered
      orthogonal_rectangle,   //  Like above, not O-centered, uses X and Y as corner
      generic_frustum,        //  Generic frustum
      stereoscopic_horizontal,
      stereoscopic_vertical
   };
}

class projection
{
private:
   projection_type::value  m_projection_type;
   float                   m_near;
   float                   m_far;
   float                   m_fov_x;
   float                   m_fov_y;
   float                   m_ortho_left;
   float                   m_ortho_width;
   float                   m_ortho_bottom;
   float                   m_ortho_height;
   float                   m_frustum_left;
   float                   m_frustum_right;
   float                   m_frustum_bottom;
   float                   m_frustum_top;
   //stereo_parameters m_stereo_parameters;

public:
   projection_type::value  projection_type      () { return m_projection_type; }
   void                    set_projection_type  (projection_type::value value) { m_projection_type = value; }

   float near_             ()             { return m_near; }
   void  set_near          (float value)  { m_near = value; }
   float far_              ()             { return m_far; }
   void  set_far           (float value)  { m_far = value; }
   float fov_x             ()             { return m_fov_x; }
   void  set_fov_x         (float value)  { m_fov_x = value; }
   float fov_y             ()             { return m_fov_y; }
   void  set_fov_y         (float value)  { m_fov_y = value; }
   float ortho_left        ()             { return m_ortho_left; }
   void  set_ortho_left    (float value)  { m_ortho_left = value; }
   float ortho_width       ()             { return m_ortho_width; }
   void  set_ortho_width   (float value)  { m_ortho_width = value; }
   float ortho_bottom      ()             { return m_ortho_bottom; }
   void  set_ortho_bottom  (float value)  { m_ortho_bottom = value; }
   float ortho_height      ()             { return m_ortho_height; }
   void  set_ortho_height  (float value)  { m_ortho_height = value; }
   float frustum_left      ()             { return m_frustum_left; }
   void  set_frustum_left  (float value)  { m_frustum_left = value; }
   float frustum_right     ()             { return m_frustum_right; }
   void  set_frustum_right (float value)  { m_frustum_right = value; }
   float frustum_bottom    ()             { return m_frustum_bottom; }
   void  set_frustum_bottom(float value)  { m_frustum_bottom = value; }
   float frustum_top       ()             { return m_frustum_top; }
   void  set_frustum_top   (float value)  { m_frustum_top = value; }

#if 0
public:
   stereo_parameters const &stereo_parameters () { return m_stereo_parameters; }
#endif

public:
   projection();

   void update(transform &transform, viewport &viewport);
};

} }

#endif

