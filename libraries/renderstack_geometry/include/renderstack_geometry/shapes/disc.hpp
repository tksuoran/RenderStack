#ifndef disc_hpp_renderstack_geometry
#define disc_hpp_renderstack_geometry

#include "renderstack_geometry/geometry.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{

class point;
class corner;
class polygon;

} // namespace geometry
} // namespace renderstack

namespace renderstack
{
namespace geometry
{
namespace shapes
{

Geometry make_disc(double outer_radius,
                   double inner_radius,
                   int    slice_count,
                   int    stack_count);

} // namespace shapes
} // namespace geometry
} // namespace renderstack

#endif
