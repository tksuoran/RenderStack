#ifndef dock_hpp_renderstack_ui
#define dock_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/area.hpp"
#include <limits>
#include <memory>

namespace renderstack
{
namespace ui
{

class Dock : public Area
{
public:
    Dock(Gui_renderer &renderer,
         Style        &style,
         Orientation  orientation,
         Flow_mode    child_layout_style = Flow_mode::normal);

    virtual ~Dock() = default;

    Orientation orientation() const
    {
        return m_orientation;
    }

    void set_orientation(Orientation value)
    {
        m_orientation = value;
    }

    Area::Flow_mode child_layout_style() const
    {
        return m_child_layout_style;
    }

    void set_child_layout_style(Area::Flow_mode value)
    {
        m_child_layout_style = value;
    }

    void begin_size(glm::vec2 free_size_reference) override;

    void call_size(Area *area) override;

    Area *add(Area *area) override;

    void end_size() override;

    void begin_place(Rectangle reference, glm::vec2 container_grow_direction) override;

    void call_place(Area *area) override;

private:
    Orientation     m_orientation;
    glm::vec2       m_cursor_start;
    glm::vec2       m_cursor_end;
    glm::vec2       m_grow_direction;
    Area::Flow_mode m_child_layout_style;
};

} // namespace ui
} // namespace renderstack

#endif
