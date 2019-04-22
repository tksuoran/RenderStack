#include "renderstack_graphics/face_cull_state.hpp"
#include "renderstack_toolkit/platform.hpp"

// #define DISABLE_CACHE 1

namespace renderstack
{
namespace graphics
{

bool face_cull_state::enabled() const
{
    return m_enabled;
}
gl::cull_face_mode::value face_cull_state::cull_face_mode() const
{
    return m_cull_face_mode;
}
gl::front_face_direction::value face_cull_state::front_face_direction() const
{
    return m_front_face_direction;
}
void face_cull_state::set_enabled(bool value)
{
    m_enabled = value;
}
void face_cull_state::set_cull_face_mode(gl::cull_face_mode::value value)
{
    m_cull_face_mode = value;
}
void face_cull_state::set_front_face_direction(gl::front_face_direction::value value)
{
    m_front_face_direction = value;
}

face_cull_state::face_cull_state()
    : m_enabled(false), m_cull_face_mode(gl::cull_face_mode::back), m_front_face_direction(gl::front_face_direction::ccw)
{
}

face_cull_state::face_cull_state(bool enabled)
    : m_enabled(enabled), m_cull_face_mode(gl::cull_face_mode::back), m_front_face_direction(gl::front_face_direction::ccw)
{
}

void face_cull_state::reset()
{
    set_enabled(true);
    set_cull_face_mode(gl::cull_face_mode::back);
    set_front_face_direction(gl::front_face_direction::ccw);
}

face_cull_state_tracker::face_cull_state_tracker()
{
    reset();
}

void face_cull_state_tracker::reset()
{
    gl::cull_face(gl::cull_face_mode::back);
    m_cache.set_cull_face_mode(gl::cull_face_mode::back);
    gl::front_face(gl::front_face_direction::ccw);
    m_cache.set_front_face_direction(gl::front_face_direction::ccw);
    gl::disable(gl::enable_cap::cull_face);
    m_cache.set_enabled(false);
    m_last = nullptr;
}

void face_cull_state_tracker::execute(face_cull_state const *state)
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
            gl::enable(gl::enable_cap::cull_face);
            m_cache.set_enabled(true);
        }
#if !DISABLE_CACHE
        if (m_cache.cull_face_mode() != state->cull_face_mode())
#endif
        {
            gl::cull_face(state->cull_face_mode());
            m_cache.set_cull_face_mode(state->cull_face_mode());
        }
#if !DISABLE_CACHE
        if (m_cache.front_face_direction() != state->front_face_direction())
#endif
        {
            gl::front_face(state->front_face_direction());
            m_cache.set_front_face_direction(state->front_face_direction());
        }
    }
    else
    {
#if !DISABLE_CACHE
        if (m_cache.enabled())
#endif
        {
            gl::disable(gl::enable_cap::cull_face);
            m_cache.set_enabled(false);
        }
    }
    m_last = state;
}

} // namespace graphics
} // namespace renderstack
