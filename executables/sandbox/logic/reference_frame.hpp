#ifndef reference_frame_hpp_logic
#define reference_frame_hpp_logic

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>

class reference_frame
{
public:
   reference_frame();

   float scale() const;

   void transform_by(glm::mat4 const &m);
   glm::mat4 get_transform() const;

private:
   glm::vec3 m_center;
   glm::vec3 m_normal;
   glm::vec3 m_last_position;
   glm::vec3 m_first_position;
};


#endif

