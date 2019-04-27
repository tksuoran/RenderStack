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

class Frame : public std::enable_shared_from_this<Frame>
{
public:
    void update_hierarchical_no_cache();

    void update_hierarchical(uint32_t update_serial);

    std::string name;
    Frame       *parent{nullptr};
    Transform   parent_from_local;
    Transform   world_from_local;

protected:
    bool          m_updated{false};
    std::uint32_t m_last_update_serial{0U};
};

} // namespace scene
} // namespace renderstack

#endif
