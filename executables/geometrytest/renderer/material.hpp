#ifndef material_hpp
#define material_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"

#include <memory>
#include <string>

class material
{
private:
   std::string                                                    m_name;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_uniform_buffer_range;
   std::shared_ptr<renderstack::graphics::program>                m_program;
   renderstack::graphics::render_states                           m_render_states;
   bool                                                           m_dirty;

public:
   material(
      std::string const &name,
      std::shared_ptr<renderstack::graphics::program> program,
      std::shared_ptr<renderstack::graphics::uniform_block> uniform_block
   );
   
   void use();
};

#endif
