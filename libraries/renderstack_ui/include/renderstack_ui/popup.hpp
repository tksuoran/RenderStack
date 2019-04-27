#ifndef popup_hpp_renderstack_ui
#define popup_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/dock.hpp"

namespace renderstack
{
namespace ui
{

class Popup : public Dock
{
public:
    Popup(Gui_renderer &renderer, Area *closed, Area *open);

    virtual ~Popup() = default;  

    Area *current() const
    {
        return (m_is_open ? m_open : m_closed);
    }

    Area *closed() const
    {
        return m_closed;
    }

    void set_closed(Area *value);

    Area *open() const
    {
        return m_open;
    }

    void set_open(Area *value);

    bool is_open() const
    {
        return m_is_open;
    }

    void toggle();

    //  Set(true) to open
    //  Set(false) to close
    void set(bool open);

private:
    Area *m_closed{nullptr};
    Area *m_open{nullptr};
    bool m_is_open{true};
};

} // namespace ui
} // namespace renderstack

#endif
