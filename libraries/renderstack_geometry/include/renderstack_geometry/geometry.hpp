#ifndef geometry_hpp_renderstack_geometry
#define geometry_hpp_renderstack_geometry

#include "renderstack_geometry/property_map.hpp"
#include "renderstack_geometry/property_map_collection.hpp"
#include <glm/glm.hpp>
#include <set>
#include <vector>

namespace renderstack
{
namespace geometry
{

struct Corner;
class Point;
class Polygon;
class Geometry;
struct Edge;

struct Corner
{
public:
    template<typename T>
    void smooth_normalize(Property_map<Corner *, T>                &corner_attribute,
                          const Property_map<Polygon *, T>         &polygon_attribute,
                          const Property_map<Polygon *, glm::vec3> &polygon_normals,
                          float                                    cos_max_smoothing_angle);

    template<typename T>
    void smooth_average(Property_map<Corner *, T>               &new_corner_attribute,
                        const Property_map<Corner *, T>         &old_corner_attribute,
                        const Property_map<Corner *, glm::vec3> &corner_normals,
                        const Property_map<Point *, glm::vec3>  &point_normals);

    Corner(Point *point, Polygon *polygon)
        : point(point), polygon(polygon)
    {
    }

    Point *point;
    Polygon *polygon;
};

class Point
{
public:
    typedef std::vector<Corner *> Corner_collection;

    Corner_collection &corners()
    {
        return m_corners;
    }

    Corner_collection const &corners() const
    {
        return m_corners;
    }

private:
    friend class Geometry;
    friend class Polygon;

    void add_corner(Corner *corner)
    {
        m_corners.push_back(corner);
    };

    Corner_collection m_corners;
};

class Polygon
{
public:
    typedef std::vector<Corner *> Corner_collection;

    Polygon() {}
    ~Polygon();

    // Copies polygon property to corners
    template <typename T>
    void copy_to_corners(Property_map<Corner *, T>        &corner_attribute,
                         const Property_map<Polygon *, T> &polygon_attribute);

    template <typename T>
    void smooth_normalize(Property_map<Corner *, T>                &corner_attribute,
                          const Property_map<Polygon *, T>         &polygon_attribute,
                          const Property_map<Polygon *, glm::vec3> &polygon_normals,
                          float                                    cos_max_smoothing_angle);

    template <typename T>
    void smooth_average(Property_map<Corner *, T>               &new_corner_attribute,
                        const Property_map<Corner *, T>         &old_corner_attribute,
                        const Property_map<Corner *, glm::vec3> &corner_normals,
                        const Property_map<Point *, glm::vec3>  &point_normals);

    void compute_normal(Property_map<Polygon *, glm::vec3>     &polygon_normals,
                        const Property_map<Point *, glm::vec3> &point_locations);

    void compute_centroid(Property_map<Polygon *, glm::vec3>     &polygon_centroids,
                          const Property_map<Point *, glm::vec3> &point_locations);

    Corner *corner(Point *point);
    Corner *make_corner(Point *point);
    Corner *next_corner(Corner *corner);
    Corner *prev_corner(Corner *corner);

    Corner_collection &corners()
    {
        return m_corners;
    }

    Corner_collection const &corners() const
    {
        return m_corners;
    }

    void reverse()
    {
        std::reverse(m_corners.begin(), m_corners.end());
    }

private:
    Corner_collection m_corners;
};

struct Edge
{
public:
    Edge(const Edge &other) : pair(other.pair) {}

    Edge(Point *p0, Point *p1)
    {
        assert(p0 != p1);

        if (p0 < p1)
        {
            pair.first  = p0;
            pair.second = p1;
        }
        else
        {
            pair.first  = p1;
            pair.second = p0;
        }
    }

    bool operator<(Edge const &other) const
    {
        assert(pair.first != pair.second);
        assert(other.pair.first != other.pair.second);
        //assert(!(pair.first == other.pair.first && pair.second == other.pair.second));
        assert(!(pair.first == other.pair.second && pair.second == other.pair.first));

        return pair < other.pair;
    }

    Point *a() const
    {
        return pair.first;
    }

    Point *b() const
    {
        return pair.second;
    }

    std::pair<Point *, Point *> pair;
};

class Geometry
{
public:
    using Point_property_map_collection   = Property_map_collection<Point *>;
    using Corner_property_map_collection  = Property_map_collection<Corner *>;
    using Polygon_property_map_collection = Property_map_collection<Polygon *>;
    using Edge_property_map_collection    = Property_map_collection<Edge *>;

    using Point_collection   = std::vector<Point *>;
    using Polygon_collection = std::vector<Polygon *>;
    using Edge_collection    = std::map<Edge, std::set<Polygon *>>;

