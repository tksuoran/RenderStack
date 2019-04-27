#include "parsers/xml_polyhedron.hpp"
#include "renderstack_toolkit/file.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "tinyxml2.h"
#include <algorithm>
#include <cctype>
#include <glm/glm.hpp>
#include <sstream>
#include <string>

using namespace tinyxml2;
using namespace glm;
using namespace renderstack::geometry;

Geometry make_xml_polyhedron(const std::string &path)
{
    Geometry geometry;

    std::string text = renderstack::toolkit::read(path);

    XMLDocument doc;
    doc.Parse(text.c_str());

    auto xml_node = doc.FirstChild();
    if (xml_node == nullptr)
    {
        return geometry;
    }

    auto m = xml_node->NextSibling();
    if (m == nullptr)
    {
        return geometry;
    }

    auto e = m->FirstChildElement();
    while (e != nullptr)
    {
        const char *element_type = e->Value();
        bool        done         = false;
        bool        p            = !done && !strcmp("p", element_type);
        bool        f            = !done && !strcmp("f", element_type);

        if (p)
        {
            float x = e->FloatAttribute("x");
            float y = e->FloatAttribute("y");
            float z = e->FloatAttribute("z");

            geometry.make_point(-x, -y, -z);
        }

        if (f)
        {
            auto polygon = geometry.make_polygon();

            auto fe = e->FirstChildElement();
            while (fe != nullptr)
            {
                const char *fe_type = fe->Value();
                bool        v       = !strcmp("v", fe_type);
                if (v)
                {
                    int  index;
                    auto error = fe->QueryIntText(&index);
                    if (error == XML_SUCCESS)
                    {
                        if (index < (int)geometry.points().size())
                        {
                            polygon->make_corner(geometry.points()[index]);
                        }
                    }
                }
                fe = fe->NextSiblingElement();
            }
        }

        e = e->NextSiblingElement();
    }
    return geometry;
}
