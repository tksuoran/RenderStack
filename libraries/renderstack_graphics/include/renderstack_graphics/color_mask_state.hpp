#ifndef color_mask_state_hpp_renderstack_graphics
#define color_mask_state_hpp_renderstack_graphics

#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace graphics
{

struct Color_mask_state
{
    void reset();

    bool red{true};
    bool green{true};
    bool blue{true};
    bool alpha{true};
};

struct Color_mask_state_tracker
{
public:
    void reset();
    void execute(Color_mask_state const *state);

private:
    Color_mask_state const *m_last{nullptr};
    Color_mask_state        m_cache;
};

} // namespace graphics
} // namespace renderstack

#endif
