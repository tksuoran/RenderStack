#ifndef sphere_hpp_renderstack_geometry
#define sphere_hpp_renderstack_geometry

#include "renderstack_geometry/geometry.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace shapes
{

Geometry make_sphere(double radius, unsigned int slice_count, unsigned int stack_division);

} // namespace shapes
} // namespace geometry
} // namespace renderstack

#endif
