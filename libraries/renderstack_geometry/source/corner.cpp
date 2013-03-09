#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/corner.hpp"
#include "renderstack_geometry/attribute_map.hpp"
#include "renderstack_geometry/point.hpp"
#include <cassert>

namespace renderstack { namespace geometry {

using namespace glm;

void corner::smooth_normalize(
   std::shared_ptr<attribute_map<renderstack::geometry::corner*,  vec3> >  corner_attribute,
   std::shared_ptr<attribute_map<renderstack::geometry::polygon*, vec3> >  polygon_attribute,
   std::shared_ptr<attribute_map<renderstack::geometry::polygon*, vec3> >  polygon_normals,
   float                                                                   cos_max_smoothing_angle
)
{
   if (polygon_normals->has(polygon()) == false)
      return;

   vec3 polygon_normal = polygon_normals->value(polygon());
   vec3 polygon_value = polygon_attribute->value(polygon());
   vec3 corner_value = polygon_value;

   int point_corners = 0;
   int participants = 0;
   for (auto i = point()->corners().begin(); i != point()->corners().end(); ++i) {
      auto point_corner = *i;
      ++point_corners;
      auto neighbor_polygon = point_corner->polygon();

      if (
         (polygon() != neighbor_polygon) &&
         (polygon_normals->has(neighbor_polygon) == true) &&
         (polygon_attribute->has(neighbor_polygon) == true) &&
         (neighbor_polygon->corners().size() > 2)
      )
      {
         vec3 neighbor_normal = polygon_normals->value(neighbor_polygon);
         //float pLen = polygonNormal.Length;
         //float nLen = neighborNormal.Length;
         float cos_angle = dot(polygon_normal, neighbor_normal);
         if (cos_angle > 1.0f)
            cos_angle = 1.0f;

         if (cos_angle < -1.0f)
            cos_angle = -1.0f;

         //  Smaller cosine means larger angle means less sharp
         //  Higher cosine means lesser angle means more sharp
         //  Cosine == 1 == maximum sharpness
         //  Cosine == -1 == minimum sharpness (flat)
         if (cos_angle <= cos_max_smoothing_angle) {
            corner_value += polygon_attribute->value(neighbor_polygon);
            ++participants;
         }
      }
   }

   corner_value = normalize(corner_value);
   corner_attribute->set_value(this, corner_value);
}

void corner::smooth_average(
   std::shared_ptr<attribute_map<corner*,      vec4> >   new_corner_attribute,
   std::shared_ptr<attribute_map<corner*,      vec4> >   old_corner_attribute,
   std::shared_ptr<attribute_map<corner*,      vec3> >   corner_normals,
   std::shared_ptr<attribute_map<class point*, vec3> >   point_normals
)
{
   bool has_corner_normal = corner_normals->has(this);
   if (
      (has_corner_normal == false) &&
      point_normals->has(point()) == false
   )
      return;

   vec3 corner_normal = has_corner_normal ? corner_normals->value(this) : point_normals->value(point());
   vec4 corner_value(0.0f, 0.0f, 0.0f, 0.0f);

   int point_corners = 0;
   int participants = 0;
   for (auto i = point()->corners().begin(); i != point()->corners().end(); ++i)
   {
      auto point_corner = *i;
      ++point_corners;

      if (
         !has_corner_normal ||
         (corner_normals->value(point_corner) == corner_normal)
      )
      {
         if (old_corner_attribute->has(point_corner)) {
            corner_value += old_corner_attribute->value(point_corner);
            ++participants;
         }
      }
   }

   corner_value = corner_value / (float)(participants);
   new_corner_attribute->set_value(this, corner_value);
}

} }

