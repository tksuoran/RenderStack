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
using namespace std;

struct disc::make_info
{
    double inner_radius;
    double outer_radius;
    int    slice_count;
    int    stack_count;

    map<pair<int, int>, class point *> points;
    class point *                      center;

    shared_ptr<property_map<point *, vec3>>   point_locations;
    shared_ptr<property_map<point *, vec3>>   point_normals;
    shared_ptr<property_map<point *, vec2>>   point_texcoords;
    shared_ptr<property_map<corner *, vec3>>  corner_normals;
    shared_ptr<property_map<corner *, vec2>>  corner_texcoords;
    shared_ptr<property_map<polygon *, vec3>> polygon_centroids;
    shared_ptr<property_map<polygon *, vec3>> polygon_normals;

    make_info(
        double inner_radius_,
        double outer_radius_,
        int    slice_count_,
        int    stack_count_)
    {
        inner_radius = inner_radius_;
        outer_radius = outer_radius_;
        slice_count  = slice_count_;
        stack_count  = stack_count_;
    }
};

point *disc::get_point(make_info &info, int slice, int stack)
{
    return info.points[make_pair(slice, stack)];
}

//  relStackIn is in range -1..1
//  relStack is in range 0..1
point *disc::make_point(make_info &info, double rel_slice, double rel_stack)
{
    double phi                 = pi<double>() * 2.0 * rel_slice;
    double sin_phi             = sin(phi);
    double cos_phi             = cos(phi);
    double one_minus_rel_stack = 1.0 - rel_stack;

    vec3 position(
        static_cast<float>(one_minus_rel_stack * (info.outer_radius * cos_phi) + rel_stack * (info.inner_radius * cos_phi)),
        static_cast<float>(one_minus_rel_stack * (info.outer_radius * sin_phi) + rel_stack * (info.inner_radius * sin_phi)),
        0.0f);

    float s = static_cast<float>(rel_slice);
    float t = static_cast<float>(rel_stack);

    class point *point = geometry::make_point();

    info.point_locations->put(point, vec3(position.x, position.y, position.z));
    info.point_normals->put(point, vec3(0.0f, 0.0f, 1.0f));
    info.point_texcoords->put(point, vec2(s, t));

    return point;
}

corner *disc::make_corner(make_info &info, polygon *polygon, int slice, int stack)
{
    double       rel_slice        = (double)slice / info.slice_count;
    double       rel_stack        = (info.stack_count == 1) ? 1.0 : (double)stack / (double)(info.stack_count - 1);
    bool         slice_seam       = (slice == 0) || (slice == info.slice_count);
    bool         center           = (stack == 0) && (info.inner_radius == 0.0);
    bool         uv_discontinuity = slice_seam || center;
    class point *point;

    if (center)
    {
        point = info.center;
    }
    else
    {
        if (slice == info.slice_count)
            point = info.points[make_pair(0, stack)];
        else
            point = info.points[make_pair(slice, stack)];
    }

    class corner *corner = polygon->make_corner(point);

    if (uv_discontinuity)
    {
        float s = static_cast<float>(rel_slice);
        float t = static_cast<float>(rel_stack);

        info.corner_texcoords->put(corner, vec2(s, t));
    }

    return corner;
}

disc::disc(
    double outer_radius,
    double inner_radius,
    int    slice_count,
    int    stack_count)
    : geometry("disc")
{
    make_info info(outer_radius, inner_radius, slice_count, stack_count);

    info.point_locations   = point_attributes().find_or_create<vec3>("point_locations");
    info.point_normals     = point_attributes().find_or_create<vec3>("point_normals");
    info.point_texcoords   = point_attributes().find_or_create<vec2>("point_texcoords");
    info.polygon_centroids = polygon_attributes().find_or_create<vec3>("polygon_centroids");
    info.polygon_normals   = polygon_attributes().find_or_create<vec3>("polygon_normals");
    info.corner_normals    = corner_attributes().find_or_create<vec3>("corner_normals");
    info.corner_texcoords  = corner_attributes().find_or_create<vec2>("corner_texcoords");

    //  Make points
    for (int stack = 0; stack < stack_count; ++stack)
    {
        double rel_stack = (stack_count == 1) ? 1.0 : (double)stack / (double)(stack_count - 1);
        for (int slice = 0; slice <= slice_count; ++slice)
        {
            double rel_slice = (double)slice / (double)slice_count;

            info.points[make_pair(slice, stack)] = make_point(info, rel_slice, rel_stack);
        }
    }

    //  Special case without center point
    if (stack_count == 1)
    {
        class polygon *polygon = make_polygon();
        info.polygon_centroids->put(polygon, vec3(0.0f, 0.0f, 0.0f));
        info.polygon_normals->put(polygon, vec3(0.0f, 0.0f, 1.0f));

        for (int slice = slice_count - 1; slice >= 0; --slice)
            make_corner(info, polygon, slice, 0);
        return;
    }

    /*  Make center point if needed  */
    if (inner_radius == 0.0f)
        info.center = geometry::make_point(0.0f, 0.0f, 0.0f);

    /*  Quads/triangles  */
    for (int stack = 0; stack < stack_count - 1; ++stack)
    {
        double rel_stack_centroid = (stack_count == 1) ? 0.5 : (double)stack / (double)(stack_count - 1);

        for (int slice = 0; slice < slice_count; ++slice)
        {
            double   rel_slice_centroid = ((double)(slice) + 0.5) / (double)(slice_count);
            point *  centroid           = make_point(info, rel_slice_centroid, rel_stack_centroid);
            polygon *polygon            = make_polygon();

            info.polygon_centroids->put(polygon, info.point_locations->get(centroid));
            info.polygon_normals->put(polygon, vec3(0.0f, 0.0f, 1.0f));
            if ((stack == 0) && (inner_radius == 0.0))
            {
                corner *tip = make_corner(info, polygon, slice, stack);
                make_corner(info, polygon, slice + 1, stack + 1);
                make_corner(info, polygon, slice, stack + 1);

                vec2 t1               = info.point_texcoords->get(get_point(info, slice, stack));
                vec2 t2               = info.point_texcoords->get(get_point(info, slice + 1, stack));
                vec2 average_texcoord = (t1 + t2) / 2.0f;
                info.corner_texcoords->put(tip, average_texcoord);
            }
            else
            {
                make_corner(info, polygon, slice + 1, stack + 1);
                make_corner(info, polygon, slice, stack + 1);
                make_corner(info, polygon, slice, stack);
                make_corner(info, polygon, slice + 1, stack);
            }
        }
    }
    build_edges();
}

} // namespace shapes
} // namespace geometry
} // namespace renderstack
