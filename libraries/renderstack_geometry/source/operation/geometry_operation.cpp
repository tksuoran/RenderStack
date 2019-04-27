#include "renderstack_geometry/operation/geometry_operation.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace operation
{

using namespace std;

Point *Geometry_operation::make_new_point_from_point(float weight, Point *old_point)
{
    assert(old_point != nullptr);

    Point *new_point = m_destination.make_point();
    add_point_source(new_point, weight, old_point);
    m_point_old_to_new[old_point] = new_point;
    return new_point;
}

Point *Geometry_operation::make_new_point_from_point(Point *old_point)
{
    assert(old_point != nullptr);

    Point *new_point = m_destination.make_point();
    add_point_source(new_point, 1.0f, old_point);
    m_point_old_to_new[old_point] = new_point;
    return new_point;
}

Point *Geometry_operation::make_new_point_from_polygon_centroid(Polygon *old_polygon)
{
    assert(old_polygon != nullptr);

    Point *new_point = m_destination.make_point();
    m_old_polygon_centroid_to_new_points[old_polygon] = new_point;
    add_polygon_centroid(new_point, 1.0f, old_polygon);
    return new_point;
}

void Geometry_operation::add_polygon_centroid(Point *new_point, float weight, Polygon *old_polygon)
{
    for (auto old_corner : old_polygon->corners())
    {
        add_point_source(new_point, weight, old_corner);
        add_point_source(new_point, weight, old_corner->point);
    }
}

void Geometry_operation::add_point_ring(Point *new_point, float weight, Point *old_point)
{
    assert(new_point != nullptr);
    assert(old_point != nullptr);

    for (auto ring_corner : old_point->corners())
    {
        Polygon *ring_polygon = ring_corner->polygon;
        Corner *next_ring_corner = ring_polygon->next_corner(ring_corner);
        Point *next_ring_point = next_ring_corner->point;
        add_point_source(new_point, weight, next_ring_point);
    }
}

Polygon *Geometry_operation::make_new_polygon_from_polygon(Polygon *old_polygon)
{
    assert(old_polygon != nullptr);

    Polygon *new_polygon = m_destination.make_polygon();

    add_polygon_source(new_polygon, 1.0f, old_polygon);
    m_polygon_old_to_new[old_polygon] = new_polygon;

    return new_polygon;
}

Corner *Geometry_operation::make_new_corner_from_polygon_centroid(Polygon *new_polygon, Polygon *old_polygon)
{
    assert(new_polygon != nullptr);
    assert(old_polygon != nullptr);

    Point *new_point = m_old_polygon_centroid_to_new_points[old_polygon];
    Corner *new_corner = new_polygon->make_corner(new_point);
    distribute_corner_sources(new_corner, 1.0f, new_point);
    return new_corner;
}

Corner *Geometry_operation::make_new_corner_from_corner(Polygon *new_polygon, Corner *old_corner)
{
    assert(new_polygon != nullptr);
    assert(old_corner != nullptr);

    Point *old_point = old_corner->point;
    Point *new_point = m_point_old_to_new[old_point];
    Corner *new_corner = new_polygon->make_corner(new_point);
    add_corner_source(new_corner, 1.0f, old_corner);
    return new_corner;
}

void Geometry_operation::add_polygon_corners(Polygon *new_polygon, Polygon *old_polygon)
{
    assert(new_polygon != nullptr);
    assert(old_polygon != nullptr);

    for (auto old_corner : old_polygon->corners())
    {
        Point *old_point = old_corner->point;
        Point *new_point = m_point_old_to_new[old_point];
        Corner *new_corner = new_polygon->make_corner(new_point);
        add_corner_source(new_corner, 1.0f, old_corner);
    }
}

void Geometry_operation::add_point_source(Point *new_point, float weight, Point *old_point)
{
    assert(old_point != nullptr);

    auto fi = m_new_point_sources.find(new_point);
    if (fi == m_new_point_sources.end())
    {
        vector<pair<float, Point *>> v;
        m_new_point_sources[new_point] = v;
    }

    m_new_point_sources[new_point].push_back(make_pair(weight, old_point));
}

void Geometry_operation::add_point_source(Point *new_point, float weight, Corner *old_corner)
{
    assert(new_point != nullptr);
    assert(old_corner != nullptr);

    auto fi = m_new_point_corner_sources.find(new_point);
    if (fi == m_new_point_corner_sources.end())
    {
        vector<pair<float, Point *>> v;
        m_new_point_sources[new_point] = v;
    }

    m_new_point_corner_sources[new_point].push_back(make_pair(weight, old_corner));
}

void Geometry_operation::add_corner_source(Corner *new_corner, float weight, Corner *old_corner)
{
    assert(new_corner != nullptr);
    assert(old_corner != nullptr);

    auto fi = m_new_corner_sources.find(new_corner);
    if (fi == m_new_corner_sources.end())
    {
        vector<pair<float, Corner *>> v;
        m_new_corner_sources[new_corner] = v;
    }
    m_new_corner_sources[new_corner].push_back(make_pair(weight, old_corner));
}

void Geometry_operation::distribute_corner_sources(Corner *new_corner, float weight, Point *new_point)
{
    assert(new_corner != nullptr);
    assert(new_point != nullptr);

    auto sources = m_new_point_corner_sources[new_point];
    for (auto &source : sources)
    {
        add_corner_source(new_corner, weight * source.first, source.second);
    }
}

void Geometry_operation::add_polygon_source(Polygon *new_polygon, float weight, Polygon *old_polygon)
{
    assert(new_polygon != nullptr);
    assert(old_polygon != nullptr);

    auto fi = m_new_polygon_sources.find(new_polygon);
    if (fi == m_new_polygon_sources.end())
    {
        vector<pair<float, Polygon *>> v;
        m_new_polygon_sources[new_polygon] = v;
    }
    m_new_polygon_sources[new_polygon].push_back(make_pair(weight, old_polygon));
}

void Geometry_operation::add_edge_source(Edge *new_edge, float weight, Edge *old_edge)
{
    assert(new_edge->a() != nullptr);
    assert(new_edge->b() != nullptr);
    assert(old_edge->a() != nullptr);
    assert(old_edge->b() != nullptr);

    auto fi = m_new_edge_sources.find(new_edge);
    if (fi == m_new_edge_sources.end())
    {
        vector<pair<float, Edge *>> v;
        m_new_edge_sources[new_edge] = v;
    }
    m_new_edge_sources[new_edge].push_back(make_pair(weight, old_edge));
}

void Geometry_operation::build_destination_edges_with_sourcing()
{
    m_destination.build_edges();

    for (auto i : m_source.edges())
    {
        auto old_edge = i.first;
        Edge new_edge(m_point_old_to_new[old_edge.a()],
                      m_point_old_to_new[old_edge.b()]);
        add_edge_source(&new_edge, 1.0f, &old_edge);
        m_edge_old_to_new[&old_edge] = &new_edge;
    }
}

void Geometry_operation::interpolate_all_property_maps()
{
    m_source.point_attributes().interpolate(m_destination.point_attributes(), m_new_point_sources);
    m_source.polygon_attributes().interpolate(m_destination.polygon_attributes(), m_new_polygon_sources);
    m_source.corner_attributes().interpolate(m_destination.corner_attributes(), m_new_corner_sources);
    m_source.edge_attributes().interpolate(m_destination.edge_attributes(), m_new_edge_sources);
}

} // namespace operation
} // namespace geometry
} // namespace renderstack
