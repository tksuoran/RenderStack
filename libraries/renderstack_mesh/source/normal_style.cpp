#include "renderstack_mesh/normal_style.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace mesh
{

const char *Normal_style::desc(enum value a)
{
    switch (a)
    {
        case Normal_style::corner_normals: return "corner_normals";
        case Normal_style::polygon_normals: return "polygon_normals";
        case Normal_style::point_normals: return "point_normals";
        default: return "???";
    }
}

} // namespace mesh
} // namespace renderstack
