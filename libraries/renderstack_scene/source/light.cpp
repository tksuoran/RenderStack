#include "renderstack_scene/light.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"

namespace renderstack { namespace scene {

using namespace std;
using namespace glm;
using namespace renderstack::graphics;

mat4 light::s_texture = mat4(
   0.5f, 0.0f, 0.0f, 0.5f,
   0.0f, 0.5f, 0.0f, 0.5f,
   0.0f, 0.0f, 0.5f, 0.5f,
   0.0f, 0.0f, 0.0f, 1.0f
);
mat4 light::s_texture_inverse = mat4(
   2.0f, 0.0f, 0.0f, -1.0f,
   0.0f, 2.0f, 0.0f, -1.0f,
   0.0f, 0.0f, 2.0f, -1.0f,
   0.0f, 0.0f, 0.0f,  1.0f
);

lights_uniforms::lights_uniforms(
   renderstack::graphics::renderer &renderer,
   string const &name,
   string const &block_name,
   int binding_point,
   int max_light_count
)
{
   m_uniform_block = make_shared<uniform_block>(binding_point, name, block_name);
   m_uniform_block->add_int(spec.count);
   m_uniform_block->add_vec4(spec.exposure);
   m_uniform_block->add_vec4(spec.bias);
   m_uniform_block->add_vec4(spec.ambient_light_color);
   m_uniform_block->add_vec4(spec.color,               max_light_count);
   m_uniform_block->add_vec4(spec.direction,           max_light_count);
   m_uniform_block->add_mat4(spec.light_from_world,    max_light_count);
   m_uniform_block->add_mat4(spec.shadow_from_world,   max_light_count);
   m_uniform_block->seal();

   m_uniform_buffer = make_shared<buffer>(
      buffer_target::uniform_buffer,
      m_uniform_block->size_bytes(),
      1
   );
   m_uniform_buffer->allocate_storage(renderer);

   m_uniform_buffer_range = make_shared<uniform_buffer_range>(
      m_uniform_block,
      m_uniform_buffer
   );
}

light::light()
:  m_camera(nullptr)
,  m_shadow_from_world(mat4(1.0f), mat4(1.0f))
{
   m_camera = make_shared<class camera>();
}

shared_ptr<camera> light::camera()
{
   return m_camera;
}
string const &light::name()
{
   return m_camera->name();
}
void light::set_name(string const &value)
{
   m_camera->set_name(value);
}
shared_ptr<frame> light::frame()
{
   return m_camera->frame();
}
class projection const &light::projection() const
{
   return m_camera->projection();
}
class projection &light::projection()
{
   return m_camera->projection();
}
transform const &light::shadow_from_world() const
{
   return m_shadow_from_world;
}
transform &light::shadow_from_world()
{
   return m_shadow_from_world;
}

void light::update(viewport &viewport)
{
   m_camera->update(viewport);

   // world from light = world from view
   m_shadow_from_world.set(
      s_texture * camera()->clip_from_world().matrix(),
      camera()->clip_from_world().inverse_matrix() * s_texture_inverse
   );
}

} }


