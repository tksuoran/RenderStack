#include "renderstack_scene/frame.hpp"

namespace renderstack { namespace scene {

using namespace std;

frame::frame()
:  m_updated            (false)
,  m_last_update_serial (0)
,  m_parent             (nullptr)
{
}

void frame::update_hierarchical_no_cache()
{
   if (m_parent == shared_from_this())
      throw runtime_error("frame parent is self");
 
   if (m_parent)
   {
      m_parent->update_hierarchical_no_cache();
      m_world_from_local.set(
         m_parent->world_from_local().matrix() * m_parent_from_local.matrix(),
         m_parent_from_local.inverse_matrix() * m_parent->world_from_local().inverse_matrix()
      );
   }
   else
   {
      m_world_from_local.set(
         m_parent_from_local.matrix(),
         m_parent_from_local.inverse_matrix()
      );
   }
}
void frame::update_hierarchical(uint32_t update_serial)
{
   if (m_updated)
      return;

   if (m_last_update_serial == update_serial)
      return;

   m_last_update_serial = update_serial;
   if (m_parent == shared_from_this())
      throw runtime_error("frame parent is self");

   if (m_parent)
   {
      m_parent->update_hierarchical(update_serial);
      m_world_from_local.set(
         m_parent->world_from_local().matrix() * m_parent_from_local.matrix(),
         m_parent_from_local.inverse_matrix() * m_parent->world_from_local().inverse_matrix()
      );
   }
   else
   {
      m_world_from_local.set(
         m_parent_from_local.matrix(),
         m_parent_from_local.inverse_matrix()
      );
   }
}

} }
