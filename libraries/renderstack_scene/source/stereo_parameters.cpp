#include "renderstack_scene/stereo_parameters.hpp"

namespace renderstack
{
namespace scene
{

stereo_parameters::stereo_parameters()
    : eye_separation(0.065f), perspective(1.0f), viewport_center(0.0f, 0.0f, 4.0f), eye_position(0.0f, 0.0f, 0.0f), view_plane_size(2, 1)
{
}

} // namespace scene
} // namespace renderstack
