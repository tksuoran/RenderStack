#ifndef index_range_hpp_renderstack_mesh
#define index_range_hpp_renderstack_mesh

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cstddef>

namespace renderstack { namespace mesh {

struct index_range
{
   index_range()
   :  begin_mode(gl::begin_mode::points)
   ,  first_index(0)
   ,  index_count(0)
   {
   }

   gl::begin_mode::value   begin_mode;
   std::size_t             first_index;
   std::size_t             index_count;
};

} }

#endif
