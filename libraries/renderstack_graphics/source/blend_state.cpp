#include "renderstack_graphics/blend_state.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/platform.hpp"

// #define DISABLE_CACHE 1

namespace renderstack
{
namespace graphics
{

using namespace glm;

blend_state_component::blend_state_component()
    : m_equation_mode(gl::blend_equation_mode::func_add), m_source_factor(gl::blending_factor_src::one), m_destination_factor(gl::blending_factor_dest::one)
{
}

gl::blend_equation_mode::value blend_state_component::equation_mode() const
{
    return m_equation_mode;
}
gl::blending_factor_src::value blend_state_component::source_factor() const
{
    return m_source_factor;
}
gl::blending_factor_dest::value blend_state_component::destination_factor() const
{
    return m_destination_factor;
}

void blend_state_component::set_equation_mode(gl::blend_equation_mode::value value)
{
    m_equation_mode = value;
}
void blend_state_component::set_source_factor(gl::blending_factor_src::value value)
{
    m_source_factor = value;
}
void blend_state_component::set_destination_factor(gl::blending_factor_dest::value value)
{
    m_destination_factor = value;
}

void blend_state_component::reset()
{
    m_equation_mode      = gl::blend_equation_mode::func_add;
    m_source_factor      = gl::blending_factor_src::one;
    m_destination_factor = gl::blending_factor_dest::one;
}

blend_state::blend_state()
{
    reset();
}

bool blend_state::enabled() const
{
    return m_enabled;
}
blend_state_component const &blend_state::rgb() const
{
    return m_rgb;
}
blend_state_component &blend_state::rgb()
{
    return m_rgb;
}
blend_state_component const &blend_state::alpha() const
{
    return m_alpha;
}
blend_state_component &blend_state::alpha()
{
    return m_alpha;
}
vec4 const &blend_state::color() const
{
    return m_color;
}
vec4 &blend_state::color()
{
    return m_color;
}
void blend_state::set_enabled(bool value)
{
    m_enabled = value;
}
void blend_state::set_color(vec4 const &value)
{
    m_color = value;
}
void blend_state::reset()
{
    m_enabled = false;
    m_rgb.reset();
    m_alpha.reset();
    m_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

blend_state_tracker::blend_state_tracker()
{
    reset();
}

void blend_state_tracker::reset()
{
    gl::blend_color(0.0f, 0.0f, 0.0f, 0.0f);
    m_cache.set_color(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    gl::blend_equation_separate(gl::blend_equation_mode::func_add, gl::blend_equation_mode::func_add);
    m_cache.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
    m_cache.alpha().set_equation_mode(gl::blend_equation_mode::func_add);
    gl::blend_func_separate(
        gl::blending_factor_src::one,
        gl::blending_factor_dest::one,
        gl::blending_factor_src::one,
        gl::blending_factor_dest::one);
    m_cache.rgb().set_source_factor(gl::blending_factor_src::one);
    m_cache.rgb().set_source_factor(gl::blending_factor_src::one);
    m_cache.alpha().set_destination_factor(gl::blending_factor_dest::one);
    m_cache.alpha().set_destination_factor(gl::blending_factor_dest::one);
    gl::disable(gl::enable_cap::blend);
    m_cache.set_enabled(false);
    m_last = nullptr;
}

void blend_state_tracker::execute(blend_state const *state)
{
#if !DISABLE_CACHE
    if (m_last == state)
        return;

#endif
    if (state->enabled())
    {
#if !DISABLE_CACHE
        if (m_cache.enabled() == false)
#endif
        {
            gl::enable(gl::enable_cap::blend);
            m_cache.set_enabled(true);
        }
        auto color = state->color();
#if !DISABLE_CACHE
        if (m_cache.color() != state->color())
#endif
        {
            gl::blend_color(
                color.x,
                color.y,
                color.z,
                color.w);
            m_cache.set_color(color);
        }
#if !DISABLE_CACHE
        if (
            (m_cache.rgb().equation_mode() != state->rgb().equation_mode()) ||
            (m_cache.alpha().equation_mode() != state->alpha().equation_mode()))
#endif
        {
            gl::blend_equation_separate(state->rgb().equation_mode(), state->alpha().equation_mode());
            m_cache.rgb().set_equation_mode(state->rgb().equation_mode());
            m_cache.alpha().set_equation_mode(state->alpha().equation_mode());
        }
#if !DISABLE_CACHE
        if (
            (m_cache.rgb().source_factor() != state->rgb().source_factor()) ||
            (m_cache.rgb().destination_factor() != state->rgb().destination_factor()) ||
            (m_cache.alpha().source_factor() != state->alpha().source_factor()) ||
            (m_cache.alpha().destination_factor() != state->alpha().destination_factor()))
#endif
        {
            gl::blend_func_separate(
                state->rgb().source_factor(),
                state->rgb().destination_factor(),
                state->alpha().source_factor(),
                state->alpha().destination_factor());
            m_cache.rgb().set_source_factor(state->rgb().source_factor());
            m_cache.rgb().set_destination_factor(state->rgb().destination_factor());
            m_cache.alpha().set_source_factor(state->alpha().source_factor());
            m_cache.alpha().set_destination_factor(state->alpha().destination_factor());
        }
    }
    else
    {

#if !DISABLE_CACHE
        if (m_cache.enabled() == true)
#endif
        {
            gl::disable(gl::enable_cap::blend);
            m_cache.set_enabled(false);
        }
    }
    m_last = state;
}

//////
} // namespace graphics
} // namespace renderstack
