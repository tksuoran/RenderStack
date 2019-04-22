#include "renderstack_graphics/depth_state.hpp"
#include "renderstack_toolkit/platform.hpp"

//#define DISABLE_CACHE 1

namespace renderstack
{
namespace graphics
{

bool depth_state::enabled() const
{
    return m_enabled;
}
gl::depth_function::value depth_state::function() const
{
    return m_function;
}
float depth_state::near_() const
{
    return m_near;
}
float depth_state::far_() const
{
    return m_far;
}
bool depth_state::depth_mask() const
{
    return m_depth_mask;
}
void depth_state::set_depth_mask(bool value)
{
    m_depth_mask = value;
}

void depth_state::set_enabled(bool value)
{
    m_enabled = value;
}
void depth_state::set_function(gl::depth_function::value value)
{
    m_function = value;
}
void depth_state::set_near(float value)
{
    m_near = value;
}
void depth_state::set_far(float value)
{
    m_far = value;
}

depth_state::depth_state()
    : m_enabled(false), m_function(gl::depth_function::less), m_near(0.0f), m_far(1.0f), m_depth_mask(true)
{
}
depth_state::depth_state(bool enabled)
    : m_enabled(enabled), m_function(gl::depth_function::less), m_near(0.0f), m_far(1.0f), m_depth_mask(true)
{
}

void depth_state::reset()
{
    set_enabled(false);
    set_function(gl::depth_function::less);
    set_near(0.0f);
    set_far(1.0);
    set_depth_mask(true);
}

depth_state_tracker::depth_state_tracker()
{
    reset();
}

void depth_state_tracker::reset()
{
    gl::depth_func(gl::depth_function::less);
    m_cache.set_function(gl::depth_function::less);
    gl::depth_range(0.0f, 1.0f);
    m_cache.set_near(0.0f);
    m_cache.set_far(1.0f);
    gl::enable(gl::enable_cap::depth_test);
    m_cache.set_enabled(true);
    gl::depth_mask(GL_TRUE);
    m_cache.set_depth_mask(true);
    m_last = nullptr;
}
void depth_state_tracker::execute(depth_state const *state)
{
#if !DISABLE_CACHE
    if (m_last == state)
        return;
#endif

    if (state->enabled())
    {
#if !DISABLE_CACHE
        if (!m_cache.enabled())
#endif
        {
            gl::enable(gl::enable_cap::depth_test);
            m_cache.set_enabled(true);
        }

#if !DISABLE_CACHE
        if (m_cache.function() != state->function())
#endif
        {
            gl::depth_func(state->function());
            m_cache.set_function(state->function());
        }

#if !DISABLE_CACHE
        if (
            (m_cache.near_() != state->near_()) ||
            (m_cache.far_() != state->far_()))
#endif
        {
            gl::depth_range(state->near_(), state->far_());
            m_cache.set_near(state->near_());
            m_cache.set_far(state->far_());
        }
    }
    else
    {

#if !DISABLE_CACHE
        if (m_cache.enabled())
#endif
        {
            gl::disable(gl::enable_cap::depth_test);
            m_cache.set_enabled(false);
        }
    }

#if !DISABLE_CACHE
    if (m_cache.depth_mask() != state->depth_mask())
#endif
    {
        gl::depth_mask(state->depth_mask());
        m_cache.set_depth_mask(state->depth_mask());
    }

    m_last = state;
}

} // namespace graphics
} // namespace renderstack
