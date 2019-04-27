#ifndef corner_inl_renderstack_geometry
#define corner_inl_renderstack_geometry

namespace renderstack
{
namespace geometry
{

template <typename T>
void Corner::smooth_normalize(Property_map<Corner *, T>  &corner_attribute,
                              const Property_map<Polygon *, T> &polygon_attribute,
                              const Property_map<Polygon *, glm::vec3> &polygon_normals,
                              float cos_max_smoothing_angle)
{
    if (polygon_normals.has(polygon) == false)
    {
        return;
    }

    glm::vec3 polygon_normal = polygon_normals.get(polygon);
    T polygon_value  = polygon_attribute.get(polygon);
    T corner_value   = polygon_value;

    size_t point_corner_count = 0U;
    size_t participant_count  = 0U;
    for (auto point_corner : point->corners())
    {
        ++point_corner_count;
        auto neighbor_polygon = point_corner->polygon;

        if ((polygon != neighbor_polygon) &&
            polygon_normals.has(neighbor_polygon) &&
            polygon_attribute.has(neighbor_polygon) &&
            (neighbor_polygon->corners().size() > 2))
        {
            glm::vec3 neighbor_normal = polygon_normals.get(neighbor_polygon);
            float cos_angle = glm::dot(polygon_normal, neighbor_normal);
            if (cos_angle > 1.0f)
            {
                cos_angle = 1.0f;
            }

            if (cos_angle < -1.0f)
            {
                cos_angle = -1.0f;
            }

            //  Smaller cosine means larger angle means less sharp
            //  Higher cosine means lesser angle means more sharp
            //  Cosine == 1 == maximum sharpness
            //  Cosine == -1 == minimum sharpness (flat)
            if (cos_angle <= cos_max_smoothing_angle)
            {
                corner_value += polygon_attribute.get(neighbor_polygon);
                ++participant_count;
            }
        }
    }

    corner_value = normalize(corner_value);
    corner_attribute.put(this, corner_value);
}

template <typename T>
void Corner::smooth_average(Property_map<Corner *, T>               &new_corner_attribute,
                            const Property_map<Corner *, T>         &old_corner_attribute,
                            const Property_map<Corner *, glm::vec3> &corner_normals,
                            const Property_map<Point *, glm::vec3>  &point_normals)
{
    bool has_corner_normal = corner_normals.has(this);
    if (!has_corner_normal && !point_normals.has(point))
    {
        return;
    }

    glm::vec3 corner_normal = has_corner_normal ? corner_normals.get(this) 
                                                : point_normals.get(point);
    T corner_value{};

    size_t point_corner_count = 0U;
    size_t participant_count  = 0U;
    for (auto point_corner : point->corners())
    {
        ++point_corner_count;

        if (!has_corner_normal ||
            (corner_normals.get(point_corner) == corner_normal))
        {
            if (old_corner_attribute.has(point_corner))
            {
                corner_value += old_corner_attribute.get(point_corner);
                ++participant_count;
            }
        }
    }

    corner_value = corner_value / static_cast<float>(participant_count);
    new_corner_attribute.put(this, corner_value);
}

} // namespace geometry
} // namespace renderstack

#endif
