#ifndef netlib_polyhedron_hpp_parsers
#define netlib_polyhedron_hpp_parsers

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/geometry.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <unordered_set>

class xml_polyhedron : public renderstack::geometry::geometry
{
public:
   xml_polyhedron::xml_polyhedron(std::string const &path);
};

#endif
