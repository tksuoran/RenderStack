#ifndef polygon_inl_renderstack_geometry
#define polygon_inl_renderstack_geometry

namespace renderstack
{
namespace geometry
{

template <typename T>
void Polygon::copy_to_corners(Property_map<Corner *, T> &corner_attribute,
                              const Property_map<Polygon *, T> &polygon_attribute)
{
    T polygon_value = polygon_attribute.get(this);
    for (auto corner : m_corners)
    {
        corner_attribute.put(corner, polygon_value);
    }
}

template <typename T>
void Polygon::smooth_normalize(Property_map<Corner *, T>                &corner_attribute,
                               const Property_map<Polygon *, T>         &polygon_attribute,
                               const Property_map<Polygon *, glm::vec3> &polygon_normals,
                               float                                     cos_max_smoothing_angle)
{
    for (auto corner : m_corners)
    {
        corner->smooth_normalize(corner_attribute,
                                 polygon_attribute,
                                 polygon_normals,
                                 cos_max_smoothing_angle);
    }
}

template <typename T>
void Polygon::smooth_average(Property_map<Corner *, T>               &new_corner_attribute,
                             const Property_map<Corner *, T>         &old_corner_attribute,
                             const Property_map<Corner *, glm::vec3> &normer_normals,
                             const Property_map<Point *, glm::vec3>  &point_normals)
{
    for (auto corner : m_corners)
    {
        corner->smooth_average(new_corner_attribute,
                               old_corner_attribute,
                               normer_normals,
                               point_normals);
    }
}

} // namespace geometry
} // namespace renderstack

#endif
