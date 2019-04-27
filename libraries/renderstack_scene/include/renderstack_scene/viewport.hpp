#ifndef viewport_hpp_renderstack_scene
#define viewport_hpp_renderstack_scene

#include "renderstack_toolkit/platform.hpp"
#include <cstdint>

namespace renderstack
{
namespace scene
{

struct Viewport
{
    float aspect_ratio() const
    {
        return (height != 0.0f) ? (static_cast<float>(width) / static_cast<float>(height))
                                : 1.0f;
    }

    int border{0};
    int x{0};
    int y{0};
    int width{0};
    int height{0};
};

} // namespace scene
} // namespace renderstack

#endif
