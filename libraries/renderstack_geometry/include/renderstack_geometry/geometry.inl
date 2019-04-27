#ifndef geometry_inl_renderstack_geometry
#define geometry_inl_renderstack_geometry

namespace renderstack
{
namespace geometry
{

//template <typename T>
//void Geometry::smooth_normalize(const string &corner_attribute,
//                                const string &polygon_attribute,
//                                float         max_smoothing_angle_radians)
//{
//    auto corner_attributes2  = corner_attributes().find_or_create<vec3>(corner_attribute /*"corner_normals"*/);
//    auto polygon_attributes2 = polygon_attributes().maybe_find<vec3>(polygon_attribute /*"polygon_normals"*/);
//    auto polygon_normals     = polygon_attributes().maybe_find<vec3>("polygon_normals");
//
//    if (polygon_attributes2 == nullptr || polygon_normals == nullptr)
//    {
//        return;
//    }
//}

template <typename T>
void Geometry::smooth_normalize(Property_map<Corner *, T>                &corner_attribute,
                                const Property_map<Polygon *, T>         &polygon_attribute,
                                const Property_map<Polygon *, glm::vec3> &polygon_normals,
                                float                                    max_smoothing_angle_radians) const
{
    float cos_max_smoothing_angle = cos(max_smoothing_angle_radians);

    corner_attribute.clear();
    for (auto pol : polygons())
    {
        if (max_smoothing_angle_radians == 0.0f)
        {
            pol->copy_to_corners(corner_attribute, polygon_attribute);
        }
        else
        {
            pol->smooth_normalize(corner_attribute,
                                  polygon_attribute,
                                  polygon_normals,
                                  cos_max_smoothing_angle);
        }
    }
}


template <typename T>
void Geometry::smooth_average(Property_map<Corner *, T>                &smoothed_corner_attribute,
                              const Property_map<Corner *, T>          &corner_attribute,
                              const Property_map<Corner *, glm::vec3>  &corner_normals,
                              const Property_map<Polygon *, glm::vec3> &point_normals) const
{
    // auto corner_attributes2 = corner_attributes().find_or_create<vec4>(corner_attribute);
    // auto corner_normals     = corner_attributes().find_or_create<vec3>("corner_normals");
    // auto point_normals      = point_attributes().maybe_find<vec3>(point_normal_name);
    // 
    // if (point_normals == nullptr)
    // {
    //     return; // TODO?
    // }
    // auto new_corner_attributes = corner_attributes().find_or_create<vec4>("temp");
    for (auto polygon : polygons())
    {
        polygon->smooth_average(smoothed_corner_attribute,
                                corner_attribute,
                                corner_normals,
                                point_normals);
    }
    //corner_attributes().replace<vec4>(corner_attribute, "temp");
}

} // namespace geometry
} // namespace renderstack

#endif
