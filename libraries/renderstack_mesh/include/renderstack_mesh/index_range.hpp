#ifndef index_range_hpp_renderstack_mesh
#define index_range_hpp_renderstack_mesh

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cstddef>

namespace renderstack
{
namespace mesh
{

struct Index_range
{
    gl::begin_mode::value begin_mode{gl::begin_mode::points};
    size_t first_index{0U};
    size_t index_count{0U};
};

} // namespace mesh
} // namespace renderstack

#endif
