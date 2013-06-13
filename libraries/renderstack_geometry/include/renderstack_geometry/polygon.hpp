#ifndef polygon_hpp_renderstack_geometry
#define polygon_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include "renderstack_geometry/attribute_map.hpp"
#include <vector>
#include <memory>
#include <algorithm>

namespace renderstack { namespace geometry {

class geometry;
class corner;
class point;

class polygon 
{
public:
   typedef std::vector<renderstack::geometry::corner*> corner_collection;

   polygon(){}
   ~polygon();

public:
   template<typename T>
   void copy_to_corners(
      std::shared_ptr<attribute_map<renderstack::geometry::corner*,  T> >    corner_attribute,
      std::shared_ptr<attribute_map<renderstack::geometry::polygon*, T> >    polygon_attribute
   );
   void smooth_normalize(
      std::shared_ptr<attribute_map<renderstack::geometry::corner*,   glm::vec3> >    corner_attribute,
      std::shared_ptr<attribute_map<renderstack::geometry::polygon*,  glm::vec3> >    polygon_attribute,
      std::shared_ptr<attribute_map<renderstack::geometry::polygon*,  glm::vec3> >    polygon_normals,
      float                                                                           cos_max_smoothing_angle
   );
   void smooth_average(
      std::shared_ptr<attribute_map<renderstack::geometry::corner*,   glm::vec4> >    new_corner_attribute,
      std::shared_ptr<attribute_map<renderstack::geometry::corner*,   glm::vec4> >    old_corner_attribute,
      std::shared_ptr<attribute_map<renderstack::geometry::corner*,   glm::vec3> >    normer_normals,
      std::shared_ptr<attribute_map<renderstack::geometry::point*,    glm::vec3> >    point_normals
   );

   void compute_normal(
      std::shared_ptr<attribute_map<renderstack::geometry::polygon*, glm::vec3> >    polygon_normals,
      std::shared_ptr<attribute_map<renderstack::geometry::point*,   glm::vec3> >    point_locations
   );
   void compute_centroid(
      std::shared_ptr<attribute_map<renderstack::geometry::polygon*, glm::vec3> >    polygon_centroids,
      std::shared_ptr<attribute_map<renderstack::geometry::point*,   glm::vec3> >    point_locations
   );
   renderstack::geometry::corner *corner      (renderstack::geometry::point *pnt);
   renderstack::geometry::corner *make_corner (renderstack::geometry::point *pnt);

   corner_collection       &corners()       { return m_corners; }
   corner_collection const &corners() const { return m_corners; }

   void                    reverse()   { std::reverse(m_corners.begin(), m_corners.end()); }

private:
   corner_collection   m_corners;
};

} }

#include "polygon.inl"

#endif 
