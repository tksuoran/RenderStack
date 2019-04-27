#ifndef depth_state_hpp_renderstack_graphics
#define depth_state_hpp_renderstack_graphics

#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace graphics
{

struct Depth_state
{
    Depth_state() = default;

    Depth_state(bool enabled) : enabled(enabled) {}

    void reset();

    bool                      enabled{false};
    gl::depth_function::value function{gl::depth_function::less};
    float                     near_{0.0f};
    float                     far_{1.0f};
    bool                      depth_mask{true};
};

class Depth_state_tracker
{
public:
    void reset();

    void execute(Depth_state const *state);

private:
    Depth_state const *m_last{nullptr};
    Depth_state        m_cache;
};

} // namespace graphics
} // namespace renderstack

#endif
