#include "renderstack_toolkit/platform.hpp"
#include "renderstack_renderer/face_cull_state.hpp"

namespace renderstack { namespace renderer {

face_cull_state face_cull_state::s_default;
face_cull_state face_cull_state::s_disabled;
face_cull_state *face_cull_state::s_last = nullptr;
face_cull_state face_cull_state::s_state_cache;

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

/*static*/ face_cull_state const &face_cull_state::default_()
{
   return s_default;
}
/*static*/ face_cull_state const &face_cull_state::disabled()
{
   return s_disabled;
}

face_cull_state::face_cull_state()
:   m_enabled(false)
,   m_cull_face_mode(gl::cull_face_mode::back)
,   m_front_face_direction(gl::front_face_direction::ccw)
{
}

face_cull_state::face_cull_state(bool enabled)
:   m_enabled(enabled)
,   m_cull_face_mode(gl::cull_face_mode::back)
,   m_front_face_direction(gl::front_face_direction::ccw)
{
}

/*static*/ void face_cull_state::reset_state()
{
   gl::cull_face(gl::cull_face_mode::back);
   s_state_cache.set_cull_face_mode(gl::cull_face_mode::back);
   gl::front_face(gl::front_face_direction::ccw);
   s_state_cache.set_front_face_direction(gl::front_face_direction::ccw);
   gl::disable(gl::enable_cap::cull_face);
   s_state_cache.set_enabled(false);
   s_last = nullptr;
}

void face_cull_state::reset()
{
   set_enabled(true);
   set_cull_face_mode(gl::cull_face_mode::back);
   set_front_face_direction(gl::front_face_direction::ccw);
}
void face_cull_state::execute()
{
#if !DISABLE_CACHE
   if (s_last == this)
   {
      return;
   }
#endif
   if (m_enabled)
   {
#if !DISABLE_CACHE
      if (s_state_cache.enabled() == false)
#endif
      {
            gl::enable(gl::enable_cap::cull_face);
            s_state_cache.set_enabled(true);
      }
#if !DISABLE_CACHE
      if (s_state_cache.cull_face_mode() != m_cull_face_mode)
#endif
      {
            gl::cull_face(m_cull_face_mode);
            s_state_cache.set_cull_face_mode(m_cull_face_mode);
      }
#if !DISABLE_CACHE
      if (s_state_cache.front_face_direction() != m_front_face_direction)
#endif
      {
         gl::front_face(m_front_face_direction);
         s_state_cache.set_front_face_direction(m_front_face_direction);
      }
   }
   else
   {
#if !DISABLE_CACHE
      if (s_state_cache.enabled() == true)
#endif
      {
         gl::disable(gl::enable_cap::cull_face);
         s_state_cache.set_enabled(false);
      }
   }
   s_last = this;
}

} }
