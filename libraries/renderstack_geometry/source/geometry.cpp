#include "renderstack_geometry/geometry.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <stdexcept>

namespace renderstack
{
namespace geometry
{

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

Geometry::~Geometry()
{
    for (auto polygon : polygons())
    {
        delete polygon;
    }

    for (auto point : points())
    {
        delete point;
    }
}

size_t Geometry::count_polygons() const
{
    return polygons().size();
}

size_t Geometry::count_edges() const
{
    return edges().size();
}

size_t Geometry::count_polygon_triangles() const
{
    size_t triangle_count = 0U;
    for (auto polygon : polygons())
    {
        triangle_count += polygon->corners().size() - 2;
    }

    return triangle_count;
}

size_t Geometry::count_corners() const
{
    size_t corner_count = 0U;
    for (auto polygon : polygons())
    {
        corner_count += polygon->corners().size();
    }

    return corner_count;
}

void Geometry::info(Mesh_info &info) const
{
    info.polygon_count  = count_polygons();
    info.corner_count   = count_corners();
    info.triangle_count = count_polygon_triangles();
    info.edge_count     = count_edges();

    //  Additional vertices needed for centroids
    //  3 indices per triangle after triangulation, 2 indices per edge, 1 per corner, 1 index per centroid
    info.vertex_count_corners        = info.corner_count;
    info.vertex_count_centroids      = info.polygon_count;
    info.index_count_fill_triangles  = 3 * info.triangle_count;
    info.index_count_edge_lines      = 2 * info.edge_count;
    info.index_count_corner_points   = info.corner_count;
    info.index_count_centroid_points = info.polygon_count;
}

Geometry::Mesh_info &Geometry::Mesh_info::operator+=(Geometry::Mesh_info const &o)
{
    polygon_count += o.polygon_count;
    corner_count += o.corner_count;
    triangle_count += o.triangle_count;
    edge_count += o.edge_count;
    vertex_count_corners += o.vertex_count_corners;
    vertex_count_centroids += o.vertex_count_centroids;
    index_count_fill_triangles += o.index_count_fill_triangles;
    index_count_edge_lines += o.index_count_edge_lines;
    index_count_corner_points += o.index_count_corner_points;
    index_count_centroid_points += o.index_count_centroid_points;
    return *this;
}

void Geometry::reserve_points(size_t point_count)
{
    if (point_count > m_points.size())
    {
        m_points.reserve(point_count);
    }
}

void Geometry::reserve_polygons(size_t polygon_count)
{
    if (polygon_count > m_polygons.size())
    {
        m_polygons.reserve(polygon_count);
    }
}

Point *Geometry::make_point()
{
    auto pnt = new Point();
    m_points.push_back(pnt);
    return pnt;
}

Polygon *Geometry::make_polygon()
{
    auto pol = new Polygon();
    m_polygons.push_back(pol);
    return pol;
}

Geometry::Point_collection &Geometry::points()
{
    return m_points;
}

Geometry::Point_collection const &Geometry::points() const
{
    return m_points;
}

Geometry::Polygon_collection &Geometry::polygons()
{
    return m_polygons;
}

Geometry::Polygon_collection const &Geometry::polygons() const
{
    return m_polygons;
}

Geometry::Edge_collection &Geometry::edges()
{
    return m_edges;
}

const Geometry::Edge_collection &Geometry::edges() const
{
    return m_edges;
}

void Geometry::compute_polygon_normals()
{
    if (points().empty())
    {
        return;
    }

    auto polygon_normals = polygon_attributes().find_or_create<vec3>("polygon_normals");
    auto point_locations = point_attributes().maybe_find<vec3>("point_locations");

    if (point_locations == nullptr)
    {
        return;
    }

    for (auto pol : polygons())
    {
        pol->compute_normal(*polygon_normals, *point_locations);
    }
}

void Geometry::compute_polygon_centroids()
{
    auto polygon_centroids = polygon_attributes().find_or_create<vec3>("polygon_centroids");
    auto point_locations   = point_attributes().maybe_find<vec3>("point_locations");

    if (point_locations == nullptr)
    {
        return;
    }

    for (auto pol : polygons())
    {
        pol->compute_centroid(*polygon_centroids, *point_locations);
    }
}

//void Geometry::smooth_normalize(const string &corner_attribute,
//                                const string &polygon_attribute,
//                                float         max_smoothing_angle_radians)
//{
//    Property_map_base<Corner*> = find_any
//    auto corner_attributes2  = corner_attributes().find_or_create<vec3>(corner_attribute /*"corner_normals"*/);
//    auto polygon_attributes2 = polygon_attributes().maybe_find<vec3>(polygon_attribute /*"polygon_normals"*/);
//    auto polygon_normals     = polygon_attributes().maybe_find<vec3>("polygon_normals");
//}

// void Geometry::smooth_normalize(const std::string &corner_attribute,
//                                 const std::string &polygon_attribute,
//                                 float         max_smoothing_angle_radians)
// {
//     auto corner_attributes2  = corner_attributes().find_or_create<vec3>(corner_attribute /*"corner_normals"*/);
//     auto polygon_attributes2 = polygon_attributes().maybe_find<vec3>(polygon_attribute /*"polygon_normals"*/);
//     auto polygon_normals     = polygon_attributes().maybe_find<vec3>("polygon_normals");
// 
//     if (polygon_attributes2 == nullptr || polygon_normals == nullptr)
//     {
//         return;
//     }
// 
//     float cos_max_smoothing_angle = cos(max_smoothing_angle_radians);
// 
//     corner_attributes2->clear();
//     for (auto pol : polygons())
//     {
//         if (max_smoothing_angle_radians == 0.0f)
//         {
//             pol->copy_to_corners(*corner_attributes2, *polygon_attributes2);
//         }
//         else
//         {
//             pol->smooth_normalize(*corner_attributes2,
//                                   *polygon_attributes2,
//                                   *polygon_normals,
//                                   cos_max_smoothing_angle);
//         }
//     }
// }
// 
// void Geometry::smooth_average(const std::string &corner_attribute,
//                               const std::string &point_normal_name)
// {
//     auto corner_attributes2 = corner_attributes().find_or_create<vec4>(corner_attribute);
//     auto corner_normals     = corner_attributes().find_or_create<vec3>("corner_normals");
//     auto point_normals      = point_attributes().maybe_find<vec3>(point_normal_name);
// 
//     if (point_normals == nullptr)
//     {
//         return; // TODO?
//     }
// 
//     auto new_corner_attributes = corner_attributes().find_or_create<vec4>("temp");
//     for (auto pol : polygons())
//     {
//         pol->smooth_average(*new_corner_attributes,
//                             *corner_attributes2,
//                             *corner_normals,
//                             *point_normals);
//     }
//     corner_attributes().replace<vec4>(corner_attribute, "temp");
// }

void Geometry::build_edges()
{
    m_edges.clear();

    size_t polygon_index = 0U;
    for (auto polygon : polygons())
    {
        auto first_point = polygon->corners().front()->point;
        Point *previous_point{nullptr};

        size_t corner_index = 0U;
        for (auto corner : polygon->corners())
        {
            if (previous_point != nullptr)
            {
                auto a = previous_point;
                auto b = corner->point;
                if (a == b)
                {
                    throw std::runtime_error("duplicate point");
                }

                Edge edge(a, b);
                //CheckEdge(edge);
                m_edges[edge].insert(polygon);
            }

            previous_point = corner->point;
            ++corner_index;
        }

        Edge last_edge(previous_point, first_point);
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

vec3 Geometry::compute_point_normal(Point *point)
{
    auto polygon_normals = polygon_attributes().find_or_create<vec3>("polygon_normals");

    vec3 normal_sum(0.0f, 0.0f, 0.0f);

    for (auto corner : point->corners())
    {
        normal_sum += polygon_normals->get(corner->polygon);
    }
    return normalize(normal_sum);
}

void Geometry::compute_point_normals(const std::string &map_name)
{
    auto point_normals   = point_attributes().find_or_create<vec3>(map_name);
    auto polygon_normals = polygon_attributes().find<vec3>("polygon_normals");

    point_normals->clear();
    for (auto point : points())
    {
        vec3 normal_sum(0.0f, 0.0f, 0.0f);
        for (auto corner : point->corners())
        {
            normal_sum += polygon_normals->get(corner->polygon);
        }
        point_normals->put(point, normalize(normal_sum));
    }
}

Point *Geometry::make_point(float x, float y, float z)
{
    auto pnt             = make_point();
    auto point_positions = point_attributes().find_or_create<vec3>("point_locations");

    point_positions->put(pnt, vec3(x, y, z));

    return pnt;
}

Point *Geometry::make_point(float x, float y, float z, float s, float t)
{
    auto pnt             = make_point();
    auto point_positions = point_attributes().find_or_create<vec3>("point_locations");
    auto point_texcoords = point_attributes().find_or_create<vec2>("point_texcoords");

    point_positions->put(pnt, vec3(x, y, z));
    point_texcoords->put(pnt, vec2(s, t));

    return pnt;
}

Point *Geometry::make_point(double x, double y, double z)
{
    return make_point(float(x), float(y), float(z));
}

Point *Geometry::make_point(double x, double y, double z, double s, double t)
{
    return make_point(float(x), float(y), float(z), float(s), float(t));
}

Polygon *Geometry::make_polygon(const std::initializer_list<Point *> point_list)
{
    auto p = make_polygon();
    for (auto point : point_list)
    {
        p->make_corner(point);
    }
    return p;
}

Polygon *Geometry::make_polygon(const std::initializer_list<size_t> point_list)
{
    auto p = make_polygon();
    for (auto i : point_list)
    {
        p->make_corner(m_points[i]);
    }
    return p;
}

void Geometry::transform(mat4 const &m)
{
    mat4 it = glm::transpose(glm::inverse(m));

    //  Check.. Did I forget something?
    //  \todo Mark each attributemap how they should be transformed

    auto polygon_centroids = polygon_attributes().maybe_find<vec3>("polygon_centroids");
    auto polygon_normals   = polygon_attributes().maybe_find<vec3>("polygon_normals");
    auto point_locations   = point_attributes().maybe_find<vec3>("point_locations");
    auto point_normals     = point_attributes().maybe_find<vec3>("point_normals");
    auto corner_normals    = corner_attributes().maybe_find<vec3>("corner_normals");

    for (auto point : points())
    {
        if (point_locations != nullptr && point_locations->has(point))
        {
            point_locations->put(point, vec3(m * vec4(point_locations->get(point), 1.0f)));
        }

        if (point_normals != nullptr && point_normals->has(point))
        {
            point_normals->put(point, vec3(it * vec4(point_normals->get(point), 0.0f)));
        }
    }

    for (auto polygon : polygons())
    {
        if (polygon_centroids != nullptr && polygon_centroids->has(polygon))
        {
            polygon_centroids->put(polygon, vec3(m * vec4(polygon_centroids->get(polygon), 1.0f)));
        }

        if (polygon_normals != nullptr && polygon_normals->has(polygon))
        {
            polygon_normals->put(polygon, vec3(it * vec4(polygon_normals->get(polygon), 0.0f)));
        }

        if (corner_normals != nullptr)
        {
            for (auto corner : polygon->corners())
            {
                if (corner_normals != nullptr && corner_normals->has(corner))
                {
                    corner_normals->put(corner, vec3(it * vec4(corner_normals->get(corner), 0.0f)));
                }
            }
        }
    }
}

void Geometry::optimize_attribute_maps()
{
    m_point_property_map_collection.ensure_optimized();
    m_corner_property_map_collection.ensure_optimized();
    m_polygon_property_map_collection.ensure_optimized();
    m_edge_property_map_collection.ensure_optimized();
}

} // namespace geometry
} // namespace renderstack
