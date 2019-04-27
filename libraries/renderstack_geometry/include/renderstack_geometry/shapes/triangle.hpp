#ifndef triangle_hpp_renderstack_geometry
#define triangle_hpp_renderstack_geometry

#include "renderstack_geometry/geometry.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace shapes
{

Geometry make_triangle(double radius);

Geometry make_quad(double edge);

Geometry make_cuboctahedron(double radius);

Geometry make_dodecahedron(double radius);

Geometry make_icosahedron(double radius);

Geometry make_octahedron(double radius);

Geometry make_tetrahedron(double radius);

} // namespace shapes
} // namespace geometry
} // namespace renderstack

#endif
