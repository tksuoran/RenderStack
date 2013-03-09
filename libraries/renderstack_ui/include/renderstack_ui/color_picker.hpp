#ifndef renderstack_ui__color_picker_hpp
#define renderstack_ui__color_picker_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_ui/area.hpp"
#include "renderstack_ui/ninepatch.hpp"
#include <memory>

namespace renderstack { namespace mesh {

   class mesh;
   class geometry_mesh;

} }

namespace renderstack { namespace ui {

class color_picker : public area
{
public:
   color_picker(std::shared_ptr<class style> style);
   virtual ~color_picker(){}

   void animate();

   void begin_place(rectangle const &reference, glm::vec2 const &grow_direction);
   void draw_self(ui_context &context);

private:
   std::shared_ptr<renderstack::mesh::mesh>           m_disc;
   std::shared_ptr<renderstack::mesh::mesh>           m_triangle;
   ninepatch                                          m_ninepatch;
   float                                              m_size;
   std::shared_ptr<renderstack::mesh::geometry_mesh>  m_hsv_disc_mesh;
   std::shared_ptr<renderstack::mesh::geometry_mesh>  m_hsv_disc2_mesh;
   std::shared_ptr<renderstack::mesh::geometry_mesh>  m_hsv_quad_mesh;
   std::shared_ptr<renderstack::mesh::geometry_mesh>  m_color_mesh;
   std::shared_ptr<renderstack::mesh::geometry_mesh>  m_handle_mesh;
   std::shared_ptr<renderstack::mesh::geometry_mesh>  m_handle2_mesh;
   glm::mat4                                          m_background_frame;
   glm::mat4                                          m_hsv_transform;
   glm::mat4                                          m_disc_handle_transform;
   glm::mat4                                          m_quad_handle_transform;
   float                                              m_disc_handle_radius;
   float                                              m_quad_edge_length;
   float                                              m_h;
   float                                              m_s;
   float                                              m_v;
};

} }

#endif
