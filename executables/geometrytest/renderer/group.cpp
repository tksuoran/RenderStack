#include "renderer/group.hpp"

#if 0
void instances::clear()
{
}
bool instances::contains(key const &k)
{
   (void)k;
   // TODO
   return false;
}

instances::instance_collection const &instances::collection() const
{
   return m_collection;
}
instances::instance_collection &instances::collection()
{
   return m_collection;
}
#endif

void group::add(std::shared_ptr<model> model)
{
   m_models.push_back(model);
}
void group::add(std::shared_ptr<renderstack::scene::camera> camera)
{
   m_cameras.push_back(camera);
}
void group::add(std::shared_ptr<renderstack::scene::light> light)
{
   m_lights.push_back(light);
}


