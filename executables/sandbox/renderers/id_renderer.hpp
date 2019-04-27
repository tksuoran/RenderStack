#ifndef id_renderer_hpp_renderers
#define id_renderer_hpp_renderers

#include "main/programs.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "scene/model.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace renderstack
{
namespace graphics
{
class Uniform_buffer;
class Uniform_buffer_range;
} // namespace graphics
namespace scene
{
class Camera;
}
} // namespace renderstack

class Id_renderer : public renderstack::toolkit::service
{
public:
    struct Render
    {
        enum class State
        {
            unused           = 0,
            waiting_for_read = 1,
            read_complete    = 2
        };

        std::shared_ptr<renderstack::graphics::Buffer> pixel_pack_buffer;
        double                                         time;
        GLsync                                         sync;
        glm::mat4                                      clip_from_world;
        int                                            x_offset;
        int                                            y_offset;
        State                                          state;
        std::vector<uint8_t>                           data;
    };

    Id_renderer();

    virtual ~Id_renderer() = default;

    void connect(std::shared_ptr<renderstack::graphics::Renderer> renderer,
                 std::shared_ptr<Programs>                        programs);

    void initialize_service() override;

    void clear();

    void render_pass(const Model_collection           &models,
                     const renderstack::scene::Camera &camera,
                     double                           time,
                     int                              x,
                     int                              y);

    bool get(int x, int y, uint32_t &id, float &depth);

    Model *get(int x, int y);

private:
    std::shared_ptr<renderstack::graphics::Renderer> m_renderer;
    std::shared_ptr<Programs>                        m_programs;

    renderstack::graphics::Render_states m_id_render_states;

    // pixel pack buffers
    int       m_last_render;
    int       m_radius{64};
    std::array<Render, 4> m_renders;

    struct Range
    {
        uint32_t offset;
        uint32_t length;
        Model    *model;
    };

    std::vector<Range> m_ranges;
};

#endif
