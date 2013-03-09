#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/point.hpp"
#include "renderstack_geometry/cube.hpp"
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <algorithm>

namespace renderstack { namespace geometry { namespace shapes {

using namespace std;
using namespace glm;

cube::cube(double x_size, double y_size, double z_size)
:  geometry("cube")
{
   double x = x_size / 2.0;
   double y = y_size / 2.0;
   double z = z_size / 2.0;

   geometry::make_point(-x,  y,  z, 0, 1);  /*  0  */ 
   geometry::make_point( x,  y,  z, 1, 1);  /*  1  */ 
   geometry::make_point( x, -y,  z, 1, 1);  /*  2  */ 
   geometry::make_point(-x, -y,  z, 0, 1);  /*  3  */ 
   geometry::make_point(-x,  y, -z, 0, 0);  /*  4  */ 
   geometry::make_point( x,  y, -z, 1, 0);  /*  5  */ 
   geometry::make_point( x, -y, -z, 1, 0);  /*  6  */ 
   geometry::make_point(-x, -y, -z, 0, 0);  /*  7  */ 

   make_polygon(0, 1, 2, 3);
   make_polygon(0, 3, 7, 4); 
   make_polygon(0, 4, 5, 1);  /*  top  */ 
   make_polygon(5, 4, 7, 6);
   make_polygon(2, 1, 5, 6);
   make_polygon(7, 3, 2, 6);  /*  bottom  */ 
}
struct cube::make_info
{
   vec3              size;
   ivec3             div;
   float             p;
   map<long, point*> points;

   shared_ptr<attribute_map<point*, vec3> >   point_locations;
   shared_ptr<attribute_map<point*, vec3> >   point_normals;
   shared_ptr<attribute_map<point*, vec2> >   point_texcoords;
   shared_ptr<attribute_map<corner*, vec3> >  corner_normals;
   shared_ptr<attribute_map<corner*, vec2> >  corner_texcoords;
   shared_ptr<attribute_map<polygon*, vec3> > polygon_centroids;
   shared_ptr<attribute_map<polygon*, vec3> > polygon_normals;

