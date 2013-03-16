#include "renderstack_toolkit/platform.hpp"
#include "renderstack_renderer/blend_state.hpp"

namespace renderstack { namespace renderer {

using namespace glm;

blend_state const *blend_state::s_last = nullptr;
blend_state blend_state::s_default;
blend_state blend_state::s_state_cache;

blend_state_component::blend_state_component()
:  m_equation_mode     (gl::blend_equation_mode::func_add)
,  m_source_factor     (gl::blending_factor_src::one)
,  m_destination_factor(gl::blending_factor_dest::one)
{
}

gl::blend_equation_mode::value blend_state_component::equation_mode() const
{
   return m_equation_mode;
}
gl::blending_factor_src::value blend_state_component::source_factor() const
{
   return m_source_factor;
}
gl::blending_factor_dest::value blend_state_component::destination_factor() const
{
   return m_destination_factor;
} 

void blend_state_component::set_equation_mode(gl::blend_equation_mode::value value)
{
   m_equation_mode = value;
}
void blend_state_component::set_source_factor(gl::blending_factor_src::value value)
{
   m_source_factor = value;
}
void blend_state_component::set_destination_factor(gl::blending_factor_dest::value value)
{
   m_destination_factor = value;
}

void blend_state_component::reset()
{
   m_equation_mode      = gl::blend_equation_mode::func_add;
   m_source_factor      = gl::blending_factor_src::one;
   m_destination_factor = gl::blending_factor_dest::one;
}

/*static*/ blend_state const &blend_state::default_()
{
   return s_default;
}

/*static*/ void blend_state::reset_state()
{
   gl::blend_color(0.0f, 0.0f, 0.0f, 0.0f);
   s_state_cache.m_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
   gl::blend_equation_separate(gl::blend_equation_mode::func_add, gl::blend_equation_mode::func_add);
   s_state_cache.m_rgb  .set_equation_mode(gl::blend_equation_mode::func_add);
   s_state_cache.m_alpha.set_equation_mode(gl::blend_equation_mode::func_add);
   gl::blend_func_separate(
      gl::blending_factor_src::one,
      gl::blending_factor_dest::one,
      gl::blending_factor_src::one,
      gl::blending_factor_dest::one
   );
   s_state_cache.m_rgb.set_source_factor       (gl::blending_factor_src::one);
   s_state_cache.m_rgb.set_source_factor       (gl::blending_factor_src::one);
   s_state_cache.m_alpha.set_destination_factor(gl::blending_factor_dest::one);
   s_state_cache.m_alpha.set_destination_factor(gl::blending_factor_dest::one);
   gl::disable(gl::enable_cap::blend);
   s_state_cache.m_enabled = false;
   s_last = nullptr;
}

blend_state::blend_state()
{
   reset();
}
blend_state::~blend_state()
{
}

bool blend_state::enabled() const
{
   return m_enabled;
}
blend_state_component const &blend_state::rgb() const
{
   return m_rgb;
}
blend_state_component &blend_state::rgb()
{
   return m_rgb;
}
blend_state_component const &blend_state::alpha() const
{
   return m_alpha;
}
blend_state_component &blend_state::alpha()
{
   return m_alpha;
}
vec4 const &blend_state::color() const
{
   return m_color;
}
vec4 &blend_state::color()
{
   return m_color;
}
void blend_state::set_enabled(bool value)
{
   m_enabled = value;
}
void blend_state::set_color(vec4 const &value)
{
   m_color = value;
}
void blend_state::reset()
{
   m_enabled = false;
   m_rgb.reset();
   m_alpha.reset();
   m_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
void blend_state::execute() const
{
#if !DISABLE_CACHE
   if (s_last == this)
      return;

#endif
   if (m_enabled)
   {
#if !DISABLE_CACHE
      if (s_state_cache.enabled() == false)
#endif
      {
         gl::enable(gl::enable_cap::blend);
         s_state_cache.set_enabled(true);
      }
#if !DISABLE_CACHE
      if (s_state_cache.color() != m_color)
#endif
      {
         gl::blend_color(
            m_color.x,
            m_color.y,
            m_color.z,
            m_color.w
         );
         s_state_cache.set_color(m_color);
      }
#if !DISABLE_CACHE
      if (
         (s_state_cache.rgb().equation_mode() != rgb().equation_mode()) ||
         (s_state_cache.alpha().equation_mode() != alpha().equation_mode())
      )
#endif
      {
         gl::blend_equation_separate(rgb().equation_mode(), alpha().equation_mode());
         s_state_cache.rgb  ().set_equation_mode(rgb  ().equation_mode());
         s_state_cache.alpha().set_equation_mode(alpha().equation_mode());
      }
#if !DISABLE_CACHE
      if (
         (s_state_cache.rgb  ().source_factor     () != rgb  ().source_factor()     ) ||
         (s_state_cache.rgb  ().destination_factor() != rgb  ().destination_factor()) ||
         (s_state_cache.alpha().source_factor     () != alpha().source_factor()     ) ||
         (s_state_cache.alpha().destination_factor() != alpha().destination_factor())
      )
#endif
      {
         gl::blend_func_separate(
            rgb().source_factor(),
            rgb().destination_factor(),
            alpha().source_factor(),
            alpha().destination_factor()
         );
         s_state_cache.rgb  ().set_source_factor     (rgb  ().source_factor()     );
         s_state_cache.rgb  ().set_destination_factor(rgb  ().destination_factor());
         s_state_cache.alpha().set_source_factor     (alpha().source_factor()     );
         s_state_cache.alpha().set_destination_factor(alpha().destination_factor());
      }

   }
   else
   {

#if !DISABLE_CACHE
      if (s_state_cache.enabled() == true)
#endif
      {
         gl::disable(gl::enable_cap::blend);
         s_state_cache.set_enabled(false);
      }
   }
   s_last = this;
}

//////
} }
