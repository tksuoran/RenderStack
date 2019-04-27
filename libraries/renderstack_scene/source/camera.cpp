#include "renderstack_scene/camera.hpp"

namespace renderstack
{
namespace scene
{

using namespace std;


void Camera::update(Viewport viewport)
{
    // Update clip from view transform / view from clip
    projection.update(clip_from_view, viewport);

    // TODO
    frame.update_hierarchical_no_cache();

    // Update clip from world / world from clip
    clip_from_world.set(clip_from_view.matrix() * frame.world_from_local.inverse_matrix(),
                        frame.world_from_local.matrix() * clip_from_view.inverse_matrix());
}

} // namespace scene
} // namespace renderstack
