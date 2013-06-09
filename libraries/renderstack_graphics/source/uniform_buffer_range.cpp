#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/renderer.hpp"
#include <stdexcept>

namespace renderstack { namespace graphics {

using namespace std;

uniform_buffer_range::uniform_buffer_range(
   weak_ptr<class uniform_block> block,
   weak_ptr<class buffer> uniform_buffer
)
:  m_uniform_block   (block)
,  m_uniform_buffer  (uniform_buffer)
{
   assert(block.lock());
   assert(uniform_buffer.lock());

   auto b = block.lock();

   m_first_byte = m_uniform_buffer.lock()->allocate(b->size());
   m_byte_count = b->size();
   m_in_edit = false;
}

uniform_buffer_range::~uniform_buffer_range()
{
}

unsigned char *uniform_buffer_range::begin_edit(class renderer &renderer)
{
   if (m_in_edit)
      throw runtime_error("buffer was already in edit");

   // We assume client is mapping
   if (
      !renderer.buffer_is_bound(
         buffer_target::uniform_buffer,
         m_uniform_buffer.lock()
      )
   )
   {
      throw runtime_error("uniform buffer is not bound, cannot begin edit");
   }

   unsigned char *start = reinterpret_cast<unsigned char*>(
      m_uniform_buffer.lock()->map(
         renderer,
         m_first_byte, 
         m_byte_count, 
         gl::buffer_access_mask::map_write_bit
      )
   );
   m_in_edit = true;
   return start;
}

void uniform_buffer_range::end_edit(class renderer &renderer)
{
   if (!m_in_edit)
      throw runtime_error("buffer was not in edit");

   m_uniform_buffer.lock()->unmap(renderer);
   m_in_edit = false;
}

} }
