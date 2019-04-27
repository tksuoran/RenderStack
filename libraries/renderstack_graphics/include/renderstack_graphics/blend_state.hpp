#ifndef blend_state_hpp_renderstack_graphics
#define blend_state_hpp_renderstack_graphics

#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace graphics
{

struct Blend_state_component
{
    void reset();

    gl::blend_equation_mode::value  equation_mode{gl::blend_equation_mode::func_add};
    gl::blending_factor_src::value  source_factor{gl::blending_factor_src::one};
    gl::blending_factor_dest::value destination_factor{gl::blending_factor_dest::one};
};

struct Blend_state
{
    void reset();

    bool                  enabled{false};
    Blend_state_component rgb;
    Blend_state_component alpha;
    glm::vec4             color{0.0f, 0.0f, 0.0f, 0.0f};
};

class Blend_state_tracker
{
public:
    void reset();
    void execute(Blend_state const *state);

private:
    Blend_state const *m_last{nullptr};
    Blend_state        m_cache;
};

} // namespace graphics
} // namespace renderstack

#endif
