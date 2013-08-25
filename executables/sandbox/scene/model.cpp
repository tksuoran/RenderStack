#include "scene/model.hpp"

using namespace renderstack::mesh;
using namespace std;
using namespace glm;

model::model()
:  m_name         ("")
,  m_frame        (nullptr)
,  m_geometry_mesh(nullptr)
,  m_material     (nullptr)
{
   m_frame = make_shared<renderstack::scene::frame>();
}

model::model(string const &name)
:  m_name         (name)
,  m_frame        (nullptr)
,  m_geometry_mesh(nullptr)
,  m_material     (nullptr)
{
   m_frame = make_shared<renderstack::scene::frame>();
}

model::model(
   string const &name,
   shared_ptr<class geometry_mesh> geometry_mesh,
   mat4 transform
)
:  m_name         (name)
,  m_frame        (nullptr)
,  m_geometry_mesh(geometry_mesh)
,  m_material     (nullptr)
{
   //m_frame.name = name + " frame";
   m_frame = make_shared<renderstack::scene::frame>();
   m_frame->parent_from_local().set(transform);
}