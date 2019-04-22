#include "renderstack_graphics/color_mask_state.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace graphics
{

color_mask_state::color_mask_state()
{
    reset();
}

bool color_mask_state::red() const
{
    return m_red;
}
bool color_mask_state::green() const
{
    return m_green;
}
bool color_mask_state::blue() const
{
    return m_blue;
}
bool color_mask_state::alpha() const
{
    return m_alpha;
}
void color_mask_state::set_red(bool value)
{
    m_red = value;
}
void color_mask_state::set_green(bool value)
{
    m_green = value;
}
void color_mask_state::set_blue(bool value)
{
    m_blue = value;
}
void color_mask_state::set_alpha(bool value)
{
    m_alpha = value;
}
void color_mask_state::reset()
{
    set_red(true);
    set_green(true);
    set_blue(true);
    set_alpha(true);
}

color_mask_state_tracker::color_mask_state_tracker()
{
    reset();
}

void color_mask_state_tracker::reset()
{
    gl::color_mask(true, true, true, true);
    m_cache.set_red(true);
    m_cache.set_green(true);
    m_cache.set_blue(true);
    m_cache.set_alpha(true);
    m_last = nullptr;
}

void color_mask_state_tracker::execute(color_mask_state const *state)
{
#if !DISABLE_CACHE
    if (m_last == state)
        return;

    if (
        (m_cache.red() != state->red()) ||
        (m_cache.green() != state->green()) ||
        (m_cache.blue() != state->blue()) ||
        (m_cache.alpha() != state->alpha()))
#endif
    {
        gl::color_mask(state->red(), state->green(), state->blue(), state->alpha());
        m_cache.set_red(state->red());
        m_cache.set_green(state->green());
        m_cache.set_blue(state->blue());
        m_cache.set_alpha(state->alpha());
    }
    m_last = state;
}

} // namespace graphics
} // namespace renderstack
