#include "renderstack_geometry/shapes/torus.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <map>
#include <vector>

namespace renderstack
{
namespace geometry
{

class point;
class corner;
class polygon;

} // namespace geometry
} // namespace renderstack

namespace renderstack
{
namespace geometry
{
namespace shapes
{

using namespace glm;
using namespace std;

struct Torus_builder
{
    Geometry &geometry;

    double major_radius;
    double minor_radius;
    int    major_axis_steps;
    int    minor_axis_steps;

    std::vector<Point *> points;

    Property_map<Point *, vec3> *point_locations{nullptr};
    Property_map<Point *, vec3> *point_normals{nullptr};
    Property_map<Point *, vec3> *point_tangents{nullptr};
    Property_map<Point *, vec2> *point_texcoords{nullptr};
    Property_map<Corner *, vec2> *corner_texcoords{nullptr};
    Property_map<Polygon *, vec3> *polygon_centroids{nullptr};
    Property_map<Polygon *, vec3> *polygon_normals{nullptr};

    Point *torus_point(double rel_major, double rel_minor)
    {
        double R         = major_radius;
        double r         = minor_radius;
        double theta     = (glm::pi<double>() * 2.0 * rel_major);
        double phi       = (glm::pi<double>() * 2.0 * rel_minor);
        double sin_theta = std::sin(theta);
        double cos_theta = std::cos(theta);
        double sin_phi   = std::sin(phi);
        double cos_phi   = std::cos(phi);

        double vx = (R + r * cos_phi) * cos_theta;
        double vz = (R + r * cos_phi) * sin_theta;
        double vy = r * sin_phi;
        vec3   V(vx, vy, vz);

        double tx = -sin_theta;
        double tz = cos_theta;
        double ty = 0.0f;
        vec3   T(tx, ty, tz);

        double bx = -sin_phi * cos_theta;
        double bz = -sin_phi * sin_theta;
        double by = cos_phi;
        vec3   B(bx, by, bz);

        vec3 N = glm::normalize(glm::cross(B, T));

        auto point = geometry.make_point();

        bool is_uv_discontinuity = (rel_major == 1.0) || (rel_minor == 1.0);

        point_locations->put(point, V);
        point_normals->put(point, N);
        point_tangents->put(point, T);
        if (!is_uv_discontinuity)
        {
            point_texcoords->put(point, vec2(rel_major, rel_minor));
        }

        return point;
    }

    void make_corner(Polygon *polygon, int major, int minor)
    {
        double rel_major           = (double)(major) / (double)(major_axis_steps);
        double rel_minor           = (double)(minor) / (double)(minor_axis_steps);
        bool   is_major_seam       = (major == major_axis_steps);
        bool   is_minor_seam       = (minor == minor_axis_steps);
        bool   is_uv_discontinuity = is_major_seam || is_minor_seam;

        if (is_major_seam)
        {
            major = 0;
        }

        if (is_minor_seam)
        {
            minor = 0;
        }

        auto point = points[(static_cast<unsigned int>(major) * static_cast<unsigned int>(minor_axis_steps)) +
                             static_cast<unsigned int>(minor)];

        auto corner = polygon->make_corner(point);

        if (is_uv_discontinuity)
        {
            float s = (float)rel_major;
            float t = (float)rel_minor;

            corner_texcoords->put(corner, vec2(s, t));
        }
    }

    Torus_builder(Geometry &geometry, double major_radius, double minor_radius, int major_axis_steps, int minor_axis_steps)
        : geometry(geometry)
        , major_radius(major_radius)
        , minor_radius(minor_radius)
        , major_axis_steps(major_axis_steps)
        , minor_axis_steps(minor_axis_steps)
    {
        point_locations   = geometry.point_attributes().create<vec3>("point_locations");
        point_normals     = geometry.point_attributes().create<vec3>("point_normals");
        point_tangents    = geometry.point_attributes().create<vec3>("point_tangents");
        point_texcoords   = geometry.point_attributes().create<vec2>("point_texcoords");
        polygon_centroids = geometry.polygon_attributes().create<vec3>("polygon_centroids");
        polygon_normals   = geometry.polygon_attributes().create<vec3>("polygon_normals");
        corner_texcoords  = geometry.corner_attributes().create<vec2>("corner_texcoords");
    }

    void build()
    {
        int major;
        int minor;
        for (major = 0; major < major_axis_steps; ++major)
        {
            double rel_major = (double)(major) / (double)(major_axis_steps);
            for (minor = 0; minor < minor_axis_steps; ++minor)
            {
                double rel_minor = (double)(minor) / (double)(minor_axis_steps);
                auto point = torus_point(rel_major, rel_minor);

                points.push_back(point);
            }
        }

        for (major = 0; major < major_axis_steps; ++major)
        {
            int    next_major     = (major + 1);
            double rel_major      = (double)(major) / (double)(major_axis_steps);
            double rel_next_major = (double)(next_major) / (double)(major_axis_steps);
            double avg_major      = (rel_major + rel_next_major) / 2.0;

            for (minor = 0; minor < minor_axis_steps; ++minor)
            {
                int    next_minor     = (minor + 1);
                double rel_minor      = (double)(minor) / (double)(minor_axis_steps);
                double rel_next_minor = (double)(next_minor) / (double)(minor_axis_steps);
                double avg_minor      = (rel_minor + rel_next_minor) / 2.0;

                auto centroid = torus_point(avg_major, avg_minor);
                auto polygon = geometry.make_polygon();
                make_corner(polygon, next_major, next_minor);
                make_corner(polygon, major, next_minor);
                make_corner(polygon, major, minor);
                make_corner(polygon, next_major, minor);

                polygon_centroids->put(polygon, point_locations->get(centroid));
                polygon_normals->put(polygon, point_normals->get(centroid));
            }
        }

        geometry.build_edges();
        geometry.optimize_attribute_maps();
    }
};

Geometry make_torus(double major_radius, double minor_radius, int major_axis_steps, int minor_axis_steps)
{
    Geometry geometry("torus");
    Torus_builder builder(geometry, major_radius, minor_radius, major_axis_steps, minor_axis_steps);
    builder.build();
    return geometry;
}

} // namespace shapes
} // namespace geometry
} // namespace renderstack
