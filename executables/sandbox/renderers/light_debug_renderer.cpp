#include "renderstack_toolkit/platform.hpp"
#include "renderers/light_debug_renderer.hpp"
#include "renderers/light_mesh.hpp"
#include "renderstack_geometry/shapes/cone.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/uniform.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_scene/camera.hpp"
#include "renderstack_scene/light.hpp"
#include "renderstack_scene/viewport.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>


using namespace renderstack::toolkit;
using namespace renderstack::graphics;
using namespace renderstack::mesh;
using namespace renderstack::scene;
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;


light_debug_renderer::light_debug_renderer()
:  service("light_debug_renderer")
{
}

/*virtual*/ light_debug_renderer::~light_debug_renderer()
{
}

void light_debug_renderer::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<class programs>                   programs_,
   shared_ptr<class light_mesh>                 light_mesh_
)
{
   base_connect(renderer_, programs_, light_mesh_);
   
   initialization_depends_on(renderer_);
   initialization_depends_on(programs_);
}

void light_debug_renderer::initialize_service()
{
   base_initialize_service();

   m_debug_light_render_states.depth.set_enabled(true);
   m_debug_light_render_states.depth.set_function(gl::depth_function::l_equal);
   m_debug_light_render_states.face_cull.set_enabled(true);
   m_debug_light_render_states.blend.set_enabled(true);
   m_debug_light_render_states.blend.rgb().set_equation_mode(gl::blend_equation_mode::func_add);
   m_debug_light_render_states.blend.rgb().set_source_factor(gl::blending_factor_src::src_alpha);
   m_debug_light_render_states.blend.rgb().set_destination_factor(gl::blending_factor_dest::one);   
   m_debug_light_render_states.blend.alpha().set_equation_mode(gl::blend_equation_mode::func_add);
   m_debug_light_render_states.blend.alpha().set_source_factor(gl::blending_factor_src::one);
   m_debug_light_render_states.blend.alpha().set_destination_factor(gl::blending_factor_dest::one);   

}

void light_debug_renderer::light_pass(
   vector<shared_ptr<light> > const &lights,
   shared_ptr<class camera> camera
)
{
   gl::viewport(0, 0, width(), height());

   update_lights_models(lights, camera);
   update_lights(lights, camera);
   update_camera(camera);

   auto &r = renderer();
   auto &t = r.track();

   t.execute(&m_debug_light_render_states);
   r.set_program(programs()->debug_light);

   bind_camera();

   size_t light_index = 0;
   for (auto i = lights.cbegin(); i != lights.cend(); ++i)
   {
      auto l = *i;

      if (light_index == max_lights())
         break;

      auto geometry_mesh   = light_mesh()->get_light_mesh(l);
      auto vertex_stream   = geometry_mesh->vertex_stream();
      auto mesh            = geometry_mesh->get_mesh();

      bind_light_model(light_index);
      bind_light(light_index);

      gl::begin_mode::value         begin_mode     = gl::begin_mode::lines;
      index_range const             &index_range   = geometry_mesh->edge_line_indices();
      GLsizei                       count          = static_cast<GLsizei>(index_range.index_count);
      gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_int;
      GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>((index_range.first_index + mesh->first_index()) * mesh->index_buffer()->stride());
      GLint                         base_vertex    = configuration::can_use.draw_elements_base_vertex
         ? static_cast<GLint>(mesh->first_vertex())
         : 0;

      assert(index_range.index_count > 0);

      r.draw_elements_base_vertex(
         geometry_mesh->vertex_stream(),
         begin_mode, count, index_type, index_pointer, base_vertex);

      ++light_index;
   }
}

