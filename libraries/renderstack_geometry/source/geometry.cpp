#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/corner.hpp"
#include "renderstack_geometry/geometry.hpp"
#include "renderstack_geometry/point.hpp"
#include "renderstack_geometry/polygon.hpp"
#include <glm/glm.hpp>
#include <stdexcept>
#include <cmath>

namespace renderstack { namespace geometry {

using namespace std;
using namespace glm;

geometry::geometry()
{
}

geometry::~geometry()
{
   for (auto i = polygons().begin(); i != polygons().end(); ++i)
      delete *i;

   for (auto i = points().begin(); i != points().end(); ++i )
      delete *i;
}

size_t geometry::count_polygons() const
{
   return polygons().size();
}

size_t geometry::count_edges() const
{
   return edges().size();
}

size_t geometry::count_polygon_triangles() const
{
   size_t triangle_count = 0;
   for (auto polygon = polygons().cbegin(); polygon != polygons().cend(); ++polygon)
      triangle_count += ((*polygon)->corners().size() - 2);

   return triangle_count;
}

size_t geometry::count_corners() const
{
   size_t corner_count = 0;
   for (auto polygon = polygons().cbegin(); polygon != polygons().cend(); ++polygon)
      corner_count += (*polygon)->corners().size();

   return corner_count;
}

void geometry::info(mesh_info &info) const
{
   info.polygon_count    = count_polygons();
   info.corner_count     = count_corners();
   info.triangle_count   = count_polygon_triangles();
   info.edge_count       = count_edges();

   //  Additional vertices needed for centroids
   //  3 indices per triangle after triangulation, 2 indices per edge, 1 per corner, 1 index per centroid
   info.vertex_count_corners        = info.corner_count;
   info.vertex_count_centroids      = info.polygon_count;
   info.index_count_fill_triangles  = 3 * info.triangle_count;
   info.index_count_edge_lines      = 2 * info.edge_count;
   info.index_count_corner_points   = info.corner_count;
   info.index_count_centroid_points = info.polygon_count;
}

geometry::mesh_info &geometry::mesh_info::operator+=(geometry::mesh_info const &o)
{
   polygon_count  += o.polygon_count;
   corner_count   += o.corner_count;
   triangle_count += o.triangle_count;
   edge_count     += o.edge_count;
   vertex_count_corners          += o.vertex_count_corners;
   vertex_count_centroids        += o.vertex_count_centroids;
   index_count_fill_triangles    += o.index_count_fill_triangles;
   index_count_edge_lines        += o.index_count_edge_lines;
   index_count_corner_points     += o.index_count_corner_points;
   index_count_centroid_points   += o.index_count_centroid_points;
   return *this;
}

void geometry::reserve_points(size_t point_count)
{
   if (point_count > m_points.size())
      m_points.reserve(point_count);
}

void geometry::reserve_polygons(size_t polygon_count)
{
   if (polygon_count > m_polygons.size())
      m_polygons.reserve(polygon_count);
}

point *geometry::make_point()
{
   renderstack::geometry::point *pnt = new renderstack::geometry::point();
   m_points.push_back(pnt);
   return pnt;
}

polygon *geometry::make_polygon()
{
   renderstack::geometry::polygon *pol = new renderstack::geometry::polygon();
   m_polygons.push_back(pol);
   return pol;
}

geometry::point_collection          &geometry::points  ()       { return m_points;   }
geometry::point_collection   const  &geometry::points  () const { return m_points;   }
geometry::polygon_collection        &geometry::polygons()       { return m_polygons; }
geometry::polygon_collection const  &geometry::polygons() const { return m_polygons; }
geometry::edge_collection           &geometry::edges   ()       { return m_edges;    }
geometry::edge_collection    const  &geometry::edges   () const { return m_edges;    }

void geometry::compute_polygon_normals()
{
   shared_ptr<property_map<polygon*, vec3> > polygon_normals = polygon_attributes().find_or_create<vec3>("polygon_normals");
   shared_ptr<property_map<point*,   vec3> > point_locations = point_attributes().find<vec3>("point_locations");
   for (auto i = polygons().begin(); i != polygons().end(); ++i)
   {
      polygon *pol = *i;
      pol->compute_normal(
         polygon_normals,
         point_locations
      );
   }
}
void geometry::compute_polygon_centroids()
{
   shared_ptr<property_map<polygon*, vec3> > polygon_centroids = polygon_attributes().find_or_create<vec3>("polygon_centroids");
   shared_ptr<property_map<point*,   vec3> > point_locations = point_attributes().find<vec3>("point_locations");

   for (auto i = polygons().begin(); i != polygons().end(); ++i)
   {
      polygon *pol = *i;
      pol->compute_centroid(
         polygon_centroids,
         point_locations
      );
   }
}

void geometry::smooth_normalize(
   string const   &corner_attribute,
   string const   &polygon_attribute,
   float          max_smoothing_angle_radians
)
{
   auto corner_attributes2 = corner_attributes().find_or_create<vec3>(corner_attribute/*"corner_normals"*/);
   auto polygon_attributes2 = polygon_attributes().find<vec3>(polygon_attribute/*"polygon_normals"*/);
   auto polygon_normals = polygon_attributes().find<vec3>("polygon_normals");

   float cos_max_smoothing_angle = cos(max_smoothing_angle_radians);

   corner_attributes2->clear();
   for (auto i = polygons().cbegin(); i != polygons().end(); ++i)
   {
      polygon *polygon = *i;

      if (max_smoothing_angle_radians == 0.0f)
         polygon->copy_to_corners(
            corner_attributes2, 
            polygon_attributes2
         );
      else
         polygon->smooth_normalize(
            corner_attributes2,
            polygon_attributes2,
            polygon_normals,
            cos_max_smoothing_angle
         );
   }
}

void geometry::smooth_average(
   string const &corner_attribute,
   string const &point_normal_name
)
{
   auto corner_attributes2 = corner_attributes().find_or_create<vec4>(corner_attribute);
   auto corner_normals = corner_attributes().find_or_create<vec3>("corner_normals");
   auto point_normals = point_attributes().find<vec3>(point_normal_name);

   auto new_corner_attributes = corner_attributes().find_or_create<vec4>("temp");
   for (auto i = polygons().begin(); i != polygons().end(); ++i)
   {
      polygon *polygon = *i;
      polygon->smooth_average(
         new_corner_attributes,
         corner_attributes2,
         corner_normals,
         point_normals
      );
   }
   corner_attributes().replace(corner_attribute, "temp");
}
void geometry::build_edges()
{
   m_edges.clear();

   int polygon_index = 0;
   for (auto i = polygons().begin(); i != polygons().end(); ++i)
   {
      polygon *polygon = *i;
      point *first_point  = polygon->corners().front()->point();
      point *previous_point = nullptr;

      int corner_index = 0;
      for (auto j = polygon->corners().begin(); j != polygon->corners().end(); ++j)
      {
         corner *corner = *j;

         if (previous_point)
         {
            point *a = previous_point;
            point *b = corner->point();
            if (a == b)
               throw std::runtime_error("duplicate point");

            edge edge(a, b);
            //CheckEdge(edge);
            m_edges[edge].insert(polygon);
         }

         previous_point = corner->point();
         ++corner_index;
      }

      edge last_edge(previous_point, first_point);
      //CheckEdge(lastEdge);
      m_edges[last_edge].insert(polygon);
      ++polygon_index;
   }

   /*foreach(HashSet<Polygon> polys in Edges.Values)
   {
   int count = polys.Count;
   if(count != 2)
   {
   Logger.Log("Warning: edge with != 2 polygons, this can cause issues");
   }
   }*/
}
vec3 geometry::compute_point_normal(point* point)
{
   auto polygon_normals = polygon_attributes ().find_or_create<vec3>("polygon_normals");

   vec3 normal_sum(0.0f, 0.0f, 0.0f);

   for (auto i = point->corners().cbegin(); i != point->corners().cend(); ++i)
   {
      corner *cor = *i;
      normal_sum += polygon_normals->get(cor->polygon());
   }
   return normalize(normal_sum);
}
void geometry::compute_point_normals(string const &map_name)
{
   auto point_normals    = point_attributes().find_or_create<vec3>(map_name);
   auto polygon_normals  = polygon_attributes().find<vec3>("polygon_normals");

   point_normals->clear();
   for (auto i = points().cbegin(); i != points().cend(); ++i)
   {
      point *point = *i;
      vec3 normal_sum(0.0f, 0.0f, 0.0f);
      for (auto j = point->corners().cbegin(); j != point->corners().cend(); ++j)
      {
         corner *corner = *j;
         normal_sum += polygon_normals->get(corner->polygon());
      }
      point_normals->put(point, normalize(normal_sum));
   }
}
point *geometry::make_point(float x, float y, float z)
{
   point *pnt = make_point();
   auto point_positions = point_attributes().find_or_create<vec3>("point_locations");

   point_positions->put(pnt, vec3(x, y, z));

   return pnt;
}
point *geometry::make_point(float x, float y, float z, float s, float t)
{
   point *pnt = make_point();
   auto point_positions = point_attributes().find_or_create<vec3>("point_locations");
   auto point_texcoords = point_attributes().find_or_create<vec2>("point_texcoords");

   point_positions->put(pnt, vec3(x, y, z));
   point_texcoords->put(pnt, vec2(s, t));

   return pnt;
}
point *geometry::make_point(double x, double y, double z)
{
   return make_point(float(x), float(y), float(z));
}
point *geometry::make_point(double x, double y, double z, double s, double t)
{
   return make_point(float(x), float(y), float(z), float(s), float(t));
}
polygon *geometry::make_polygon(point *p0)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   return p;
}
polygon *geometry::make_polygon(point *p0, point *p1)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   p->make_corner(p1);
   return p;
}
polygon *geometry::make_polygon(point *p0, point *p1, point *p2)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   p->make_corner(p1);
   p->make_corner(p2);
   return p;
}
polygon *geometry::make_polygon(point *p0, point *p1, point *p2, point *p3)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   p->make_corner(p1);
   p->make_corner(p2);
   p->make_corner(p3);
   return p;
}
polygon *geometry::make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   p->make_corner(p1);
   p->make_corner(p2);
   p->make_corner(p3);
   p->make_corner(p4);
   return p;
}
polygon *geometry::make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   p->make_corner(p1);
   p->make_corner(p2);
   p->make_corner(p3);
   p->make_corner(p4);
   p->make_corner(p5);
   return p;
}
polygon *geometry::make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5, point *p6)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   p->make_corner(p1);
   p->make_corner(p2);
   p->make_corner(p3);
   p->make_corner(p4);
   p->make_corner(p5);
   p->make_corner(p6);
   return p;
}
polygon *geometry::make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5, point *p6, point *p7)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   p->make_corner(p1);
   p->make_corner(p2);
   p->make_corner(p3);
   p->make_corner(p4);
   p->make_corner(p5);
   p->make_corner(p6);
   p->make_corner(p7);
   return p;
}
polygon *geometry::make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5, point *p6, point *p7, point *p8)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   p->make_corner(p1);
   p->make_corner(p2);
   p->make_corner(p3);
   p->make_corner(p4);
   p->make_corner(p5);
   p->make_corner(p6);
   p->make_corner(p7);
   p->make_corner(p8);
   return p;
}
polygon *geometry::make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5, point *p6, point *p7, point *p8, point *p9)
{
   polygon *p = make_polygon();
   p->make_corner(p0);
   p->make_corner(p1);
   p->make_corner(p2);
   p->make_corner(p3);
   p->make_corner(p4);
   p->make_corner(p5);
   p->make_corner(p6);
   p->make_corner(p7);
   p->make_corner(p8);
   p->make_corner(p9);
   return p;
}
polygon *geometry::make_polygon(size_t p0)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   return p;
}
polygon *geometry::make_polygon(size_t p0, size_t p1)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   p->make_corner(m_points[p1]);
   return p;
}
polygon *geometry::make_polygon(size_t p0, size_t p1, size_t p2)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   p->make_corner(m_points[p1]);
   p->make_corner(m_points[p2]);
   return p;
}
polygon *geometry::make_polygon(size_t p0, size_t p1, size_t p2, size_t p3)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   p->make_corner(m_points[p1]);
   p->make_corner(m_points[p2]);
   p->make_corner(m_points[p3]);
   return p;
}
polygon *geometry::make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   p->make_corner(m_points[p1]);
   p->make_corner(m_points[p2]);
   p->make_corner(m_points[p3]);
   p->make_corner(m_points[p4]);
   return p;
}
polygon *geometry::make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   p->make_corner(m_points[p1]);
   p->make_corner(m_points[p2]);
   p->make_corner(m_points[p3]);
   p->make_corner(m_points[p4]);
   p->make_corner(m_points[p5]);
   return p;
}
polygon *geometry::make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   p->make_corner(m_points[p1]);
   p->make_corner(m_points[p2]);
   p->make_corner(m_points[p3]);
   p->make_corner(m_points[p4]);
   p->make_corner(m_points[p5]);
   p->make_corner(m_points[p6]);
   return p;
}
polygon *geometry::make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6, size_t p7)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   p->make_corner(m_points[p1]);
   p->make_corner(m_points[p2]);
   p->make_corner(m_points[p3]);
   p->make_corner(m_points[p4]);
   p->make_corner(m_points[p5]);
   p->make_corner(m_points[p6]);
   p->make_corner(m_points[p7]);
   return p;
}
polygon *geometry::make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6, size_t p7, size_t p8)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   p->make_corner(m_points[p1]);
   p->make_corner(m_points[p2]);
   p->make_corner(m_points[p3]);
   p->make_corner(m_points[p4]);
   p->make_corner(m_points[p5]);
   p->make_corner(m_points[p6]);
   p->make_corner(m_points[p7]);
   p->make_corner(m_points[p8]);
   return p;
}
polygon *geometry::make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6, size_t p7, size_t p8, size_t p9)
{
   polygon *p = make_polygon();
   p->make_corner(m_points[p0]);
   p->make_corner(m_points[p1]);
   p->make_corner(m_points[p2]);
   p->make_corner(m_points[p3]);
   p->make_corner(m_points[p4]);
   p->make_corner(m_points[p5]);
   p->make_corner(m_points[p6]);
   p->make_corner(m_points[p7]);
   p->make_corner(m_points[p8]);
   p->make_corner(m_points[p9]);
   return p;
}

