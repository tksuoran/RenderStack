#include "renderstack_geometry/operation/clone.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace operation
{

using namespace std;

clone::clone(
    shared_ptr<geometry> src
    /*, HashSet<uint> selectedPolygonIndices*/
)
{
    set_source(src);

    for (auto i = src->points().cbegin(); i != src->points().cend(); ++i)
        make_new_point_from_point(*i);

    for (size_t polygon_index = 0; polygon_index < src->polygons().size(); ++polygon_index)
    {
        /*if(
         (selectedPolygonIndices == null) || 
         selectedPolygonIndices.Contains(polygonIndex)
      ) */
        {
            polygon *old_polygon = src->polygons()[polygon_index];
            polygon *new_polygon = make_new_polygon_from_polygon(old_polygon);
            add_polygon_corners(new_polygon, old_polygon);
        }
    }

    build_destination_edges_with_sourcing();
    interpolate_all_property_maps();
}

} // namespace operation
} // namespace geometry
} // namespace renderstack
