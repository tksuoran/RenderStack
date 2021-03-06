#ifndef netlib_polyhedron_hpp_parsers
#define netlib_polyhedron_hpp_parsers

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_set>
#include <vector>

class netlib_polyhedron
{
public:
    netlib_polyhedron(const std::string &path);

private:
    bool        seek(const std::string &label);
    void        eol();
    std::string parse_line();
    void        skip_white_spaces();
    void        seek_white_space();
    void        seek_non_number();
    int         parse_int();
    float       parse_float();
    void        parse_name();
    void        parse_number();
    void        parse_symbol();
    void        parse_dual();
    void        parse_s_faces();
    void        parse_s_vertices();
    void        parse_net();
    void        parse_solid_used_vertices();
    void        parse_solid();
    void        parse_hinges();
    void        parse_dihedrals();
    void        parse_vertices();

private:
    std::string                   m_name;
    int                           m_number;
    std::string                   m_symbol;
    std::string                   m_dual;
    std::vector<glm::vec3>        m_locations;
    std::vector<std::vector<int>> m_polygons;

    const std::string &name() const
    {
        return m_name;
    }
    int number() const
    {
        return m_number;
    }
    const std::string &symbol() const
    {
        return m_symbol;
    }
    const std::string &dual() const
    {
        return m_dual;
    }
    std::vector<glm::vec3> locations() const
    {
        return m_locations;
    }
    std::vector<std::vector<int>> polygons() const
    {
        return m_polygons;
    }

    std::string             m_text;
    size_t             m_pos;
    std::unordered_set<int> m_used_vertices;
};

#endif
