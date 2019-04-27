#include "renderstack_geometry/shapes/cube.hpp"
#include "renderstack_geometry/point.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <map>
#include <memory>

namespace renderstack
{
namespace geometry
{
namespace shapes
{

namespace
{
// http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T>
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

float spow(float x, float p)
{
    return sign(x) * std::pow(std::abs(x), p);
}

}

using namespace glm;

Geometry make_cube(double x_size, double y_size, double z_size)
{
    double x = x_size / 2.0;
    double y = y_size / 2.0;
    double z = z_size / 2.0;

    Geometry geometry;

    geometry.make_point(-x,  y,  z, 0, 1); //  0
    geometry.make_point( x,  y,  z, 1, 1); //  1
    geometry.make_point( x, -y,  z, 1, 1); //  2
    geometry.make_point(-x, -y,  z, 0, 1); //  3
    geometry.make_point(-x,  y, -z, 0, 0); //  4
    geometry.make_point( x,  y, -z, 1, 0); //  5
    geometry.make_point( x, -y, -z, 1, 0); //  6
    geometry.make_point(-x, -y, -z, 0, 0); //  7

    geometry.make_polygon( {0, 1, 2, 3} );
    geometry.make_polygon( {0, 3, 7, 4} );
    geometry.make_polygon( {0, 4, 5, 1} ); // top
    geometry.make_polygon( {5, 4, 7, 6} );
    geometry.make_polygon( {2, 1, 5, 6} );
    geometry.make_polygon( {7, 3, 2, 6} ); // bottom

    geometry.build_edges();
    geometry.optimize_attribute_maps();

    return geometry;
}

Geometry make_cube(double r)
{
    double sq3 = std::sqrt(3.0);
    return make_cube(2.0 * r / sq3, 2.0 * r / sq3, 2.0 * r / sq3);
}

struct Cube_builder
{
    Geometry                &geometry;
    vec3                    size;
    ivec3                   div;
    float                   p;
    std::map<long, Point *> points;

    Property_map<Point *, vec3>   *point_locations{nullptr};
    Property_map<Point *, vec3>   *point_normals{nullptr};
    Property_map<Point *, vec2>   *point_texcoords{nullptr};
    Property_map<Corner *, vec3>  *corner_normals{nullptr};
    Property_map<Corner *, vec2>  *corner_texcoords{nullptr};
    Property_map<Polygon *, vec3> *polygon_centroids{nullptr};
    Property_map<Polygon *, vec3> *polygon_normals{nullptr};

    Point *make_point(int x, int y, int z, glm::vec3 const &n, float s, float t)
    {
        long key = y * (div.x * 4 * div.z * 4) +
                   x * (div.z * 4) +
                   z;

        auto i = points.find(key);
        if (i != points.end())
        {
            return i->second;
        }

        float rel_x  = static_cast<float>(x) / static_cast<float>(div.x);
        float rel_y  = static_cast<float>(y) / static_cast<float>(div.y);
        float rel_z  = static_cast<float>(z) / static_cast<float>(div.z);
        float rel_xp = spow(rel_x, p);
        float rel_yp = spow(rel_y, p);
        float rel_zp = spow(rel_z, p);

        float x_p = rel_xp * size.x;
        float y_p = rel_yp * size.y;
        float z_p = rel_zp * size.z;

        auto point = geometry.make_point();

        bool is_discontinuity = (x == -div.x) || (x == div.x) || (y == -div.y) || (y == div.y) || (z == -div.z) || (z == div.z);

        point_locations->put(point, vec3(x_p, y_p, z_p));
        if (!is_discontinuity)
        {
            point_normals->put(point, n);
            point_texcoords->put(point, vec2(s, t));
        }

        points[key] = point;

        return point;
    }

    Corner *make_corner(Polygon *polygon, int x, int y, int z, glm::vec3 const &n, float s, float t)
    {
        long key = y * (div.x * 4 * div.z * 4) +
                   x * (div.z * 4) +
                   z;

        auto point  = points[key];
        auto corner = polygon->make_corner(point);

        bool is_discontinuity = (x == -div.x) || (x == div.x) || (y == -div.y) || (y == div.y) || (z == -div.z) || (z == div.z);
        if (is_discontinuity)
        {
            point_normals->put(point, n);
            point_texcoords->put(point, vec2(s, t));
        }

        return corner;
    }

    Cube_builder(Geometry &geometry, const vec3 &size, const ivec3 &div, float p)
        : geometry(geometry)
        , size(size)
        , div(div)
        , p(p)
    {
        point_locations   = geometry.point_attributes().create<vec3>("point_locations");
        point_normals     = geometry.point_attributes().create<vec3>("point_normals");
        point_texcoords   = geometry.point_attributes().create<vec2>("point_texcoords");
        corner_normals    = geometry.corner_attributes().create<vec3>("corner_normals");
        corner_texcoords  = geometry.corner_attributes().create<vec2>("corner_texcoords");
        polygon_centroids = geometry.polygon_attributes().create<vec3>("polygon_centroids");
        polygon_normals   = geometry.polygon_attributes().create<vec3>("polygon_normals");
    }

