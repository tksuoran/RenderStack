#ifndef geometry_operation_hpp_renderstack_geometry_operation
#define geometry_operation_hpp_renderstack_geometry_operation

#include "renderstack_geometry/edge.hpp"
#include "renderstack_geometry/geometry.hpp"
#include "renderstack_geometry/point.hpp"
#include "renderstack_geometry/polygon.hpp"
#include "renderstack_geometry/property_map.hpp"
#include "renderstack_geometry/property_map_collection.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <set>
#include <vector>

namespace renderstack
{
namespace geometry
{
namespace operation
{

class Geometry_operation
{
public:
    Geometry_operation(Geometry &source) : m_source(source) {}

protected:
    Geometry &m_source;
    Geometry m_destination;
    std::map<Point   *, Point   *>                                m_point_old_to_new;
    std::map<Polygon *, Polygon *>                                m_polygon_old_to_new;
    std::map<Corner  *, Corner  *>                                m_corner_old_to_new;
    std::map<Edge    *, Edge    *>                                m_edge_old_to_new;
    std::map<Polygon *, Point   *>                                m_old_polygon_centroid_to_new_points;
    std::map<Point   *, std::vector<std::pair<float, Point   *>>> m_new_point_sources;
    std::map<Point   *, std::vector<std::pair<float, Corner  *>>> m_new_point_corner_sources;
    std::map<Corner  *, std::vector<std::pair<float, Corner  *>>> m_new_corner_sources;
    std::map<Polygon *, std::vector<std::pair<float, Polygon *>>> m_new_polygon_sources;
    std::map<Edge    *, std::vector<std::pair<float, Edge    *>>> m_new_edge_sources;

    // Creates a new point to Destination from old point.
    // The new point is linked to the old point in Source.
    // Old point is set as source for the new point with specified weight.
    //
    // weight      Weight for old point as source
    // old_point   Old point used as source for the new point
    // return      The new point.
    Point *make_new_point_from_point(float weight, Point *old_point);

    // Creates a new point to Destination from old point.
    // The new point is linked to the old point in Source.
    // Old point is set as source for the new point with weight 1.0.
    //
    // old_point   Old point used as source for the new point
    // returns     The new point.
    Point *make_new_point_from_point(Point *old_point);

    // Creates a new point to Destination from centroid of old polygon.
    // The new point is linked to the old polygon in Source.
    // Each corner of the old polygon is added as source for the new point with weight 1.0.
    Point *make_new_point_from_polygon_centroid(Polygon *old_polygon);

    void add_polygon_centroid(Point *new_point, float weight, Polygon *old_polygon);

    void add_point_ring(Point *new_point, float weight, Point *old_point);

    Polygon *make_new_polygon_from_polygon(Polygon *old_polygon);

    Corner *make_new_corner_from_polygon_centroid(Polygon *new_polygon, Polygon *old_polygon);

    Corner *make_new_corner_from_corner(Polygon *new_polygon, Corner *old_corner);

    void add_polygon_corners(Polygon *new_polygon, Polygon *old_polygon);

    void add_point_source(Point *new_point, float weight, Point *old_point);

    void add_point_source(Point *new_point, float weight, Corner *old_corner);

    void add_corner_source(Corner *new_corner, float weight, Corner *old_corner);

    // Inherit point sources to corner
    void distribute_corner_sources(Corner *new_corner, float weight, Point *new_point);

    void add_polygon_source(Polygon *new_polygon, float weight, Polygon *old_polygon);

    void add_edge_source(Edge *new_edge, float weight, Edge *old_edge);

    void build_destination_edges_with_sourcing();

    void interpolate_all_property_maps();

    //void set_source(Geometry &value)
    //{
    //    m_source = value;
    //}

    Geometry &destination()
    {
        return m_destination;
    }

    const Geometry &destination() const
    {
        return m_destination;
    }
};

} // namespace operation
} // namespace geometry
} // namespace renderstack

#endif
