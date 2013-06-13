#ifndef edge_hpp_renderstack_geometry
#define edge_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include <utility>

namespace renderstack { namespace geometry {

class point;

struct edge
{
public:
   edge(point *p0, point *p1)
   {
      if (p0 < p1)
      {
         m_pair.first  = p0;
         m_pair.second = p1;
      }
      else
      {
         m_pair.first  = p1;
         m_pair.second = p0;
      }
   }

   bool operator<(edge const &other) const
   {
      return m_pair < other.m_pair;
   }

   point *a() const
   {
      return m_pair.first;
   }

   point *b() const
   {
      return m_pair.second;
   }

   std::pair<point*, point*> m_pair;
};

} }

#endif
