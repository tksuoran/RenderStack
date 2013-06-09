#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/stencil_state.hpp"

// #define DISABLE_CACHE 1

namespace renderstack { namespace graphics {

gl::stencil_op_enum::value stencil_state_component::stencil_fail_op() const
{
   return m_stencil_fail_op;
}
gl::stencil_op_enum::value stencil_state_component::z_fail_op() const
{
   return m_z_fail_op;
}
gl::stencil_op_enum::value stencil_state_component::z_pass_op() const
{
   return m_z_pass_op;
}
gl::stencil_function_enum::value stencil_state_component::function() const
{
   return m_function;
}
int stencil_state_component::reference() const
{
   return m_reference;
}
GLuint stencil_state_component::test_mask() const
{
   return m_test_mask;
}
GLuint stencil_state_component::write_mask() const
{
   return m_write_mask;
}
void stencil_state_component::set_stencil_fail_op(gl::stencil_op_enum::value value)
{
   m_stencil_fail_op = value;
}
void stencil_state_component::set_z_fail_op(gl::stencil_op_enum::value value)
{
   m_z_fail_op = value;
}
void stencil_state_component::set_z_pass_op(gl::stencil_op_enum::value value)
{
   m_z_pass_op = value;
}
void stencil_state_component::set_function(gl::stencil_function_enum::value value)
{
   m_function = value;
}
void stencil_state_component::set_reference(int value)
{
   m_reference = value;
}
void stencil_state_component::set_test_mask(GLuint value)
{
   m_test_mask = value;
}
void stencil_state_component::set_write_mask(GLuint value)
{
   m_write_mask = value;
}

stencil_state_component::stencil_state_component()
:  m_stencil_fail_op (gl::stencil_op_enum::keep)
,  m_z_fail_op       (gl::stencil_op_enum::keep)
,  m_z_pass_op       (gl::stencil_op_enum::keep)
,  m_function        (gl::stencil_function_enum::always)
,  m_reference       (0)
,  m_test_mask       (0xffff)
,  m_write_mask      (0xffff)
{
}


stencil_state::stencil_state()
:   m_enabled(false)
,   m_separate(false)
{
}

bool stencil_state::enabled() const
{
   return m_enabled;
}
bool stencil_state::separate() const
{
   return m_separate;
}
stencil_state_component const &stencil_state::front() const
{
   return m_front;
}
stencil_state_component &stencil_state::front()
{
   return m_front;
}
stencil_state_component const &stencil_state::back() const
{
   return m_back;
}
stencil_state_component &stencil_state::back()
{
   return m_back;
}
void stencil_state::set_enabled (bool value)
{
   m_enabled = value;
}
void stencil_state::set_separate(bool value)
{
   m_separate = value;
}
void stencil_state::set_front(stencil_state_component const &value)
{
   m_front = value;
}
void stencil_state::set_back(stencil_state_component const &value)
{
   m_back = value;
}

stencil_state_tracker::stencil_state_tracker()
{
   reset();
}

void stencil_state_tracker::reset()
{
   gl::stencil_op(gl::stencil_op_enum::keep, gl::stencil_op_enum::keep, gl::stencil_op_enum::keep);
   m_cache.front().set_stencil_fail_op   (gl::stencil_op_enum::keep);
   m_cache.front().set_z_fail_op         (gl::stencil_op_enum::keep);
   m_cache.front().set_z_pass_op         (gl::stencil_op_enum::keep);
   m_cache.back ().set_stencil_fail_op   (gl::stencil_op_enum::keep);
   m_cache.back ().set_z_fail_op         (gl::stencil_op_enum::keep);
   m_cache.back ().set_z_pass_op         (gl::stencil_op_enum::keep);

   gl::stencil_mask(0xffff);
   m_cache.front().set_write_mask(0xffff);
   m_cache.back ().set_write_mask(0xffff);

   gl::stencil_func(gl::stencil_function_enum::always, 0, 0xffff);
   m_cache.front().set_function  (gl::stencil_function_enum::always);
   m_cache.front().set_reference (0);
   m_cache.front().set_test_mask (0xfff);
   m_cache.back ().set_function  (gl::stencil_function_enum::always);
   m_cache.back ().set_reference (0);
   m_cache.back ().set_test_mask (0xfff);

   m_last = nullptr;
}
void stencil_state::reset()
{
   m_separate  = false;
   m_enabled   = false;

   m_front.set_stencil_fail_op (gl::stencil_op_enum::keep);
   m_front.set_z_fail_op       (gl::stencil_op_enum::keep);
   m_front.set_z_pass_op       (gl::stencil_op_enum::keep);
   m_back .set_stencil_fail_op (gl::stencil_op_enum::keep);
   m_back .set_z_fail_op       (gl::stencil_op_enum::keep);
   m_back .set_z_pass_op       (gl::stencil_op_enum::keep);

   m_front.set_write_mask(0xffff);
   m_back .set_write_mask(0xffff);

   m_front.set_function(gl::stencil_function_enum::always);
   m_back .set_function(gl::stencil_function_enum::always);

   m_front.set_reference(0);
   m_back .set_reference(0);

   m_front.set_test_mask(0xffff);
   m_back .set_test_mask(0xffff);
}
void stencil_state_tracker::execute_component(
   gl::stencil_face::value face,
   stencil_state_component const &state,
   stencil_state_component &cache
)
{
#if !DISABLE_CACHE
   if (
      (cache.stencil_fail_op() != state.stencil_fail_op()) ||
      (cache.z_fail_op()       != state.z_fail_op()      ) ||
      (cache.z_pass_op()       != state.z_pass_op()      )
   )
#endif
   {
      gl::stencil_op_separate(face, state.stencil_fail_op(), state.z_fail_op(), state.z_pass_op());
      cache.set_stencil_fail_op  (state.stencil_fail_op());
      cache.set_z_fail_op        (state.z_fail_op()      );
      cache.set_z_pass_op        (state.z_pass_op()      );
   }
#if !DISABLE_CACHE
   if (cache.write_mask() != state.write_mask())
#endif
   {
      gl::stencil_mask_separate(face, state.write_mask());
      cache.set_write_mask(state.write_mask());
   }

#if !DISABLE_CACHE
   if (
      (cache.function () != state.function())  ||
      (cache.reference() != state.reference()) ||
      (cache.test_mask() != state.test_mask()) 
   )
#endif
   {
      gl::stencil_func_Separate(face, state.function(), state.reference(), state.test_mask());
      cache.set_function   (state.function());
      cache.set_reference  (state.reference());
      cache.set_test_mask  (state.test_mask());
   }
}

void stencil_state_tracker::execute_shared(stencil_state_component const &state, stencil_state &cache)
{
#if !DISABLE_CACHE
   if (
      (cache.front().stencil_fail_op() != state.stencil_fail_op()) ||
      (cache.front().z_fail_op      () != state.z_fail_op      ()) ||
      (cache.front().z_pass_op      () != state.z_pass_op      ()) ||
      (cache.back ().stencil_fail_op() != state.stencil_fail_op()) ||
      (cache.back ().z_fail_op      () != state.z_fail_op      ()) ||
      (cache.back ().z_pass_op      () != state.z_pass_op      ()) 
   )
#endif
   {
      gl::stencil_op(state.stencil_fail_op(), state.z_fail_op(), state.z_pass_op());
      cache.front().set_stencil_fail_op   (state.stencil_fail_op());
      cache.front().set_z_fail_op         (state.z_fail_op      ());
      cache.front().set_z_pass_op         (state.z_pass_op      ());
      cache.back ().set_stencil_fail_op   (state.stencil_fail_op());
      cache.back ().set_z_fail_op         (state.z_fail_op      ());
      cache.back ().set_z_pass_op         (state.z_pass_op      ());
   }

#if !DISABLE_CACHE
   if (
      (cache.front().write_mask() != state.write_mask()) ||
      (cache.back ().write_mask() != state.write_mask())
   )
#endif
   {
      gl::stencil_mask(state.write_mask());
      cache.front().set_write_mask(state.write_mask());
      cache.back ().set_write_mask(state.write_mask());
   }

#if !DISABLE_CACHE
   if (
      (cache.front().function () != state.function ()) ||
      (cache.front().reference() != state.reference()) ||
      (cache.front().test_mask() != state.test_mask()) ||
      (cache.back ().function () != state.function ()) ||
      (cache.back ().reference() != state.reference()) ||
      (cache.back ().test_mask() != state.test_mask())
   )
#endif
   {
      gl::stencil_func(state.function(), state.reference(), state.test_mask());
      cache.front().set_function (state.function ());
      cache.front().set_reference(state.reference());
      cache.front().set_test_mask(state.test_mask());
      cache.back ().set_function (state.function ());
      cache.back ().set_reference(state.reference());
      cache.back ().set_test_mask(state.test_mask());
   }
}
void stencil_state_tracker::execute(stencil_state const *state)
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
         gl::enable(gl::enable_cap::stencil_test);
         m_cache.set_enabled(true);
      }
      if (state->separate())
      {
         execute_component(gl::stencil_face::front,   state->front(),   m_cache.front());
         execute_component(gl::stencil_face::back,    state->back(),    m_cache.back ());
         m_cache.set_separate(true);
      }
      else
      {
#if !DISABLE_CACHE
         if (!m_cache.separate())
         {
            //  Cache already in shared state
            execute_component(gl::stencil_face::front_and_back, state->front(), m_cache.front());
         }
         else
#endif
         {
            //  Cache not yet in shared state - make it shared
            execute_shared(state->front(), m_cache);
            m_cache.set_separate(false);
         }
      }
   }
   else
   {
#if !DISABLE_CACHE
      if (m_cache.enabled())
#endif
      {
         gl::disable(gl::enable_cap::stencil_test);
         m_cache.set_enabled(false);
      }
   }
   m_last = state;
}

} }
