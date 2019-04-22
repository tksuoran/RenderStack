#ifndef catmull_clark_hpp_renderstack_geometry_operation
#define catmull_clark_hpp_renderstack_geometry_operation

#include "renderstack_geometry/operation/geometry_operation.hpp"

namespace renderstack
{
namespace geometry
{
namespace operation
{

class catmull_clark : public geometry_operation
{
private:
    std::map<edge, point *> m_old_edge_to_new_edge_points;

public:
    point * make_new_point_from_edge(edge const &src_edge);
    corner *make_new_corner_from_edge_point(polygon *new_polygon, edge const &src_edge);

    catmull_clark(std::shared_ptr<geometry> src);
};

} // namespace operation
} // namespace geometry
} // namespace renderstack

#endif
