#include "renderstack_graphics/color_mask_state.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace graphics
{


void Color_mask_state::reset()
{
    red = true;
    green = true;
    blue = true;
    alpha = true;
}

void Color_mask_state_tracker::reset()
{
    gl::color_mask(true, true, true, true);
    m_cache.reset();
    m_last = nullptr;
}

void Color_mask_state_tracker::execute(Color_mask_state const *state)
{
#if !DISABLE_CACHE
    if (m_last == state)
    {
        return;
    }

    if ((m_cache.red != state->red) ||
        (m_cache.green != state->green) ||
        (m_cache.blue != state->blue) ||
        (m_cache.alpha != state->alpha))
#endif
    {
        gl::color_mask(state->red, state->green, state->blue, state->alpha);
        m_cache.red = state->red;
        m_cache.green = state->green;
        m_cache.blue = state->blue;
        m_cache.alpha = state->alpha;
    }
    m_last = state;
}

} // namespace graphics
} // namespace renderstack
