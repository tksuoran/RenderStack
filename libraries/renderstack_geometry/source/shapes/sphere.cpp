#include "renderstack_geometry/shapes/sphere.hpp"
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

namespace
{

struct Sphere_builder
{
    Geometry &geometry;
    double   radius;
    int      slice_count;
    int      stack_division;
    int      stack_count;
    int      stack_base0_bottom;
    int      stack_base0_top;

    vector<Point *> points;
    Point *         top{nullptr};
    Point *         bottom{nullptr};

    Property_map<Point *, vec3> *   point_locations{nullptr};
    Property_map<Point *, vec3> *   point_normals{nullptr};
    Property_map<Point *, vec3> *   point_tangents{nullptr};
    Property_map<Point *, vec2> *   point_texcoords{nullptr};
    Property_map<Corner *, vec2> *  corner_texcoords{nullptr};
    Property_map<Polygon *, vec3> * polygon_centroids{nullptr};
    Property_map<Polygon *, vec3> * polygon_normals{nullptr};

    Point *sphere_point(double rel_slice, double rel_stack)
    {
        double phi     = (glm::pi<double>() * 2.0 * rel_slice);
        double sin_phi = std::sin(phi);
        double cos_phi = std::cos(phi);

        double theta     = (glm::pi<double>() * 0.5 * rel_stack);
        double sin_theta = std::sin(theta);
        double cos_theta = std::cos(theta);

        double stack_radius = cos_theta;

        float xVN = (float)(stack_radius * cos_phi);
        float yVN = (float)(sin_theta);
        float zVN = (float)(stack_radius * sin_phi);

        float xP = (float)(radius * xVN);
        float yP = (float)(radius * yVN);
        float zP = (float)(radius * zVN);

        float s = 1.0f - (float)(rel_slice);
        float t = 1.0f - (float)(0.5 * (1.0 + rel_stack));

        // B = T x N
        // T = N x B
        // N = B x T

        vec3 N(xVN, yVN, zVN);
        vec3 axis(0.0f, 1.0f, 0.0f);
        vec3 B = normalize(cross(axis, N));
        vec3 T = normalize(cross(N, B));

        auto point = geometry.make_point();

        bool is_uv_discontinuity = (rel_stack == -1.0) || (rel_stack == 1.0) || /*(relSlice == 0.0) ||*/ (rel_slice == 1.0);

        point_locations->put(point, vec3(xP, yP, zP));
        point_normals->put(point, N);
        point_tangents->put(point, T);
        if (!is_uv_discontinuity)
        {
            point_texcoords->put(point, vec2(s, t));
        }

        return point;
    }

    void make_corner(Polygon *polygon, int slice, int stack_base0)
    {
        assert(slice >= 0);

        //int stack_base0 = stack + stackDivision;
        int    stack               = stack_base0 - stack_division;
        double rel_slice           = (double)(slice) / (double)(slice_count);
        double rel_stack           = (double)(stack) / (double)(stack_division + 1);
        bool   is_slice_seam       = /*(slice == 0) ||*/ (slice == slice_count);
        bool   is_bottom           = (stack_base0 == stack_base0_bottom);
        bool   is_top              = (stack_base0 == stack_base0_top);
        bool   is_uv_discontinuity = is_slice_seam || is_bottom || is_top;

        Point *point;
        if (is_top)
        {
            point = top;
        }
        else if (is_bottom)
        {
            point = bottom;
        }
        else if (slice == slice_count)
        {
            point = points[static_cast<unsigned int>(stack_base0)];
        }
        else
        {
            point = points[(static_cast<unsigned int>(slice) * static_cast<unsigned int>(stack_count)) + static_cast<unsigned int>(stack_base0)];
        }

        auto corner = polygon->make_corner(point);

        if (is_uv_discontinuity)
        {
            float s = 1.0f - (float)(rel_slice);
            float t = 1.0f - (float)(0.5 * (1.0 + rel_stack));

            corner_texcoords->put(corner, vec2(s, t));
        }
    }

