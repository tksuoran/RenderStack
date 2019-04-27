#ifndef choice_hpp_renderstack_ui
#define choice_hpp_renderstack_ui

#include "renderstack_toolkit/enable_shared_from_this.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/action.hpp"
#include "renderstack_ui/dock.hpp"
#include "renderstack_ui/push_button.hpp"
#include <memory>
#include <string>
#include <vector>

namespace renderstack
{
namespace ui
{

class Choice;
class Push_button;


class Choice : public Dock, public Action_source
{
public:

    class Item : public Push_button, public Action_sink
    {
    public:
        Item(Gui_renderer &renderer, const std::string &label, Style &style);

        virtual ~Item() = default;

        const std::string &label() const
        {
            return m_label;
        }

        void set_label(const std::string &value)
        {
            m_label = value;
        }

        void action(Action_source *source);

        void connect(Choice *choice);

    private:
        Choice      *m_choice{nullptr};
        std::string m_label;
    };

    Choice(Gui_renderer &renderer,
           Style        &style,
           Style        &choice_item_style,
           Orientation  orientation_in);

    virtual ~Choice() = default;

    std::vector<std::shared_ptr<Item>> const &items() const
    {
        return m_items;
    }

    Item *selected() const
    {
        return m_selected;
    }

    void set_selected(Item *value);

    std::shared_ptr<Item> add_item(std::shared_ptr<Item> item);

    std::shared_ptr<Item> make_item(const std::string &label, bool select = false);

private:
    Item                               *m_selected{nullptr};
    std::vector<std::shared_ptr<Item>> m_items;
    Style                              &m_choice_item_style;
};

} // namespace ui
} // namespace renderstack

#endif
