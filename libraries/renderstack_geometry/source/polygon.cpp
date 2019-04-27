#include "renderstack_geometry/geometry.hpp"
#include "renderstack_geometry/property_map.hpp"
#include <cassert>
#include <cmath>
#include <exception>

namespace renderstack
{
namespace geometry
{

using namespace glm;
using namespace std;

Polygon::~Polygon()
{
    for (auto corner : m_corners)
    {
        delete corner;
    }
    m_corners.clear();
}

void Polygon::compute_normal(Property_map<Polygon *, vec3> &polygon_normals,
                             const Property_map<Point *, vec3> &point_locations)
{
    if (m_corners.size() > 2)
    {
        Corner_collection::iterator i  = m_corners.begin();
        Corner *                    c0 = *i;
        ++i;                           //  first  point in polygon
        Corner *c1 = *i;               //  second point in polygon
        Corner *c2 = m_corners.back(); //  last   point in polygon
        Point * p0 = c0->point;
        Point * p1 = c1->point;
        Point * p2 = c2->point;

        //  Make sure all points are unique from others
        if ((p0 != p1) &&
            (p0 != p2) &&
            (p1 != p2))
        {
            vec3 pos0   = point_locations.get(p0);
            vec3 pos1   = point_locations.get(p1);
            vec3 pos2   = point_locations.get(p2);
            vec3 normal = cross((pos2 - pos0), (pos1 - pos0));
            normal      = normalize(normal);
            polygon_normals.put(this, normal);
        }
        else
        {
            throw runtime_error("polygons with duplicate points");
        }
    }
}

void Polygon::compute_centroid(Property_map<Polygon *, vec3> &polygon_centroids,
                               const Property_map<Point *, vec3> &point_locations)
{
    vec3 centroid(0.0f, 0.0f, 0.0f);
    int  count = 0;

    for (auto corner : m_corners)
    {
        Point *pnt  = corner->point;
        vec3   pos0 = point_locations.get(pnt);
        centroid += pos0;
        ++count;
    }

    if (count > 0)
    {
        centroid /= (float)(count);
        polygon_centroids.put(this, centroid);
    }
    else
    {
        polygon_centroids.put(this, centroid);
    }
}

Corner *Polygon::corner(Point *point)
{
    for (auto corner : m_corners)
    {
        if (point == corner->point)
        {
            return corner;
        }
    }
    return nullptr;
}

Corner *Polygon::make_corner(Point *point)
{
    Corner *corner = new Corner(point, this);
    point->add_corner(corner);
    m_corners.push_back(corner);
    return corner;
}

Corner *Polygon::next_corner(Corner *c)
{
    for (size_t i = 0; i < m_corners.size(); ++i)
    {
        Corner *corner0 = m_corners[i];
        if (corner0 == c)
        {
            return m_corners[(i + 1) % m_corners.size()];
        }
    }
    return nullptr;
}

Corner *Polygon::prev_corner(Corner *c)
{
    for (size_t i = 0; i < m_corners.size(); ++i)
    {
        Corner *corner0 = m_corners[i];
        if (corner0 == c)
        {
            return m_corners[(m_corners.size() + i - 1) % m_corners.size()];
        }
    }
    return nullptr;
}

} // namespace geometry
} // namespace renderstack
