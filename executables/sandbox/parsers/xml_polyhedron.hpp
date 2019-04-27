#ifndef netlib_polyhedron_hpp_parsers
#define netlib_polyhedron_hpp_parsers

#include "renderstack_geometry/geometry.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

renderstack::geometry::Geometry make_xml_polyhedron(const std::string &path);

#endif
