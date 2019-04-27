#include "renderstack_scene/projection.hpp"
#include "renderstack_scene/viewport.hpp"

namespace renderstack
{
namespace scene
{

void Projection::update(Transform &transform, Viewport viewport)
{
    switch (projection_type)
    {
        case Projection::Type::perspective:
        {
            transform.set_perspective(fov_x, fov_y, near, far);
            break;
        }

        case Projection::Type::perspective_horizontal:
        {
            transform.set_perspective_horizontal(fov_x, viewport.aspect_ratio(), near, far);
            break;
        }

        case Projection::Type::perspective_vertical:
        {
            transform.set_perspective_vertical(fov_y, viewport.aspect_ratio(), near, far);
            break;
        }

        case Projection::Type::orthogonal_horizontal:
        {
            transform.set_orthographic(-0.5f * ortho_width,
                                        0.5f * ortho_width,
                                       -0.5f * ortho_width / viewport.aspect_ratio(),
                                        0.5f * ortho_width / viewport.aspect_ratio(),
                                       near,
                                       far);
            break;
        }

        case Projection::Type::orthogonal_vertical:
        {
            transform.set_orthographic(-0.5f * ortho_height / viewport.aspect_ratio(),
                                        0.5f * ortho_height / viewport.aspect_ratio(),
                                       -0.5f * ortho_height,
                                        0.5f * ortho_height,
                                       near,
                                       far);
            break;
        }

        case Projection::Type::orthogonal:
        {
            transform.set_orthographic(-0.5f * ortho_width,
                                        0.5f * ortho_width,
                                       -0.5f * ortho_height,
                                        0.5f * ortho_height,
                                       near,
                                       far);
            break;
        }

        case Projection::Type::orthogonal_rectangle:
        {
            transform.set_orthographic(ortho_left,
                                       ortho_left + ortho_width,
                                       ortho_bottom,
                                       ortho_bottom + ortho_height,
                                       near,
                                       far);
            break;
        }

        case Projection::Type::generic_frustum:
        {
            transform.set_frustum(frustum_left,
                                  frustum_right,
                                  frustum_bottom,
                                  frustum_top,
                                  near,
                                  far);
            break;
        }

        case Projection::Type::stereoscopic_horizontal:
        case Projection::Type::stereoscopic_vertical:
        case Projection::Type::other:
            // TODO
            break;
#if 0
   case Projection::Type::stereoscopic_horizontal:
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

} // namespace scene
} // namespace renderstack
