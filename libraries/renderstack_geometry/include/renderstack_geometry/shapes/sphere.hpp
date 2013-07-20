#ifndef sphere_hpp_renderstack_geometry
#define sphere_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/geometry.hpp"

namespace renderstack { namespace geometry { namespace shapes {

class sphere : public geometry
{
public:
   sphere(double radius, unsigned int slice_count, unsigned int stack_division);

protected:
   struct make_info;

protected:
   point *sphere_point(make_info &info, double rel_slice, double rel_stack);

private:
   void make_corner(make_info &info, polygon *polygon, int slice, int stack_base0);
};

} } }


#endif



