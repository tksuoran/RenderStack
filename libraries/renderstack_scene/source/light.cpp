#include "renderstack_scene/light.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_scene/camera.hpp"

namespace renderstack
{
namespace scene
{

using namespace std;
using namespace glm;
using namespace renderstack::graphics;

mat4 Light::s_texture = mat4(0.5f, 0.0f, 0.0f, 0.5f,
                             0.0f, 0.5f, 0.0f, 0.5f,
                             0.0f, 0.0f, 0.5f, 0.5f,
                             0.0f, 0.0f, 0.0f, 1.0f);

mat4 Light::s_texture_inverse = mat4(2.0f, 0.0f, 0.0f, -1.0f,
                                     0.0f, 2.0f, 0.0f, -1.0f,
                                     0.0f, 0.0f, 2.0f, -1.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f);

#if 0
lights_uniforms::lights_uniforms(
   renderstack::graphics::renderer &renderer,
   const std::string &name,
   const std::string &block_name,
   int binding_point,
   int max_light_count
)
{
   m_uniform_block = make_shared<Uniform_block>(binding_point, name, block_name);
   m_uniform_block->add_int(spec.count);
   //m_uniform_block->add_vec4(spec.exposure);
   //m_uniform_block->add_vec4(spec.bias);
   //m_uniform_block->add_vec4(spec.ambient_light_color);
   m_uniform_block->add_vec4(spec.color,               max_light_count);
   m_uniform_block->add_vec4(spec.direction,           max_light_count);
   m_uniform_block->add_mat4(spec.light_from_world,    max_light_count);
   m_uniform_block->add_mat4(spec.shadow_from_world,   max_light_count);
   m_uniform_block->seal();

   m_uniform_buffer = make_shared<Buffer>(
      Buffer::Target::uniform_buffer,
      m_uniform_block->size_bytes(),
      1
   );
   m_uniform_buffer->allocate_storage(renderer);

   m_uniform_buffer_range = make_shared<Uniform_buffer_range>(
      m_uniform_block,
      m_uniform_buffer
   );
}
#endif


void Light::update(Viewport viewport)
{
    camera.update(viewport);

    // world from light = world from view
    shadow_from_world.set(s_texture * camera.clip_from_world.matrix(),
                          camera.clip_from_world.inverse_matrix() * s_texture_inverse);
}

} // namespace scene
} // namespace renderstack
