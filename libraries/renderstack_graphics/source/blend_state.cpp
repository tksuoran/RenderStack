#include "renderstack_graphics/blend_state.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/platform.hpp"

// #define DISABLE_CACHE 1

namespace renderstack
{
namespace graphics
{

using namespace glm;


void Blend_state_component::reset()
{
    equation_mode      = gl::blend_equation_mode::func_add;
    source_factor      = gl::blending_factor_src::one;
    destination_factor = gl::blending_factor_dest::zero;
}

void Blend_state::reset()
{
    enabled = false;
    rgb.reset();
    alpha.reset();
    color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void Blend_state_tracker::reset()
{
    gl::blend_color(0.0f, 0.0f, 0.0f, 0.0f);
    gl::blend_equation_separate(gl::blend_equation_mode::func_add, gl::blend_equation_mode::func_add);
    gl::blend_func_separate(gl::blending_factor_src::one,
                            gl::blending_factor_dest::zero,
                            gl::blending_factor_src::one,
                            gl::blending_factor_dest::zero);
    gl::disable(gl::enable_cap::blend);
    m_cache.reset();
    m_last = nullptr;
}

void Blend_state_tracker::execute(Blend_state const *state)
{
#if !DISABLE_CACHE
    if (m_last == state)
    {
        return;
    }

#endif
    if (state->enabled)
    {
#if !DISABLE_CACHE
        if (m_cache.enabled == false)
#endif
        {
            gl::enable(gl::enable_cap::blend);
            m_cache.enabled = true;
        }
        auto color = state->color;
#if !DISABLE_CACHE
        if (m_cache.color != state->color)
#endif
        {
            gl::blend_color(color.x, color.y, color.z, color.w);
            m_cache.color = color;
        }
#if !DISABLE_CACHE
        if ((m_cache.rgb.equation_mode != state->rgb.equation_mode) ||
            (m_cache.alpha.equation_mode != state->alpha.equation_mode))
#endif
        {
            gl::blend_equation_separate(state->rgb.equation_mode, state->alpha.equation_mode);
            m_cache.rgb.equation_mode = state->rgb.equation_mode;
            m_cache.alpha.equation_mode = state->alpha.equation_mode;
        }
#if !DISABLE_CACHE
        if ((m_cache.rgb.source_factor != state->rgb.source_factor) ||
            (m_cache.rgb.destination_factor != state->rgb.destination_factor) ||
            (m_cache.alpha.source_factor != state->alpha.source_factor) ||
            (m_cache.alpha.destination_factor != state->alpha.destination_factor))
#endif
        {
            gl::blend_func_separate(state->rgb.source_factor,
                                    state->rgb.destination_factor,
                                    state->alpha.source_factor,
                                    state->alpha.destination_factor);
            m_cache.rgb.source_factor = state->rgb.source_factor;
            m_cache.rgb.destination_factor = state->rgb.destination_factor;
            m_cache.alpha.source_factor = state->alpha.source_factor;
            m_cache.alpha.destination_factor = state->alpha.destination_factor;
        }
    }
    else
    {

#if !DISABLE_CACHE
        if (m_cache.enabled)
#endif
        {
            gl::disable(gl::enable_cap::blend);
            m_cache.enabled = false;
        }
    }
    m_last = state;
}

//////
} // namespace graphics
} // namespace renderstack
