#ifndef action_hpp_renderstack_ui
#define action_hpp_renderstack_ui

#include "renderstack_toolkit/enable_shared_from_this.hpp"
#include <memory>

namespace renderstack
{
namespace ui
{

class Action_sink;

class Action_source
{
public:
    Action_sink* sink() const
    {
        return m_action_sink;
    }

    void set_sink(Action_sink *value)
    {
        m_action_sink = value;
    }

protected:
    Action_source() = default;

    Action_source(Action_sink *sink)
        : m_action_sink(sink)
    {}

private:
    Action_sink *m_action_sink{nullptr};
};

class Action_sink
{
public:
    virtual ~Action_sink() = default;

    virtual void action(Action_source *source)
    {
        static_cast<void>(source);
    }
};

} // namespace ui
} // namespace renderstack

#endif
