#ifndef renderer_hpp
#define renderer_hpp

#include "renderstack_toolkit/platform.hpp"
#include <memory>

namespace renderstack { namespace graphics {
   class uniform_block;
   class uniform_buffer;
   class uniform_buffer_range;
} }
namespace renderstack { namespace scene {
   class camera;
   class frame;
} } 
namespace renderstack { namespace mesh {
   class mesh;
} } 

class renderer
{
private:
   std::shared_ptr<renderstack::graphics::uniform_block>          m_global_ub;
   std::shared_ptr<renderstack::graphics::uniform_block>          m_models_ub;
   std::shared_ptr<renderstack::graphics::uniform_block>          m_material_ub;
   std::shared_ptr<renderstack::graphics::uniform_buffer>         m_uniform_buffer;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_global;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_models;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_material;

   //std::shared_ptr<renderstack::scne

   // TODO: Timers
   // TODO: Default frame?

public:
   void push();
   void pop();

   // Set a single frame to be used when rendering current state. Not used when using instanced rendering
   void set_frame(std::shared_ptr<renderstack::scene::frame> frame);
   void update_camera();
   void handle_resize();



};

#endif
