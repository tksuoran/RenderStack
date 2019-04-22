#ifndef edge_hpp_renderstack_geometry
#define edge_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include <cassert>
#include <utility>

namespace renderstack
{
namespace geometry
{

class point;

struct edge
{
public:
    edge(point *p0, point *p1)
    {
        assert(p0 != p1);

        if (p0 < p1)
        {
            m_pair.first  = p0;
            m_pair.second = p1;
        }
        else
        {
            m_pair.first  = p1;
            m_pair.second = p0;
        }
    }

    bool operator<(edge const &other) const
    {
        assert(m_pair.first != m_pair.second);
        assert(other.m_pair.first != other.m_pair.second);
        //assert(!(m_pair.first == other.m_pair.first && m_pair.second == other.m_pair.second));
        assert(!(m_pair.first == other.m_pair.second && m_pair.second == other.m_pair.first));

        return m_pair < other.m_pair;
    }

    point *a() const
    {
        return m_pair.first;
    }

    point *b() const
    {
        return m_pair.second;
    }

    std::pair<point *, point *> m_pair;
};

} // namespace geometry
} // namespace renderstack

#endif
