#ifndef renderstack_graphics__color_mask_state_hpp
#define renderstack_graphics__color_mask_state_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/render_state.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace graphics {

class color_mask_state : public render_state
{
public:
    bool red  () const;
    bool green() const;
    bool blue () const;
    bool alpha() const;
    void set_red  (bool value);
    void set_green(bool value);
    void set_blue (bool value);
    void set_alpha(bool value);

public:
    static color_mask_state const &default_();

    static void reset_state();

    void reset();
    void execute() const;

private:
    bool m_red;
    bool m_green;
    bool m_blue;
    bool m_alpha;

private:
    static color_mask_state         s_default;
    static color_mask_state const   *s_last;
    static color_mask_state         s_state_cache;
};

} }

#endif
