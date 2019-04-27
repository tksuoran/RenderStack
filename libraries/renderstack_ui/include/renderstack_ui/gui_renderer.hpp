#ifndef gui_renderer_hpp_renderstack_ui
#define gui_renderer_hpp_renderstack_ui

#include "renderstack_graphics/blend_state.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/fragment_outputs.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_ui/font.hpp"
#include "renderstack_ui/style.hpp"
#include "renderstack_toolkit/service.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace renderstack
{
namespace graphics
{
class Buffer;
class Program;
class Shader_monitor;
class Uniform_buffer_range;
class Vertex_format;
class Vertex_stream;
} // namespace graphics
} // namespace renderstack

namespace renderstack
{
namespace ui
{

class Font;
class Ninepatch_style;
class Style;

class Gui_renderer : public renderstack::toolkit::service
{
private:
    Gui_renderer(Gui_renderer const &other);

public:
    struct gui_uniforms
    {
        size_t clip_from_model; // mat4
        size_t color;           // vec4
        size_t color_add;       // vec4
        size_t color_scale;     // vec4
        size_t hsv_matrix;      // mat4
        size_t t;               // float
    };

    Gui_renderer();

    virtual ~Gui_renderer() = default;

    void connect(std::shared_ptr<renderstack::graphics::Renderer>       renderer,
                 std::shared_ptr<renderstack::graphics::Shader_monitor> shader_monitor);

    void initialize_service() override;

    void prepare();

    void on_resize(int width, int height);

    glm::mat4 ortho() const
    {
        return m_ortho;
    }

    void set_program(renderstack::graphics::Program *value);

    void set_texture(unsigned int unit, renderstack::graphics::Texture *texture);

    void set_transform(glm::mat4 value);

    void set_color_add(glm::vec4 value);

    void set_color_scale(glm::vec4 value);

    void set_hsv_matrix(glm::mat4 value);

    void set_t(float value);

    void set_vertex_buffer();

    void set_index_buffer();

    void draw_elements_base_vertex(unsigned int begin_mode, int count, unsigned int index_type, const void *indices, int base_vertex);

    Style &default_style()
    {
        return *(m_default_style.get());
    }

    Style &null_padding_style()
    {
        return *(m_null_padding_style.get());
    }

    Style &button_style()
    {
        return *(m_button_style.get());
    }

    Style &menulist_style()
    {
        return *(m_menulist_style.get());
    }

    Style &choice_style()
    {
        return *(m_choice_style.get());
    }

    Style &colorpicker_style()
    {
        return *(m_colorpicker_style.get());
    }

    Style &slider_style()
    {
        return *(m_slider_style.get());
    }

    renderstack::graphics::Renderer& renderer() const
    {
        return *m_renderer;
    }

    const renderstack::graphics::Vertex_format &vertex_format() const
    {
        return m_vertex_format;
    }

    const renderstack::graphics::Vertex_stream &vertex_stream() const
    {
        return m_vertex_stream;
    }

    std::shared_ptr<renderstack::graphics::Buffer> vertex_buffer()
    {
        return m_vertex_buffer;
    }

    std::shared_ptr<renderstack::graphics::Buffer> index_buffer()
    {
        return m_index_buffer;
    }

    bool use_uniform_buffers() const
    {
        // Test all conditions; can_use.uniform_buffer_object can be forced to false
        bool use_uniform_buffers =
            renderstack::graphics::configuration::can_use.uniform_buffer_object &&
            //(m_glsl_version >= 140) &&
            (renderstack::graphics::configuration::shader_model_version >= 4);

        return use_uniform_buffers;
    }

    void blend_alpha();

    void blend_add();

    void blend_disable();

private:
    std::unique_ptr<renderstack::graphics::Program> load_program(const std::string &name, const std::string &shader);

    void map(std::shared_ptr<renderstack::graphics::Program> program);

    std::shared_ptr<renderstack::graphics::Renderer>       m_renderer;
    std::shared_ptr<renderstack::graphics::Shader_monitor> m_shader_monitor;

    glm::mat4                                        m_ortho;

    std::string                                      m_shader_path;
    std::vector<std::pair<std::string, int>>         m_shader_versions;
    renderstack::graphics::Program                  *m_program{nullptr};
    renderstack::graphics::Blend_state               m_blend_disabled;
    renderstack::graphics::Blend_state               m_blend_alpha;
    renderstack::graphics::Blend_state               m_blend_add;
    renderstack::graphics::Render_states             m_gui_render_states;
    renderstack::graphics::Uniform_block             m_uniform_block{"gui"};
    struct gui_uniforms                              m_uniforms;
    renderstack::graphics::Sampler                   m_nearest_sampler;
    renderstack::graphics::Samplers                  m_samplers;
    renderstack::graphics::Vertex_attribute_mappings m_vertex_attribute_mappings;
    renderstack::graphics::Fragment_outputs          m_fragment_outputs;
    renderstack::graphics::Vertex_format             m_vertex_format;
    renderstack::graphics::Vertex_stream             m_vertex_stream;
    std::shared_ptr<renderstack::graphics::Buffer>   m_vertex_buffer;
    std::shared_ptr<renderstack::graphics::Buffer>   m_index_buffer;
    std::unique_ptr<renderstack::graphics::Program>  m_ninepatch_program;
    std::unique_ptr<renderstack::graphics::Program>  m_font_program;
    std::unique_ptr<renderstack::graphics::Program>  m_hsv_program;
    std::unique_ptr<renderstack::graphics::Program>  m_slider_program;

    std::unique_ptr<Font>            m_font;
    std::unique_ptr<Style>           m_default_style;
    std::unique_ptr<Style>           m_null_padding_style;
    std::unique_ptr<Ninepatch_style> m_button_ninepatch_style;
    std::unique_ptr<Ninepatch_style> m_menulist_ninepatch_style;
    std::unique_ptr<Ninepatch_style> m_slider_ninepatch_style;
    std::unique_ptr<Style>           m_button_style;
    std::unique_ptr<Style>           m_menulist_style;
    std::unique_ptr<Style>           m_choice_style;
    std::unique_ptr<Style>           m_colorpicker_style;
    std::unique_ptr<Style>           m_slider_style;

    unsigned char *m_start{nullptr};
};

} // namespace ui
} // namespace renderstack

#endif
