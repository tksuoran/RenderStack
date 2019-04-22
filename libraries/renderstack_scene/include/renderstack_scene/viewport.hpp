#ifndef viewport_hpp_renderstack_scene
#define viewport_hpp_renderstack_scene

#include "renderstack_toolkit/platform.hpp"
#include <cstdint>

namespace renderstack
{
namespace scene
{

class viewport
{
public:
    explicit viewport()
        : m_border(0), m_x(0), m_y(0), m_width(0), m_height(0)
    {
    }

    int border() const
    {
        return m_border;
    }
    void set_border(int value)
    {
        m_border = value;
    }
    int x() const
    {
        return m_x;
    }
    void set_x(int value)
    {
        m_x = value;
    }
    int y() const
    {
        return m_y;
    }
    void set_y(int value)
    {
        m_y = value;
    }
    int width() const
    {
        return m_width;
    }
    void set_width(int value)
    {
        m_width = value;
    }
    int height() const
    {
        return m_height;
    }
    void set_height(int value)
    {
        m_height = value;
    }

    float aspect_ratio() const
    {
        return (m_height != 0.0f)
                   ? (static_cast<float>(m_width) / static_cast<float>(m_height))
                   : 1.0f;
    }

private:
    int m_border;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
};

} // namespace scene
} // namespace renderstack

#endif
