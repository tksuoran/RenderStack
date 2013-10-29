#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/shapes/triangle.hpp"

namespace renderstack { namespace geometry { namespace shapes {

triangle::triangle(double r)
:  geometry("triangle")
{
   //  0.57735027 = sqrt(3) / 3
   //  0.28867513 = sqrt(3) / 6
   make_point((float)(r * -0.28867513f), (float)(r *  0.5f), 0.0f, 0.0f, 1.0f);
   make_point((float)(r *  0.57735027f), (float)(r *  0.0f), 0.0f, 1.0f, 1.0f);
   make_point((float)(r * -0.28867513f), (float)(r * -0.5f), 0.0f, 1.0f, 0.0f);

   make_polygon(0, 1, 2);
}

quad::quad(double edge)
:  geometry("quad")
{
   // 
   //  0.707106781 = sqrt(2) / 2
   // radius version:
   // make_point((float)(r * -0.707106781f), (float)(r * -0.707106781f), 0.0f, 0.0f, 0.0f);
   // make_point((float)(r *  0.707106781f), (float)(r * -0.707106781f), 0.0f, 1.0f, 0.0f);
   // make_point((float)(r *  0.707106781f), (float)(r *  0.707106781f), 0.0f, 1.0f, 1.0f);
   // make_point((float)(r * -0.707106781f), (float)(r *  0.707106781f), 0.0f, 0.0f, 1.0f);

   make_point((float)(edge * -0.5f), (float)(edge * -0.5f), 0.0f, 0.0f, 0.0f);
   make_point((float)(edge *  0.5f), (float)(edge * -0.5f), 0.0f, 1.0f, 0.0f);
   make_point((float)(edge *  0.5f), (float)(edge *  0.5f), 0.0f, 1.0f, 1.0f);
   make_point((float)(edge * -0.5f), (float)(edge *  0.5f), 0.0f, 0.0f, 1.0f);

   make_polygon(0, 1, 2, 3);
}

cuboctahedron::cuboctahedron(double r)
:  geometry("cuboctahedron")
{
   double sq2 = sqrtf(2.0);

   make_point(      0,      r,            0 );
   make_point(  r / 2,  r / 2,  r * sq2 / 2 );
   make_point(  r / 2,  r / 2, -r * sq2 / 2 );
   make_point(      r,      0,            0 );
   make_point(  r / 2, -r / 2,  r * sq2 / 2 );
   make_point(  r / 2, -r / 2, -r * sq2 / 2 );
   make_point(      0,     -r,            0 );
   make_point( -r / 2, -r / 2,  r * sq2 / 2 );
   make_point( -r / 2, -r / 2, -r * sq2 / 2 );
   make_point(     -r,      0,            0 );
   make_point( -r / 2,  r / 2,  r * sq2 / 2 );
   make_point( -r / 2,  r / 2, -r * sq2 / 2 );

   make_polygon(  1, 4,  7, 10  );
   make_polygon(  4, 3,  5,  6  );
   make_polygon(  0, 2,  3,  1  );
   make_polygon( 11, 8,  5,  2  );
   make_polygon( 10, 9, 11,  0  );
   make_polygon(  7, 6,  8,  9  );
   make_polygon(  0, 1, 10  );
   make_polygon(  3, 4,  1  );
   make_polygon(  4, 6,  7  );
   make_polygon( 10, 7,  9  );
   make_polygon( 11, 2,  0  );
   make_polygon(  2, 5,  3  );
   make_polygon(  8, 6,  5  );
   make_polygon(  9, 8, 11  );
}

dodecahedron::dodecahedron(double r)
:  geometry("dodecahedron")
{
   double sq3 = sqrt(3.0);
   double sq5 = sqrt(5.0);
   double a = 2.0 / (sq3 + sq3 * sq5);
   double b = 1.0 / (3.0 * a);

   make_point( r / sq3,  r / sq3,  r / sq3);
   make_point( r / sq3,  r / sq3, -r / sq3);
   make_point( r / sq3, -r / sq3,  r / sq3);
   make_point( r / sq3, -r / sq3, -r / sq3);
   make_point(-r / sq3,  r / sq3,  r / sq3);
   make_point(-r / sq3,  r / sq3, -r / sq3);
   make_point(-r / sq3, -r / sq3,  r / sq3);
   make_point(-r / sq3, -r / sq3, -r / sq3);
   make_point(       0,  r * a,  r * b);
   make_point(       0,  r * a, -r * b);
   make_point(       0, -r * a,  r * b);
   make_point(       0, -r * a, -r * b);
   make_point( r * a,  r * b,        0);
   make_point( r * a, -r * b,        0);
   make_point(-r * a,  r * b,        0);
   make_point(-r * a, -r * b,        0);
   make_point( r * b,        0,  r * a);
   make_point( r * b,        0, -r * a);
   make_point(-r * b,        0,  r * a);
   make_point(-r * b,        0, -r * a);

   make_polygon( 6, 18, 4,  8, 10);
   make_polygon(10,  8, 0, 16,  2);
   make_polygon( 3, 17, 1,  9, 11);
   make_polygon( 5, 19, 7, 11,  9);
   make_polygon( 3, 13, 2, 16, 17);
   make_polygon(17, 16, 0, 12,  1);
   make_polygon(19, 18, 6, 15,  7);
   make_polygon( 5, 14, 4, 18, 19);
   make_polygon( 0,  8, 4, 14, 12);
   make_polygon(12, 14, 5,  9,  1);
   make_polygon(13, 15, 6, 10,  2);
   make_polygon( 3, 11, 7, 15, 13);
}

icosahedron::icosahedron(double r)
:  geometry("icosahedron")
{
   double sq5 = sqrt(5.0);
   double a = 2.0 / (1.0 + sq5);
   double b = sqrt((3.0 + sq5) / (1.0 + sq5));
   a /= b;

   make_point(     0,  r * a,  r / b);
   make_point(     0,  r * a, -r / b);
   make_point(     0, -r * a,  r / b);
   make_point(     0, -r * a, -r / b);
   make_point( r * a,  r / b,      0);
   make_point( r * a, -r / b,      0);
   make_point(-r * a,  r / b,      0);
   make_point(-r * a, -r / b,      0);
   make_point( r / b,      0,  r * a);
   make_point( r / b,      0, -r * a);
   make_point(-r / b,      0,  r * a);
   make_point(-r / b,      0, -r * a);

   make_polygon(1,  4,  6);
   make_polygon(0,  6,  4);
   make_polygon(0,  2, 10);
   make_polygon(0,  8,  2);
   make_polygon(1,  3,  9);
   make_polygon(1, 11,  3);
   make_polygon(2,  5,  7);
   make_polygon(3,  7,  5);
   make_polygon(6, 10, 11);
   make_polygon(7, 11, 10);
   make_polygon(4,  9,  8);
   make_polygon(5,  8,  9);
   make_polygon(0, 10,  6);
   make_polygon(0,  4,  8);
   make_polygon(1,  6, 11);
   make_polygon(1,  9,  4);
   make_polygon(3, 11,  7);
   make_polygon(3,  5,  9);
   make_polygon(2,  7, 10);
   make_polygon(2,  8,  5);
}

octahedron::octahedron(double r)
:  geometry("octahedron")
{
   make_point( 0,  r,  0);
   make_point( 0, -r,  0);
   make_point(-r,  0,  0);
   make_point( 0,  0, -r);
   make_point( r,  0,  0);
   make_point( 0,  0,  r);

   make_polygon(0, 2, 3);
   make_polygon(0, 3, 4);
   make_polygon(0, 4, 5);
   make_polygon(0, 5, 2);
   make_polygon(3, 2, 1);
   make_polygon(4, 3, 1);
   make_polygon(5, 4, 1);
   make_polygon(2, 5, 1);
}

tetrahedron::tetrahedron(double r)
:  geometry("tetrahedron")
{
   double sq2 = sqrt(2.0);
   double sq3 = sqrt(3.0);

   make_point(                 0,          r,                  0   );
   make_point(                 0,   -r / 3.0,  r * 2.0 * sq2 / 3.0 );
   make_point(-r * sq3 * sq2 / 3.0, -r / 3.0,       -r * sq2 / 3.0 );
   make_point( r * sq3 * sq2 / 3.0, -r / 3.0,       -r * sq2 / 3.0 );

   make_polygon(0, 1, 2);
   make_polygon(3, 1, 0);
   make_polygon(0, 2, 3);
   make_polygon(3, 2, 1);
}


} } }
