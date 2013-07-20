#ifndef geometry_operation_hpp_renderstack_geometry_operation
#define geometry_operation_hpp_renderstack_geometry_operation

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/edge.hpp"
#include "renderstack_geometry/point.hpp"
#include "renderstack_geometry/polygon.hpp"
#include "renderstack_geometry/property_map.hpp"
#include "renderstack_geometry/property_map_collection.hpp"
#include "renderstack_geometry/geometry.hpp"
#include <vector>
#include <set>

namespace renderstack { namespace geometry { namespace operation {

class geometry_operation
{
protected:
   std::shared_ptr<geometry>                                         m_source;
   std::shared_ptr<geometry>                                         m_destination;
   std::map<point*,     point*>                                      m_point_old_to_new;
   std::map<polygon*,   polygon*>                                    m_polygon_old_to_new;
   std::map<corner*,    corner*>                                     m_corner_old_to_new;
   std::map<edge*,      edge*>                                       m_edge_old_to_new;
   std::map<polygon*,   point*>                                      m_old_polygon_centroid_to_new_points;
   std::map<point*,     std::vector<std::pair<float, point*>   > >   m_new_point_sources;
   std::map<point*,     std::vector<std::pair<float, corner*>  > >   m_new_point_corner_sources;
   std::map<corner*,    std::vector<std::pair<float, corner*>  > >   m_new_corner_sources;
   std::map<polygon*,   std::vector<std::pair<float, polygon*> > >   m_new_polygon_sources;
   std::map<edge*,      std::vector<std::pair<float, edge*>    > >   m_new_edge_sources;

public:
   geometry_operation();

   // Creates a new point to Destination from old point.
   // The new point is linked to the old point in Source.
   // Old point is set as source for the new point with specified weight.
   // 
   // weight      Weight for old point as source
   // old_point   Old point used as source for the new point
   // return      The new point.
   point *make_new_point_from_point(float weight, point *old_point);
   
   // Creates a new point to Destination from old point.
   // The new point is linked to the old point in Source.
   // Old point is set as source for the new point with weight 1.0.
   //
   // old_point   Old point used as source for the new point
   // returns     The new point.
   point *make_new_point_from_point(point *old_point);

   // Creates a new point to Destination from centroid of old polygon.
   // The new point is linked to the old polygon in Source.
   // Each corner of the old polygon is added as source for the new point with weight 1.0.
   point *make_new_point_from_polygon_centroid(polygon *old_polygon);

   void add_polygon_centroid(point *new_point, float weight, polygon *old_polygon);
   void add_point_ring(point *new_point, float weight, point *old_point);
   polygon *make_new_polygon_from_polygon(polygon *old_polygon);
   corner *make_new_corner_from_polygon_centroid(polygon *new_polygon, polygon *old_polygon);
   corner *make_new_corner_from_corner(polygon *new_polygon, corner *old_corner);
   void add_polygon_corners(polygon *new_polygon, polygon *old_polygon);
   void add_point_source(point *new_point, float weight, point *old_point);
   void add_point_source(point *new_point, float weight, corner *old_corner);
   void add_corner_source(corner *new_corner, float weight, corner *old_corner);

   // Inherit point sources to corner
   void distribute_corner_sources(corner *new_corner, float weight, point *new_point);
   void add_polygon_source(polygon *new_polygon, float weight, polygon *old_polygon);
   void add_edge_source(edge *new_edge, float weight, edge *old_edge);

   void build_destination_edges_with_sourcing();
   void interpolate_all_property_maps();

   void set_source(std::shared_ptr<geometry> value)
   {
      m_source = value;
   }
   std::shared_ptr<geometry> destination() const
   {
      return m_destination;
   }

};

} } }

#endif 
