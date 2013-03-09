#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/context.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include <stdexcept>

namespace renderstack { namespace graphics {

using namespace std;

uniform_buffer_range::uniform_buffer_range(
   weak_ptr<uniform_block> block,
   weak_ptr<uniform_buffer> uniform_buffer
)
:  m_uniform_block(block)
,  m_uniform_buffer(uniform_buffer)
{
   auto b = block.lock();
#if 0
   m_uniform_buffer = context::current()->get_uniform_buffer(
      gl::buffer_usage_hint::static_draw,
      b->size()
   );
#else
#endif
   //m_uniform_block = block;
   m_first_byte = m_uniform_buffer.lock()->allocate_bytes(b->size());
   m_byte_count = b->size();
   m_in_edit = false;
}

uniform_buffer_range::~uniform_buffer_range()
{
}

void uniform_buffer_range::bind(unsigned int binding_point)
{
   if (configuration::can_use.uniform_buffer_object)
   {
      m_uniform_buffer.lock()->bind_range(
         binding_point,
         m_first_byte,
         m_byte_count
      );
   }
}
unsigned char *uniform_buffer_range::begin_edit()
{
   if (m_in_edit)
      throw runtime_error("buffer was already in edit");

   m_uniform_buffer.lock()->bind();
   unsigned char *start = reinterpret_cast<unsigned char*>(
      m_uniform_buffer.lock()->map_bytes(
         m_first_byte, 
         m_byte_count, 
         gl::buffer_access_mask::map_write_bit
      )
   );
   m_in_edit = true;
   return start;
}

void uniform_buffer_range::end_edit()
{
   if (!m_in_edit)
      throw runtime_error("buffer was not in edit");

   m_uniform_buffer.lock()->unmap_bytes();
   m_in_edit = false;
}

} }
