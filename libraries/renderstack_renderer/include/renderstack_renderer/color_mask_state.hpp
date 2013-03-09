#ifndef renderstack_renderer__color_mask_state_hpp
#define renderstack_renderer__color_mask_state_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_renderer/render_state.hpp"
#include <glm/glm.hpp>

namespace renderstack { namespace renderer {

class color_mask_state : public render_state
{
private:
    bool m_red;
    bool m_green;
    bool m_blue;
    bool m_alpha;
    bool m_depth;

public:
    bool red  () const;
    bool green() const;
    bool blue () const;
    bool alpha() const;
    bool depth() const;
    void set_red  (bool value);
    void set_green(bool value);
    void set_blue (bool value);
    void set_alpha(bool value);
    void set_depth(bool value);

private:
    static color_mask_state s_default;
    static color_mask_state *s_last;
    static color_mask_state s_state_cache;

public:
    static color_mask_state const &default_();

    static void reset_state();

    void reset();
    void execute();
};

} }

#endif
