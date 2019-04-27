#ifndef face_cull_state_hpp_renderstack_graphics
#define face_cull_state_hpp_renderstack_graphics

#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace graphics
{

struct Face_cull_state
{
    Face_cull_state() = default;

    Face_cull_state(bool enabled) : enabled(enabled) {}

    void reset();

    bool                            enabled{false};
    gl::cull_face_mode::value       cull_face_mode{gl::cull_face_mode::back};
    gl::front_face_direction::value front_face_direction{gl::front_face_direction::ccw};
};

class Face_cull_state_tracker
{
public:
    void reset();
    void execute(Face_cull_state const *state);

private:
    Face_cull_state const *m_last{nullptr};
    Face_cull_state        m_cache;
};

} // namespace graphics
} // namespace renderstack

#endif
