#include "renderstack_geometry/shapes/cone.hpp"
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

/*  Stack numbering example                                                                 
with stackDivision = 2                                                                  

stack                        rel    known                                               
base0  stack                 stack  as                                                  
----------------------------------------------------------------------------------------
 5      3         /\         1.0    top        (singular = single vertex in this case)  
 4      2        /--\                                                                   
 3      1       /----\                                                                  
 2      0      /======\      0.0    equator                                             
 1     -1     /--------\                                                                
 0     -2    /----------\                                                               
-1     -3   /============\   -1.0    bottom    (not singular in this case)              

bottom: relStack = -1.0  <=>  stack = -stackDivision - 1                                
top:    relStack =  1.0  <=>  stack =  stackDivision + 1                                
*/
struct Conical_frustum_builder
{
    Geometry &geometry;

    double min_x;
    double max_x;
    double bottom_radius;
    double top_radius;
    bool   use_bottom;
    bool   use_top;
    int    slice_count;
    int    stack_division;

    int bottom_not_singular;
    int top_not_singular;
    int stack_count;

    double rel_slice;
    double rel_stack;

    std::map<std::pair<int, int>, Point *> points;
    
    Point *top;
    Point *bottom;

    Property_map<Point *, vec3>   *point_locations;
    Property_map<Point *, vec3>   *point_normals;
    Property_map<Point *, vec2>   *point_texcoords;
    Property_map<Corner *, vec3>  *corner_normals;
    Property_map<Corner *, vec2>  *corner_texcoords;
    Property_map<Polygon *, vec3> *polygon_centroids;
    Property_map<Polygon *, vec3> *polygon_normals;

    Point *get_point(int slice, int stack)
    {
        return points[std::make_pair(slice, stack)];
    }

    //  relStackIn is in range -1..1
    //  relStack is in range 0..1
    Point *cone_point(double rel_slice, double rel_stack_in)
    {
        //var pointLocations      = PointAttributes.FindOrCreate<Vector3>("point_locations");
        //var pointNormals        = PointAttributes.FindOrCreate<Vector3>("point_normals");
        //var pointTexCoords      = PointAttributes.FindOrCreate<Vector2>("point_texcoords");

        double phi                 = glm::pi<double>() * 2.0 * rel_slice;
        double sin_phi             = std::sin(phi);
        double cos_phi             = std::cos(phi);
        double rel_stack           = 0.5 + 0.5 * rel_stack_in;
        double one_minus_rel_stack = 1.0 - rel_stack;

        vec3 position((float)(one_minus_rel_stack * (min_x) + rel_stack * (max_x)),
                      (float)(one_minus_rel_stack * (bottom_radius * sin_phi) + rel_stack * (top_radius * sin_phi)),
                      (float)(one_minus_rel_stack * (bottom_radius * cos_phi) + rel_stack * (top_radius * cos_phi)));
        vec3 bottom((float)(min_x),
                    (float)(bottom_radius * sin_phi),
                    (float)(bottom_radius * cos_phi));
        vec3 top((float)(max_x),
                (float)(top_radius * sin_phi),
                (float)(top_radius * cos_phi));

        vec3 bitangent = normalize(top - bottom); /*  generatrix  */
        vec3 tangent(0.0f,
                     static_cast<float>(std::sin(phi + (glm::pi<double>() * 0.5))),
                     static_cast<float>(std::cos(phi + (glm::pi<double>() * 0.5))));
        vec3 normal = cross(bitangent, tangent);
        normal      = normalize(normal);

        double s = rel_slice;
        double t = 0.5 + 0.5 * rel_stack;

        auto point = geometry.make_point();

        point_locations->put(point, vec3((float)position.x, (float)position.y, (float)position.z));
        point_normals  ->put(point, vec3((float)normal.x, (float)normal.y, (float)normal.z));
        point_texcoords->put(point, vec2((float)s, (float)t));

        return point;
    }

    Corner *make_corner(Polygon *polygon, int slice, int stack)
    {
        return make_corner(polygon, slice, stack, false);
    }

