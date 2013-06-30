#ifndef clone_hpp_renderstack_geometry_operation
#define clone_hpp_renderstack_geometry_operation

#include "renderstack_geometry/operation/geometry_operation.hpp"

namespace renderstack { namespace geometry { namespace operation {

class clone : public geometry_operation
{
public:
   clone(
      std::shared_ptr<geometry> src
      /*, HashSet<uint> selectedPolygonIndices*/
   );
};

} } }

#endif 
