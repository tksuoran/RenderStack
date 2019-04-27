#ifndef cone_hpp_renderstack_geometry
#define cone_hpp_renderstack_geometry

#include "renderstack_geometry/geometry.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace shapes
{

Geometry make_conical_frustum(double min_x,
                              double max_x,
                              double bottom_radius,
                              double top_radius,
                              bool   use_bottom,
                              bool   use_top,
                              int    slice_count,
                              int    stack_division);

// Cone is special of conical frustum which is not cut from top
// top is at max_x, bottom is at min_x
Geometry make_cone(double min_x,
                   double max_x,
                   double bottom_radius,
                   bool   use_bottom,
                   int    slice_count,
                   int    stack_division);

// Cylinder is special of conical frustum which has top radius equal to bottom radius
Geometry make_cylinder(double min_x,
                       double max_x,
                       double radius,
                       bool   use_bottom,
                       bool   use_top,
                       int    slice_count,
                       int    stack_division);

} // namespace shapes
} // namespace geometry
} // namespace renderstack

#endif
