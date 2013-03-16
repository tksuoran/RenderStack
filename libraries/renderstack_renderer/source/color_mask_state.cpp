#include "renderstack_toolkit/platform.hpp"
#include "renderstack_renderer/color_mask_state.hpp"

namespace renderstack { namespace renderer {

color_mask_state color_mask_state::s_default;
color_mask_state const *color_mask_state::s_last = nullptr;
color_mask_state color_mask_state::s_state_cache;

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
/*static*/ color_mask_state const &color_mask_state::default_()
{
   return s_default;
}
/*static*/ void color_mask_state::reset_state()
{
   gl::color_mask(true, true, true, true);
   s_state_cache.set_red  (true);
   s_state_cache.set_green(true);
   s_state_cache.set_blue (true);
   s_state_cache.set_alpha(true);
   s_last = nullptr;
}
void color_mask_state::reset()
{
   set_red  (true);
   set_green(true);
   set_blue (true);
   set_alpha(true);
}
void color_mask_state::execute() const
{
#if !DISABLE_CACHE
   if (s_last == this)
   {
      return;
   }
   if (
      (s_state_cache.red  () != m_red  ) ||
      (s_state_cache.green() != m_green) ||
      (s_state_cache.blue () != m_blue ) ||
      (s_state_cache.alpha() != m_alpha)
   )
#endif
   {
      gl::color_mask(m_red, m_green, m_blue, m_alpha);
      s_state_cache.set_red  (m_red  );
      s_state_cache.set_green(m_green);
      s_state_cache.set_blue (m_blue );
      s_state_cache.set_alpha(m_alpha);
   }
   s_last = this;
}

} }
