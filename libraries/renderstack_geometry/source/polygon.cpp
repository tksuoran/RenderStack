#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/polygon.hpp"
#include "renderstack_geometry/point.hpp"
#include "renderstack_geometry/corner.hpp"
#include "renderstack_geometry/property_map.hpp"
#include <cassert>
#include <cmath>
#include <exception>

namespace renderstack { namespace geometry {

using namespace glm;

polygon::~polygon()
{
   for (auto i = m_corners.begin(); i != m_corners.end(); ++i)
   {
      class corner *cor = *i;
      delete cor;
      *i = 0;
   }
   m_corners.clear();
}

void polygon::smooth_normalize(
   std::shared_ptr<property_map<renderstack::geometry::corner*,  vec3> >  corner_attribute,
   std::shared_ptr<property_map<renderstack::geometry::polygon*, vec3> >  polygon_attribute,
   std::shared_ptr<property_map<renderstack::geometry::polygon*, vec3> >  polygon_normals,
   float                                                                   cos_max_smoothing_angle
)
{
   for (auto i = m_corners.begin(); i != m_corners.end(); ++i)
   {
      class corner *cor = *i;
      cor->smooth_normalize(
         corner_attribute,
         polygon_attribute,
         polygon_normals,
         cos_max_smoothing_angle
      );
   }
}

void polygon::smooth_average(
   std::shared_ptr<property_map<renderstack::geometry::corner*, vec4> >   new_corner_attribute,
   std::shared_ptr<property_map<renderstack::geometry::corner*, vec4> >   old_corner_attribute,
   std::shared_ptr<property_map<renderstack::geometry::corner*, vec3> >   normer_normals,
   std::shared_ptr<property_map<renderstack::geometry::point*,  vec3> >   point_normals   
)
{
   for (auto i = m_corners.begin(); i != m_corners.end(); ++i)
   {
      class corner *cor = *i;
      cor->smooth_average(
         new_corner_attribute,
         old_corner_attribute,
         normer_normals,
         point_normals
      );
   }
}

void polygon::compute_normal(
   std::shared_ptr<property_map<polygon*, vec3> > polygon_normals,
   std::shared_ptr<property_map<point*,   vec3> > point_locations
)
{
   if (m_corners.size() > 2)
   {
      corner_collection::iterator i = m_corners.begin();
      class corner *c0 = *i; ++i;           /*  first  point in polygon  */ 
      class corner *c1 = *i;                /*  second point in polygon  */ 
      class corner *c2 = m_corners.back();  /*  last   point in polygon  */ 
      point  *p0 = c0->point();
      point  *p1 = c1->point();
      point  *p2 = c2->point();

      /*  Make sure all points are unique from others  */ 
      if (
         (p0 != p1) &&
         (p0 != p2) &&
         (p1 != p2)
      )
      {
         vec3 pos0   = point_locations->get(p0);
         vec3 pos1   = point_locations->get(p1);
         vec3 pos2   = point_locations->get(p2);
         vec3 normal = cross((pos2 - pos0), (pos1 - pos0));
         normal = normalize(normal);
         polygon_normals->put(this, normal);
      }
      else
         throw std::runtime_error("polygons with duplicate points");

   }
}

void polygon::compute_centroid(
   std::shared_ptr<property_map<polygon*, vec3> > polygon_centroids,
   std::shared_ptr<property_map<point*,   vec3> > point_locations
)
{
   vec3 centroid(0.0f, 0.0f, 0.0f);
   int count = 0;

   for (auto i = m_corners.begin(); i != m_corners.end(); ++i)
   {
      class corner *cor  = *i;
      point *pnt  = cor->point();
      vec3  pos0  = point_locations->get(pnt);
      centroid += pos0;
      ++count;
   }
   if (count > 0)
   {
      centroid /= (float)(count);
      polygon_centroids->put(this, centroid);
   }
   else
   {
      polygon_centroids->put(this, centroid);
   }
}

renderstack::geometry::corner *polygon::corner(point *pnt)
{
   for (auto i = m_corners.begin(); i != m_corners.end(); ++i)
   {
      class corner *cor = *i;
      if (pnt == cor->point())
         return cor;
   }
   return 0;
}

corner *polygon::make_corner(point *pnt)
{
   class corner *cor = new class corner(pnt, this);
   pnt->add_corner(cor);
   m_corners.push_back(cor);
   return cor;
}


renderstack::geometry::corner *polygon::next_corner(renderstack::geometry::corner *c)
{
   for (size_t i = 0; i < m_corners.size(); ++i)
   {
      renderstack::geometry::corner *corner0 = m_corners[i];
      if (corner0 == c)
         return m_corners[(i + 1) % m_corners.size()];
   }
   return nullptr;
}

renderstack::geometry::corner *polygon::prev_corner(renderstack::geometry::corner *c)
{
   for (size_t i = 0; i < m_corners.size(); ++i)
   {
      renderstack::geometry::corner *corner0 = m_corners[i];
      if (corner0 == c)
         return m_corners[(m_corners.size() + i - 1) % m_corners.size()];
   }
   return nullptr;
}

} }

