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
      std::string const                               &name,
      std::shared_ptr<renderstack::graphics::program> program
   );
   
   void use();

private:
   std::string                                     m_name;
   std::shared_ptr<renderstack::graphics::program> m_program;
   glm::vec4                                       m_color;
   float                                           m_roughness;
   float                                           m_isotropy;
};

#endif
