#ifndef layer_hpp_renderstack_ui
#define layer_hpp_renderstack_ui

#include "renderstack_ui/area.hpp"

namespace renderstack
{
namespace ui
{

class Layer : public Area
{
public:
    Layer(Gui_renderer &renderer, Rectangle fixed_size)
        : Area(renderer)
        , m_fixed_size(fixed_size)
    {
        draw_ordering = Order::post_self;
        event_ordering = Order::separate;
        update();
    }

    virtual ~Layer() = default;

    void set_layer_size(float width, float height)
    {
        m_fixed_size = Rectangle(width, height);
    }

    void update()
    {
        rect = m_fixed_size;
        size = rect.size();
        place();
    }

    void place()
    {
        for (auto child : children)
        {
            child->do_size(size);
        }

        for (auto child : children)
        {
            child->do_place(rect, glm::vec2(1.0f, 1.0f));
        }
    }

private:
    Rectangle m_fixed_size;
};

} // namespace ui
} // namespace renderstack

#endif
