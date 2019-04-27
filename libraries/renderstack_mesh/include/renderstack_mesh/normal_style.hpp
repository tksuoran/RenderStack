#ifndef normal_style_hpp_renderstack_mesh
#define normal_style_hpp_renderstack_mesh

namespace renderstack
{
namespace mesh
{

namespace Normal_style
{
enum value
{
    corner_normals,
    polygon_normals,
    point_normals
};
const char *desc(enum value a);
} // namespace Normal_style

} // namespace mesh
} // namespace renderstack

#endif
