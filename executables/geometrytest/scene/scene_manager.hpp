#ifndef scene_manager_hpp_scene
#define scene_manager_hpp_scene

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "renderstack_scene/camera.hpp"
#include "scene/group.hpp"
#include "scene/material_manager.hpp"
#include "util/frame_controller.hpp"
#include <memory>


class scene
{
private:
   void add(std::shared_ptr<group> group);

   std::vector<std::shared_ptr<group> >         &groups() { return m_groups; }
   std::vector<std::shared_ptr<group> > const   &groups() const { return m_groups; }

private:
   std::vector<std::shared_ptr<group> >   m_groups;
};

class scene_manager : public renderstack::toolkit::service
{
public:
   scene_manager();
   /* virtual */ ~scene_manager();

   void connect(std::shared_ptr<material_manager> material_manager);

   /*virtual*/ void initialize_service();

   void add_floor(float size);
   void add_simple_scene();

   void initialize_cameras();

   std::shared_ptr<model> add_model(std::shared_ptr<model> m);

private:
   std::shared_ptr<material_manager>            m_material_manager;
   std::shared_ptr<renderstack::scene::camera>  m_camera;
   std::shared_ptr<frame_controller>            m_camera_controls;
   std::shared_ptr<group>                       m_render_group;
};



#endif

