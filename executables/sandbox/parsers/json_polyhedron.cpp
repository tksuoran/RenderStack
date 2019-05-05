#include "parsers/json_polyhedron.hpp"
#include "renderstack_toolkit/file.hpp"
#include "json.hpp"
#include <algorithm>
#include <cctype>
#include <glm/glm.hpp>
#include <sstream>
#include <string>

using namespace glm;
using namespace renderstack::geometry;

using json::JSON;

//class Point {
//public:
//    float x;
//    float y;
//    float z;
//    Point (float x, float y, float z) : x(x), y(y), z(z) {}
//    Json to_json() const { return Json::array { x, y, z }; }
//};

Geometry make_json_polyhedron(const std::string &path)
{
    printf("path = %s\n", path.c_str());

    std::string text = renderstack::toolkit::read(path);
    printf("text = %s\n", text.c_str());

    Geometry geometry;

    auto json = JSON::Load(text);
    auto &mesh = json["mesh"];
    auto &points = mesh["vertices"];
    for (auto &point : points.ArrayRange())
    {
        auto xo = point["x"];
        auto yo = point["y"];
        auto zo = point["z"];
        std::string xs = xo.ToString();
        std::string ys = yo.ToString();
        std::string zs = zo.ToString();
        printf("%s, %s, %s\n", xs.c_str(), ys.c_str(), zs.c_str());
        float x = std::stof(point["x"].ToString());
        float y = std::stof(point["y"].ToString());
        float z = std::stof(point["z"].ToString());
        geometry.make_point(-x, -y, -z);
    }
    auto &polygons = mesh["polygons"];
    for (auto &polygon : polygons.ArrayRange())
    {
        auto &corners = polygon["corners"];
        auto g_polygon = geometry.make_polygon();
        for (auto &corner : corners.ArrayRange())
        {
            std::string s_value = corner.ToString();
            //printf("%s\n", s_value.c_str());
            int index = std::stoi(s_value);//corner.ToInt();
            if (index < (int)geometry.points().size())
            {
                g_polygon->make_corner(geometry.points()[index]);
            }
        }
    }
    return geometry;
}
