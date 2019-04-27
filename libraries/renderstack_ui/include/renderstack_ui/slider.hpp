#ifndef slider_hpp_renderstack_ui
#define slider_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/action.hpp"
#include "renderstack_ui/area.hpp"
#include "renderstack_ui/gui_renderer.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include "renderstack_ui/text_buffer.hpp"
#include <glm/glm.hpp>

namespace renderstack
{
namespace ui
{

class Slider : public Area, public Action_source
{
public:
    Slider(Gui_renderer      &renderer,
           Style             &style,
           const std::string &label,
           float             min,
           float             max);

    virtual ~Slider() = default;

    const std::string &label() const;

    float current_display_value() const;

    float slider_relative_value() const;

    float nonlinear_value() const;

    float relative_value() const;

    float min() const;

    float max() const;

    bool trigger() const
    {
        return m_trigger;
    }

    void set_value(const std::string &value);

    void set_current_display_value(float value);

    void set_slider_relative_value(float value);

    void set_nonlinear_value(float value);

    void set_relative_value(float value);

    void set_min(float value);

    void set_max(float value);

    void set_trigger(bool value)
    {
        m_trigger = value;
    }

    void begin_size(glm::vec2 free_size_reference) override;

    void update_size();

    void begin_place(Rectangle reference, glm::vec2 grow_direction) override;

    void update_place();

    void draw_self(ui_context &context) override;

private:
    glm::mat4                    m_text_frame;
    glm::mat4                    m_background_frame;
    std::unique_ptr<Text_buffer> m_text_buffer;
    std::unique_ptr<Ninepatch>   m_ninepatch;
    Rectangle                    m_bounds;
    std::string                  m_label;
    float                        m_min_value;
    float                        m_max_value;
    bool                         m_label_dirty;
    bool                         m_value_dirty;
    bool                         m_nonlinear;
    bool                         m_trigger;
    float                        m_current_relative_value;
};

} // namespace ui
} // namespace renderstack

#endif
