#ifndef aabb_hpp
#define aabb_hpp

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>

#define OUTSIDE   0
#define INSIDE    1
#define INTERSECT 2


struct plane
{
   glm::dvec3  n;
   glm::dvec3  abs_n;
   double      d;
};

struct frustum
{
   plane p[6];
};

class aabb
{
public:
   glm::dvec3 m_center;
   glm::dvec3 m_extent;

public:
   int intersect(frustum const &f)
   {
      for (unsigned int j = 0; j < 6; ++j) {
         plane const &frustum_plane = f.p[j];

         double d = glm::dot(m_center, frustum_plane.n);
         double r = glm::dot(m_extent, frustum_plane.abs_n);

         double d_p_r = d + r + frustum_plane.d;
         if (d_p_r < 0.0) {
            return OUTSIDE;
         }

         double d_m_r = d - r + frustum_plane.d;
         if (d_m_r < 0.0) {
            return INTERSECT;
         }
      }

      return 1;
   }
};


#endif
