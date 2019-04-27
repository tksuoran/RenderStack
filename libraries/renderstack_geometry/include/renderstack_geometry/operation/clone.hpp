#ifndef clone_hpp_renderstack_geometry_operation
#define clone_hpp_renderstack_geometry_operation

#include "renderstack_geometry/operation/geometry_operation.hpp"

namespace renderstack
{
namespace geometry
{
namespace operation
{

class Clone : public Geometry_operation
{
public:
    Clone(Geometry &src/*, HashSet<uint> selectedPolygonIndices*/ );
};

} // namespace operation
} // namespace geometry
} // namespace renderstack

#endif
