#include "scene/scene_manager.hpp"
#include "renderstack_scene/camera.hpp"

using namespace renderstack::toolkit;
using namespace renderstack::scene;
using namespace std;

scene_manager::scene_manager()
:  service("scene_manager")
//,  m_material_manager(nullptr)
,  m_camera          (nullptr)
,  m_camera_controls (nullptr)
{
}

/* virtual */ scene_manager::~scene_manager()
{
}

void scene_manager::connect(/*shared_ptr<material_manager> material_manager_*/)
{
   /*m_material_manager = material_manager_;

   initialization_depends_on(material_manager_);
   */
}

/*virtual*/ void scene_manager::initialize_service()
{
   add_floor(20.0f);
   add_simple_scene();

   initialize_cameras();
}

void scene_manager::add_floor(float size)
{
   (void) size;
   // TODO
}

void scene_manager::add_simple_scene()
{
   // TODO
}

void scene_manager::initialize_cameras()
{
   m_camera = make_shared<renderstack::scene::camera>();
   m_camera->projection().set_fov_y(degrees_to_radians(50.0f));
   m_camera->projection().set_projection_type(renderstack::scene::projection_type::perspective_vertical);
   m_camera->projection().set_near(0.02f);
   m_camera->projection().set_far(40.0f);

   glm::mat4 m;
   create_look_at(
      glm::vec3(0.0f, 4.0f, 15.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f),
      m
   );
   m_camera->frame()->parent_from_local().set(m);

   //m_camera_controls->s
}

shared_ptr<model> scene_manager::add(shared_ptr<model> m)
{
   if (!m)
      throw runtime_error("scene_manager::add() no model to add");

   m_models->push_back(m);

   return m;
}

shared_ptr<light> scene_manager::add(shared_ptr<light> l)
{
   if (!l)
      throw runtime_error("scene_manager::add() no light to add");

   m_lights->push_back(l);

   return l;
}

shared_ptr<vector<shared_ptr<light> > > &scene_manager::lights()
{
   return m_lights;
}

shared_ptr<vector<shared_ptr<light> > > const &scene_manager::lights() const
{
   return m_lights;
}

