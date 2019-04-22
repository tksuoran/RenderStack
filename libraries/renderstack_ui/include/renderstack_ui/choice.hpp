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

class choice;
class push_button;

class choice_item
    : public push_button,
      public action_sink,
      public renderstack::toolkit::enable_shared_from_this<choice_item>
{
private:
    std::weak_ptr<class choice> m_choice;
    std::string                 m_label;

public:
    std::string const &label() const
    {
        return m_label;
    }
    void set_label(std::string const &value)
    {
        m_label = value;
    }

public:
    choice_item(
        std::shared_ptr<class gui_renderer> renderer,
        std::string const &                 label,
        std::shared_ptr<class style>        style);
    virtual ~choice_item();

    void action(std::weak_ptr<action_source> source);
    void connect(std::weak_ptr<class choice> choice);
};

class choice
    : public dock,
      public action_source,
      public renderstack::toolkit::enable_shared_from_this<choice>
{
private:
    std::weak_ptr<choice_item>                m_selected;
    std::vector<std::shared_ptr<choice_item>> m_items;
    std::shared_ptr<class style>              m_choice_item_style;

public:
    std::vector<std::shared_ptr<choice_item>> const &items() const
    {
        return m_items;
    }
    std::weak_ptr<choice_item> selected() const
    {
        return m_selected;
    }
    void set_selected(std::weak_ptr<choice_item> value);

    choice(
        std::shared_ptr<class gui_renderer> renderer,
        std::shared_ptr<class style>        style,
        std::shared_ptr<class style>        choice_item_style,
        orientation::value                  orientation_in);
    virtual ~choice();

    std::shared_ptr<choice_item> add(std::shared_ptr<choice_item> item);
    std::shared_ptr<choice_item> add_choice_item(std::string const &label, bool select = false);
};

} // namespace ui
} // namespace renderstack

#endif
