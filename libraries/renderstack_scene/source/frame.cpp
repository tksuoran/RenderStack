#include "renderstack_scene/frame.hpp"

namespace renderstack
{
namespace scene
{

using namespace std;


void Frame::update_hierarchical_no_cache()
{
    if (parent == this)
    {
        throw runtime_error("frame parent is self");
    }

    if (parent)
    {
        parent->update_hierarchical_no_cache();
        world_from_local.set(parent->world_from_local.matrix() * parent_from_local.matrix(),
                             parent_from_local.inverse_matrix() * parent->world_from_local.inverse_matrix());
    }
    else
    {
        world_from_local.set(parent_from_local.matrix(),
                             parent_from_local.inverse_matrix());
    }
}

void Frame::update_hierarchical(uint32_t update_serial)
{
    if (m_updated)
    {
        return;
    }

    if (m_last_update_serial == update_serial)
    {
        return;
    }

    m_last_update_serial = update_serial;
    if (parent == this)
    {
        throw runtime_error("frame parent is self");
    }

    if (parent)
    {
        parent->update_hierarchical(update_serial);
        world_from_local.set(parent->world_from_local.matrix() * parent_from_local.matrix(),
                             parent_from_local.inverse_matrix() * parent->world_from_local.inverse_matrix());
    }
    else
    {
        world_from_local.set(parent_from_local.matrix(),
                             parent_from_local.inverse_matrix());
    }
}

} // namespace scene
} // namespace renderstack
