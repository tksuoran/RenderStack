#ifndef group_hpp_scene
#define group_hpp_scene

#include "renderstack_toolkit/platform.hpp"
#include <vector>
#include <memory>

class model;

class group
{
public:
   void clear()
   {
      m_models.clear();
   }

   void add(std::shared_ptr<model> model);

   std::vector<std::shared_ptr<model> >       &models() { return m_models; }
   std::vector<std::shared_ptr<model> > const &models() const { return m_models; }
   bool visible() const { return m_visible; }
   void set_visible(bool value) { m_visible = value; }

private:
   std::vector<std::shared_ptr<model> >   m_models;
   bool                                   m_visible;
};

#endif
