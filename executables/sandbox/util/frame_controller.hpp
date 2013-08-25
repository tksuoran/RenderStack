#ifndef frame_controller_hpp_util
#define frame_controller_hpp_util

#include "renderstack_toolkit/platform.hpp"
#include "util/controller.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace renderstack { namespace scene {
   class frame;
} }

class frame_controller
{
public:
   frame_controller();

   void set_frame(std::shared_ptr<renderstack::scene::frame> value);
   std::shared_ptr<renderstack::scene::frame> frame();

   void clear();
   void update();
   void update_fixed_step();

   void              set_position   (glm::vec3 const &position);
   void              set_elevation  (float value);
   void              set_heading    (float value);
   glm::vec3 const   &position      () const;
   float             elevation      () const;
   float             heading        () const;
   glm::vec3         right          () const;
   glm::vec3         up             () const;
   glm::vec3         back           () const;
   //glm::mat4 const   &parent_from_local() const { return m_parent_from_local; }
   //glm::mat4 const   &local_from_parent() const { return m_local_from_parent; }

   controller &rotate_x        ();
   controller &rotate_y        ();
   controller &rotate_z        ();
   controller &translate_x     ();
   controller &translate_y     ();
   controller &translate_z     ();
   controller &speed_modifier  ();

private:
   std::shared_ptr<renderstack::scene::frame>   m_frame;
   float       m_elevation;
   float       m_heading;
   glm::mat4   m_heading_matrix;
   glm::mat4   m_rotation_matrix;
   glm::vec3   m_position;
   controller  m_rotate_x;
   controller  m_rotate_y;
   controller  m_rotate_z;
   controller  m_translate_x;
   controller  m_translate_y;
   controller  m_translate_z;
   controller  m_speed_modifier;
};

#endif
