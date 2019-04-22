#ifndef push_button_hpp_renderstack_ui
#define push_button_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/button.hpp"
#include <memory>

namespace renderstack
{
namespace ui
{

class push_button : public button
{
private:
    bool m_pressed;

public:
    bool pressed() const
    {
        return m_pressed;
    }
    void set_pressed(bool value)
    {
        m_pressed = value;
    }

public:
    push_button(
        std::shared_ptr<class gui_renderer> renderer,
        std::string const &                 label,
        std::shared_ptr<class style>        style);
    virtual ~push_button() {}

    void draw_self(ui_context &context);
};

} // namespace ui
} // namespace renderstack

#endif
