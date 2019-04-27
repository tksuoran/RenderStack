#include "renderstack_geometry/shapes/disc.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <map>
#include <vector>

namespace renderstack
{
namespace geometry
{
namespace shapes
{

using namespace glm;

struct Disc_builder
{
    Geometry &geometry;

    double inner_radius;
    double outer_radius;
    int    slice_count;
    int    stack_count;

    std::map<std::pair<int, int>, Point *> points;
    Point *center{nullptr};

    Property_map<Point *, vec3>   *point_locations{nullptr};
    Property_map<Point *, vec3>   *point_normals{nullptr};
    Property_map<Point *, vec2>   *point_texcoords{nullptr};
    Property_map<Corner *, vec3>  *corner_normals{nullptr};
    Property_map<Corner *, vec2>  *corner_texcoords{nullptr};
    Property_map<Polygon *, vec3> *polygon_centroids{nullptr};
    Property_map<Polygon *, vec3> *polygon_normals{nullptr};

    Point *get_point(int slice, int stack)
    {
        return points[std::make_pair(slice, stack)];
    }

    // relStackIn is in range -1..1
    // relStack is in range 0..1
    Point *make_point(double rel_slice, double rel_stack)
    {
        double phi                 = glm::pi<double>() * 2.0 * rel_slice;
        double sin_phi             = std::sin(phi);
        double cos_phi             = std::cos(phi);
        double one_minus_rel_stack = 1.0 - rel_stack;

        vec3 position(static_cast<float>(one_minus_rel_stack * (outer_radius * cos_phi) + rel_stack * (inner_radius * cos_phi)),
                      static_cast<float>(one_minus_rel_stack * (outer_radius * sin_phi) + rel_stack * (inner_radius * sin_phi)),
                      0.0f);

        float s = static_cast<float>(rel_slice);
        float t = static_cast<float>(rel_stack);

        auto point = geometry.make_point();

        point_locations->put(point, vec3(position.x, position.y, position.z));
        point_normals->put(point, vec3(0.0f, 0.0f, 1.0f));
        point_texcoords->put(point, vec2(s, t));

        return point;
    }

    Corner *make_corner(Polygon *polygon, int slice, int stack)
    {
        double rel_slice           = (double)slice / slice_count;
        double rel_stack           = (stack_count == 1) ? 1.0 : (double)stack / (double)(stack_count - 1);
        bool   is_slice_seam       = (slice == 0) || (slice == slice_count);
        bool   is_center           = (stack == 0) && (inner_radius == 0.0);
        bool   is_uv_discontinuity = is_slice_seam || is_center;

        Point *point;
        if (center)
        {
            point = center;
        }
        else
        {
            if (slice == slice_count)
            {
                point = points[std::make_pair(0, stack)];
            }
            else
            {
                point = points[std::make_pair(slice, stack)];
            }
        }

        auto corner = polygon->make_corner(point);

        if (is_uv_discontinuity)
        {
            float s = static_cast<float>(rel_slice);
            float t = static_cast<float>(rel_stack);

            corner_texcoords->put(corner, vec2(s, t));
        }

        return corner;
    }

    Disc_builder(Geometry &geometry,
                 double   inner_radius,
                 double   outer_radius,
                 int      slice_count,
                 int      stack_count)
        : geometry(geometry)
        , inner_radius(inner_radius)
        , outer_radius(outer_radius)
        , slice_count(slice_count)
        , stack_count(stack_count)
    {
        point_locations   = geometry.point_attributes().create<vec3>("point_locations");
        point_normals     = geometry.point_attributes().create<vec3>("point_normals");
        point_texcoords   = geometry.point_attributes().create<vec2>("point_texcoords");
        polygon_centroids = geometry.polygon_attributes().create<vec3>("polygon_centroids");
        polygon_normals   = geometry.polygon_attributes().create<vec3>("polygon_normals");
        corner_normals    = geometry.corner_attributes().create<vec3>("corner_normals");
        corner_texcoords  = geometry.corner_attributes().create<vec2>("corner_texcoords");
    }

    void build()
    {
        // Make points
        for (int stack = 0; stack < stack_count; ++stack)
        {
            double rel_stack = (stack_count == 1) ? 1.0 : (double)stack / (double)(stack_count - 1);
            for (int slice = 0; slice <= slice_count; ++slice)
            {
                double rel_slice = (double)slice / (double)slice_count;

                points[std::make_pair(slice, stack)] = make_point(rel_slice, rel_stack);
            }
        }

        // Special case without center point
        if (stack_count == 1)
        {
            auto polygon = geometry.make_polygon();
            polygon_centroids->put(polygon, vec3(0.0f, 0.0f, 0.0f));
            polygon_normals->put(polygon, vec3(0.0f, 0.0f, 1.0f));

            for (int slice = slice_count - 1; slice >= 0; --slice)
            {
                make_corner(polygon, slice, 0);
            }
            return;
        }

        // Make center point if needed
        if (inner_radius == 0.0f)
        {
            center = geometry.make_point(0.0f, 0.0f, 0.0f);
        }

        // Quads/triangles
        for (int stack = 0; stack < stack_count - 1; ++stack)
        {
            double rel_stack_centroid = (stack_count == 1) ? 0.5 : (double)stack / (double)(stack_count - 1);

            for (int slice = 0; slice < slice_count; ++slice)
            {
                double rel_slice_centroid = ((double)(slice) + 0.5) / (double)(slice_count);
                auto centroid = make_point(rel_slice_centroid, rel_stack_centroid);
                auto polygon = geometry.make_polygon();

                polygon_centroids->put(polygon, point_locations->get(centroid));
                polygon_normals->put(polygon, vec3(0.0f, 0.0f, 1.0f));
                if ((stack == 0) && (inner_radius == 0.0))
                {
                    auto tip = make_corner(polygon, slice, stack);
                    make_corner(polygon, slice + 1, stack + 1);
                    make_corner(polygon, slice, stack + 1);

                    vec2 t1               = point_texcoords->get(get_point(slice, stack));
                    vec2 t2               = point_texcoords->get(get_point(slice + 1, stack));
                    vec2 average_texcoord = (t1 + t2) / 2.0f;
                    corner_texcoords->put(tip, average_texcoord);
                }
                else
                {
                    make_corner(polygon, slice + 1, stack + 1);
                    make_corner(polygon, slice, stack + 1);
                    make_corner(polygon, slice, stack);
                    make_corner(polygon, slice + 1, stack);
                }
            }
        }

        geometry.build_edges();
        geometry.optimize_attribute_maps();
    }
};

Geometry make_disc(double outer_radius,
                   double inner_radius,
                   int    slice_count,
                   int    stack_count)
{
    Geometry geometry("disc");
    Disc_builder builder(geometry, outer_radius, inner_radius, slice_count, stack_count);
    builder.build();
    return geometry;
}


} // namespace shapes
} // namespace geometry
} // namespace renderstack
