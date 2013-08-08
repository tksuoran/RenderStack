#include "renderstack_toolkit/math_util.hpp"
#include "logic/reference_frame.hpp"

using namespace renderstack::toolkit;
using namespace glm;

reference_frame::reference_frame()
{
}

float reference_frame::scale() const
{
   vec3 delta = m_first_position - m_last_position;
   return length(delta);
}

void reference_frame::transform_by(glm::mat4 const &m)
{
   m_center          = vec3(m * vec4(m_center, 1.0f));
   m_normal          = normalize(vec3(m * vec4(m_normal, 0.0f)));
   m_last_position   = vec3(m * vec4(m_last_position, 1.0f));
   m_first_position  = vec3(m * vec4(m_first_position, 1.0f));
}

mat4 reference_frame::get_transform() const
{
   mat4 transform(1.0f);

   vec3 view = m_normal;
   
   vec3 edge;
   
   if (length(edge) > 0.00001f)
      edge = normalize(m_first_position - m_center);
   else
      edge = min_axis(m_normal);

   vec3 side = normalize(cross(view, edge));

   /*  Right axis is column 0  */ 
   transform[0].x = edge.x;
   transform[0].y = edge.y;
   transform[0].z = edge.z;

   /*  Up    axis is column 1  */ 
   transform[1].x = side.x;
   transform[1].y = side.y;
   transform[1].z = side.z;

   /*  View  axis is column 2  */ 
   transform[2].x = view.x;
   transform[2].y = view.y;
   transform[2].z = view.z;

   /*  Translation is column 3  */ 
   transform[3].x = m_center.x;
   transform[3].y = m_center.y;
   transform[3].z = m_center.z;
   return transform;
};



