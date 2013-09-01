#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/constants.hpp>
#include "renderstack_geometry/shapes/torus.hpp"
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

struct torus::make_info
{
   double          major_radius;
   double          minor_radius;
   int             major_axis_steps;
   int             minor_axis_steps;

   vector<point*>  points;

   shared_ptr<property_map<point*, vec3> >   point_locations;
   shared_ptr<property_map<point*, vec3> >   point_normals;
   shared_ptr<property_map<point*, vec3> >   point_tangents;
   shared_ptr<property_map<point*, vec2> >   point_texcoords;
   shared_ptr<property_map<corner*, vec2> >  corner_texcoords;
   shared_ptr<property_map<polygon*, vec3> > polygon_centroids;
   shared_ptr<property_map<polygon*, vec3> > polygon_normals;

   make_info(double major_radius_, double minor_radius_, int major_axis_steps_, int minor_axis_steps_)
   {
      major_radius      = major_radius_;
      minor_radius      = minor_radius_;
      major_axis_steps  = major_axis_steps_;
      minor_axis_steps  = minor_axis_steps_;
   }
};

torus::torus(
   double         major_radius,
   double         minor_radius,
   unsigned int   major_axis_steps,
   unsigned int   minor_axis_steps
)
:  geometry("torus")
{
   make_info info(major_radius, minor_radius, static_cast<int>(major_axis_steps), static_cast<int>(minor_axis_steps));

   info.point_locations    = point_attributes().find_or_create<vec3>("point_locations");
   info.point_normals      = point_attributes().find_or_create<vec3>("point_normals");
   info.point_tangents     = point_attributes().find_or_create<vec3>("point_tangents");
   info.point_texcoords    = point_attributes().find_or_create<vec2>("point_texcoords");
   info.polygon_centroids  = polygon_attributes().find_or_create<vec3>("polygon_centroids");
   info.polygon_normals    = polygon_attributes().find_or_create<vec3>("polygon_normals");
   info.corner_texcoords   = corner_attributes().find_or_create<vec2>("corner_texcoords");

   int major;
   int minor;
   for (major = 0; major < info.major_axis_steps; ++major)
   {
      double rel_major = (double)(major) / (double)(info.major_axis_steps);
      for (minor = 0; minor < info.minor_axis_steps; ++minor)
      {
         double rel_minor = (double)(minor) / (double)(info.minor_axis_steps);
         point *point = torus_point(info, rel_major, rel_minor);

         info.points.push_back(point);
      }
   }

   for (major = 0; major < info.major_axis_steps; ++major)
   {
      int    next_major = (major + 1);
      double rel_major = (double)(major) / (double)(info.major_axis_steps);
      double rel_next_major = (double)(next_major) / (double)(info.major_axis_steps);
      double avg_major = (rel_major + rel_next_major) / 2.0;

      for (minor = 0; minor < info.minor_axis_steps; ++minor)
      {
         int    next_minor = (minor + 1);
         double rel_minor = (double)(minor) / (double)(info.minor_axis_steps);
         double rel_next_minor = (double)(next_minor) / (double)(info.minor_axis_steps);
         double avg_minor = (rel_minor + rel_next_minor) / 2.0;

         point *centroid = torus_point(info, avg_major, avg_minor);

         auto polygon = make_polygon();
         make_corner(info, polygon, next_major, next_minor);
         make_corner(info, polygon, major,      next_minor);
         make_corner(info, polygon, major,      minor);
         make_corner(info, polygon, next_major, minor);

         info.polygon_centroids->put(polygon, info.point_locations->get(centroid));
         info.polygon_normals->put(polygon, info.point_normals->get(centroid));
      }
   }
}

point *torus::torus_point(make_info &info, double rel_major, double rel_minor)
{
   double R = info.major_radius;
   double r = info.minor_radius;
   double theta      = (glm::pi<double>() * 2.0 * rel_major);
   double phi        = (glm::pi<double>() * 2.0 * rel_minor);
   double sin_theta  = sin(theta);
   double cos_theta  = cos(theta);
   double sin_phi    = sin(phi);
   double cos_phi    = cos(phi);

   double vx = (R + r * cos_phi) * cos_theta;
   double vz = (R + r * cos_phi) * sin_theta;
   double vy = r * sin_phi;
   vec3 V(vx, vy, vz);

   double tx = -sin_theta;
   double tz =  cos_theta;
   double ty = 0.0f;
   vec3 T(tx, ty, tz);

   double bx = -sin_phi * cos_theta;
   double bz = -sin_phi * sin_theta;
   double by =  cos_phi;
   vec3 B(bx, by, bz);

   vec3 N = normalize(cross(B, T));

   point *point = make_point();

   bool uv_discontinuity = (rel_major == 1.0) || (rel_minor == 1.0);

   info.point_locations->put(point, V);
   info.point_normals->put(point, N);
   info.point_tangents->put(point, T);
   if (uv_discontinuity == false)
      info.point_texcoords->put(point, vec2(rel_major, rel_minor));

   return point;
}

void torus::make_corner(make_info &info, polygon *polygon, int major, int minor)
{
   double   rel_major          = (double)(major) / (double)(info.major_axis_steps);
   double   rel_minor          = (double)(minor) / (double)(info.minor_axis_steps);
   bool     major_seam         = (major == info.major_axis_steps);
   bool     minor_seam         = (minor == info.minor_axis_steps);
   bool     uv_discontinuity   = (major_seam || minor_seam);

   if (major_seam)
      major = 0;

   if (minor_seam)
      minor = 0;

   point *point;
   point = info.points[
      (
         static_cast<unsigned int>(major) * static_cast<unsigned int>(info.minor_axis_steps)
      ) +
      static_cast<unsigned int>(minor)
   ];

   corner *corner = polygon->make_corner(point);

   if (uv_discontinuity)
   {
      float s = (float)rel_major;
      float t = (float)rel_minor;

      info.corner_texcoords->put(corner, vec2(s, t));
   }
}

} } }
