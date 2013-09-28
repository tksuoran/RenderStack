#ifndef scene_manager_hpp_scene
#define scene_manager_hpp_scene

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "scene/material_manager.hpp"
#include "renderstack_mesh/build_info.hpp"
#include "util/frame_controller.hpp"
#include <memory>
#include <vector>

namespace renderstack { namespace geometry {
   class geometry;
} }

namespace renderstack { namespace scene {
   class camera;
   class light;
} }

class model;
class programs;

class scene_manager : public renderstack::toolkit::service
{
public:
   scene_manager();
   /* virtual */ ~scene_manager();

   void connect(
      std::shared_ptr<programs>                          programs_,
      std::shared_ptr<renderstack::graphics::renderer>   renderer_
   );

   /*virtual*/ void initialize_service();

   void add_floor(float size);
   void add_simple_scene();

   void initialize_cameras();

   std::shared_ptr<model>                       add(std::shared_ptr<model> m);
   std::shared_ptr<renderstack::scene::light>   add(std::shared_ptr<renderstack::scene::light> l);

   std::shared_ptr<renderstack::scene::camera>  camera() const { return m_camera; }

   std::shared_ptr<std::vector<std::shared_ptr<model> > >       &models();
   std::shared_ptr<std::vector<std::shared_ptr<model> > > const &models() const;

   std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > >       &lights();
   std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > > const &lights() const;

private:
   void reset_build_info();
   std::shared_ptr<model> make_model(
      std::shared_ptr<renderstack::scene::frame>         parent,
      std::shared_ptr<renderstack::geometry::geometry>   geometry,
      glm::vec3                                          position = glm::vec3(0.0f)
   );

private:
   // services
   std::shared_ptr<programs>                          m_programs;
   std::shared_ptr<renderstack::graphics::renderer>   m_renderer;

   // self owned parts
   std::shared_ptr<std::vector<std::shared_ptr<model> > >                     m_models;
   std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > > m_lights;
   std::shared_ptr<renderstack::scene::camera>                                m_camera;
   std::shared_ptr<frame_controller>                                          m_camera_controls;

   renderstack::mesh::geometry_mesh_format_info m_format_info;
   renderstack::mesh::geometry_mesh_buffer_info m_buffer_info;
};



#endif

