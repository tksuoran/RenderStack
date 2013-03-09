#ifndef Renderstack_Geometry_point_hpp
#define Renderstack_Geometry_point_hpp

#include "renderstack_toolkit/platform.hpp"
#include <vector>

namespace renderstack { namespace geometry {

class corner;
class geometry;

class point 
{
public:
   typedef std::vector<corner*> corner_collection;

   corner_collection       &corners()       { return m_corners; }
   corner_collection const &corners() const { return m_corners; }

private:
   friend class geometry;

private:
   friend class polygon;

   void add_corner(corner *corner)
   {
      m_corners.push_back(corner);
   };

private:
   corner_collection  m_corners;
};

} }

#endif 