    Corner *make_corner(Polygon *polygon, int slice, int stack, bool cap)
    {
        double rel_slice = (double)slice / (double)slice_count;
        double rel_stack = (double)stack / (double)(stack_division + 1);
        // centroid: double relSlice = ((double)slice + 0.5) / (double)sliceCount;
        //double relStack = -1.0 + (0.5 / (double)(stackDivision + 1));

        bool is_slice_seam       = (slice == 0) || (slice == slice_count);
        bool is_bottom           = stack == -stack_division - 1;
        bool is_top              = stack == stack_division + 1;
        bool is_uv_discontinuity = is_slice_seam || is_bottom || is_top;

        Point *point;

        if (is_top && (top_radius == 0.0))
        {
            point = top;
        }
        else if (is_bottom && (bottom_radius == 0.0))
        {
            point = bottom;
        }
        else if (slice == slice_count)
        {
            point = points[std::make_pair(0, stack)];
        }
        else
        {
            point = points[std::make_pair(slice, stack)];
        }

        auto corner = polygon->make_corner(point);

        if (is_uv_discontinuity)
        {
            float s = (float)(rel_slice);
            float t = (float)(0.5 + 0.5 * rel_stack);

            corner_texcoords->put(corner, vec2(s, t));
        }

        if (is_top || is_bottom)
        {
            if (cap && is_bottom && (bottom_radius != 0.0) && use_bottom)
            {
                corner_normals->put(corner, vec3(-1.0f, 0.0f, 0.0f));
            }

            if (cap && is_top && (top_radius != 0.0) && use_top)
            {
                corner_normals->put(corner, vec3(1.0f, 0.0f, 0.0f));
            }
        }
        return corner;
    }

    Conical_frustum_builder(Geometry &geometry,
                            double   min_x,
                            double   max_x,
                            double   bottom_radius,
                            double   top_radius,
                            bool     use_bottom,
                            bool     use_top,
                            int      slice_count,
                            int      stack_division)
        : geometry(geometry)
        , min_x(min_x)
        , max_x(max_x)
        , bottom_radius(bottom_radius)
        , top_radius(top_radius)
        , use_bottom(use_bottom)
        , use_top(use_top)
        , slice_count(slice_count)
        , stack_division(stack_division)
        , bottom_not_singular((bottom_radius != 0.0) ? 1 : 0)
        , top_not_singular((top_radius != 0.0) ? 1 : 0)
        , stack_count((2 * stack_division) + 1 + bottom_not_singular + top_not_singular)
        , rel_slice(0.0)
        , rel_stack(0.0)
        , top(nullptr)
        , bottom(nullptr)
    {
    }

