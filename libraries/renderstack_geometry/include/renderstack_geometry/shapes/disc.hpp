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

class disc : public geometry
{
public:
    disc(
        double outer_radius,
        double inner_radius,
        int    slice_count,
        int    stack_count);

protected:
    struct make_info;

    point *get_point(make_info &info, int slice, int stack);

    //  relStackIn is in range -1..1
    //  relStack is in range 0..1
    point * make_point(make_info &info, double rel_slice, double rel_stack);
    corner *make_corner(make_info &info, polygon *polygon, int slice, int stack);
};

} // namespace shapes
} // namespace geometry
} // namespace renderstack

#endif
