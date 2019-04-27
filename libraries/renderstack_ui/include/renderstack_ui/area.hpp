#ifndef area_hpp_renderstack_ui
#define area_hpp_renderstack_ui

#include "renderstack_toolkit/enable_shared_from_this.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/rectangle.hpp"
#include "renderstack_ui/style.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace ui
{

struct ui_context
{
    glm::vec2 mouse;
    bool      mouse_buttons[3]; // 0 left 1 right 2 middle
};

class Gui_renderer;

class Area
{
public:
    using Collection = std::vector<Area*>;

    enum class Orientation
    {
        horizontal,
        vertical
    };

    enum class Flow_mode
    {
        normal,
        extend_horizontal,
        extend_vertical
    };

    enum class Flow_direction
    {
        increasing,
        decreasing,
        none
    };

    enum class Order
    {
        self_first, //  Draw self before children
        post_self,  //  Draw self last, after children
        separate    //  Separate DrawSelf() call
    };

    Area(Gui_renderer &renderer)
        : renderer(renderer)
        , style(renderer.default_style())
    {

    }

    Area(Gui_renderer &renderer, Style &style)
        : renderer(renderer)
        , style(style)
    {

    }

    Area(Gui_renderer &renderer, Style &style, Flow_direction layout_x_order, Flow_direction layout_y_order)
        : renderer(renderer)
        , style(style)
        , layout_x_order(layout_x_order)
        , layout_y_order(layout_y_order)
    {

    }

    virtual ~Area() = default;

    virtual Area *add(Area *area);

    Area *remove(Area *area);
    
    Area *get_hit(glm::vec2 hit_position);

    virtual void draw_self(ui_context &context);
    
    void draw(ui_context &context);

    // Layout
    virtual void begin_size(glm::vec2 free_size_reference);

    virtual void call_size(Area *area);

    virtual void end_size();

    // Do not make this virtual.
    // Derived classes should override begin_size() instead
    glm::vec2 do_size(glm::vec2 free_size_reference);

    virtual void begin_place(Rectangle reference, glm::vec2 container_grow_direction);

    virtual void call_place(Area *area);

    virtual void end_place();

    // Do not make this virtual.
    // Derived classes should override begin_place() instead
    glm::vec2 do_place(Rectangle reference_location, glm::vec2 grow_direction);

    std::string    name;
    Gui_renderer   &renderer;
    Area           *parent{nullptr};
    Area           *link{nullptr};
    const Style    &style;
    bool           hidden{false};
    bool           clip_to_reference{true};
    glm::vec2      offset_pixels{0.0f};
    glm::vec2      offset_self_size_relative{0.0f};
    glm::vec2      offset_free_size_relative{0.0f};
    glm::vec2      fill_base_pixels{0.0f};
    glm::vec2      fill_free_size_relative{0.0f};
    glm::vec2      size{0.0f};
    Rectangle      rect;
    Rectangle      in_rect;
    Order          draw_ordering{Order::self_first};
    Order          event_ordering{Order::post_self};
    Flow_mode      layout_style{Flow_mode::normal};
    Flow_direction layout_x_order{Flow_direction::none};
    Flow_direction layout_y_order{Flow_direction::none};
    Collection     children;

protected:
    void update_in_rect()
    {
        in_rect = rect.shrink(style.padding);
    }

    inline void assert_size_valid()
    {
        assert(std::isfinite(size.x));
        assert(std::isfinite(size.y));
    }

private:
    bool       m_in_draw{false};
    Collection m_add_list;
    Collection m_remove_list;
};

} // namespace ui
} // namespace renderstack

#endif