    void build()
    {
        int x;
        int y;
        int z;

        // Generate vertices
        // Top and bottom
        vec3 unit_x(1.0f, 0.0f, 0.0f);
        vec3 unit_y(0.0f, 1.0f, 0.0f);
        vec3 unit_z(0.0f, 0.0f, 1.0f);
        for (x = -div.x; x <= div.x; x++)
        {
            float rel_x = 0.5f + static_cast<float>(x) / size.x;
            for (z = -div.z; z <= div.z; z++)
            {
                float rel_z = 0.5f + static_cast<float>(z) / size.z;
                make_point(x,  div.y, z,  unit_y, rel_x, rel_z);
                make_point(x, -div.y, z, -unit_y, rel_x, rel_z);
            }
            for (y = -div.y; y <= div.y; y++)
            {
                float rel_y = 0.5f + static_cast<float>(y) / size.y;
                make_point(x, y,  div.z,  unit_z, rel_x, rel_y);
                make_point(x, y, -div.z, -unit_z, rel_x, rel_y);
            }
        }

        // Left and right
        for (z = -div.z; z <= div.z; z++)
        {
            float rel_z = 0.5f + static_cast<float>(z) / size.z;
            for (y = -div.y; y <= div.y; y++)
            {
                float rel_y = 0.5f + static_cast<float>(y) / size.y;
                make_point( div.x, y, z,  unit_x, rel_y, rel_z);
                make_point(-div.x, y, z, -unit_x, rel_y, rel_z);
            }
        }

        // Generate quads
        // Top and bottom
        for (x = -div.x; x < div.x; x++)
        {
            float rel_x1 = 0.5f + static_cast<float>(x) / size.x;
            float rel_x2 = 0.5f + static_cast<float>(x + 1) / size.x;
            for (z = -div.z; z < div.z; z++)
            {
                float rel_z1 = 0.5f + static_cast<float>(z) / size.z;
                float rel_z2 = 0.5f + static_cast<float>(z + 1) / size.z;

                auto top = geometry.make_polygon();
                make_corner(top, x + 1, div.y, z, unit_y, rel_x2, rel_z1);
                make_corner(top, x + 1, div.y, z + 1, unit_y, rel_x2, rel_z2);
                make_corner(top, x,     div.y, z + 1, unit_y, rel_x1, rel_z2);
                make_corner(top, x,     div.y, z, unit_y, rel_x1, rel_z1);

                auto bottom = geometry.make_polygon();
                make_corner(bottom, x, -div.y, z, -unit_y, rel_x1, rel_z1);
                make_corner(bottom, x, -div.y, z + 1, -unit_y, rel_x1, rel_z2);
                make_corner(bottom, x + 1, -div.y, z + 1, -unit_y, rel_x2, rel_z2);
                make_corner(bottom, x + 1, -div.y, z, -unit_y, rel_x2, rel_z1);

                polygon_normals->put(top, unit_y);
                polygon_normals->put(bottom, -unit_y);
            }
            for (y = -div.y; y < div.y; y++)
            {
                float rel_y1 = 0.5f + static_cast<float>(y) / size.y;
                float rel_y2 = 0.5f + static_cast<float>(y + 1) / size.y;

                auto back = geometry.make_polygon();
                make_corner(back, x, y, div.z, unit_z, rel_x1, rel_y1);
                make_corner(back, x, y + 1, div.z, unit_z, rel_x1, rel_y2);
                make_corner(back, x + 1, y + 1, div.z, unit_z, rel_x2, rel_y2);
                make_corner(back, x + 1, y, div.z, unit_z, rel_x2, rel_y1);

                auto front = geometry.make_polygon();
                make_corner(front, x + 1, y, -div.z, -unit_z, rel_x2, rel_y1);
                make_corner(front, x + 1, y + 1, -div.z, -unit_z, rel_x2, rel_y2);
                make_corner(front, x, y + 1, -div.z, -unit_z, rel_x1, rel_y2);
                make_corner(front, x, y, -div.z, -unit_z, rel_x1, rel_y1);

                polygon_normals->put(back, unit_z);
                polygon_normals->put(front, -unit_z);
            }
        }

        // Left and right
        for (z = -div.z; z < div.z; z++)
        {
            float rel_z1 = 0.5f + static_cast<float>(z) / size.z;
            float rel_z2 = 0.5f + static_cast<float>(z + 1) / size.z;

            for (y = -div.y; y < div.y; y++)
            {
                float rel_y1 = 0.5f + static_cast<float>(y) / size.y;
                float rel_y2 = 0.5f + static_cast<float>(y + 1) / size.y;

                auto right = geometry.make_polygon();
                make_corner(right, div.x, y, z, unit_x, rel_y1, rel_z1);
                make_corner(right, div.x, y, z + 1, unit_x, rel_y1, rel_z2);
                make_corner(right, div.x, y + 1, z + 1, unit_x, rel_y2, rel_z2);
                make_corner(right, div.x, y + 1, z, unit_x, rel_y2, rel_z1);

                auto left = geometry.make_polygon();
                make_corner(left, -div.x, y + 1, z, -unit_x, rel_y2, rel_z1);
                make_corner(left, -div.x, y + 1, z + 1, -unit_x, rel_y2, rel_z2);
                make_corner(left, -div.x, y, z + 1, -unit_x, rel_y1, rel_z2);
                make_corner(left, -div.x, y, z, -unit_x, rel_y1, rel_z1);

                polygon_normals->put(right, unit_x);
                polygon_normals->put(left, -unit_x);
            }
        }

        geometry.compute_polygon_centroids();
        geometry.optimize_attribute_maps();
    }
};

Geometry make_cube(const glm::vec3 &size, const glm::ivec3 &div, float p)
{
    Geometry geometry("cube");
    Cube_builder builder(geometry, size / 2.0f, div, p);
    builder.build();
    return geometry;
}

} // namespace shapes
} // namespace geometry
} // namespace renderstack
