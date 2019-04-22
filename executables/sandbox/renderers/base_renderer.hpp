#ifndef base_renderer_hpp_renderers
#define base_renderer_hpp_renderers

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include "main/programs.hpp"
#include "renderers/quad_renderer.hpp"
#include "renderers/light_mesh.hpp"
#include "scene/model.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <array>

#define NUM_UNIFORM_BUFFERS 3

namespace renderstack
{
   namespace graphics
   {
      class uniform_buffer;
      class uniform_buffer_range;
   }
   namespace mesh
   {
      class geometry_mesh;
   }
   namespace scene
   {
      class camera;
      class light;
      class viewport;
   }
}

class ring_uniform_buffer
{
public:
   ring_uniform_buffer()
   :  m_current_buffer(0)
   {
      for (std::size_t i = 0; i < NUM_UNIFORM_BUFFERS; ++i)
         m_storage[i] = nullptr;
   }

   void initialize(
      renderstack::graphics::renderer &renderer,
      std::size_t num,
      std::size_t stride
   )
   {
      for (std::size_t i = 0; i < NUM_UNIFORM_BUFFERS; ++i)
      {
         m_storage[i] = std::make_shared<renderstack::graphics::buffer>(
            renderstack::graphics::buffer_target::uniform_buffer,
            num,
            stride,
            gl::buffer_usage_hint::stream_draw
         );
         m_storage[i]->allocate_storage(renderer);
      }
   }

   std::shared_ptr<renderstack::graphics::buffer> &current_buffer()
   {
      return m_storage[m_current_buffer];
   }

   void advance()
   {
      m_current_buffer = (m_current_buffer + 1) % NUM_UNIFORM_BUFFERS;
   }

private:
   std::array<
      std::shared_ptr<renderstack::graphics::buffer>,
      NUM_UNIFORM_BUFFERS
   >              m_storage;
   std::size_t    m_current_buffer;
};

enum uniform_buffer_usage
{
   models        = 0,
   lights        = 1,
   light_models  = 2,
   materials     = 3,
   camera        = 4,
   count         = 5
};

class light_mesh;
class material;
class programs;

class base_renderer
{
public:
   base_renderer();
   /*virtual*/ ~base_renderer();

   void set_max_lights(std::size_t max_lights);
   std::size_t max_lights() { return m_max_lights; }

   bool  use_stencil    () const { return m_use_stencil; }
   int   scale          () const { return m_scale; }
   void  set_use_stencil(bool value);
   void  set_scale      (int value);
   void  base_resize    (int width, int height);
   int   width          ();
   int   height         ();
   int   width_full     () { return m_width_full; }
   int   height_full    () { return m_height_full; }

   renderstack::graphics::renderer   &renderer();
   std::shared_ptr<class programs>   programs();
   std::shared_ptr<class light_mesh> light_mesh() { return m_light_mesh; }

   void bind_camera     ();
   void bind_model      (std::size_t model_index);
   void bind_light_model(std::size_t light_index);
   void bind_light      (std::size_t light_index);
   void bind_material   (std::size_t material_index);

   bool point_in_light(std::shared_ptr<renderstack::scene::light> l);

protected:
   void base_connect(
      std::shared_ptr<renderstack::graphics::renderer>   renderer_,
      std::shared_ptr<class programs>                    programs_,
      std::shared_ptr<class light_mesh>                  light_mesh
   );
   void base_initialize_service();

protected:
   unsigned char *begin_edit(uniform_buffer_usage buffer, std::size_t count);

   void end_edit(uniform_buffer_usage buffer);

   void update_models(std::vector<std::shared_ptr<model> > const &models,
                      std::shared_ptr<renderstack::scene::camera> camera);

   void update_lights_models(std::vector<std::shared_ptr<renderstack::scene::light> > const &lights,
                             std::shared_ptr<renderstack::scene::camera> camera);

   void update_lights(std::vector<std::shared_ptr<renderstack::scene::light> > const &lights,
                      std::shared_ptr<renderstack::scene::camera> camera);
                      
   void update_materials(std::vector<std::shared_ptr<material> > const &materials);
   void update_camera(std::shared_ptr<renderstack::scene::camera> camera);

private:
   std::shared_ptr<renderstack::graphics::renderer>               m_renderer;
   std::shared_ptr<class programs>                                m_programs;
   std::shared_ptr<class light_mesh>                              m_light_mesh;

   std::array<ring_uniform_buffer, uniform_buffer_usage::count>   m_uniform_buffers;

   int                                                            m_width;
   int                                                            m_height;
   int                                                            m_width_full;
   int                                                            m_height_full;

   std::size_t                                                    m_max_lights;
   bool                                                           m_use_stencil;
   int                                                            m_scale;
};


#endif
