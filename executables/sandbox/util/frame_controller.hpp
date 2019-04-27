#ifndef frame_controller_hpp_util
#define frame_controller_hpp_util

#include "renderstack_toolkit/platform.hpp"
#include "util/controller.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace renderstack
{
namespace scene
{
class Frame;
}
} // namespace renderstack

class frame_controller
{
public:
    frame_controller();

    void set_frame(renderstack::scene::Frame *value);

    renderstack::scene::Frame *frame();

    void clear();

    void update();

    void update_fixed_step();

    void set_position(glm::vec3 position);

    void set_elevation(float value);

    void set_heading(float value);

    glm::vec3 position() const;

    float elevation() const;

    float heading() const;

    glm::vec3 right() const;

    glm::vec3 up() const;

    glm::vec3 back() const;
    //glm::mat4 const   &parent_from_local() const { return m_parent_from_local; }
    //glm::mat4 const   &local_from_parent() const { return m_local_from_parent; }

    controller rotate_x;
    controller rotate_y;
    controller rotate_z;
    controller translate_x;
    controller translate_y;
    controller translate_z;
    controller speed_modifier;

private:
    renderstack::scene::Frame* m_frame{nullptr};
    float      m_elevation{0.0f};
    float      m_heading{0.0f};
    glm::mat4  m_heading_matrix{1.0f};
    glm::mat4  m_rotation_matrix{1.0f};
    glm::vec3  m_position{0.0f};
};

#endif
