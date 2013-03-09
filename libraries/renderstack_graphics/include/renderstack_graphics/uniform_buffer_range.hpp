#ifndef renderstack_graphics__uniform_buffer_range_hpp
#define renderstack_graphics__uniform_buffer_range_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/uniform_buffer.hpp"
#include <memory>

namespace renderstack { namespace graphics {

class uniform_block;
class uniform_buffer;

class uniform_buffer_range
{
public:
   uniform_buffer_range(
      std::weak_ptr<uniform_block>  block,
      std::weak_ptr<uniform_buffer> uniform_buffer
   );
   ~uniform_buffer_range();

   void           bind(unsigned int binding_point);
   unsigned char  *begin_edit();
   void           end_edit();

private:
   std::weak_ptr<uniform_block>  m_uniform_block;
   std::weak_ptr<uniform_buffer> m_uniform_buffer;
   std::size_t                   m_first_byte;
   std::size_t                   m_byte_count;
   bool                          m_in_edit;
};

} }

#endif
