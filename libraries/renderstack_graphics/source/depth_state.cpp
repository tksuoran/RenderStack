#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/depth_state.hpp"

namespace renderstack { namespace graphics {

depth_state depth_state::s_default;
depth_state depth_state::s_disabled;
depth_state const *depth_state::s_last = nullptr;
depth_state depth_state::s_state_cache;


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

/*static*/ depth_state const &depth_state::default_()
{
   return s_default;
}
/*static*/ depth_state const &depth_state::disabled()
{
   return s_disabled;
}
depth_state::depth_state()
:  m_enabled   (false)
,  m_function  (gl::depth_function::less)
,  m_near      (0.0f)
,  m_far       (1.0f)
,  m_depth_mask(false)
{
}
depth_state::depth_state(bool enabled)
:  m_enabled   (enabled)
,  m_function  (gl::depth_function::less)
,  m_near      (0.0f)
,  m_far       (1.0f)
,  m_depth_mask(enabled)  // TODO Is this correct?
{
}
/*virtual*/ depth_state::~depth_state()
{
}
/*static*/ void depth_state::reset_state()
{
   gl::depth_func(gl::depth_function::less);
   s_state_cache.set_function(gl::depth_function::less);
   gl::depth_range(0.0f, 1.0f);
   s_state_cache.set_near(0.0f);
   s_state_cache.set_far(1.0f);
   gl::enable(gl::enable_cap::depth_test);
   s_state_cache.set_enabled(true);
   s_last = nullptr;
}
void depth_state::reset()
{
   set_enabled(true);
   set_function(gl::depth_function::less);
   set_near(0.0f);
   set_far(1.0);
}
void depth_state::execute() const
{
#if !DISABLE_CACHE
   if (s_last == this)
      return;

#endif
   if (enabled())
   {
#if !DISABLE_CACHE
      if (s_state_cache.enabled() == false)
#endif
      {
         gl::enable(gl::enable_cap::depth_test);
         s_state_cache.set_enabled(true);
      }
#if !DISABLE_CACHE
      if (s_state_cache.function() != function())
#endif
      {
         gl::depth_func(function());
         s_state_cache.set_function(function());
      }
#if !DISABLE_CACHE
      if (
         (s_state_cache.near_() != near_()) ||
         (s_state_cache.far_()  != far_())
      )
#endif
      {
         gl::depth_range(near_(), far_());
         s_state_cache.set_near(near_());
         s_state_cache.set_far(far_());
      }

   }
   else
   {

#if !DISABLE_CACHE
      if (s_state_cache.enabled() == true)
#endif
      {
         gl::disable(gl::enable_cap::depth_test);
         s_state_cache.set_enabled(false);
      }
   }

#if !DISABLE_CACHE
   if (s_state_cache.depth_mask() != m_depth_mask)
#endif
   {
      gl::depth_mask(m_depth_mask);
      s_state_cache.m_depth_mask = m_depth_mask;
   }

   s_last = this;
}

} }
