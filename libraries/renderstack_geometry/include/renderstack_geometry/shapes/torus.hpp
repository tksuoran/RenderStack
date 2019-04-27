#ifndef torus_hpp_renderstack_geometry
#define torus_hpp_renderstack_geometry

#include "renderstack_geometry/geometry.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace shapes
{

Geometry make_torus(double major_radius, double minor_radius, int major_axis_steps, int minor_axis_steps);

} // namespace shapes
} // namespace geometry
} // namespace renderstack

#endif
