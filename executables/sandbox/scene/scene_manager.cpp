#include "scene/scene_manager.hpp"

using namespace renderstack::toolkit;
using namespace std;

scene_manager::scene_manager()
:  service("scene_manager")
,  m_material_manager(nullptr)
,  m_camera          (nullptr)
,  m_camera_controls (nullptr)
,  m_render_group    (nullptr)
{
}

/* virtual */ scene_manager::~scene_manager()
{
}

void scene_manager::connect(shared_ptr<material_manager> material_manager_)
{
   m_material_manager = material_manager_;

   initialization_depends_on(material_manager_);
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

shared_ptr<model> scene_manager::add_model(shared_ptr<model> m)
{
   if (!m)
      throw runtime_error("add_model(nullptr)");

   m_render_group->add(m);

   return m;
}
