#ifndef mesh_mode_hpp_renderstack_mesh
#define mesh_mode_hpp_renderstack_mesh

#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace mesh
{

class mesh_mode
{
public:
    enum value
    {
        not_set           = 0,
        polygon_fill      = 1,
        edge_lines        = 2,
        corner_points     = 3,
        corner_normals    = 4,
        polygon_centroids = 5,
        count             = 6
    };
    static const char *desc(enum value a);
};

} // namespace mesh
} // namespace renderstack

#endif
