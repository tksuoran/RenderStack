#ifndef light_hpp_renderstack_scene
#define light_hpp_renderstack_scene

#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/frame.hpp"
#include "renderstack_scene/projection.hpp"
#include "renderstack_scene/transform.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <memory>
#include <string>

namespace renderstack
{
namespace graphics
{
class Renderer;
}
} // namespace renderstack

namespace renderstack
{
namespace scene
{

class Light
{
public:
    enum class Type
    {
        directional = 0,
        point,
        spot
    };

    void update(Viewport viewport);

    Type       type{Type::directional};
    glm::vec3  color{glm::vec3(1.0f, 1.0f, 1.0f)};
    float      intensity{1.0f};
    float      range{100.0f};
    float      spot_angle{glm::pi<float>() * 0.5f};
    Camera     camera;
    Transform  shadow_from_world{glm::mat4(1.0f), glm::mat4(1.0f)};

    static glm::mat4 s_texture;
    static glm::mat4 s_texture_inverse;
};

} // namespace scene
} // namespace renderstack

#endif
