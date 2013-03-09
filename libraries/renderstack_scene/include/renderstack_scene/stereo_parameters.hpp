#ifndef renderstack_scene__stereo_parameters_hpp
#define renderstack_scene__stereo_parameters_hpp

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace scene {

class stereo_parameters
{
public:
   float       eye_separation;
   float       perspective;
   glm::vec3   viewport_center;
   glm::vec3   eye_position;
   glm::vec2   view_plane_size;

   stereo_parameters();
};

} }

#endif