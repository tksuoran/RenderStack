#include "renderstack_geometry/operation/geometry_operation.hpp"
#include "renderstack_geometry/corner.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace operation
{

using namespace std;

geometry_operation::geometry_operation()
    : m_source(nullptr)
{
    m_destination = make_shared<geometry>();
}

point *geometry_operation::make_new_point_from_point(float weight, point *old_point)
{
    assert(old_point != nullptr);

    point *new_point = m_destination->make_point();
    add_point_source(new_point, weight, old_point);
    m_point_old_to_new[old_point] = new_point;
    return new_point;
}

point *geometry_operation::make_new_point_from_point(point *old_point)
{
    assert(old_point != nullptr);

    point *new_point = m_destination->make_point();
    add_point_source(new_point, 1.0f, old_point);
    m_point_old_to_new[old_point] = new_point;
    return new_point;
}

point *geometry_operation::make_new_point_from_polygon_centroid(polygon *old_polygon)
{
    assert(old_polygon != nullptr);

    point *new_point                                  = m_destination->make_point();
    m_old_polygon_centroid_to_new_points[old_polygon] = new_point;
    add_polygon_centroid(new_point, 1.0f, old_polygon);
    return new_point;
}

void geometry_operation::add_polygon_centroid(point *new_point, float weight, polygon *old_polygon)
{
    for (auto i = old_polygon->corners().cbegin(); i != old_polygon->corners().cend(); ++i)
    {
        corner *old_corner = *i;
        add_point_source(new_point, weight, old_corner);
        add_point_source(new_point, weight, old_corner->point());
    }
}

void geometry_operation::add_point_ring(point *new_point, float weight, point *old_point)
{
    assert(new_point != nullptr);
    assert(old_point != nullptr);

    for (auto i = old_point->corners().cbegin(); i != old_point->corners().cend(); ++i)
    {
        auto     ring_corner      = *i;
        polygon *ring_polygon     = ring_corner->polygon();
        corner * next_ring_corner = ring_polygon->next_corner(ring_corner);
        point *  next_ring_point  = next_ring_corner->point();
        add_point_source(new_point, weight, next_ring_point);
    }
}

polygon *geometry_operation::make_new_polygon_from_polygon(polygon *old_polygon)
{
    assert(old_polygon != nullptr);

    polygon *new_polygon = m_destination->make_polygon();

    add_polygon_source(new_polygon, 1.0f, old_polygon);
    m_polygon_old_to_new[old_polygon] = new_polygon;

    return new_polygon;
}

corner *geometry_operation::make_new_corner_from_polygon_centroid(polygon *new_polygon, polygon *old_polygon)
{
    assert(new_polygon != nullptr);
    assert(old_polygon != nullptr);

    point * new_point  = m_old_polygon_centroid_to_new_points[old_polygon];
    corner *new_corner = new_polygon->make_corner(new_point);
    distribute_corner_sources(new_corner, 1.0f, new_point);
    return new_corner;
}

corner *geometry_operation::make_new_corner_from_corner(polygon *new_polygon, corner *old_corner)
{
    assert(new_polygon != nullptr);
    assert(old_corner != nullptr);

    point * old_point  = old_corner->point();
    point * new_point  = m_point_old_to_new[old_point];
    corner *new_corner = new_polygon->make_corner(new_point);
    add_corner_source(new_corner, 1.0f, old_corner);
    return new_corner;
}

void geometry_operation::add_polygon_corners(polygon *new_polygon, polygon *old_polygon)
{
    assert(new_polygon != nullptr);
    assert(old_polygon != nullptr);

    for (auto i = old_polygon->corners().cbegin(); i != old_polygon->corners().cend(); ++i)
    {
        auto    old_corner = *i;
        point * old_point  = old_corner->point();
        point * new_point  = m_point_old_to_new[old_point];
        corner *new_corner = new_polygon->make_corner(new_point);
        add_corner_source(new_corner, 1.0f, old_corner);
    }
}

void geometry_operation::add_point_source(point *new_point, float weight, point *old_point)
{
    assert(old_point != nullptr);

    auto fi = m_new_point_sources.find(new_point);
    if (fi == m_new_point_sources.end())
    {
        vector<pair<float, point *>> v;
        m_new_point_sources[new_point] = v;
    }

    m_new_point_sources[new_point].push_back(make_pair(weight, old_point));
}

void geometry_operation::add_point_source(point *new_point, float weight, corner *old_corner)
{
    assert(new_point != nullptr);
    assert(old_corner != nullptr);

    auto fi = m_new_point_corner_sources.find(new_point);
    if (fi == m_new_point_corner_sources.end())
    {
        vector<pair<float, point *>> v;
        m_new_point_sources[new_point] = v;
    }

    m_new_point_corner_sources[new_point].push_back(make_pair(weight, old_corner));
}

void geometry_operation::add_corner_source(corner *new_corner, float weight, corner *old_corner)
{
    assert(new_corner != nullptr);
    assert(old_corner != nullptr);

    auto fi = m_new_corner_sources.find(new_corner);
    if (fi == m_new_corner_sources.end())
    {
        vector<pair<float, corner *>> v;
        m_new_corner_sources[new_corner] = v;
    }
    m_new_corner_sources[new_corner].push_back(make_pair(weight, old_corner));
}

void geometry_operation::distribute_corner_sources(corner *new_corner, float weight, point *new_point)
{
    assert(new_corner != nullptr);
    assert(new_point != nullptr);

    auto sources = m_new_point_corner_sources[new_point];
    for (auto i = sources.cbegin(); i != sources.cend(); ++i)
    {
        auto &source = *i;
        add_corner_source(new_corner, weight * source.first, source.second);
    }
}

void geometry_operation::add_polygon_source(polygon *new_polygon, float weight, polygon *old_polygon)
{
    assert(new_polygon != nullptr);
    assert(old_polygon != nullptr);

    auto fi = m_new_polygon_sources.find(new_polygon);
    if (fi == m_new_polygon_sources.end())
    {
        vector<pair<float, polygon *>> v;
        m_new_polygon_sources[new_polygon] = v;
    }
    m_new_polygon_sources[new_polygon].push_back(make_pair(weight, old_polygon));
}

void geometry_operation::add_edge_source(edge *new_edge, float weight, edge *old_edge)
{
    assert(new_edge->a() != nullptr);
    assert(new_edge->b() != nullptr);
    assert(old_edge->a() != nullptr);
    assert(old_edge->b() != nullptr);

    auto fi = m_new_edge_sources.find(new_edge);
    if (fi == m_new_edge_sources.end())
    {
        vector<pair<float, edge *>> v;
        m_new_edge_sources[new_edge] = v;
    }
    m_new_edge_sources[new_edge].push_back(make_pair(weight, old_edge));
}

void geometry_operation::build_destination_edges_with_sourcing()
{
    m_destination->build_edges();

    for (auto i = m_source->edges().begin(); i != m_source->edges().end(); ++i)
    {
        auto old_edge = i->first;
        edge new_edge(m_point_old_to_new[old_edge.a()], m_point_old_to_new[old_edge.b()]);
        add_edge_source(&new_edge, 1.0f, &old_edge);
        m_edge_old_to_new[&old_edge] = &new_edge;
    }
}

void geometry_operation::interpolate_all_property_maps()
{
    m_source->point_attributes().interpolate(m_destination->point_attributes(), m_new_point_sources);
    m_source->polygon_attributes().interpolate(m_destination->polygon_attributes(), m_new_polygon_sources);
    m_source->corner_attributes().interpolate(m_destination->corner_attributes(), m_new_corner_sources);
    m_source->edge_attributes().interpolate(m_destination->edge_attributes(), m_new_edge_sources);
}

} // namespace operation
} // namespace geometry
} // namespace renderstack