    struct Mesh_info
    {
        size_t polygon_count{0U};
        size_t corner_count{0U};
        size_t triangle_count{0U};
        size_t edge_count{0U};
        size_t vertex_count_corners{0U};
        size_t vertex_count_centroids{0U};
        size_t index_count_fill_triangles{0U};
        size_t index_count_edge_lines{0U};
        size_t index_count_corner_points{0U};
        size_t index_count_centroid_points{0U};

        Mesh_info &operator+=(Mesh_info const &o);
    };

public:
    Geometry() = default;
    Geometry(const std::string &name)
        : m_name(name) {}

    Geometry(const Geometry &) = delete;
    Geometry &operator=(const Geometry &) = delete;

    Geometry(Geometry &&other)
    {
        m_name = std::move(other.m_name);
        m_point_property_map_collection = std::move(other.m_point_property_map_collection);
        m_corner_property_map_collection = std::move(other.m_corner_property_map_collection);
        m_polygon_property_map_collection = std::move(other.m_polygon_property_map_collection);
        m_edge_property_map_collection = std::move(other.m_edge_property_map_collection);
        m_points = std::move(other.m_points);
        m_polygons = std::move(other.m_polygons);
        m_edges = std::move(other.m_edges);
    }

    ~Geometry();

public:
    const std::string &name() const
    {
        return m_name;
    }

    size_t count_polygons() const;
    size_t count_corners() const;
    size_t count_polygon_triangles() const;
    size_t count_edges() const;

    void info(Mesh_info &info) const;

    Point_property_map_collection &point_attributes()
    {
        return m_point_property_map_collection;
    }

    Corner_property_map_collection &corner_attributes()
    {
        return m_corner_property_map_collection;
    }

    Polygon_property_map_collection &polygon_attributes()
    {
        return m_polygon_property_map_collection;
    }

    Edge_property_map_collection &edge_attributes()
    {
        return m_edge_property_map_collection;
    }

    Point_property_map_collection const &point_attributes() const
    {
        return m_point_property_map_collection;
    }

    Corner_property_map_collection const &corner_attributes() const
    {
        return m_corner_property_map_collection;
    }

    Polygon_property_map_collection const &polygon_attributes() const
    {
        return m_polygon_property_map_collection;
    }

    Edge_property_map_collection const &edge_attributes() const
    {
        return m_edge_property_map_collection;
    }

    void     reserve_points(size_t point_count);
    void     reserve_polygons(size_t polygon_count);
    Point   *make_point();
    Polygon *make_polygon();

    Point   *make_point(float x, float y, float z);
    Point   *make_point(float x, float y, float z, float s, float t);
    Point   *make_point(double x, double y, double z);
    Point   *make_point(double x, double y, double z, double s, double t);

    Polygon *make_polygon(const std::initializer_list<Point *> point_list);
    Polygon *make_polygon(const std::initializer_list<size_t> point_list);

    Point_collection &        points();
    Point_collection const &  points() const;
    Polygon_collection &      polygons();
    Polygon_collection const &polygons() const;
    Edge_collection &         edges();
    Edge_collection const &   edges() const;

    void compute_polygon_normals();
    void compute_polygon_centroids();

    glm::vec3 compute_point_normal(Point *point);
    void      compute_point_normals(const std::string &map_name);

    // template <typename T>
    // bool smooth_normalize(const std::string &corner_attribute,
    //                       const std::string &polygon_attribute,
    //                       float              max_smoothing_angle_radians);

    template <typename T>
    void smooth_normalize(Property_map<Corner *, T>                &corner_attribute,
                          const Property_map<Polygon *, T>         &polygon_attribute,
                          const Property_map<Polygon *, glm::vec3> &polygon_normals,
                          float                                    max_smoothing_angle_radians) const;

    //bool smooth_average(const std::string &corner_attribute,
    //                    const std::string &point_normal_name);

    template <typename T>
    void smooth_average(Property_map<Corner *, T>                &smoothed_corner_attribute,
                        const Property_map<Corner *, T>          &corner_attribute,
                        const Property_map<Corner *, glm::vec3>  &corner_normals,
                        const Property_map<Polygon *, glm::vec3> &point_normals) const;

    void build_edges();

    void transform(glm::mat4 const &m);

    void optimize_attribute_maps();

private:
    std::string                     m_name;
    Point_property_map_collection   m_point_property_map_collection;
    Corner_property_map_collection  m_corner_property_map_collection;
    Polygon_property_map_collection m_polygon_property_map_collection;
    Edge_property_map_collection    m_edge_property_map_collection;
    Point_collection                m_points;
    Polygon_collection              m_polygons;
    Edge_collection                 m_edges;
};

} // namespace geometry
} // namespace renderstack

#include "corner.inl"
#include "polygon.inl"
#include "geometry.inl"

#endif
