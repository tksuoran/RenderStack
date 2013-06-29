#ifndef corner_hpp_renderstack_geometry
#define corner_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include "renderstack_geometry/property_map.hpp"
#include "renderstack_geometry/polygon.hpp"
#include <memory>

namespace renderstack { namespace geometry {

class point;
class polygon;
class geometry;

class corner 
{
public:
   renderstack::geometry::point    *point  (){ return m_point;   }
   renderstack::geometry::polygon  *polygon(){ return m_polygon; }

   void smooth_normalize(
      std::shared_ptr<property_map<renderstack::geometry::corner*,   glm::vec3> >    corner_attribute,
      std::shared_ptr<property_map<renderstack::geometry::polygon*,  glm::vec3> >    polygon_attribute,
      std::shared_ptr<property_map<renderstack::geometry::polygon*,  glm::vec3> >    polygon_normals,
      float                                                                           cos_max_smoothing_angle
   );                                                                  
   void smooth_average(                                                
      std::shared_ptr<property_map<renderstack::geometry::corner*,   glm::vec4> >    new_corner_attribute,
      std::shared_ptr<property_map<renderstack::geometry::corner*,   glm::vec4> >    old_corner_attribute,
      std::shared_ptr<property_map<renderstack::geometry::corner*,   glm::vec3> >    corner_normals,
      std::shared_ptr<property_map<renderstack::geometry::point*,    glm::vec3> >    point_normals
   );

private:
   friend class polygon;

   corner(renderstack::geometry::point *point, renderstack::geometry::polygon *polygon)
   :   m_point  (point)
   ,   m_polygon(polygon)
   {
   }

private:
   renderstack::geometry::point    *m_point;
   renderstack::geometry::polygon  *m_polygon;
};

} }

#endif 
