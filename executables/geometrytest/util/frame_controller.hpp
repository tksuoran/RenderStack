#ifndef frame_controller_hpp
#define frame_controller_hpp

#include "renderstack_toolkit/platform.hpp"
#include "util/controller.hpp"
#include <glm/glm.hpp>

class frame_controller
{
private:
   float       m_elevation;
   float       m_heading;
   glm::mat4   m_heading_matrix;
   glm::mat4   m_rotation_matrix;
   glm::mat4   m_local_to_parent;
   glm::mat4   m_parent_to_local;
   glm::vec3   m_position;
   controller  m_rotate_x;
   controller  m_rotate_y;
   controller  m_rotate_z;
   controller  m_translate_x;
   controller  m_translate_y;
   controller  m_translate_z;
   controller  m_speed_modifier;

public:
   frame_controller();

   void clear();
   void update();
   void update_fixed_step();

   void              set_position      (glm::vec3 const &position);
   void              set_elevation     (float value);
   void              set_heading       (float value);
   glm::vec3 const   &position         () const;
   float             elevation         () const;
   float             heading           () const;
   glm::vec3         right             () const;
   glm::vec3         up                () const;
   glm::vec3         back              () const;
   glm::mat4 const   &local_to_parent  () const { return m_local_to_parent; }
   glm::mat4 const   &parent_to_local  () const { return m_parent_to_local; }

   controller &rotate_x        ();
   controller &rotate_y        ();
   controller &rotate_z        ();
   controller &translate_x     ();
   controller &translate_y     ();
   controller &translate_z     ();
   controller &speed_modifier  ();
};

#endif
