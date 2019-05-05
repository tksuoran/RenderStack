#ifndef netlib_polyhedron_hpp_parsers
#define netlib_polyhedron_hpp_parsers

#include "renderstack_geometry/geometry.hpp"
#include <string>

renderstack::geometry::Geometry make_json_polyhedron(const std::string &path);

#endif
