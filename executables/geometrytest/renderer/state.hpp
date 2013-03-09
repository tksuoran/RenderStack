#ifndef state_hpp
#define state_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_mesh/mesh_mode.hpp"
#include "renderstack_scene/frame.hpp"
#include "renderer/material.hpp"
#include <memory>

namespace renderstack { namespace graphics {
   class program;
   class index_buffer;
   class vertex_buffer;
   class vertex_stream;
} }
namespace renderstack { namespace scene {
   class camera;
} } 

class viewport;
class material;

class state
{
private:
   std::shared_ptr<renderstack::scene::camera>  m_camera;
   std::shared_ptr<viewport>                    m_viewport;
   std::shared_ptr<renderstack::mesh::mesh>     m_mesh;
   renderstack::mesh::mesh_mode                 m_mesh_mode;
   std::shared_ptr<material>                    m_material;

public:
   state();
   state(state const &other);
};

#endif
