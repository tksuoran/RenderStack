#include "renderstack_scene/camera.hpp"

namespace renderstack { namespace scene {

camera::camera()
{
   //parameters = new CameraParameters(uniformBufferGL, uniformBufferRL);
}

void camera::update_viewport(class viewport &viewport)
{
#if 0
   parameters.FovXFovYAspect.Set(Projection.FovXRadians, Projection.FovYRadians, viewport.AspectRatio);
   parameters.NearFar.Set(Projection.Near, Projection.Far);
   parameters.Viewport.Set(
         (float)viewport.X, 
         (float)viewport.Y, 
         (float)viewport.Width, 
         (float)viewport.Height
   );
#endif

#if 0
   try
#endif
   {
      m_projection.update(m_clip_from_view, viewport);
   }
#if 0
   catch (...)
   {
      // TODO Log error
   }
#endif

   //  world to (view) local  =>  view to clip
   m_clip_from_world.set(
      m_clip_from_view.matrix() * m_frame->world_from_local().inverse_matrix(),
      m_frame->world_from_local().matrix() * m_clip_from_view.inverse_matrix()
   );
#if 0
   parameters.ViewToClip.Set(ViewToClip.Matrix);
   parameters.ClipToView.Set(ViewToClip.InverseMatrix);
   parameters.WorldToClip.Set(WorldToClip.Matrix);
   parameters.ClipToWorld.Set(WorldToClip.InverseMatrix);
#endif
}

/// Camera Frame has been modified, update transformations
void camera::update_frame()
{
#if 0
   parameters.ViewToWorld.Set(frame.LocalToWorld.Matrix);
   parameters.WorldToView.Set(frame.LocalToWorld.InverseMatrix);

   parameters.ViewPositionInWorld.Set(
         frame.LocalToWorld.Matrix._03,
         frame.LocalToWorld.Matrix._13,
         frame.LocalToWorld.Matrix._23,
         1.0f
   );
#endif
}

} } 

