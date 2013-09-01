#ifndef torus_hpp_renderstack_geometry
#define torus_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/geometry.hpp"

namespace renderstack { namespace geometry { namespace shapes {

class torus : public geometry
{
public:
   torus(double major_radius, double minor_radius, unsigned int slice_count, unsigned int stack_division);

protected:
   struct make_info;

protected:
   point *torus_point(make_info &info, double rel_slice, double rel_stack);

private:
   void make_corner(make_info &info, polygon *polygon, int slice, int stack_base0);
};

} } }


#endif



