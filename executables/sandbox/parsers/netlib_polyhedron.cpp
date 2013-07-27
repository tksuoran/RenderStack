#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/file.hpp"
#include "parsers/netlib_polyhedron.hpp"
#include <glm/glm.hpp>
#include <algorithm>
#include <sstream>
#include <cctype>

using namespace std;
using namespace glm;

template <typename T>
T string_to_number(string const &text)
{
   stringstream ss(text);
   T result;
   return ss >> result ? result : 0;
}

bool replace(string &str, string const &from, string const &to)
{
   size_t start_pos = str.find(from);
   if (start_pos == string::npos)
      return false;

   str.replace(start_pos, from.length(), to);
   return true;
}

bool netlib_polyhedron::seek(string const &label)
{
   auto fi = m_text.find(label, 0);
   if (fi == string::npos)
      return false;

   m_pos = fi;
   m_pos += label.size() + 1;
   return true;
}

void netlib_polyhedron::eol()
{
   auto fi = m_text.find("\n", m_pos);
   if (fi == string::npos)
      return;

   m_pos = fi;
}

string netlib_polyhedron::parse_line()
{
   size_t start_pos = m_pos;
   eol();
   return m_text.substr(start_pos, m_pos - start_pos);
}

void netlib_polyhedron::skip_white_spaces()
{
   while (m_pos < m_text.size() && ::isspace(m_text[m_pos]))
      ++m_pos;
}

void netlib_polyhedron::seek_white_space()
{
   while (m_pos < m_text.size() && !::isspace(m_text[m_pos]))
      ++m_pos;
}

void netlib_polyhedron::seek_non_number()
{
   while (
      (m_pos < m_text.size()) &&
      (!::isspace(m_text[m_pos]) == false) &&
      (m_text[m_pos] != '[')
   )
      ++m_pos;
}

int netlib_polyhedron::parse_int()
{
   skip_white_spaces();
   size_t start_pos = m_pos;
   seek_non_number();
   string value_string = m_text.substr(start_pos, m_pos - start_pos);
   int value = string_to_number<int>(value_string);
   return value;
}

float netlib_polyhedron::parse_float()
{
   //  A value consists of a floating point number optionally
   //  followed by a expression enclosed by `[]'. The expression
   //  is the exact value represented in bc(1) code with the
   //  following function meanings: a(x) = tan-1(x), b(x) = (x)1/3,
   //  c(x) = cos(x), d(x) = tan(x), p = J, q(x) = x2,
   //  r(x) = cos-1(x), s(x) = sin(x), t = U. The code may include
   //  assignments but does not include white space.

   skip_white_spaces();
   size_t start_pos = m_pos;
   seek_non_number();
   string value_string = m_text.substr(start_pos, m_pos - start_pos);
   float value = string_to_number<float>(value_string);
   return value;
}

void netlib_polyhedron::parse_name()
{
   //  The polyhedron's name is less than 128 characters
   //  long and is not capitalized.
   if (seek(":name"))
      m_name = parse_line();
}

void netlib_polyhedron::parse_number()
{
   //  The polyhedron's number (written and read with the
   //  %d printf/scanf format).
   if (seek(":number"))
      m_number = parse_int();
}

void netlib_polyhedron::parse_symbol()
{
   //  The eqn(1) input for two symbols separated by a
   //  tab; the Johnson symbol, and the Schlafli symbol.
   if (seek(":symbol"))
      m_symbol = parse_line();
}

void netlib_polyhedron::parse_dual()
{
   //  The name of the dual polyhedron optionally followed
   //  by a horizontal tab and the number of the dual.
   if (seek(":dual"))
      m_dual = parse_line();
}

void netlib_polyhedron::parse_s_faces()
{
   seek(":sfaces");
}

void netlib_polyhedron::parse_s_vertices()
{
   seek(":svertices");
}

void netlib_polyhedron::parse_net()
{
   //  The first line contains the number of faces and
   //  the maximum number of vertices in a face. The
   //  remaining lines are the faces in the planar net.
   //  Each face has a vertex count followed by the
   //  vertex numbers. The vertices are listed in
   //  counter-clockwise order as viewed from outside
   //  the polyhedron.
   seek(":net");
   int face_count = parse_int();
   int max_vertices = parse_int();

   (void)max_vertices;

   for (int i = 0; i < face_count; ++i)
   {
      int vertex_count = parse_int();
      for (int j = 0; j < vertex_count; ++j)
      {
         int vertex = parse_int();
         (void)vertex;
      }
   }
}

