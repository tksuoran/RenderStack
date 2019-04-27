#include "renderstack_ui/slider.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_ui/context.hpp"
#include "renderstack_ui/ninepatch_style.hpp"
#include <sstream>

namespace renderstack
{
namespace ui
{

using namespace renderstack::toolkit;
using namespace std;
using namespace glm;

const std::string &Slider::label() const
{
    return m_label;
}

float Slider::current_display_value() const
{
    return m_min_value + m_current_relative_value * (m_max_value - m_min_value);
}

float Slider::slider_relative_value() const
{
    if (m_nonlinear)
    {
        return nonlinear_value();
    }

    return relative_value();
}

float Slider::nonlinear_value() const
{
    return (float)std::pow(relative_value(), 1.0f / 6.0f);
}

float Slider::relative_value() const
{
    return m_current_relative_value;
}

float Slider::min() const
{
    return m_min_value;
}

float Slider::max() const
{
    return m_max_value;
}

void Slider::set_value(const std::string &value)
{
    m_label = value;
}

void Slider::set_current_display_value(float value)
{
    m_current_relative_value = (value - m_min_value) / (m_max_value - m_min_value);
}

void Slider::set_slider_relative_value(float value)
{
    if (m_nonlinear)
    {
        set_nonlinear_value(value);
    }
    else
    {
        set_relative_value(value);
    }
}

void Slider::set_nonlinear_value(float value)
{
    set_relative_value(std::pow(value, 6.0f));
}
void Slider::set_relative_value(float value)

{
    if (value != m_current_relative_value)
    {
        m_current_relative_value = value;
        m_value_dirty            = true;
    }
}

void Slider::set_min(float value)
{
    m_min_value = value;
}

void Slider::set_max(float value)
{
    m_max_value = value;
}

Slider::Slider(Gui_renderer      &renderer,
               Style             &style,
               const std::string &label,
               float             min,
               float             max)
    : Area(renderer, style)
    , m_label(label)
    , m_min_value(min)
    , m_max_value(max)
    , m_label_dirty(true)
    , m_value_dirty(true)
    , m_nonlinear(true)
{
    if (style.font != nullptr)
    {
        m_text_buffer = std::make_unique<Text_buffer>(renderer, *style.font);
    }
    if (style.ninepatch_style != nullptr)
    {
        m_ninepatch = std::make_unique<Ninepatch>(renderer, *style.ninepatch_style);
    }
    name = label;
    set_slider_relative_value(0.5f);
}

void Slider::begin_size(vec2 free_size_reference)
{
    update_size();
    Area::begin_size(free_size_reference);
}

void Slider::update_size()
{
    if (m_label_dirty)
    {
        if (m_text_buffer)
        {
            m_text_buffer->measure(m_label + ": 180.99");
            m_bounds = m_text_buffer->bounding_box();

            fill_base_pixels = m_bounds.max() + 2.0f * style.padding;
        }
        else
        {
            fill_base_pixels = vec2(30.0f, 10.0f);
        }

        if (m_ninepatch)
        {
            m_ninepatch->place(renderer, 0.0f, 0.0f, fill_base_pixels.x, fill_base_pixels.y);
        }
        m_label_dirty = false;
        m_value_dirty = true;
    }
}

void Slider::begin_place(Rectangle reference, vec2 grow_direction)
{
    Area::begin_place(reference, grow_direction);
    update_place();
    mat4 a;
    mat4 b;
    create_translation(rect.min() + style.padding, a);
    create_translation(rect.min(), b);
    mat4 o      = renderer.ortho();
    m_text_frame       = o * a;
    m_background_frame = o * b;
}

void Slider::update_place()
{
    if (m_ninepatch)
    {
        if (size.x != m_bounds.max().x + 2.0f * style.padding.x)
        {
            m_ninepatch->place(renderer, 0.0f, 0.0f, size.x, size.y);
        }
    }
}

void Slider::draw_self(ui_context &context)
{
    if (m_value_dirty && m_text_buffer)
    {
        stringstream ss;
        ss << m_label << ": " << current_display_value();
        m_text_buffer->begin_print();
        m_text_buffer->print(ss.str(), 0, 0);
        m_text_buffer->end_print();
    }

    auto &gr = renderer;

    //gr->push();

    gr.set_program(style.ninepatch_style->program);
    gr.set_texture(style.ninepatch_style->texture_unit, style.ninepatch_style->texture.get());
    gr.set_transform(m_background_frame);
    gr.set_color_scale(vec4(1.0f, 1.0f, 1.0f, 1.0f));

    if (rect.hit(context.mouse))
    {
        if (context.mouse_buttons[0])
        {
            float x              = context.mouse.x - rect.min().x;
            float relative_value = x / (rect.size().x - 1.0f);
            set_relative_value(relative_value);
            gr.set_color_add(vec4(0.3f, 0.4f, 0.5f, 0.0f));
            set_trigger(true);
        }
        else
        {
            if (trigger())
            {
                auto s = sink();
                if (s)
                {
                    s->action(this);
                }

                set_trigger(false);
            }
            gr.set_color_add(vec4(0.2f, 0.3f, 0.4f, 0.0f));
        }
    }
    else
    {
        set_trigger(false);
        gr.set_color_add(vec4(0.1f, 0.2f, 0.3f, 0.0f));
    }

    float t       = relative_value();
    float pixel_x = rect.min().x + t * (rect.size().x - 1.0f);

    gr.set_t(pixel_x);

    if (m_ninepatch)
    {
        m_ninepatch->render(gr);
    }

    // Then draw text
    if (m_text_buffer)
    {
        gr.blend_alpha();
        gr.set_program(style.program);
        gr.set_texture(style.texture_unit, style.font->texture());
        // TODO Font shader currently does not have color add
        // gr->set_color_add  (vec4(0.00f, 0.00f, 0.00f, 0.0f));
        gr.set_color_scale(vec4(0.72f, 0.72f, 0.72f, 2.0f));
        gr.set_transform(m_text_frame);
        m_text_buffer->render();
        gr.blend_disable();
    }
    //gr->pop();
}

} // namespace ui
} // namespace renderstack
