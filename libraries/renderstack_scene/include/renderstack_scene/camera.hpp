
#ifndef camera_hpp_renderstack_scene
#define camera_hpp_renderstack_scene

#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
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

class camera
{
public:
    camera();

    void update(class viewport const &viewport);

public:
    std::string const &name()
    {
        return m_name;
    }
    void set_name(std::string const &value)
    {
        m_name = value;
    }

    std::shared_ptr<class frame> frame() const
    {
        return m_frame;
    }

    class projection const &projection() const
    {
        return m_projection;
    }
    class projection &projection()
    {
        return m_projection;
    }

    transform const &clip_from_view() const
    {
        return m_clip_from_view;
    }
    transform &clip_from_view()
    {
        return m_clip_from_view;
    }

    transform const &clip_from_world() const
    {
        return m_clip_from_world;
    }
    transform &clip_from_world()
    {
        return m_clip_from_world;
    }

private:
    std::string                  m_name;
    std::shared_ptr<class frame> m_frame;
    class projection             m_projection;
    transform                    m_clip_from_view;
    transform                    m_clip_from_world;
};

} // namespace scene
} // namespace renderstack

#endif
