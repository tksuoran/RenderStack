#ifndef geometry_hpp_renderstack_geometry
#define geometry_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/edge.hpp"
#include "renderstack_geometry/point.hpp"
#include "renderstack_geometry/polygon.hpp"
#include "renderstack_geometry/property_map.hpp"
#include "renderstack_geometry/property_map_collection.hpp"
#include <vector>
#include <unordered_set>

namespace renderstack { namespace geometry {

/*  "point_locations"  */ 
/*  "polygon_normals"  */ 
/*  "corner normals"   */ 
/**  \brief collection of points, polygons, corners and their attributes */ 
class geometry 
{
public:
   typedef property_map_collection<point*>     point_property_map_collection;
   typedef property_map_collection<corner*>    corner_property_map_collection;
   typedef property_map_collection<polygon*>   polygon_property_map_collection;
   typedef property_map_collection<edge*>      edge_property_map_collection;

   typedef std::vector<renderstack::geometry::point*>    point_collection;
   typedef std::vector<renderstack::geometry::polygon*>  polygon_collection;
   typedef std::map<edge, std::unordered_set<polygon*> > edge_collection;

   struct mesh_info
   {
      std::size_t polygon_count;
      std::size_t corner_count;
      std::size_t triangle_count;
      std::size_t edge_count;
      std::size_t vertex_count_corners;
      std::size_t vertex_count_centroids;
      std::size_t index_count_fill_triangles;
      std::size_t index_count_edge_lines;
      std::size_t index_count_corner_points;
      std::size_t index_count_centroid_points;

      mesh_info()
      :  polygon_count              (0)
      ,  corner_count               (0)
      ,  triangle_count             (0)
      ,  edge_count                 (0)
      ,  vertex_count_corners       (0)
      ,  vertex_count_centroids     (0)
      ,  index_count_fill_triangles (0)
      ,  index_count_edge_lines     (0)
      ,  index_count_corner_points  (0)
      ,  index_count_centroid_points(0)
      {
      }
      mesh_info &operator+=(mesh_info const &o);
   };

public:
   geometry();
   geometry(std::string const &name) : m_name(name){}
   ~geometry();

public:
   std::string const &name() const { return m_name; }

   std::size_t count_polygons() const;
   std::size_t count_corners() const;
   std::size_t count_polygon_triangles() const;
   std::size_t count_edges() const;

   void info(mesh_info &info) const;

   point_property_map_collection      &point_attributes   (){ return m_point_property_map_collection; }
   corner_property_map_collection     &corner_attributes  (){ return m_corner_property_map_collection; }
   polygon_property_map_collection    &polygon_attributes (){ return m_polygon_property_map_collection; }
   edge_property_map_collection       &edge_attributes    (){ return m_edge_property_map_collection; }

   point_property_map_collection      const &point_attributes     () const { return m_point_property_map_collection; }
   corner_property_map_collection     const &corner_attributes    () const { return m_corner_property_map_collection; }
   polygon_property_map_collection    const &polygon_attributes   () const { return m_polygon_property_map_collection; }
   edge_property_map_collection       const &edge_attributes      () const { return m_edge_property_map_collection; }

   void    reserve_points  (std::size_t point_count);
   void    reserve_polygons(std::size_t polygon_count);
   point   *make_point     ();
   polygon *make_polygon   ();

   point   *make_point  (float x, float y, float z);
   point   *make_point  (float x, float y, float z, float s, float t);
   point   *make_point  (double x, double y, double z);
   point   *make_point  (double x, double y, double z, double s, double t);
   polygon *make_polygon(point *p0);
   polygon *make_polygon(point *p0, point *p1);
   polygon *make_polygon(point *p0, point *p1, point *p2);
   polygon *make_polygon(point *p0, point *p1, point *p2, point *p3);
   polygon *make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4);
   polygon *make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5);
   polygon *make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5, point *p6);
   polygon *make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5, point *p6, point *p7);
   polygon *make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5, point *p6, point *p7, point *p8);
   polygon *make_polygon(point *p0, point *p1, point *p2, point *p3, point *p4, point *p5, point *p6, point *p7, point *p8, point *p9);
   polygon *make_polygon(size_t p0);
   polygon *make_polygon(size_t p0, size_t p1);
   polygon *make_polygon(size_t p0, size_t p1, size_t p2);
   polygon *make_polygon(size_t p0, size_t p1, size_t p2, size_t p3);
   polygon *make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4);
   polygon *make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5);
   polygon *make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6);
   polygon *make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6, size_t p7);
   polygon *make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6, size_t p7, size_t p8);
   polygon *make_polygon(size_t p0, size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6, size_t p7, size_t p8, size_t p9);

   point_collection            &points     ();
   point_collection   const    &points     () const;
   polygon_collection          &polygons   ();
   polygon_collection const    &polygons   () const;
   edge_collection             &edges      ();
   edge_collection    const    &edges      () const;

   void compute_polygon_normals    ();
   void compute_polygon_centroids  ();

   glm::vec3 compute_point_normal(point* point);
   void compute_point_normals(std::string const &map_name);
   void smooth_normalize(
      std::string const &corner_attribute,
      std::string const &polygon_attribute,
      float             max_smoothing_angle_radians
   );
   void smooth_average(
      std::string const &corner_attribute,
      std::string const &point_normal_name
   );

   void build_edges();

   void transform(glm::mat4 m);

private:
   std::string                      m_name;
   point_property_map_collection   m_point_property_map_collection;
   corner_property_map_collection  m_corner_property_map_collection;
   polygon_property_map_collection m_polygon_property_map_collection;
   edge_property_map_collection    m_edge_property_map_collection;

   point_collection                 m_points;
   polygon_collection               m_polygons;
   edge_collection                  m_edges;
};

} }

#endif 
