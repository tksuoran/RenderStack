#ifndef push_button_hpp_renderstack_ui
#define push_button_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/button.hpp"
#include <memory>

namespace renderstack
{
namespace ui
{

class Push_button : public Button
{
public:
    Push_button(Gui_renderer &renderer, const std::string &label, Style &style);

    virtual ~Push_button() = default;

    void draw_self(ui_context &context);

    bool pressed() const
    {
        return m_pressed;
    }

    void set_pressed(bool value)
    {
        m_pressed = value;
    }

private:
    bool m_pressed;
};

} // namespace ui
} // namespace renderstack

#endif