void geometry::transform(mat4 m)
{
   mat4 it = glm::transpose(glm::inverse(m));

   //  Check.. Did I forget something?
   //  \todo Mark each attributemap how they should be transformed

   auto polygon_centroids  = polygon_attributes().maybe_find<vec3>("polygon_centroids");
   auto polygon_normals    = polygon_attributes().maybe_find<vec3>("polygon_normals");
   auto point_locations    = point_attributes().maybe_find<vec3>("point_locations");
   auto point_normals      = point_attributes().maybe_find<vec3>("point_normals");
   auto corner_normals     = corner_attributes().maybe_find<vec3>("corner_normals");

   for (auto i = points().cbegin(); i != points().cend(); ++i)
   {
      auto point = *i;

      if (point_locations && point_locations->has(point))
         point_locations->put(point, vec3(m * vec4(point_locations->get(point), 1.0f)));

      if (point_normals && point_normals  ->has(point))
         point_normals->put(point, vec3(it * vec4(point_normals->get(point), 0.0f)));
   }

   for (auto i = polygons().cbegin(); i != polygons().cend(); ++i)
   {
      auto *polygon = *i;

      if (polygon_centroids && polygon_centroids->has(polygon))
         polygon_centroids->put(polygon, vec3(m  * vec4(polygon_centroids->get(polygon), 1.0f)));

      if (polygon_normals && polygon_normals->has(polygon))
         polygon_normals->put(polygon, vec3(it * vec4(polygon_normals->get(polygon), 0.0f)));

      if (corner_normals)
      {
         for (auto j = polygon->corners().cbegin(); j != polygon->corners().cend(); ++j)
         {
            auto corner = *j;

            if (corner_normals && corner_normals->has(corner))
               corner_normals->put(corner, vec3(it * vec4(corner_normals->get(corner), 0.0f)));
         }
      }
   }
}

} }
