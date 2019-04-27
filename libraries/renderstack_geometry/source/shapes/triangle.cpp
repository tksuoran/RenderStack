#include "renderstack_geometry/shapes/triangle.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace geometry
{
namespace shapes
{

Geometry make_triangle(double r)
{
    // 0.57735027 = sqrt(3) / 3
    // 0.28867513 = sqrt(3) / 6
    Geometry geometry;

    geometry.make_point((float)(r * -0.28867513f), (float)(r * 0.5f), 0.0f, 0.0f, 1.0f);
    geometry.make_point((float)(r * 0.57735027f), (float)(r * 0.0f), 0.0f, 1.0f, 1.0f);
    geometry.make_point((float)(r * -0.28867513f), (float)(r * -0.5f), 0.0f, 1.0f, 0.0f);

    geometry.make_polygon( {0, 1, 2} );

    geometry.build_edges();
    geometry.optimize_attribute_maps();

    return geometry;
}

Geometry make_quad(double edge)
{
    //
    //  0.707106781 = sqrt(2) / 2
    // radius version:
    // make_point((float)(r * -0.707106781f), (float)(r * -0.707106781f), 0.0f, 0.0f, 0.0f);
    // make_point((float)(r *  0.707106781f), (float)(r * -0.707106781f), 0.0f, 1.0f, 0.0f);
    // make_point((float)(r *  0.707106781f), (float)(r *  0.707106781f), 0.0f, 1.0f, 1.0f);
    // make_point((float)(r * -0.707106781f), (float)(r *  0.707106781f), 0.0f, 0.0f, 1.0f);
    Geometry geometry;

    geometry.make_point((float)(edge * -0.5f), (float)(edge * -0.5f), 0.0f, 0.0f, 0.0f);
    geometry.make_point((float)(edge * 0.5f), (float)(edge * -0.5f), 0.0f, 1.0f, 0.0f);
    geometry.make_point((float)(edge * 0.5f), (float)(edge * 0.5f), 0.0f, 1.0f, 1.0f);
    geometry.make_point((float)(edge * -0.5f), (float)(edge * 0.5f), 0.0f, 0.0f, 1.0f);

    geometry.make_polygon( {0, 1, 2, 3} );

    // Double sided
    geometry.make_polygon( {3, 2, 1, 0} );

    geometry.build_edges();
    geometry.optimize_attribute_maps();

    return geometry;
}

Geometry make_cuboctahedron(double r)
{
    double sq2 = std::sqrt(2.0);

    Geometry geometry;

    geometry.make_point(0, r, 0);
    geometry.make_point(r / 2, r / 2, r * sq2 / 2);
    geometry.make_point(r / 2, r / 2, -r * sq2 / 2);
    geometry.make_point(r, 0, 0);
    geometry.make_point(r / 2, -r / 2, r * sq2 / 2);
    geometry.make_point(r / 2, -r / 2, -r * sq2 / 2);
    geometry.make_point(0, -r, 0);
    geometry.make_point(-r / 2, -r / 2, r * sq2 / 2);
    geometry.make_point(-r / 2, -r / 2, -r * sq2 / 2);
    geometry.make_point(-r, 0, 0);
    geometry.make_point(-r / 2, r / 2, r * sq2 / 2);
    geometry.make_point(-r / 2, r / 2, -r * sq2 / 2);

    geometry.make_polygon( {1, 4, 7, 10} );
    geometry.make_polygon( {4, 3, 5, 6} );
    geometry.make_polygon( {0, 2, 3, 1} );
    geometry.make_polygon( {11, 8, 5, 2} );
    geometry.make_polygon( {10, 9, 11, 0} );
    geometry.make_polygon( {7, 6, 8, 9} );
    geometry.make_polygon( {0, 1, 10} );
    geometry.make_polygon( {3, 4, 1} );
    geometry.make_polygon( {4, 6, 7} );
    geometry.make_polygon( {10, 7, 9} );
    geometry.make_polygon( {11, 2, 0} );
    geometry.make_polygon( {2, 5, 3} );
    geometry.make_polygon( {8, 6, 5} );
    geometry.make_polygon( {9, 8, 11} );

    geometry.build_edges();
    geometry.optimize_attribute_maps();

    return geometry;
}

Geometry make_dodecahedron(double r)
{
    double sq3 = std::sqrt(3.0);
    double sq5 = std::sqrt(5.0);
    double a   = 2.0 / (sq3 + sq3 * sq5);
    double b   = 1.0 / (3.0 * a);

    Geometry geometry;

    geometry.make_point(r / sq3, r / sq3, r / sq3);
    geometry.make_point(r / sq3, r / sq3, -r / sq3);
    geometry.make_point(r / sq3, -r / sq3, r / sq3);
    geometry.make_point(r / sq3, -r / sq3, -r / sq3);
    geometry.make_point(-r / sq3, r / sq3, r / sq3);
    geometry.make_point(-r / sq3, r / sq3, -r / sq3);
    geometry.make_point(-r / sq3, -r / sq3, r / sq3);
    geometry.make_point(-r / sq3, -r / sq3, -r / sq3);
    geometry.make_point(0, r * a, r * b);
    geometry.make_point(0, r * a, -r * b);
    geometry.make_point(0, -r * a, r * b);
    geometry.make_point(0, -r * a, -r * b);
    geometry.make_point(r * a, r * b, 0);
    geometry.make_point(r * a, -r * b, 0);
    geometry.make_point(-r * a, r * b, 0);
    geometry.make_point(-r * a, -r * b, 0);
    geometry.make_point(r * b, 0, r * a);
    geometry.make_point(r * b, 0, -r * a);
    geometry.make_point(-r * b, 0, r * a);
    geometry.make_point(-r * b, 0, -r * a);

    geometry.make_polygon( {6, 18, 4, 8, 10} );
    geometry.make_polygon( {10, 8, 0, 16, 2} );
    geometry.make_polygon( {3, 17, 1, 9, 11} );
    geometry.make_polygon( {5, 19, 7, 11, 9} );
    geometry.make_polygon( {3, 13, 2, 16, 17} );
    geometry.make_polygon( {17, 16, 0, 12, 1} );
    geometry.make_polygon( {19, 18, 6, 15, 7} );
    geometry.make_polygon( {5, 14, 4, 18, 19} );
    geometry.make_polygon( {0, 8, 4, 14, 12} );
    geometry.make_polygon( {12, 14, 5, 9, 1} );
    geometry.make_polygon( {13, 15, 6, 10, 2} );
    geometry.make_polygon( {3, 11, 7, 15, 13} );

    geometry.build_edges();
    geometry.optimize_attribute_maps();

    return geometry;
}

Geometry make_icosahedron(double r)
{
    double sq5 = std::sqrt(5.0);
    double a   = 2.0 / (1.0 + sq5);
    double b   = std::sqrt((3.0 + sq5) / (1.0 + sq5));
    a /= b;

    Geometry geometry;

    geometry.make_point(0, r * a, r / b);
    geometry.make_point(0, r * a, -r / b);
    geometry.make_point(0, -r * a, r / b);
    geometry.make_point(0, -r * a, -r / b);
    geometry.make_point(r * a, r / b, 0);
    geometry.make_point(r * a, -r / b, 0);
    geometry.make_point(-r * a, r / b, 0);
    geometry.make_point(-r * a, -r / b, 0);
    geometry.make_point(r / b, 0, r * a);
    geometry.make_point(r / b, 0, -r * a);
    geometry.make_point(-r / b, 0, r * a);
    geometry.make_point(-r / b, 0, -r * a);

    geometry.make_polygon( {1, 4, 6} );
    geometry.make_polygon( {0, 6, 4} );
    geometry.make_polygon( {0, 2, 10} );
    geometry.make_polygon( {0, 8, 2} );
    geometry.make_polygon( {1, 3, 9} );
    geometry.make_polygon( {1, 11, 3} );
    geometry.make_polygon( {2, 5, 7} );
    geometry.make_polygon( {3, 7, 5} );
    geometry.make_polygon( {6, 10, 11} );
    geometry.make_polygon( {7, 11, 10} );
    geometry.make_polygon( {4, 9, 8} );
    geometry.make_polygon( {5, 8, 9} );
    geometry.make_polygon( {0, 10, 6} );
    geometry.make_polygon( {0, 4, 8} );
    geometry.make_polygon( {1, 6, 11} );
    geometry.make_polygon( {1, 9, 4} );
    geometry.make_polygon( {3, 11, 7} );
    geometry.make_polygon( {3, 5, 9} );
    geometry.make_polygon( {2, 7, 10} );
    geometry.make_polygon( {2, 8, 5} );

    geometry.build_edges();
    geometry.optimize_attribute_maps();

    return geometry;
}

Geometry make_octahedron(double r)
{
    Geometry geometry;

    geometry.make_point(0, r, 0);
    geometry.make_point(0, -r, 0);
    geometry.make_point(-r, 0, 0);
    geometry.make_point(0, 0, -r);
    geometry.make_point(r, 0, 0);
    geometry.make_point(0, 0, r);

    geometry.make_polygon( {0, 2, 3} );
    geometry.make_polygon( {0, 3, 4} );
    geometry.make_polygon( {0, 4, 5} );
    geometry.make_polygon( {0, 5, 2} );
    geometry.make_polygon( {3, 2, 1} );
    geometry.make_polygon( {4, 3, 1} );
    geometry.make_polygon( {5, 4, 1} );
    geometry.make_polygon( {2, 5, 1} );

    geometry.build_edges();
    geometry.optimize_attribute_maps();

    return geometry;
}

Geometry make_tetrahedron(double r)
{
    double sq2 = std::sqrt(2.0);
    double sq3 = std::sqrt(3.0);

    Geometry geometry;

    geometry.make_point(0, r, 0);
    geometry.make_point(0, -r / 3.0, r * 2.0 * sq2 / 3.0);
    geometry.make_point(-r * sq3 * sq2 / 3.0, -r / 3.0, -r * sq2 / 3.0);
    geometry.make_point(r * sq3 * sq2 / 3.0, -r / 3.0, -r * sq2 / 3.0);

    geometry.make_polygon( {0, 1, 2} );
    geometry.make_polygon( {3, 1, 0} );
    geometry.make_polygon( {0, 2, 3} );
    geometry.make_polygon( {3, 2, 1} );

    geometry.build_edges();
    geometry.optimize_attribute_maps();

    return geometry;
}

} // namespace shapes
} // namespace geometry
} // namespace renderstack
