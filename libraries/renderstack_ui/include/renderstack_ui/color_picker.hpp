#ifndef color_picker_hpp_renderstack_ui
#define color_picker_hpp_renderstack_ui

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/area.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include <memory>

namespace renderstack
{
namespace mesh
{

class mesh;
class Geometry_mesh;

} // namespace mesh
} // namespace renderstack

namespace renderstack
{
namespace ui
{

class Color_picker : public Area
{
public:
    Color_picker(Gui_renderer &renderer, Style &style);

    virtual ~Color_picker() = default;

    void animate();

    void begin_place(Rectangle reference, glm::vec2 grow_direction);

    void draw_self(ui_context &context);

private:
    std::unique_ptr<renderstack::mesh::mesh>          m_disc;
    std::unique_ptr<renderstack::mesh::mesh>          m_triangle;
    std::unique_ptr<renderstack::mesh::Geometry_mesh> m_hsv_disc_mesh;
    std::unique_ptr<renderstack::mesh::Geometry_mesh> m_hsv_disc2_mesh;
    std::unique_ptr<renderstack::mesh::Geometry_mesh> m_hsv_quad_mesh;
    std::unique_ptr<renderstack::mesh::Geometry_mesh> m_color_mesh;
    std::unique_ptr<renderstack::mesh::Geometry_mesh> m_handle_mesh;
    std::unique_ptr<renderstack::mesh::Geometry_mesh> m_handle2_mesh;

    std::unique_ptr<Ninepatch> m_ninepatch;
    
    float     m_size;
    glm::mat4 m_background_frame;
    glm::mat4 m_hsv_transform;
    glm::mat4 m_disc_handle_transform;
    glm::mat4 m_quad_handle_transform;
    float     m_disc_handle_radius;
    float     m_quad_edge_length;
    float     m_h;
    float     m_s;
    float     m_v;
};

} // namespace ui
} // namespace renderstack

#endif
