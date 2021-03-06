#include "renderstack_geometry/operation/catmull_clark.hpp"
#include "renderstack_geometry/corner.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace operation
{

using namespace std;

#if 0
static int s_last_id = 1;
static map<point *, int> s_point_id;

static int point_id(point *p)
{
   auto fi = s_point_id.find(p);
   if (fi != s_point_id.end())
      return fi->second;

   s_point_id[p] = s_last_id++;
}
#endif

// Create a new point at the midpoint of source edge / average of source edge end points
Point *Catmull_clark::make_new_point_from_edge(Edge const &src_edge)
{
    assert(src_edge.a());
    assert(src_edge.b());
    Point *new_point = destination().make_point();
    assert(new_point);

    //printf("adding edge %d, %d\n", point_id(src_edge.a()), point_id(src_edge.b()));
    m_old_edge_to_new_edge_points[src_edge] = new_point;
    add_point_source(new_point, 1.0f, src_edge.a());
    add_point_source(new_point, 1.0f, src_edge.b());
    return new_point;
}

Corner *Catmull_clark::make_new_corner_from_edge_point(Polygon *new_polygon, Edge const &src_edge)
{
    //printf("looking for edge %d, %d\n", point_id(src_edge.a()), point_id(src_edge.b()));

    Point *edge_midpoint = m_old_edge_to_new_edge_points[src_edge];
    assert(edge_midpoint);

    Corner *new_corner = new_polygon->make_corner(edge_midpoint);
    assert(new_corner);

    distribute_corner_sources(new_corner, 1.0f, edge_midpoint);
    return new_corner;
}

//  E = average of two neighboring face points and original endpoints
//
//  Compute P'             F = average F of all n face points for faces touching P
//                         R = average R of all n edge midpoints for edges touching P
//       F + 2R + (n-3)P   P = old point location
//  P' = ----------------
//             n           -> F weight is     1/n
//                         -> R weight is     2/n
//       F   2R   (n-3)P   -> P weight is (n-3)/n
//  P' = - + -- + ------
//       n    n      n
//
//  For each corner in the old polygon, add one quad
//  (centroid, previous edge 'edge midpoint', corner, next edge 'edge midpoint')
Catmull_clark::Catmull_clark(Geometry &src) : Geometry_operation(src)
{
    //s_point_id.clear();

    //                        (n-3)P
    //  Make initial P's with ------
    //                           n
    for (auto src_point : src.points())
    {
        assert(src_point);

        float n      = (float)src_point->corners().size();
        float weight = (n - 3.0f) / n;
        //printf("point %d (n = %d)\n", point_id(src_point), src_point->corners().size());
        make_new_point_from_point(weight, src_point);
    }

    //  Make new edge midpoints
    //  "average of two neighboring face points and original endpoints"
    //  Add midpoint (R) to each new end points
    //    R = average R of all n edge midpoints for edges touching P
    //   2R  we add both edge end points with weight 1 so total edge weight is 2
    //   --
    //    n
    for (auto i : src.edges())
    {
        const auto &src_edge = i.first;
        assert(src_edge.a());
        assert(src_edge.b());
        auto &src_edge_polygons = i.second;
        Point *new_point = make_new_point_from_edge(src_edge); //  these get weights 1 + 1
        assert(new_point);

        for (auto src_polygon : src_edge_polygons)
        {
            float weight = 1.0f / (float)src_polygon->corners().size();
            add_polygon_centroid(new_point, weight, src_polygon);
        }
        Point *new_point_a = m_point_old_to_new[src_edge.a()];
        Point *new_point_b = m_point_old_to_new[src_edge.b()];
        assert(new_point_a);
        assert(new_point_b);

        float n_a      = (float)(src_edge.a()->corners().size());
        float n_b      = (float)(src_edge.b()->corners().size());
        float weight_a = 1.0f / n_a;
        float weight_b = 1.0f / n_b;
        add_point_source(new_point_a, weight_a, src_edge.a());
        add_point_source(new_point_a, weight_a, src_edge.b());
        add_point_source(new_point_b, weight_b, src_edge.a());
        add_point_source(new_point_b, weight_b, src_edge.b());
    }

    for (auto src_polygon : src.polygons())
    {
        assert(src_polygon);

        //  Make centroid point
        make_new_point_from_polygon_centroid(src_polygon);

        //  Add polygon centroids (F) to all corners' point sources
        //  F = average F of all n face points for faces touching P
        //   F    <- because F is average of all centroids, it adds extra /n
        //  ---
        //   n
        for (auto src_corner : src_polygon->corners())
        {
            Point *src_point = src_corner->point;
            assert(src_point);

            Point *new_point = m_point_old_to_new[src_point];
            assert(new_point);

            float point_weight  = 1.0f / (float)(src_point->corners().size());
            float corner_weight = 1.0f / (float)(src_polygon->corners().size());
            add_polygon_centroid(new_point, point_weight * point_weight * corner_weight, src_polygon);
        }
    }

    //  Subdivide polygons, clone (and corners);
    for (size_t polygon_index = 0; polygon_index < src.polygons().size(); ++polygon_index)
    {
        Polygon *src_polygon = src.polygons()[polygon_index];
        assert(src_polygon);

        for (size_t i = 0; i < src_polygon->corners().size(); ++i) // Corner oldCorner in oldPolygon.Corners)
        {
            Corner *src_corner = src_polygon->corners()[i];
            assert(src_corner);

            Corner *previous_corner = src_polygon->corners()[(src_polygon->corners().size() + i - 1) % src_polygon->corners().size()];
            assert(previous_corner);

            Corner *next_corner = src_polygon->corners()[(i + 1) % src_polygon->corners().size()];
            assert(next_corner);

            Edge previous_edge(previous_corner->point, src_corner->point);
            Edge next_edge(src_corner->point, next_corner->point);

            Polygon *new_polygon = make_new_polygon_from_polygon(src_polygon);
            assert(new_polygon);

            make_new_corner_from_polygon_centroid(new_polygon, src_polygon);
            make_new_corner_from_edge_point(new_polygon, previous_edge);
            make_new_corner_from_corner(new_polygon, src_corner);
            make_new_corner_from_edge_point(new_polygon, next_edge);
        }
    }

    destination().build_edges();
    //  TODO can we map some edges to old edges? yes. Do it.
    interpolate_all_property_maps();
}

} // namespace operation
} // namespace geometry
} // namespace renderstack
