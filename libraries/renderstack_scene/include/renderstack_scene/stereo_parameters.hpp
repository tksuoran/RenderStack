#ifndef stereo_parameters_hpp_renderstack_scene
#define stereo_parameters_hpp_renderstack_scene

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace scene
{

struct Stereo_parameters
{
    float     eye_separation{0.065f};
    float     perspective{1.0f};
    glm::vec3 viewport_center{0.0f, 0.0f, 4.0f};
    glm::vec3 eye_position{0.0f, 0.0f, 0.0f};
    glm::vec2 view_plane_size{2.0f, 1.0f};
};

} // namespace scene
} // namespace renderstack

#endif
