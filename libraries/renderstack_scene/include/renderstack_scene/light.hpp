#ifndef light_hpp_renderstack_scene
#define light_hpp_renderstack_scene

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/projection.hpp"
#include "renderstack_scene/frame.hpp"
#include "renderstack_scene/transform.hpp"
#include <string>
#include <memory>

namespace renderstack { namespace graphics {

   class buffer;
   class uniform_block;
   class uniform_buffer_range;
   class renderer;

} }


namespace renderstack { namespace scene {

class lights_uniforms
{
public:
   std::shared_ptr<renderstack::graphics::uniform_block>          m_uniform_block;
   std::shared_ptr<renderstack::graphics::buffer>                 m_uniform_buffer;
   std::shared_ptr<renderstack::graphics::uniform_buffer_range>   m_uniform_buffer_range;

   lights_uniforms(
      renderstack::graphics::renderer &renderer,
      std::string const &name,
      std::string const &block_name,
      int binding_point,
      int max_light_count
   );

   struct spec
   {
      std::string count;
      std::string exposure;
      std::string bias;
      std::string ambient_light_color;
      std::string light_from_world;
      std::string shadow_from_world;
      std::string direction;
      std::string color;
   };

   static spec      spec;
};

class light
{
private:
   std::shared_ptr<class camera> m_camera;

public:
   std::shared_ptr<class camera> camera      ();
   std::string const             &name       ();
   void                          set_name    (std::string const &value);
   std::shared_ptr<class frame>  frame       ();
   class projection const        &projection () const;
   class projection              &projection ();

private:
   static glm::mat4 s_texture;
   static glm::mat4 s_texture_inverse;

   transform m_shadow_from_world;

   int m_light_index;

public:
   int               light_index() const;
   void              set_light_index(int value);
   transform const   &shadow_from_world() const;
   transform         &shadow_from_world();

public:
   light(int light_index);

   void update_frame();
   void update_viewport(viewport &viewport);
};

} }

#endif
