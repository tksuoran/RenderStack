#ifndef stereo_parameters_hpp_renderstack_scene
#define stereo_parameters_hpp_renderstack_scene

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace scene
{

class stereo_parameters
{
public:
    float     eye_separation;
    float     perspective;
    glm::vec3 viewport_center;
    glm::vec3 eye_position;
    glm::vec2 view_plane_size;

    stereo_parameters();
};

} // namespace scene
} // namespace renderstack

#endif