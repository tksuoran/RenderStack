#ifndef debug_renderer_hpp_renderers
#define debug_renderer_hpp_renderers

#include "main/application.hpp"
#include "main/programs.hpp"
#include "renderers/base_renderer.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_toolkit/service.hpp"
#include <cstdint>
#include <deque>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace renderstack
{
namespace graphics
{
class Renderer;
class Uniform_buffer;
class Uniform_buffer_range;
} // namespace graphics
} // namespace renderstack

namespace renderstack
{
namespace scene
{
class Camera;
struct Viewport;
} // namespace scene
} // namespace renderstack

namespace renderstack
{
namespace ui
{
class Font;
class Gui_renderer;
class Text_buffer;
} // namespace ui
} // namespace renderstack

// This structure matches DrawElementsIndirectCommand from]
// http://www.opengl.org/registry/specs/ARB/draw_indirect.txt
struct draw
{
    unsigned count;         // number of indices / vertices
    unsigned prim_count;    // number of instances
    unsigned first_index;   // offset added to index buffer
    unsigned base_vertex;   // offset added to vertex buffer
    unsigned base_instance; // offset added to instance buffer
};

struct print_at
{
    int         x;
    int         y;
    std::string text;
};

class Application;

class Debug_renderer
    : public renderstack::toolkit::service,
      public Base_renderer
{
public:
    Debug_renderer();

    virtual ~Debug_renderer();

    void connect(std::shared_ptr<renderstack::graphics::Renderer> renderer,
                 std::shared_ptr<renderstack::ui::Gui_renderer>   gui_renderer,
                 std::shared_ptr<Programs>                        programs,
                 std::shared_ptr<Application>                     application);

    void initialize_service();

    void begin_edit();

    void end_edit();

    void render();

    void clear_text_lines();

    void record_frame_duration(float frame_duration);

    void add_frame_duration_graph(renderstack::scene::Viewport vp);

    void printf(const char *format, ...);

    void printf(int x, int y, const char *format, ...);

    void render_text_lines(renderstack::scene::Viewport p);

    void set_camera(renderstack::scene::Camera *camera);

    void set_model(glm::mat4 world_from_model);

    void set_clip_from_model(glm::mat4 clip_from_model);

    void set_ortho(renderstack::scene::Viewport p);

    void set_color(glm::vec4 color);

    void add_line(glm::vec3 start, glm::vec3 end);

    void add_box(glm::vec3 min_, glm::vec3 max_);

private:
    void          begin_edit_vbo();
    void          begin_edit_ibo();
    std::uint16_t add_point(glm::vec3 p);

private:
    std::shared_ptr<renderstack::ui::Gui_renderer> m_gui_renderer;
    std::shared_ptr<Application>                   m_application;

    renderstack::scene::Camera *m_camera{nullptr};

    // Self owned
    renderstack::graphics::Vertex_format           m_vertex_format;
    //renderstack::graphics::Vertex_stream_mappings  m_mappings;
    renderstack::graphics::Vertex_stream           m_vertex_stream;
    std::shared_ptr<renderstack::graphics::Buffer> m_vertex_buffer;
    std::shared_ptr<renderstack::graphics::Buffer> m_index_buffer;
    
    // Debug text lines
    std::vector<std::string> m_debug_lines;
    std::vector<print_at>    m_debug_print_ats;

#if defined(RENDERSTACK_USE_FREETYPE)
    std::shared_ptr<renderstack::ui::Font>        m_font;
    std::shared_ptr<renderstack::ui::Text_buffer> m_text_buffer;
    renderstack::graphics::Render_states          m_font_render_states;
#endif

    renderstack::graphics::Render_states m_render_states;

    bool           m_in_edit{false};
    float *        m_vertex_ptr_start{nullptr};
    std::uint16_t *m_index_ptr_start{nullptr};
    float *        m_vertex_ptr{nullptr};
    std::uint16_t *m_index_ptr{nullptr};
    int            m_capacity_lines{0};
    std::uint16_t  m_vertex_offset{0U};
    std::uint16_t  m_index_offset{0U};
    unsigned char *m_models_start{nullptr};
    size_t         m_model_index{0U};

    glm::vec4      m_color;

    draw              m_current_draw;
    std::vector<draw> m_draws;
    std::deque<float> m_frame_durations; // TODO use ringbuffer, implement on top of vector<>
    size_t            m_frame_duration_graph_size;
};

#endif
