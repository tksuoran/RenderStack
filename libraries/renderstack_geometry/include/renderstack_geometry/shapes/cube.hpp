#ifndef cube_hpp_renderstack_geometry
#define cube_hpp_renderstack_geometry

#include "renderstack_geometry/geometry.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cmath>

namespace renderstack
{
namespace geometry
{
class point;
}
} // namespace renderstack

namespace renderstack
{
namespace geometry
{
namespace shapes
{

Geometry make_cube(double x_size, double y_size, double z_size);

Geometry make_cube(double r);

Geometry make_cube(const glm::vec3 &size, const glm::ivec3 &div, float p = 1.0f);


} // namespace shapes
} // namespace geometry
} // namespace renderstack

#endif
