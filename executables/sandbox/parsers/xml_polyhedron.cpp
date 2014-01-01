#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/file.hpp"
#include "parsers/xml_polyhedron.hpp"
#include "tinyxml2.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <cctype>

using namespace tinyxml2;
using namespace glm;
using namespace std;

xml_polyhedron::xml_polyhedron(string const &path)
{
   string text = renderstack::toolkit::read(path);

	XMLDocument doc;
   doc.Parse(text.c_str());

   XMLNode *xml_node = doc.FirstChild();
   if (xml_node == nullptr)
      return;

   XMLNode *m = xml_node->NextSibling();
   if (m == nullptr)
      return;

   XMLElement *e = m->FirstChildElement();
   while (e != nullptr)
   {
      const char *element_type = e->Value();
      bool done = false;
      bool p = !done && !strcmp("p", element_type);
      bool f = !done && !strcmp("f", element_type);

      if (p)
      {
         float x = e->FloatAttribute("x");
         float y = e->FloatAttribute("y");
         float z = e->FloatAttribute("z");

         make_point(-x, -y, -z);
      }

      if (f)
      {
         auto polygon = make_polygon();

         XMLElement *fe = e->FirstChildElement();
         while (fe != nullptr)
         {
            const char *fe_type = fe->Value();
            bool v = !strcmp("v", fe_type);
            if (v)
            {
               int index;
               XMLError error = fe->QueryIntText(&index);
               if (error == XML_SUCCESS)
                  if (index < (int)points().size())
                     polygon->make_corner(points()[index]);
            }
            fe = fe->NextSiblingElement();
         }
      }

      e = e->NextSiblingElement();
   }
}

