#ifndef renderstack_mesh__index_range_hpp
#define renderstack_mesh__index_range_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cstddef>

namespace renderstack { namespace mesh {

struct index_range
{
   gl::begin_mode::value   begin_mode;
   std::size_t             first_index;
   std::size_t             index_count;
};

} }

#endif
