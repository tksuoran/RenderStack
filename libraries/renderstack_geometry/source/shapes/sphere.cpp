#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/constants.hpp>
#include "renderstack_geometry/shapes/sphere.hpp"
#include <map>
#include <vector>

namespace renderstack { namespace geometry { 

   class point;
   class corner;
   class polygon;

} }

namespace renderstack { namespace geometry { namespace shapes {

using namespace glm;
using namespace std;

struct sphere::make_info
{
   double          radius;
   int             slice_count;
   int             stack_division;
   int             stack_count;
   int             stack_base0_bottom;
   int             stack_base0_top;

   vector<point*>  points;
   point*          top;
   point*          bottom;

   shared_ptr<property_map<point*, vec3> >   point_locations;
   shared_ptr<property_map<point*, vec3> >   point_normals;
   shared_ptr<property_map<point*, vec3> >   point_tangents;
   shared_ptr<property_map<point*, vec2> >   point_texcoords;
   shared_ptr<property_map<corner*, vec2> >  corner_texcoords;
   shared_ptr<property_map<polygon*, vec3> > polygon_centroids;
   shared_ptr<property_map<polygon*, vec3> > polygon_normals;

   make_info(double radius_, int slice_count_, int stack_division_)
   {
      radius              = radius_;
      slice_count         = slice_count_;
      stack_division      = stack_division_;
      stack_count         = (2 * stack_division_) + 1;
      stack_base0_bottom  = -1;
      stack_base0_top     = static_cast<int>(2 * stack_division_ + 1);
   }
};

sphere::sphere(
   double         radius,
   unsigned int   slice_count,
   unsigned int   stack_division
)
:  geometry("sphere")
{
   make_info info(radius, static_cast<int>(slice_count), static_cast<int>(stack_division));

   info.point_locations    = point_attributes().find_or_create<vec3>("point_locations");
   info.point_normals      = point_attributes().find_or_create<vec3>("point_normals");
   info.point_tangents     = point_attributes().find_or_create<vec3>("point_tangents");
   info.point_texcoords    = point_attributes().find_or_create<vec2>("point_texcoords");
   info.polygon_centroids  = polygon_attributes().find_or_create<vec3>("polygon_centroids");
   info.polygon_normals    = polygon_attributes().find_or_create<vec3>("polygon_normals");
   info.corner_texcoords   = corner_attributes().find_or_create<vec2>("corner_texcoords");

   int slice;
   int stack;
   for (slice = 0; slice < info.slice_count; ++slice)
   {
      double rel_slice = (double)(slice) / (double)(slice_count);
      for (stack = -info.stack_division; stack <= info.stack_division; ++stack)
      {
         double rel_stack = (double)(stack) / (double)(stack_division + 1);
         point  *point    = sphere_point(info, rel_slice, rel_stack);

         info.points.push_back(point);
      }
   }
   info.bottom = sphere_point(info, 0.5f, -1.0f);
   info.top    = sphere_point(info, 0.5f,  1.0f);

#  if 1 /* bottom fan */
   {
      for (slice = 0; slice < info.slice_count; ++slice)
      {
         int next_slice  = slice + 1;
         int stack_base0 = 0;

         double rel_slice = ((double)(slice) + 0.5) / (double)(slice_count);
         double rel_stack = -1.0 + (0.5 / (double)(stack_division + 1));

         point *centroid = sphere_point(info, rel_slice, rel_stack);

         auto polygon = make_polygon();
         make_corner(info, polygon, slice, stack_base0);
         make_corner(info, polygon, slice, info.stack_base0_bottom);
         make_corner(info, polygon, next_slice, stack_base0);

         info.polygon_centroids->put(polygon, info.point_locations->get(centroid));
         info.polygon_normals->put(polygon, info.point_normals->get(centroid));
      }
   }
#  endif

#  if 1 /* middle quads, bottom up */
   for (stack = -info.stack_division; stack < info.stack_division; ++stack)
   {
      int stack_base0      = stack + info.stack_division;
      int next_stack_base0 = stack_base0 + 1;

      double rel_stack = ((double)(stack) + 0.5) / (double)(stack_division + 1);

      for (slice = 0; slice < info.slice_count; ++slice)
      {
         int    next_slice = (slice + 1);
         double rel_slice  = ((double)(slice) + 0.5) / (double)(slice_count);
         point  *centroid  = sphere_point(info, rel_slice, rel_stack);

         auto polygon = make_polygon();
         make_corner(info, polygon, next_slice,  next_stack_base0);
         make_corner(info, polygon, slice,       next_stack_base0);
         make_corner(info, polygon, slice,       stack_base0);
         make_corner(info, polygon, next_slice,  stack_base0);

         info.polygon_centroids->put(polygon, info.point_locations->get(centroid));
         info.polygon_normals->put(polygon, info.point_normals->get(centroid));
      }
   }
#  endif

#  if 1 /* top fan */
   for (slice = 0; slice < info.slice_count; ++slice)
   {
      int next_slice  = (slice + 1);
      int stack_base0 = info.stack_division + info.stack_division;

      double rel_slice = ((double)(slice) + 0.5) / (double)(slice_count);
      double rel_stack = 1.0 - (0.5 / (double)(stack_division + 1));

      point *centroid = sphere_point(info, rel_slice, rel_stack);

      auto polygon = make_polygon();
      make_corner(info, polygon, slice,       info.stack_base0_top);
      make_corner(info, polygon, slice,       stack_base0);
      make_corner(info, polygon, next_slice,  stack_base0);

      info.polygon_centroids->put(polygon, info.point_locations->get(centroid));
      info.polygon_normals->put(polygon, info.point_normals->get(centroid));
   }
#  endif
}

point *sphere::sphere_point(make_info &info, double rel_slice, double rel_stack)
{
   double  phi          = (glm::pi<double>() * 2.0 * rel_slice);
   double  sin_phi      = sin(phi);
   double  cos_phi      = cos(phi);

   double  theta        = (glm::pi<double>() * 0.5  * rel_stack);
   double  sin_theta    = sin(theta);
   double  cos_theta    = cos(theta);

   double  stack_radius = cos_theta;

   float   xVN          = (float)(stack_radius * cos_phi);
   float   yVN          = (float)(sin_theta);
   float   zVN          = (float)(stack_radius * sin_phi);

   float   xP           = (float)(info.radius * xVN);
   float   yP           = (float)(info.radius * yVN);
   float   zP           = (float)(info.radius * zVN);

   float   s            = 1.0f - (float)(rel_slice);
   float   t            = 1.0f - (float)(0.5 * (1.0 + rel_stack));

   // B = T x N
   // T = N x B
   // N = B x T

   vec3 N(xVN, yVN, zVN);
   vec3 axis(0.0f, 1.0f, 0.0f);
   vec3 B = normalize(cross(axis, N));
   vec3 T = normalize(cross(N, B));

   point *point = make_point();

   bool uv_discontinuity = (rel_stack == -1.0) || (rel_stack == 1.0) || /*(relSlice == 0.0) ||*/ (rel_slice == 1.0);

   info.point_locations->put(point, vec3(xP, yP, zP));
   info.point_normals->put(point, N);
   info.point_tangents->put(point, T);
   if (uv_discontinuity == false)
      info.point_texcoords->put(point, vec2(s, t));

   return point;
}

void sphere::make_corner(make_info &info, polygon *polygon, int slice, int stack_base0)
{
   assert(slice >= 0);

   //int stack_base0 = stack + info.stackDivision;
   int     stack       = stack_base0 - info.stack_division;
   double  rel_slice   = (double)(slice) / (double)(info.slice_count);
   double  rel_stack   = (double)(stack) / (double)(info.stack_division + 1);
   bool    slice_seam  = /*(slice == 0) ||*/ (slice == info.slice_count);
   bool    bottom      = (stack_base0 == info.stack_base0_bottom);
   bool    top         = (stack_base0 == info.stack_base0_top);
   bool    uv_discontinuity = slice_seam || bottom || top;

   point *point;
   if (top)
      point = info.top;
   else if (bottom)
      point = info.bottom;
   else
      if (slice == info.slice_count)
         point = info.points[static_cast<unsigned int>(stack_base0)];
      else
         point = info.points[(static_cast<unsigned int>(slice) * static_cast<unsigned int>(info.stack_count)) + static_cast<unsigned int>(stack_base0)];

   corner *corner = polygon->make_corner(point);

   if (uv_discontinuity)
   {
      float s = 1.0f - (float)(rel_slice);
      float t = 1.0f - (float)(0.5 * (1.0 + rel_stack));

      info.corner_texcoords->put(corner, vec2(s, t));
   }
}

} } }
