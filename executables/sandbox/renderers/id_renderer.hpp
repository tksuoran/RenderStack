#ifndef id_renderer_hpp_renderers
#define id_renderer_hpp_renderers

#include "main/programs.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace renderstack
{
namespace graphics
{
class uniform_buffer;
class uniform_buffer_range;
} // namespace graphics
namespace scene
{
class camera;
}
} // namespace renderstack

class model;

namespace id_render_state
{
enum value
{
    unused           = 0,
    waiting_for_read = 1,
    read_complete    = 2
};
};

struct id_render
{
    std::shared_ptr<renderstack::graphics::buffer> pixel_pack_buffer;
    double                                         time;
    GLsync                                         sync;
    glm::mat4                                      clip_from_world;
    int                                            x_offset;
    int                                            y_offset;
    id_render_state::value                         state;
    std::vector<uint8_t>                           data;
};

class id_renderer : public renderstack::toolkit::service
{
public:
    id_renderer();
    /*virtual*/ ~id_renderer();

    void connect(
        std::shared_ptr<renderstack::graphics::renderer> renderer,
        std::shared_ptr<programs>                        programs);

    /*virtual*/ void initialize_service();

    void clear();
    void render_pass(
        std::shared_ptr<std::vector<std::shared_ptr<model>>> models,
        std::shared_ptr<renderstack::scene::camera>          camera,
        double                                               time,
        int                                                  x,
        int                                                  y);

    bool get(int x, int y, uint32_t &id, float &depth);

    std::shared_ptr<class model> get(int x, int y);

private:
    std::shared_ptr<renderstack::graphics::renderer> m_renderer;
    std::shared_ptr<programs>                        m_programs;

    renderstack::graphics::render_states m_id_render_states;

    // pixel pack buffers
    int       m_last_render;
    int       m_radius;
    id_render m_renders[4];

    struct id_range
    {
        uint32_t                     offset;
        uint32_t                     length;
        std::shared_ptr<class model> model;
    };

    std::vector<id_range> m_ranges;
};

#endif
