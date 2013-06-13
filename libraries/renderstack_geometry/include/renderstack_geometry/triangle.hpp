#ifndef triangle_hpp_renderstack_geometry
#define triangle_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/geometry.hpp"

namespace renderstack { namespace geometry { namespace shapes {

class triangle : public geometry
{
public:
   triangle(double radius);
};

class quad : public geometry
{
public:
   quad(double radius);
};

class cuboctahedron : public geometry
{
public:
   cuboctahedron(double radius);
};

class dodecahedron : public geometry
{
public:
   dodecahedron(double radius);
};

class icosahedron : public geometry
{
public:
   icosahedron(double radius);
};

class octahedron : public geometry
{
public:
   octahedron(double radius);
};

class tetrahedron : public geometry
{
public:
   tetrahedron(double radius);
};

} } }


#endif