   make_info(
      vec3  const &size_,
      ivec3 const &div_,
      float       p_
   )
   {
      size = size_;
      div = div_;
      p = p_;
   }
};
float cube::spow(float x, float p) const
{
   return sign(x) * std::pow(std::abs(x), p);
}
point *cube::make_point(make_info &info, int x, int y, int z, glm::vec3 const &n, float s, float t)
{
   long key = 
      y * (info.div.x * 4 * info.div.z * 4) + 
      x * (info.div.z * 4) + 
      z;

   auto i = info.points.find(key);
   if (i != info.points.end())
   {
      return i->second;
   }

   float rel_x = static_cast<float>(x) / static_cast<float>(info.div.x);
   float rel_y = static_cast<float>(y) / static_cast<float>(info.div.y);
   float rel_z = static_cast<float>(z) / static_cast<float>(info.div.z);
   float rel_xp = spow(rel_x, info.p);
   float rel_yp = spow(rel_y, info.p);
   float rel_zp = spow(rel_z, info.p);

   float x_p  = rel_xp * info.size.x;
   float y_p  = rel_yp * info.size.y;
   float z_p  = rel_zp * info.size.z;

   point *point = geometry::make_point();

   bool discontinuity = (x == -info.div.x) || (x == info.div.x) || (y == -info.div.y) || (y == info.div.y) || (z == -info.div.z) || (z == info.div.z);

   info.point_locations->set_value(point, vec3(x_p, y_p, z_p));
   if (!discontinuity)
   {
      info.point_normals->set_value(point, n);
      info.point_texcoords->set_value(point, vec2(s, t));
   }

   info.points[key] = point;

   return point;
}
corner *cube::make_corner(make_info &info, polygon *polygon, int x, int y, int z, glm::vec3 const &n, float s, float t)
{
   long key = 
      y * (info.div.x * 4 * info.div.z * 4) + 
      x * (info.div.z * 4) + 
      z;

   // TODO What is this for?
#if 0
   auto i = std::find(info.points.begin(), info.points.end(), key);
   if (i == info.points.end())
   {
      key = 0;
   }
#endif

   point *point = info.points[key];
   bool discontinuity = (x == -info.div.x) || (x == info.div.x) || (y == -info.div.y) || (y == info.div.y) || (z == -info.div.z) || (z == info.div.z);
   corner *corner = polygon->make_corner(point);

   if (discontinuity)
   {
      info.point_normals->set_value(point, n);
      info.point_texcoords->set_value(point, vec2(s, t));
   }

   return corner;
}
cube::cube(glm::vec3 const &size, glm::ivec3 const &div, float p)
:  geometry("cube")
{
   int x;
   int y;
   int z;

   make_info info(0.5f * size, div, p);

   info.point_locations    = point_attributes().find_or_create<vec3>("point_locations");
   info.point_normals      = point_attributes().find_or_create<vec3>("point_normals");
   info.point_texcoords    = point_attributes().find_or_create<vec2>("point_texcoords");
   info.corner_normals     = corner_attributes().find_or_create<vec3>("corner_normals");
   info.corner_texcoords   = corner_attributes().find_or_create<vec2>("corner_texcoords");
   info.polygon_centroids  = polygon_attributes().find_or_create<vec3>("polygon_centroids");
   info.polygon_normals    = polygon_attributes().find_or_create<vec3>("polygon_normals");

   //  Generate vertices
   //  Top and bottom
   vec3 unit_x(1.0f, 0.0f, 0.0f);
   vec3 unit_y(0.0f, 1.0f, 0.0f);
   vec3 unit_z(0.0f, 0.0f, 1.0f);
   for (x = -info.div.x; x <= info.div.x; x++)
   {
      float rel_x = 0.5f + static_cast<float>(x) / info.size.x;

      for (z = -info.div.z; z <= info.div.z; z++ )
      {
         float rel_z = 0.5f + static_cast<float>(z) / info.size.z;

         make_point(info, x,  info.div.y, z,  unit_y, rel_x, rel_z);
         make_point(info, x, -info.div.y, z, -unit_y, rel_x, rel_z);
      }
      for (y = -info.div.y; y <= info.div.y; y++)
      {
         float rel_y = 0.5f + static_cast<float>(y) / info.size.y;

         make_point(info, x, y,  info.div.z,  unit_z, rel_x, rel_y);
         make_point(info, x, y, -info.div.z, -unit_z, rel_x, rel_y);
      }
   }

   //  Left and right
   for (z = -info.div.z; z <= info.div.z; z++)
   {
      float rel_z = 0.5f + static_cast<float>(z) / info.size.z;

      for (y = -info.div.y; y <= info.div.y; y++)
      {
         float rel_y = 0.5f + static_cast<float>(y) / info.size.y;

         make_point(info,  info.div.x, y, z,  unit_x, rel_y, rel_z);
         make_point(info, -info.div.x, y, z, -unit_x, rel_y, rel_z);
      }
   }

   //  Generate quads
   //  Top and bottom
   for (x = -info.div.x; x < info.div.x; x++)
   {
      float rel_x1 = 0.5f + static_cast<float>(x) / info.size.x;
      float rel_x2 = 0.5f + static_cast<float>(x + 1) / info.size.x;
      for (z = -info.div.z; z < info.div.z; z++)
      {
         float rel_z1 = 0.5f + static_cast<float>(z) / info.size.z;
         float rel_z2 = 0.5f + static_cast<float>(z + 1) / info.size.z;

         auto top = geometry::make_polygon();
         make_corner(info, top, x + 1, info.div.y, z,     unit_y, rel_x2, rel_z1);
         make_corner(info, top, x + 1, info.div.y, z + 1, unit_y, rel_x2, rel_z2);
         make_corner(info, top, x,     info.div.y, z + 1, unit_y, rel_x1, rel_z2);
         make_corner(info, top, x,     info.div.y, z,     unit_y, rel_x1, rel_z1);

         auto bottom = geometry::make_polygon();
         make_corner(info, bottom, x,     -info.div.y, z,     -unit_y, rel_x1, rel_z1);
         make_corner(info, bottom, x,     -info.div.y, z + 1, -unit_y, rel_x1, rel_z2);
         make_corner(info, bottom, x + 1, -info.div.y, z + 1, -unit_y, rel_x2, rel_z2);
         make_corner(info, bottom, x + 1, -info.div.y, z,     -unit_y, rel_x2, rel_z1);

         info.polygon_normals->set_value(top,     unit_y);
         info.polygon_normals->set_value(bottom, -unit_y);
      }
      for (y = -info.div.y; y < info.div.y; y++)
      {
         float rel_y1 = 0.5f + static_cast<float>(y) / info.size.y;
         float rel_y2 = 0.5f + static_cast<float>(y + 1) / info.size.y;

         auto back = geometry::make_polygon();
         make_corner(info, back, x,     y,     info.div.z, unit_z, rel_x1, rel_y1);
         make_corner(info, back, x,     y + 1, info.div.z, unit_z, rel_x1, rel_y2);
         make_corner(info, back, x + 1, y + 1, info.div.z, unit_z, rel_x2, rel_y2);
         make_corner(info, back, x + 1, y,     info.div.z, unit_z, rel_x2, rel_y1);

         auto front = geometry::make_polygon();
         make_corner(info, front, x + 1, y,     -info.div.z, -unit_z, rel_x2, rel_y1);
         make_corner(info, front, x + 1, y + 1, -info.div.z, -unit_z, rel_x2, rel_y2);
         make_corner(info, front, x,     y + 1, -info.div.z, -unit_z, rel_x1, rel_y2);
         make_corner(info, front, x,     y,     -info.div.z, -unit_z, rel_x1, rel_y1);

         info.polygon_normals->set_value(back,   unit_z);
         info.polygon_normals->set_value(front, -unit_z);
      }
   }

   //  Left and right
   for (z = -info.div.z; z < info.div.z; z++)
   {
      float rel_z1 = 0.5f + static_cast<float>(z) / info.size.z;
      float rel_z2 = 0.5f + static_cast<float>(z + 1) / info.size.z;

      for(y = -info.div.y; y < info.div.y; y++)
      {
         float rel_y1 = 0.5f + static_cast<float>(y) / info.size.y;
         float rel_y2 = 0.5f + static_cast<float>(y + 1) / info.size.y;

         auto right = geometry::make_polygon();
         make_corner(info, right, info.div.x, y,     z,     unit_x, rel_y1, rel_z1);
         make_corner(info, right, info.div.x, y,     z + 1, unit_x, rel_y1, rel_z2);
         make_corner(info, right, info.div.x, y + 1, z + 1, unit_x, rel_y2, rel_z2);
         make_corner(info, right, info.div.x, y + 1, z,     unit_x, rel_y2, rel_z1);

         auto left = geometry::make_polygon();
         make_corner(info, left, -info.div.x, y + 1, z,     -unit_x, rel_y2, rel_z1);
         make_corner(info, left, -info.div.x, y + 1, z + 1, -unit_x, rel_y2, rel_z2);
         make_corner(info, left, -info.div.x, y,     z + 1, -unit_x, rel_y1, rel_z2);
         make_corner(info, left, -info.div.x, y,     z,     -unit_x, rel_y1, rel_z1);

         info.polygon_normals->set_value(right,  unit_x);
         info.polygon_normals->set_value(left,  -unit_x);
      }
   }
   geometry::compute_polygon_centroids();
}
cube::cube(double r)
:  geometry("cube")
{
   double sq3 = sqrt(3.0);

   geometry::make_point(-r / sq3,  r / sq3,  r / sq3);
   geometry::make_point( r / sq3,  r / sq3,  r / sq3);
   geometry::make_point( r / sq3, -r / sq3,  r / sq3);
   geometry::make_point(-r / sq3, -r / sq3,  r / sq3);
   geometry::make_point(-r / sq3,  r / sq3, -r / sq3);
   geometry::make_point( r / sq3,  r / sq3, -r / sq3);
   geometry::make_point( r / sq3, -r / sq3, -r / sq3);
   geometry::make_point(-r / sq3, -r / sq3, -r / sq3);

   make_polygon(  0, 1, 2, 3  );
   make_polygon(  0, 3, 7, 4  );
   make_polygon(  0, 4, 5, 1  );
   make_polygon(  5, 4, 7, 6  );
   make_polygon(  2, 1, 5, 6  );
   make_polygon(  7, 3, 2, 6  );
}


} } }
