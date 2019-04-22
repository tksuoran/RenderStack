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

// http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T>
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

class cube : public geometry
{
public:
    cube(double x_size, double y_size, double z_size);
    cube(glm::vec3 const &size, glm::ivec3 const &div, float p = 1.0f);
    cube(double r);

protected:
    struct make_info;

    point *make_point(make_info &info, int x, int y, int z, glm::vec3 const &n, float s, float t);

private:
    float spow(float x, float p) const;

private:
    corner *make_corner(make_info &info, polygon *polygon, int x, int y, int z, glm::vec3 const &n, float s, float t);
};

} // namespace shapes
} // namespace geometry
} // namespace renderstack

#endif
