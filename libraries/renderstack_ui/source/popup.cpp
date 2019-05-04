#include "renderstack_ui/popup.hpp"
#include "renderstack_ui/gui_renderer.hpp"

namespace renderstack
{
namespace ui
{

using namespace std;

Popup::Popup(Gui_renderer &renderer, Area *closed, Area *open)
    : Dock(renderer, renderer.null_padding_style(), Orientation::horizontal)
    , m_closed(closed)
    , m_open(open)
    , m_is_open(false)
{
    add(closed);
}

void Popup::toggle()
{
    set(!m_is_open);
}

void Popup::set(bool open)
{
    if (open)
    {
        if (!m_is_open)
        {
            m_is_open = true;
            if (m_closed)
            {
                remove(m_closed);
            }

            if (m_open)
            {
                add(m_open);
            }

            //if (parent().lock() != nullptr)
            // window_manager->update();
        }
    }
    else
    {
        if (m_is_open)
        {
            m_is_open = false;
            if (m_open)
            {
                remove(m_open);
            }

            if (m_closed)
            {
                add(m_closed);
            }

            // if (parent().lock() != nullptr)
            //window_manager->update();
        }
    }
}

void Popup::set_open(Area *value)
{
    if (m_open != value)
    {
        toggle();
        m_open = value;
        toggle();
    }
}

void Popup::set_closed(Area *value)
{
    if (m_closed != value)
    {
        toggle();
        m_closed = value;
        toggle();
    }
}

} // namespace ui
} // namespace renderstack
