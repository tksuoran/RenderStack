#ifndef group_hpp
#define group_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/light.hpp"
#include "renderer/model.hpp"
#include "renderer/material.hpp"
#include <vector>
#include <memory>
#include <map>

#if 0
class instances
{
   typedef std::pair<
      renderstack::mesh::mesh,
      material
   > key;

   typedef std::map<key, std::vector<model> > instance_collection;

private:
   instance_collection m_collection;

public:
   void clear();
   bool contains(key const &k);
   instance_collection const &collection() const;
   instance_collection &collection();
};
#endif

// Group arranges models into sets for renderin
class group
{
private:
   std::vector<std::shared_ptr<model> >                        m_models;
   std::vector<std::shared_ptr<renderstack::scene::camera> >   m_cameras;
   std::vector<std::shared_ptr<renderstack::scene::light> >    m_lights;

public:
   void add(std::shared_ptr<model> model);
   void add(std::shared_ptr<renderstack::scene::camera> camera);
   void add(std::shared_ptr<renderstack::scene::light> light);

#if 0
   instances   m_opaque_instances;
   instances   m_transparent_instances;
   instances   m_all_instances;
#endif
};

#endif
