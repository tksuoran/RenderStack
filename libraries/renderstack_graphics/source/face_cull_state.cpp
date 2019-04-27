#include "renderstack_graphics/face_cull_state.hpp"
#include "renderstack_toolkit/platform.hpp"

// #define DISABLE_CACHE 1


namespace renderstack
{
namespace graphics
{


void Face_cull_state::reset()
{
    enabled = false;
    cull_face_mode = gl::cull_face_mode::back;
    front_face_direction = gl::front_face_direction::ccw;
}

void Face_cull_state_tracker::reset()
{
    gl::disable(gl::enable_cap::cull_face);
    gl::cull_face(gl::cull_face_mode::back);
    gl::front_face(gl::front_face_direction::ccw);
    m_cache.reset();
    m_last = nullptr;
}

void Face_cull_state_tracker::execute(Face_cull_state const *state)
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
            gl::enable(gl::enable_cap::cull_face);
            m_cache.enabled = true;
        }
#if !DISABLE_CACHE
        if (m_cache.cull_face_mode != state->cull_face_mode)
#endif
        {
            gl::cull_face(state->cull_face_mode);
            m_cache.cull_face_mode = state->cull_face_mode;
        }
#if !DISABLE_CACHE
        if (m_cache.front_face_direction != state->front_face_direction)
#endif
        {
            gl::front_face(state->front_face_direction);
            m_cache.front_face_direction = state->front_face_direction;
        }
    }
    else
    {
#if !DISABLE_CACHE
        if (m_cache.enabled)
#endif
        {
            gl::disable(gl::enable_cap::cull_face);
            m_cache.enabled = false;
        }
    }
    m_last = state;
}

} // namespace graphics
} // namespace renderstack
