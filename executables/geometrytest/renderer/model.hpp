#ifndef model_hpp
#define model_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_scene/frame.hpp"
#include "renderer/material.hpp"
#include <memory>

class model
{
public:
   model();
   model(std::string const &name);
   model(
      std::string const &name,
      std::shared_ptr<renderstack::mesh::geometry_mesh> mesh,
      glm::mat4 transform
   );

   std::shared_ptr<renderstack::scene::frame> frame() const { return m_frame; }
   std::string const &name() { return m_name; }
   std::shared_ptr<class renderstack::mesh::geometry_mesh> geometry_mesh() { return m_geometry_mesh; }
   void set_geometry_mesh(std::shared_ptr<renderstack::mesh::geometry_mesh> value) { m_geometry_mesh = value; }

private:
   std::shared_ptr<renderstack::scene::frame>               m_frame;
   std::shared_ptr<class renderstack::mesh::geometry_mesh>  m_geometry_mesh;
   std::string                                              m_name;
};

#endif







#if 0
   // std::shared_ptr<material>              m_material;
   // bool                                   m_selected;
   // m_bounding_box;
   // m_bounding_sphere
   struct {
      // shape
      // rigidbody
      bool m_static;
      bool m_use_position;
      bool m_use_rotation;
   } physics;

   struct {
      bool m_shadow_caster;
   } effects;
#endif
