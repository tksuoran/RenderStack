#ifndef frame_hpp_renderstack_scene
#define frame_hpp_renderstack_scene

#include "renderstack_scene/transform.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>

namespace renderstack
{
namespace scene
{

class frame : public std::enable_shared_from_this<frame>
{
public:
    frame();

    std::string const &name() const
    {
        return m_name;
    }
    void set_name(std::string const &value)
    {
        m_name = value;
    }

    bool updated() const
    {
        return m_updated;
    }
    void set_updated(bool value)
    {
        m_updated = value;
    }

    std::shared_ptr<frame> parent()
    {
        return m_parent;
    }
    void set_parent(std::shared_ptr<frame> value)
    {
        m_parent = value;
    }

    transform const &parent_from_local() const
    {
        return m_parent_from_local;
    }
    transform &parent_from_local()
    {
        return m_parent_from_local;
    }

    transform const &world_from_local() const
    {
        return m_world_from_local;
    }
    transform &world_from_local()
    {
        return m_world_from_local;
    }

    void update_hierarchical_no_cache();
    void update_hierarchical(uint32_t update_serial);

private:
    std::string            m_name;
    bool                   m_updated;
    std::uint32_t          m_last_update_serial;
    std::shared_ptr<frame> m_parent;
    transform              m_parent_from_local;
    transform              m_world_from_local;
};

} // namespace scene
} // namespace renderstack

#endif
