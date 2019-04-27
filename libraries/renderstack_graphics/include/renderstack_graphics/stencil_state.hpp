#ifndef stencil_state_hpp_renderstack_graphics
#define stencil_state_hpp_renderstack_graphics

#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace graphics
{

struct Stencil_state;

struct stencil_state_component
{
    void apply(gl::stencil_face::value face, stencil_state_component &cache) const;
    void apply_shared(struct Stencil_state &cache) const;

    void reset();

    gl::stencil_op_enum::value       stencil_fail_op{gl::stencil_op_enum::keep};
    gl::stencil_op_enum::value       z_fail_op{gl::stencil_op_enum::keep};
    gl::stencil_op_enum::value       z_pass_op{gl::stencil_op_enum::keep};
    gl::stencil_function_enum::value function{gl::stencil_function_enum::always};
    unsigned int                     reference{0U};
    unsigned int                     test_mask{0xffffU};
    unsigned int                     write_mask{0xffffU};
};

struct Stencil_state
{
    void reset();

    bool                    enabled{false};
    bool                    separate{false};
    stencil_state_component front;
    stencil_state_component back;
};

class Stencil_state_tracker
{
public:
    void reset();
    void execute(Stencil_state const *state);

private:
    void execute_component(gl::stencil_face::value        face,
                           stencil_state_component const &state,
                           stencil_state_component &      cache);
    void execute_shared(stencil_state_component const &state, Stencil_state &cache);

private:
    Stencil_state const *m_last{nullptr};
    Stencil_state        m_cache;
};

} // namespace graphics
} // namespace renderstack

#endif
