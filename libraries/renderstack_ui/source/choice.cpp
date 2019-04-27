#include "renderstack_ui/choice.hpp"

namespace renderstack
{
namespace ui
{

using namespace std;
using namespace renderstack::toolkit;

Choice::Item::Item(Gui_renderer &     renderer,
                   const std::string &label,
                   Style &            style)
    : Push_button(renderer, label, style)
{
    name = "choice item: " + label;
}

void Choice::Item::action(Action_source *source)
{
    static_cast<void>(source); // TODO
    if (m_choice)
    {
        auto selected = m_choice->selected();
        if (selected != this)
        {
            m_choice->set_selected(this);
        }
    }
    else
    {
        throw runtime_error("choice item is not connected");
    }
}

void Choice::Item::connect(Choice *choice)
{
    m_choice = choice;
}

Choice::Choice(Gui_renderer &renderer,
               Style        &style,
               Style        &choice_item_style,
               Orientation  orientation_in)
    : Dock(renderer, style, orientation_in)
    , m_choice_item_style(choice_item_style)
{
    name = "choice";
}

std::shared_ptr<Choice::Item> Choice::add_item(std::shared_ptr<Choice::Item> item)
{
    item->connect(this);

    switch (orientation())
    {
        case Orientation::horizontal:
        {
            item->layout_style = Flow_mode::extend_vertical;
            break;
        }

        case Orientation::vertical:
        {
            item->layout_style = Flow_mode::extend_horizontal;
            break;
        }
    }

    m_items.push_back(item);
    Dock::add(item.get());
    if (m_selected != item.get())
    {
        item->set_pressed(false);
    }

    return item;
}

void Choice::set_selected(Choice::Item *new_selected)
{
    auto old_selected = m_selected;

    if (new_selected != old_selected)
    {
        if (old_selected)
        {
            old_selected->set_pressed(false);
        }

        m_selected = new_selected;
        if (new_selected)
        {
            new_selected->set_pressed(true);
        }

        if (sink())
        {
            sink()->action(m_selected);
        }
    }
}

shared_ptr<Choice::Item> Choice::make_item(const std::string &label, bool select)
{
    shared_ptr<Item> item = std::make_shared<Item>(renderer, label, m_choice_item_style);
    // choice_item handles actions itself
    item->set_sink(item.get()); // this can not be done in constructor as shared_from_this won't work
    add_item(item);
    if (select)
    {
        set_selected(item.get());
    }

    return item;
}

} // namespace ui
} // namespace renderstack
