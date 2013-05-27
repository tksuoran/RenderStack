#ifndef renderstack_graphics__blend_state_hpp
#define renderstack_graphics__blend_state_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/render_state.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace graphics {

class blend_state_component
{
public:
   blend_state_component();

   gl::blend_equation_mode::value   equation_mode() const;
   gl::blending_factor_src::value   source_factor() const;
   gl::blending_factor_dest::value  destination_factor() const;

   void set_equation_mode(gl::blend_equation_mode::value value);
   void set_source_factor(gl::blending_factor_src::value value);
   void set_destination_factor(gl::blending_factor_dest::value value);

   void reset();

private:
   gl::blend_equation_mode::value   m_equation_mode;
   gl::blending_factor_src::value   m_source_factor;
   gl::blending_factor_dest::value  m_destination_factor;
};

class blend_state : public render_state
{
public:
   static blend_state const &default_();

   static void reset_state();

public:
   blend_state();
   virtual ~blend_state();

   bool                        enabled() const;
   blend_state_component const &rgb()    const;
   blend_state_component       &rgb();
   blend_state_component const &alpha()  const;
   blend_state_component       &alpha();
   glm::vec4             const &color()  const;
   glm::vec4                   &color();

   void set_enabled(bool value);
   void set_color(glm::vec4 const &value);

   void reset();
   void execute() const;

private:
   bool                    m_enabled;
   blend_state_component   m_rgb;
   blend_state_component   m_alpha;
   glm::vec4               m_color;

private:
   static blend_state const   *s_last;
   static blend_state         s_default;
   static blend_state         s_state_cache;
};

} }

#endif
