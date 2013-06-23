#ifndef cone_hpp_renderstack_geometry
#define cone_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/geometry.hpp"

namespace renderstack { namespace geometry { namespace shapes {

class conical_frustum : public geometry
{
public:
   conical_frustum(
      double   min_x, 
      double   max_x, 
      double   bottom_radius, 
      double   top_radius, 
      bool     use_bottom, 
      bool     use_top, 
      int      slice_count,
      int      stack_division
   );

protected:
   struct make_info;

protected:
   point    *get_point(make_info &info, int slice, int stack);
   point    *cone_point(make_info &info, double rel_slice, double rel_stack_in);
   corner   *make_corner(make_info &info, polygon *polygon, int slice, int stack);
   corner   *make_corner(make_info &info, polygon *polygon, int slice, int stack, bool cap);
};

// Cone is special of conical frustum which is not cut from top
class cone : public conical_frustum
{
   cone(
      double   min_x,
      double   max_x,
      double   bottom_radius,
      bool     use_bottom,
      int      slice_count,
      int      stack_division
   )
   :  conical_frustum(
      min_x,
      max_x,
      bottom_radius,
      0.0,
      use_bottom,
      false,
      slice_count,
      stack_division
   )
   {
   }
};

// Cylinder is special of conical frustum which has top radius equal to bottom radius
class cylinder : public conical_frustum
{
   cylinder(
      double   min_x,
      double   max_x,
      double   radius,
      bool     use_bottom,
      bool     use_top,
      int      slice_count,
      int      stack_division
   )
   :  conical_frustum(
      min_x,
      max_x,
      radius,
      radius,
      use_bottom,
      use_top,
      slice_count,
      stack_division
   )
   {
   }
};

} } }

#endif
