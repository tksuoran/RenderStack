
#ifndef camera_hpp_renderstack_scene
#define camera_hpp_renderstack_scene

#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_scene/viewport.hpp"
#include "renderstack_scene/frame.hpp"
#include "renderstack_scene/projection.hpp"
#include "renderstack_scene/transform.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <memory>
#include <string>

namespace renderstack
{
namespace scene
{

struct Viewport;

class Camera
{
public:
    void update(Viewport viewport);

    std::string name;
    Frame       frame;
    Projection  projection;
    Transform   clip_from_view;
    Transform   clip_from_world;
};

} // namespace scene
} // namespace renderstack

#endif
