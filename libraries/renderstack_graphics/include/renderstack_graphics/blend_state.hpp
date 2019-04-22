#ifndef blend_state_hpp_renderstack_graphics
#define blend_state_hpp_renderstack_graphics

#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace graphics
{

class blend_state_component
{
public:
    blend_state_component();

    gl::blend_equation_mode::value  equation_mode() const;
    gl::blending_factor_src::value  source_factor() const;
    gl::blending_factor_dest::value destination_factor() const;

    void set_equation_mode(gl::blend_equation_mode::value value);
    void set_source_factor(gl::blending_factor_src::value value);
    void set_destination_factor(gl::blending_factor_dest::value value);

    void reset();

private:
    gl::blend_equation_mode::value  m_equation_mode;
    gl::blending_factor_src::value  m_source_factor;
    gl::blending_factor_dest::value m_destination_factor;
};

class blend_state
{
public:
    blend_state();

    void reset();

    bool                         enabled() const;
    blend_state_component const &rgb() const;
    blend_state_component &      rgb();
    blend_state_component const &alpha() const;
    blend_state_component &      alpha();
    glm::vec4 const &            color() const;
    glm::vec4 &                  color();

    void set_enabled(bool value);
    void set_color(glm::vec4 const &value);

private:
    bool                  m_enabled;
    blend_state_component m_rgb;
    blend_state_component m_alpha;
    glm::vec4             m_color;
};

class blend_state_tracker
{
public:
    blend_state_tracker();

    void reset();
    void execute(blend_state const *state);

private:
    blend_state const *m_last;
    blend_state        m_cache;
};

} // namespace graphics
} // namespace renderstack

#endif
