#ifndef material_hpp_scene
#define material_hpp_scene
   
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"

#include <memory>
#include <map>
#include <string>

class material
{
public:
   material(
      std::size_t index,
      std::string const &name,
      glm::vec4 color,
      float roughness,
      float isotropy
   );
   
   std::size_t index() const { return m_index; }
   std::string const &name() const { return m_name; }
   glm::vec4 const &color() const { return m_color; }
   float roughness() const { return m_roughness; }
   float isotropy() const { return m_isotropy; }

private:
   std::size_t m_index;
   std::string m_name;
   glm::vec4   m_color;
   float       m_roughness;
   float       m_isotropy;
};

#endif