void netlib_polyhedron::parse_solid_used_vertices()
{
   if (!seek(":solid"))
      return;

   int face_count = parse_int();
   int max_vertices = parse_int();

   (void)max_vertices;

   m_used_vertices.clear();
   for (int i = 0; i < face_count; ++i)
   {
      int index_count = parse_int();
      for (int j = 0; j < index_count; ++j)
      {
         int index = parse_int();
         m_used_vertices.insert(index);
      }
   }
}

void netlib_polyhedron::parse_solid()
{
   m_polygons.clear();
   //  The first line contains the number of faces and
   //  the maximum number of vertices in a face. The
   //  remaining lines are the faces in the 3D
   //  polyhedron. Each face has a vertex count
   //  followed by the vertex numbers. The vertices
   //  are listed in counter-clockwise order as viewed
   //  from outside the polyhedron.
   if (!seek(":solid"))
      return;

   int face_count = parse_int();
   int max_vertices = parse_int();

   (void)max_vertices;

   for (int i = 0; i < face_count; ++i)
   {
      vector<int> polygon;
      int index_count = parse_int();
      for (int j = 0; j < index_count; ++j)
      {
         int index = parse_int();
         polygon.push_back(index);
      }
      std::reverse(polygon.begin(), polygon.end());
      m_polygons.push_back(polygon);
   }
}

void netlib_polyhedron::parse_hinges()
{
   // the first line contains the number of hinges in
   // the planar net. The remaining lines are hinge
   // connections. The format is face1 side1 face2
   // side2 value. Sides are numbered from zero.  If
   // the dihedral value is greater than J, it is a
   // reflex or re-entrant hinge.
   if (!seek(":hinges"))
      return;

   int hinge_count = parse_int();

   for (int i = 0; i < hinge_count; ++i)
   {
      int face1 = parse_int();
      int side1 = parse_int();
      int face2 = parse_int();
      int side2 = parse_int();
      int value = parse_int();
      (void)face1;
      (void)side1;
      (void)face2;
      (void)side2;
      (void)value;
   }
}

void netlib_polyhedron::parse_dihedrals()
{
   //  The first line contains the number of distinct
   //  dihedrals. Each dihedral starts on a new line
   //  and has a count and a value. If the count is
   //  non-zero, then that many face edge pairs (one
   //  per line) follow the dihedral value.
   seek(":dih");
}

void netlib_polyhedron::parse_vertices()
{
   //  The first line contains the number of vertices.
   //  The vertices are arranged one per line as an
   //  (x, y, z) coordinate of white-space separated
   //  values (described below). The vertices are
   //  implicitly numbered starting at zero.

   if (!seek(":vertices"))
      return;

   int vertex_count = parse_int();
   eol(); // sometimes there is extra int

   vec3 sum(0.0f, 0.0f, 0.0f);

   for (int i = 0; i < vertex_count; ++i)
   {
      float scale = 1.0f / 2.0f;
      float x = parse_float() * scale;
      float y = parse_float() * scale;
      float z = parse_float() * scale;

      m_locations.push_back(vec3(x, y, z));

      auto fi = m_used_vertices.find(i);
      if ( fi != m_used_vertices.end())
         sum += m_locations[i];
   }

   if (m_used_vertices.size())
   {
      vec3 center = sum / (float)(m_used_vertices.size());

      for (size_t i = 0; i < m_locations.size(); ++i)
         m_locations[i] -= center;
   }
}

netlib_polyhedron::netlib_polyhedron(string const &path)
{
   m_text = renderstack::toolkit::read(path);
   replace(m_text, "\r\n", "\n");

   parse_name();
   parse_number();
   //ParseSymbol();
   parse_dual();
   //ParseSFaces();
   //ParseSVertices();
   //ParseNet();
   parse_solid_used_vertices();
   parse_vertices();
   parse_solid();
   //ParseHinges();
   //ParseDihedrals();
   //":EOF"
}
