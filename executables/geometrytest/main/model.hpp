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

class group
{
public:
   void clear()
   {
      m_models.clear();
   }

   void add(std::shared_ptr<model> model)
   {
      m_models.push_back(model);
   }

   std::vector<std::shared_ptr<model> >         &models() { return m_models; }
   std::vector<std::shared_ptr<model> > const   &models() const { return m_models; }

private:
   std::vector<std::shared_ptr<model> > m_models;
};

class scene
{
private:
   void add(std::shared_ptr<group> group);

   std::vector<std::shared_ptr<group> >         &groups() { return m_groups; }
   std::vector<std::shared_ptr<group> > const   &groups() const { return m_groups; }

private:
   std::vector<std::shared_ptr<group> >   m_groups;

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
