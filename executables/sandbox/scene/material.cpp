#include "scene/material.hpp"

using namespace std;

material::material(
   size_t index,
   string const &name,
   glm::vec4 const &color,
   float roughness,
   float isotropy
)
:  m_index(index)
,  m_name(name)
,  m_color(color)
,  m_roughness(roughness)
,  m_isotropy(isotropy)
{
}