    Sphere_builder(Geometry &geometry, double radius, int slice_count, int stack_division)
        : geometry(geometry)
        , radius(radius)
        , slice_count(slice_count)
        , stack_division(stack_division)
        , stack_count((2 * stack_division) + 1)
        , stack_base0_bottom(-1)
        , stack_base0_top(static_cast<int>(2 * stack_division + 1))
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
        int slice;
        int stack;
        for (slice = 0; slice < slice_count; ++slice)
        {
            double rel_slice = (double)(slice) / (double)(slice_count);
            for (stack = -stack_division; stack <= stack_division; ++stack)
            {
                double rel_stack = (double)(stack) / (double)(stack_division + 1);
                auto point = sphere_point(rel_slice, rel_stack);
                points.push_back(point);
            }
        }
        bottom = sphere_point(0.5f, -1.0f);
        top    = sphere_point(0.5f,  1.0f);

        // Bottom fan
        {
            for (slice = 0; slice < slice_count; ++slice)
            {
                int next_slice  = slice + 1;
                int stack_base0 = 0;

                double rel_slice = ((double)(slice) + 0.5) / (double)(slice_count);
                double rel_stack = -1.0 + (0.5 / (double)(stack_division + 1));

                auto centroid = sphere_point(rel_slice, rel_stack);
                auto polygon  = geometry.make_polygon();
                make_corner(polygon, slice, stack_base0);
                make_corner(polygon, slice, stack_base0_bottom);
                make_corner(polygon, next_slice, stack_base0);

                polygon_centroids->put(polygon, point_locations->get(centroid));
                polygon_normals->put(polygon, point_normals->get(centroid));
            }
        }

        // Middle quads, bottom up
        for (stack = -stack_division; stack < stack_division; ++stack)
        {
            int stack_base0      = stack + stack_division;
            int next_stack_base0 = stack_base0 + 1;

            double rel_stack = ((double)(stack) + 0.5) / (double)(stack_division + 1);

            for (slice = 0; slice < slice_count; ++slice)
            {
                int    next_slice = (slice + 1);
                double rel_slice  = ((double)(slice) + 0.5) / (double)(slice_count);

                auto centroid = sphere_point(rel_slice, rel_stack);
                auto polygon  = geometry.make_polygon();
                make_corner(polygon, next_slice, next_stack_base0);
                make_corner(polygon, slice, next_stack_base0);
                make_corner(polygon, slice, stack_base0);
                make_corner(polygon, next_slice, stack_base0);

                polygon_centroids->put(polygon, point_locations->get(centroid));
                polygon_normals->put(polygon, point_normals->get(centroid));
            }
        }

        // Top fan
        for (slice = 0; slice < slice_count; ++slice)
        {
            int next_slice  = (slice + 1);
            int stack_base0 = stack_division + stack_division;

            double rel_slice = ((double)(slice) + 0.5) / (double)(slice_count);
            double rel_stack = 1.0 - (0.5 / (double)(stack_division + 1));

            auto centroid = sphere_point(rel_slice, rel_stack);
            auto polygon  = geometry.make_polygon();
            make_corner(polygon, slice, stack_base0_top);
            make_corner(polygon, slice, stack_base0);
            make_corner(polygon, next_slice, stack_base0);

            polygon_centroids->put(polygon, point_locations->get(centroid));
            polygon_normals->put(polygon, point_normals->get(centroid));
        }

        geometry.build_edges();
        geometry.optimize_attribute_maps();
    }
};

} // namespace

Geometry make_sphere(double radius, unsigned int slice_count, unsigned int stack_division)
{
    Geometry geometry("sphere");
    Sphere_builder builder(geometry, radius, static_cast<int>(slice_count), static_cast<int>(stack_division));
    builder.build();\
    return geometry;
}

} // namespace shapes
} // namespace geometry
} // namespace renderstack
