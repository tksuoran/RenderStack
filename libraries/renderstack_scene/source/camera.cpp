#include "renderstack_scene/camera.hpp"

namespace renderstack { namespace scene {

using namespace std;

camera::camera()
:  m_frame(nullptr)
{
   m_frame = make_shared<renderstack::scene::frame>();
}

void camera::update(class viewport const &viewport)
{
   // Update clip from view transform / view from clip
   m_projection.update(m_clip_from_view, viewport);

   // TODO
   m_frame->update_hierarchical_no_cache();

   // Update clip from world / world from clip
   m_clip_from_world.set(
      m_clip_from_view.matrix() * m_frame->world_from_local().inverse_matrix(),
      m_frame->world_from_local().matrix() * m_clip_from_view.inverse_matrix()
   );
}

} } 

