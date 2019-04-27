#include "renderstack_graphics/stencil_state.hpp"
#include "renderstack_toolkit/platform.hpp"

// #define DISABLE_CACHE 1

namespace renderstack
{
namespace graphics
{

void stencil_state_component::reset()
{
    stencil_fail_op = gl::stencil_op_enum::keep;
    z_fail_op       = gl::stencil_op_enum::keep;
    z_pass_op       = gl::stencil_op_enum::keep;
    function        = gl::stencil_function_enum::always;
    reference       = 0U;
    test_mask       = 0xffffU;
    write_mask      = 0xffffU;
}

void Stencil_state_tracker::reset()
{
    gl::stencil_op(gl::stencil_op_enum::keep, gl::stencil_op_enum::keep, gl::stencil_op_enum::keep);
    gl::stencil_mask(0xffffU);
    gl::stencil_func(gl::stencil_function_enum::always, 0, 0xffffU);
    m_cache.reset();
    m_last = nullptr;
}

void Stencil_state::reset()
{
    separate = false;
    enabled  = false;
    front.reset();
    back.reset();
}

void Stencil_state_tracker::execute_component(gl::stencil_face::value        face,
                                              stencil_state_component const &state,
                                              stencil_state_component &      cache)
{
#if !DISABLE_CACHE
    if ((cache.stencil_fail_op != state.stencil_fail_op) ||
        (cache.z_fail_op != state.z_fail_op) ||
        (cache.z_pass_op != state.z_pass_op))
#endif
    {
        gl::stencil_op_separate(face, state.stencil_fail_op, state.z_fail_op, state.z_pass_op);
        cache.stencil_fail_op = state.stencil_fail_op;
        cache.z_fail_op       = state.z_fail_op;
        cache.z_pass_op       = state.z_pass_op;
    }
#if !DISABLE_CACHE
    if (cache.write_mask != state.write_mask)
#endif
    {
        gl::stencil_mask_separate(face, state.write_mask);
        cache.write_mask = state.write_mask;
    }

#if !DISABLE_CACHE
    if ((cache.function != state.function) ||
        (cache.reference != state.reference) ||
        (cache.test_mask != state.test_mask))
#endif
    {
        gl::stencil_func_Separate(face, state.function, state.reference, state.test_mask);
        cache.function  = state.function;
        cache.reference = state.reference;
        cache.test_mask = state.test_mask;
    }
}

void Stencil_state_tracker::execute_shared(stencil_state_component const &state, Stencil_state &cache)
{
#if !DISABLE_CACHE
    if ((cache.front.stencil_fail_op != state.stencil_fail_op) ||
        (cache.front.z_fail_op != state.z_fail_op) ||
        (cache.front.z_pass_op != state.z_pass_op) ||
        (cache.back.stencil_fail_op != state.stencil_fail_op) ||
        (cache.back.z_fail_op != state.z_fail_op) ||
        (cache.back.z_pass_op != state.z_pass_op))
#endif
    {
        gl::stencil_op(state.stencil_fail_op, state.z_fail_op, state.z_pass_op);
        cache.front.stencil_fail_op = state.stencil_fail_op;
        cache.front.z_fail_op       = state.z_fail_op;
        cache.front.z_pass_op       = state.z_pass_op;
        cache.back.stencil_fail_op  = state.stencil_fail_op;
        cache.back.z_fail_op        = state.z_fail_op;
        cache.back.z_pass_op        = state.z_pass_op;
    }

#if !DISABLE_CACHE
    if ((cache.front.write_mask != state.write_mask) ||
        (cache.back.write_mask != state.write_mask))
#endif
    {
        gl::stencil_mask(state.write_mask);
        cache.front.write_mask = state.write_mask;
        cache.back.write_mask  = state.write_mask;
    }

#if !DISABLE_CACHE
    if ((cache.front.function != state.function) ||
        (cache.front.reference != state.reference) ||
        (cache.front.test_mask != state.test_mask) ||
        (cache.back.function != state.function) ||
        (cache.back.reference != state.reference) ||
        (cache.back.test_mask != state.test_mask))
#endif
    {
        gl::stencil_func(state.function, state.reference, state.test_mask);
        cache.front.function  = state.function;
        cache.front.reference = state.reference;
        cache.front.test_mask = state.test_mask;
        cache.back.function   = state.function;
        cache.back.reference  = state.reference;
        cache.back.test_mask  = state.test_mask;
    }
}

void Stencil_state_tracker::execute(Stencil_state const *state)
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
            gl::enable(gl::enable_cap::stencil_test);
            m_cache.enabled = true;
        }
        if (state->separate)
        {
            execute_component(gl::stencil_face::front, state->front, m_cache.front);
            execute_component(gl::stencil_face::back, state->back, m_cache.back);
            m_cache.separate = true;
        }
        else
        {
#if !DISABLE_CACHE
            if (!m_cache.separate)
            {
                //  Cache already in shared state
                execute_component(gl::stencil_face::front_and_back, state->front, m_cache.front);
            }
            else
#endif
            {
                //  Cache not yet in shared state - make it shared
                execute_shared(state->front, m_cache);
                m_cache.separate = false;
            }
        }
    }
    else
    {
#if !DISABLE_CACHE
        if (m_cache.enabled)
#endif
        {
            gl::disable(gl::enable_cap::stencil_test);
            m_cache.enabled = false;
        }
    }
    m_last = state;
}

} // namespace graphics
} // namespace renderstack