    void build()
    {
        point_locations   = geometry.point_attributes  ().find_or_create<vec3>("point_locations");
        point_normals     = geometry.point_attributes  ().find_or_create<vec3>("point_normals");
        point_texcoords   = geometry.point_attributes  ().find_or_create<vec2>("point_texcoords");
        polygon_centroids = geometry.polygon_attributes().find_or_create<vec3>("polygon_centroids");
        polygon_normals   = geometry.polygon_attributes().find_or_create<vec3>("polygon_normals");
        corner_normals    = geometry.corner_attributes ().find_or_create<vec3>("corner_normals");
        corner_texcoords  = geometry.corner_attributes ().find_or_create<vec2>("corner_texcoords");

        //  Points
        for (int slice = 0; slice <= slice_count; ++slice)
        {
            double rel_slice = (double)slice / (double)slice_count;

            for (int stack = -stack_division - bottom_not_singular;
                stack <= stack_division + top_not_singular;
                ++stack)
            {
                double rel_stack = (double)stack / (double)(stack_division + 1);

                points[std::make_pair(slice, stack)] = cone_point(rel_slice, rel_stack);
            }
        }

        //  Bottom parts
        if (bottom_radius == 0.0)
        {
            bottom = geometry.make_point(min_x, 0.0, 0.0); //  apex
            for (int slice = 0; slice < slice_count; ++slice)
            {
                int    stack              = -stack_division; //  second last stack, bottom is -stackDivision - 1
                double rel_slice_centroid = ((double)slice + 0.5) / (double)slice_count;
                double rel_stack_centroid = -1.0 + (0.5 / (double)(stack_division + 1));

                auto centroid = cone_point(rel_slice_centroid, rel_stack_centroid);
                auto polygon  = geometry.make_polygon();

                make_corner(polygon, slice + 1, stack);
                make_corner(polygon, slice, stack);
                auto tip = make_corner(polygon, slice, -stack_division - 1);

                vec3 n1             = point_normals->get(get_point(slice, stack));
                vec3 n2             = point_normals->get(get_point(slice + 1, stack));
                vec3 average_normal = normalize(n1 + n2);
                corner_normals->put(tip, average_normal);

                vec2 t1               = point_texcoords->get(get_point(slice, stack));
                vec2 t2               = point_texcoords->get(get_point(slice + 1, stack));
                vec2 average_texcoord = (t1 + t2) / 2.0f;
                corner_texcoords->put(tip, average_texcoord);

                polygon_centroids->put(polygon, point_locations->get(centroid));
                polygon_normals->put(polygon, point_normals->get(centroid));
            }
        }
        else
        {
            if (use_bottom)
            {
                auto polygon = geometry.make_polygon();
                polygon_centroids->put(polygon, vec3((float)min_x, 0.0f, 0.0f));
                polygon_normals->put(polygon, vec3(-1.0f, 0.0f, 0.0f));

                for (int slice = 0; slice < slice_count; ++slice)
                {
                    int reverse_slice = slice_count - 1 - slice;
                    make_corner(polygon, reverse_slice, -stack_division - 1, true);
                }
            }
        }

        //  Middle quads, bottom up
        for (int stack = -stack_division - bottom_not_singular;
            stack < stack_division + top_not_singular;
            ++stack)
        {
            double rel_stack_centroid = ((double)stack + 0.5) / (double)(stack_division + 1);

            for (int slice = 0; slice < slice_count; ++slice)
            {
                double rel_slice_centroid = ((double)(slice) + 0.5) / (double)(slice_count);

                auto centroid = cone_point(rel_slice_centroid, rel_stack_centroid);
                auto polygon = geometry.make_polygon();
                make_corner(polygon, slice + 1, stack + 1);
                make_corner(polygon, slice, stack + 1);
                make_corner(polygon, slice, stack);
                make_corner(polygon, slice + 1, stack);

                polygon_centroids->put(polygon, point_locations->get(centroid));
                polygon_normals->put(polygon, point_normals->get(centroid));
            }
        }

        //  Top parts
        if (top_radius == 0.0)
        {
            top = geometry.make_point(max_x, 0.0, 0.0); //  apex

            for (int slice = 0; slice < slice_count; ++slice)
            {
                int    stack              = stack_division;
                double rel_slice_centroid = ((double)(slice) + 0.5) / (double)(slice_count);
                double rel_stack_centroid = 1.0 - (0.5 / (double)(stack_division + 1));

                auto centroid = cone_point(rel_slice_centroid, rel_stack_centroid);
                auto polygon  = geometry.make_polygon();
                auto tip      = make_corner(polygon, slice, stack_division + 1);
                make_corner(polygon, slice, stack);
                make_corner(polygon, slice + 1, stack);

                vec3 n1             = point_normals->get(get_point(slice, stack));
                vec3 n2             = point_normals->get(get_point(slice + 1, stack));
                vec3 average_normal = normalize(n1 + n2);
                corner_normals->put(tip, average_normal);

                vec2 t1               = point_texcoords->get(get_point(slice, stack));
                vec2 t2               = point_texcoords->get(get_point(slice + 1, stack));
                vec2 average_texcoord = (t1 + t2) / 2.0f;
                corner_texcoords->put(tip, average_texcoord);

                polygon_centroids->put(polygon, point_locations->get(centroid));
                polygon_normals->put(polygon, point_normals->get(centroid));
            }
        }
        else
        {
            if (use_top)
            {
                auto polygon = geometry.make_polygon();
                polygon_centroids->put(polygon, vec3((float)max_x, 0.0f, 0.0f));
                polygon_normals->put(polygon, vec3(1.0f, 0.0f, 0.0f));

                for (int slice = 0; slice < slice_count; ++slice)
                {
                    make_corner(polygon, slice, stack_division + 1, true);
                }
            }
        }
        geometry.build_edges();
        geometry.optimize_attribute_maps();
    }
};

Geometry make_conical_frustum(double   min_x,
                              double   max_x,
                              double   bottom_radius,
                              double   top_radius,
                              bool     use_bottom,
                              bool     use_top,
                              int      slice_count,
                              int      stack_division)
{
    Geometry geometry("conical frustum");
    Conical_frustum_builder builder(geometry, 
                                    min_x,
                                    max_x,
                                    bottom_radius,
                                    top_radius,
                                    use_bottom,
                                    use_top,
                                    slice_count,
                                    stack_division);
    builder.build();
    return geometry;
}

Geometry make_cone(double min_x,
                   double max_x,
                   double bottom_radius,
                   bool   use_bottom,
                   int    slice_count,
                   int    stack_division)
{
    Geometry geometry("cone");
    Conical_frustum_builder builder(geometry,
                                    min_x,
                                    max_x,
                                    bottom_radius,
                                    0.0,
                                    use_bottom,
                                    false,
                                    slice_count,
                                    stack_division);
    builder.build();
    return geometry;
}

Geometry make_cylinder(double min_x,
                       double max_x,
                       double radius,
                       bool   use_bottom,
                       bool   use_top,
                       int    slice_count,
                       int    stack_division)
{
    Geometry geometry("cylinder");
    Conical_frustum_builder builder(geometry,
                                    min_x,
                                    max_x,
                                    radius,
                                    radius,
                                    use_bottom,
                                    use_top,
                                    slice_count,
                                    stack_division);
    builder.build();
    return geometry;
}

} // namespace shapes
} // namespace geometry
} // namespace renderstack
