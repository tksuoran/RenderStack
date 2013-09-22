#ifndef scene_manager_hpp_scene
#define scene_manager_hpp_scene

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "scene/material_manager.hpp"
#include "util/frame_controller.hpp"
#include <memory>
#include <vector>

namespace renderstack { namespace scene {
   class camera;
   class light;
} }

class model;

/*class scene
{
private:
   void add(std::shared_ptr<group> group);

   std::vector<std::shared_ptr<group> >         &groups() { return m_groups; }
   std::vector<std::shared_ptr<group> > const   &groups() const { return m_groups; }

private:
   std::vector<std::shared_ptr<group> >   m_groups;
};*/

class scene_manager : public renderstack::toolkit::service
{
public:
   scene_manager();
   /* virtual */ ~scene_manager();

   void connect(/*std::shared_ptr<material_manager> material_manager*/);

   /*virtual*/ void initialize_service();

   void add_floor(float size);
   void add_simple_scene();

   void initialize_cameras();

   std::shared_ptr<model> add(std::shared_ptr<model> m);
   std::shared_ptr<renderstack::scene::light> add(std::shared_ptr<renderstack::scene::light> l);

   std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > >       &models();
   std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > > const &models() const;

   std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > >       &lights();
   std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > > const &lights() const;

private:
   //std::shared_ptr<material_manager>            m_material_manager;
   std::shared_ptr<renderstack::scene::camera>                                m_camera;
   std::shared_ptr<frame_controller>                                          m_camera_controls;

   std::shared_ptr<std::vector<std::shared_ptr<model> > >                     m_models;
   std::shared_ptr<std::vector<std::shared_ptr<renderstack::scene::light> > > m_lights;
};



#endif

