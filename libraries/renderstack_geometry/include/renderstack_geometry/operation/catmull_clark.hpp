#ifndef catmull_clark_hpp_renderstack_geometry_operation
#define catmull_clark_hpp_renderstack_geometry_operation

#include "renderstack_geometry/operation/geometry_operation.hpp"

namespace renderstack
{
namespace geometry
{
namespace operation
{

class Catmull_clark : public Geometry_operation
{
public:
    Catmull_clark(Geometry &src);

    Point *make_new_point_from_edge(Edge const &src_edge);
    Corner *make_new_corner_from_edge_point(Polygon *new_polygon, Edge const &src_edge);

private:
    std::map<Edge, Point*> m_old_edge_to_new_edge_points;
};

} // namespace operation
} // namespace geometry
} // namespace renderstack

#endif
