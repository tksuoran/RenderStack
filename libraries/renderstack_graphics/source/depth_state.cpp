#include "renderstack_graphics/depth_state.hpp"
#include "renderstack_toolkit/platform.hpp"

//#define DISABLE_CACHE 1

namespace renderstack
{
namespace graphics
{


void Depth_state::reset()
{
    enabled = false;
    function = gl::depth_function::less;
    near_ = 0.0f;
    far_ = 1.0;
    depth_mask = true;
}

void Depth_state_tracker::reset()
{
    gl::disable(gl::enable_cap::depth_test);
    gl::depth_func(gl::depth_function::less);
    gl::depth_range(0.0f, 1.0f);
    gl::depth_mask(GL_TRUE);
    m_cache.reset();
    m_last = nullptr;
}

void Depth_state_tracker::execute(Depth_state const *state)
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
        if (!m_cache.enabled)
#endif
        {
            gl::enable(gl::enable_cap::depth_test);
            m_cache.enabled = true;
        }

#if !DISABLE_CACHE
        if (m_cache.function != state->function)
#endif
        {
            gl::depth_func(state->function);
            m_cache.function = state->function;
        }

#if !DISABLE_CACHE
        if ((m_cache.near_ != state->near_) ||
            (m_cache.far_ != state->far_))
#endif
        {
            gl::depth_range(state->near_, state->far_);
            m_cache.near_ = state->near_;
            m_cache.far_ = state->far_;
        }
    }
    else
    {

#if !DISABLE_CACHE
        if (m_cache.enabled)
#endif
        {
            gl::disable(gl::enable_cap::depth_test);
            m_cache.enabled = false;
        }
    }

#if !DISABLE_CACHE
    if (m_cache.depth_mask != state->depth_mask)
#endif
    {
        gl::depth_mask(state->depth_mask);
        m_cache.depth_mask = state->depth_mask;
    }

    m_last = state;
}

} // namespace graphics
} // namespace renderstack
