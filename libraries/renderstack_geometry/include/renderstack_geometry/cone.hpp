#ifndef Renderstack_Geometry_cone_hpp
#define Renderstack_Geometry_cone_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/geometry.hpp"

namespace renderstack { namespace geometry { namespace shapes {

class cone : public geometry
{
protected:
   struct make_info;

protected:
   point *get_point(make_info &info, int slice, int stack);
   point *cone_point(make_info &info, double rel_slice, double rel_stack_in);
   corner *make_corner(make_info &info, polygon *polygon, int slice, int stack);
   corner *make_corner(make_info &info, polygon *polygon, int slice, int stack, bool cap);
public:
   cone(
      double  min_x, 
      double  max_x, 
      double  bottom_radius, 
      double  top_radius, 
      bool    use_bottom, 
      bool    use_top, 
      int     slice_count,
      int     stack_division
   );
};

} } }

#endif
