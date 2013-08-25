#include "renderstack_scene/projection.hpp"
#include "renderstack_scene/viewport.hpp"

namespace renderstack { namespace scene {

projection::projection()
{
   m_near            =   1.0f;
   m_far             = 100.0f;
   m_projection_type = projection_type::perspective_vertical;
   m_fov_x           = glm::half_pi<float>();
   m_fov_y           = glm::half_pi<float>();
   m_ortho_width     = 1.0f;
   m_ortho_height    = 1.0f;
}

void projection::update(transform &transform, viewport const &viewport)
{
   switch (m_projection_type)
   {
   case projection_type::perspective:
      transform.set_perspective(
         m_fov_x,
         m_fov_y,
         m_near,
         m_far
      );
      break;

   case projection_type::perspective_horizontal:
      transform.set_perspective_horizontal(
         m_fov_x,
         viewport.aspect_ratio(),
         m_near,
         m_far
      );
      break;

   case projection_type::perspective_vertical:
      transform.set_perspective_vertical(
         m_fov_y,
         viewport.aspect_ratio(),
         m_near,
         m_far
      );
      break;

   case projection_type::orthogonal_horizontal:
      transform.set_orthographic(
         -0.5f * m_ortho_width,
          0.5f * m_ortho_width,
         -0.5f * m_ortho_width / viewport.aspect_ratio(),
          0.5f * m_ortho_width / viewport.aspect_ratio(),
         m_near,
         m_far
      );
      break;

   case projection_type::orthogonal_vertical:
      transform.set_orthographic(
         -0.5f * m_ortho_height / viewport.aspect_ratio(),
          0.5f * m_ortho_height / viewport.aspect_ratio(),
         -0.5f * m_ortho_height,
          0.5f * m_ortho_height,
         m_near,
         m_far
      );
      break;
   case projection_type::orthogonal:
      transform.set_orthographic(
         -0.5f * m_ortho_width,
          0.5f * m_ortho_width,
         -0.5f * m_ortho_height,
          0.5f * m_ortho_height,
         m_near,
         m_far
      );
      break;

   case projection_type::orthogonal_rectangle:
      transform.set_orthographic(
         m_ortho_left,
         m_ortho_left + m_ortho_width,
         m_ortho_bottom,
         m_ortho_bottom + m_ortho_height,
         m_near,
         m_far
      );
      break;

   case projection_type::generic_frustum:
      transform.set_frustum(
         m_frustum_left,
         m_frustum_right,
         m_frustum_bottom,
         m_frustum_top,
         m_near,
         m_far
      );
      break;

   case projection_type::stereoscopic_horizontal:
   case projection_type::stereoscopic_vertical:
   case projection_type::other:
      // TODO
      break;
#if 0
   case projection_type::stereoscopic_horizontal:
   {
      float fov = m_fov_x;
      fov = std::max(fov, 0.001f);
      fov = std::min(fov, glm::pi<float>() * 0.999f);

      float z = std::max(m_stereo_parameters.ViewportCenter.Z, 0.01f);

      float w = 2.0f * z * (float)(System.Math.Tan(fov * 0.5f));
      float h = w / viewport.AspectRatio;

      StereoParameters.ViewPlaneSize.Set(w, h);

      transform.SetProjection(
         StereoParameters.EyeSeparation.X,
         StereoParameters.Perspective.X,
         Near,
         Far,
         w,
         h,
         new Vector3(stereoParameters.ViewportCenter.X, stereoParameters.ViewportCenter.Y, stereoParameters.ViewportCenter.Z),
         new Vector3(stereoParameters.EyePosition.X, stereoParameters.EyePosition.Y, stereoParameters.EyePosition.Z)
      );
      break;
   }
   case ProjectionType.StereoscopicVertical:
   {
      float fov = FovYRadians;
      fov = System.Math.Max(fov, 0.01f);
      fov = System.Math.Min(fov, (float)(System.Math.PI * 0.99));

      float z = System.Math.Max(stereoParameters.ViewportCenter.Z, 0.01f);

      float h = 2.0f * z * (float)(System.Math.Tan(fov * 0.5f));
      float w = h * viewport.AspectRatio;

      StereoParameters.ViewPlaneSize.Set(w, h);

      transform.SetProjection(
         StereoParameters.EyeSeparation.X,
         StereoParameters.Perspective.X,
         Near,
         Far,
         w,
         h,
         new Vector3(stereoParameters.ViewportCenter.X, stereoParameters.ViewportCenter.Y, stereoParameters.ViewportCenter.Z),
         new Vector3(stereoParameters.EyePosition.X, stereoParameters.EyePosition.Y, stereoParameters.EyePosition.Z)
      );
      break;
   }
#endif
   }
}

} }
