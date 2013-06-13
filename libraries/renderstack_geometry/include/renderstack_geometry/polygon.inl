#ifndef polygon_inl_renderstack_geometry
#define polygon_inl_renderstack_geometry

namespace renderstack { namespace geometry {

template<typename T>
void polygon::copy_to_corners(
   std::shared_ptr<attribute_map<renderstack::geometry::corner*,  T> >  corner_attribute,
   std::shared_ptr<attribute_map<renderstack::geometry::polygon*, T> >  polygon_attribute
)
{
   T polygon_value = polygon_attribute->value(this);
   for (auto i = m_corners.begin(); i != m_corners.end(); ++i)
   {
      class corner *cor = *i;
      corner_attribute->set_value(cor, polygon_value);
   }
}

} }

#endif
