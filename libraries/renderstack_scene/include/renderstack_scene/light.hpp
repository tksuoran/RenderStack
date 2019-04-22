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
class renderer;
}
} // namespace renderstack

namespace renderstack
{
namespace scene
{

namespace light_type
{
enum value
{
    directional = 0,
    point,
    spot
};
}

class light
{
public:
    light();

    void update(viewport &viewport);

    std::shared_ptr<class camera> camera();
    std::string const &           name();
    void                          set_name(std::string const &value);
    std::shared_ptr<class frame>  frame();

    light_type::value type() const;
    void              set_type(light_type::value value);
    glm::vec3         color() const;
    void              set_color(glm::vec3 value);
    float             intensity() const;
    void              set_intensity(float value);
    float             range() const;
    void              set_range(float value);
    float             spot_angle() const;
    void              set_spot_angle(float value);

    class projection const &projection() const;
    class projection &      projection();
    transform const &       shadow_from_world() const;
    transform &             shadow_from_world();

private:
    light_type::value m_type;
    glm::vec3         m_color;
    float             m_intensity;
    float             m_range;
    float             m_spot_angle;

    std::shared_ptr<class camera> m_camera;
    transform                     m_shadow_from_world;

    static glm::mat4 s_texture;
    static glm::mat4 s_texture_inverse;
};

} // namespace scene
} // namespace renderstack

#endif
