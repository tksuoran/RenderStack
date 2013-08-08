#include "renderstack_toolkit/platform.hpp"
#include "renderers/id_renderer.hpp"
#include "scene/model.hpp"
#include "scene/group.hpp"
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
using namespace renderstack;
using namespace gl;
using namespace glm;
using namespace std;


id_renderer::id_renderer()
:  service("id_renderer")
,  m_renderer(nullptr)
,  m_programs(nullptr)
,  m_last_render(0)
,  m_radius(64)
{
}

/*virtual*/ id_renderer::~id_renderer()
{
}

void id_renderer::connect(
   shared_ptr<renderstack::graphics::renderer>  renderer_,
   shared_ptr<programs>                         programs_
)
{
   m_renderer = renderer_;
   m_programs = programs_;

   initialization_depends_on(renderer_);
   initialization_depends_on(programs_);
}

void id_renderer::initialize_service()
{
   assert(m_renderer);
   assert(m_programs);

   m_id_render_states.depth.set_enabled(true);
   m_id_render_states.face_cull.set_enabled(true);

   for (int i = 0; i < 4; ++i)
   {
      id_render &idr = m_renders[i];

      idr.pixel_pack_buffer = make_shared<buffer>(
         renderstack::graphics::buffer_target::pixel_pack_buffer,
         m_radius * m_radius * 8,
         1,
         gl::buffer_usage_hint::stream_read
      );
      idr.pixel_pack_buffer->allocate_storage(*m_renderer);
      idr.state = id_render_state::unused;
      idr.data.resize(m_radius * m_radius * 8);
  }

}

void id_renderer::clear()
{
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   m_ranges.clear();
}

void id_renderer::render_pass(
   shared_ptr<class group> group,
   mat4 const &clip_from_world,
   double time,
   int x,
   int y
)
{
   auto const &models = group->models();

   if (models.size() == 0)
      return;

   auto &r = *m_renderer;
   auto &t = r.track();
   auto p = m_programs->id;

   id_render &idr = m_renders[m_last_render];
   idr.time = time;
   idr.x_offset = std::max(x - (m_radius / 2), 0);
   idr.y_offset = std::max(y - (m_radius / 2), 0);
   idr.clip_from_world = clip_from_world;

   gl::scissor(idr.x_offset, idr.y_offset, m_radius, m_radius);
   gl::enable(GL_SCISSOR_TEST);

   t.execute(&m_id_render_states);
   r.set_program(p);
   
   vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
   if (m_programs->use_uniform_buffers())
   {
      assert(m_programs);

      unsigned char *start          = m_programs->begin_edit_uniforms();
      unsigned char *material_start = &start[m_programs->material_ubr->first_byte()];
      ::memcpy(&material_start[m_programs->material_block_access.color], value_ptr(white), 4 * sizeof(float));
      m_programs->material_ubr->flush(r);
      m_programs->end_edit_uniforms();
   }
   else
   {
      gl::uniform_4fv(p->uniform_at(m_programs->material_block_access.color), 1, value_ptr(white));
   }

   uint32_t id_offset = 0;
   for (auto i = models.cbegin(); i != models.cend(); ++i)
   {
      auto model              = *i;
      mat4 world_from_model   = model->frame()->world_from_local().matrix();
      mat4 clip_from_model    = clip_from_world * world_from_model;
      auto geometry_mesh      = model->geometry_mesh();
      auto vertex_stream      = geometry_mesh->vertex_stream();
      auto mesh               = geometry_mesh->get_mesh();

      gl::begin_mode::value         begin_mode     = gl::begin_mode::triangles;
      index_range const             &index_range   = geometry_mesh->fill_indices();
      GLsizei                       count          = static_cast<GLsizei>(index_range.index_count);
      gl::draw_elements_type::value index_type     = gl::draw_elements_type::unsigned_int;
      GLvoid                        *index_pointer = reinterpret_cast<GLvoid*>((index_range.first_index + mesh->first_index()) * mesh->index_buffer()->stride());
      GLint                         base_vertex    = configuration::can_use.draw_elements_base_vertex
         ? static_cast<GLint>(mesh->first_vertex())
         : 0;

      uint32_t power_of_two   = next_power_of_two(count);
      uint32_t mask           = power_of_two - 1;
      uint32_t current_bits   = id_offset & mask;
      if (current_bits != 0)
      {
         uint add = power_of_two - current_bits;
         id_offset += add;
      }

      vec3 id_offset_vec3 = vec3_from_uint(id_offset);

      if (m_programs->use_uniform_buffers())
      {
         unsigned char *start = m_programs->begin_edit_uniforms();
         ::memcpy(&start[m_programs->model_ubr->first_byte() + m_programs->model_block_access.clip_from_model], value_ptr(clip_from_model),  16 * sizeof(float));
         ::memcpy(&start[m_programs->model_ubr->first_byte() + m_programs->model_block_access.id_offset      ], value_ptr(id_offset_vec3),    3 * sizeof(float));
         m_programs->model_ubr->flush(r);
         m_programs->end_edit_uniforms();
      }
      else
      {
         gl::uniform_matrix_4fv(p->uniform_at(m_programs->model_block_access.clip_from_model), 1, GL_FALSE, value_ptr(clip_from_model));
         gl::uniform_3fv(p->uniform_at(m_programs->model_block_access.id_offset), 1, value_ptr(id_offset_vec3));
      }

      r.draw_elements_base_vertex(
         model->geometry_mesh()->vertex_stream(),
         begin_mode, count, index_type, index_pointer, base_vertex);

      id_range r;
      r.offset = id_offset;
      r.length = count;
      r.model = model;
      m_ranges.push_back(r);

      id_offset += count;
   }

   gl::disable(GL_SCISSOR_TEST);

   r.set_buffer(renderstack::graphics::buffer_target::pixel_pack_buffer, idr.pixel_pack_buffer);
   gl::read_pixels(idr.x_offset, idr.y_offset, m_radius, m_radius, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
   gl::read_pixels(idr.x_offset, idr.y_offset, m_radius, m_radius, GL_DEPTH_COMPONENT, GL_FLOAT, reinterpret_cast<void*>(m_radius * m_radius * 4));
   idr.sync = gl::fence_sync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
   idr.state = id_render_state::waiting_for_read;

   ++m_last_render;
   if (m_last_render == 4)
      m_last_render = 0;
}

bool id_renderer::get(int x, int y, uint32_t &id, float &depth)
{
   int try_render = m_last_render;

   // TODO: Testing for sync done is not free. Start testing only older syncs?
   for (int i = 0; i < 4; ++i)
   {
      try_render = --try_render;
      if (try_render < 0)
         try_render = 3;

      id_render &idr = m_renders[try_render];

      if (idr.state == id_render_state::waiting_for_read)
      {
         GLint sync_status = GL_UNSIGNALED;
         gl::get_sync_iv(idr.sync, GL_SYNC_STATUS, 4, nullptr, &sync_status);

         if (sync_status == GL_SIGNALED)
         {
            auto &r = *m_renderer;
            r.set_buffer(renderstack::graphics::buffer_target::pixel_pack_buffer, idr.pixel_pack_buffer);

            void *src = idr.pixel_pack_buffer->map(
               *m_renderer,
               0,
               m_radius * m_radius * 8,
               gl::buffer_access_mask::map_read_bit
            );
            ::memcpy(&idr.data[0], src, m_radius * m_radius * 8);
            idr.pixel_pack_buffer->unmap(*m_renderer);
            idr.state = id_render_state::read_complete;
         }         
      }

      if (idr.state == id_render_state::read_complete)
      {
         int x_ = x - idr.x_offset;
         int y_ = y - idr.y_offset;
         if ((x_ >= 0) && (y_ >= 0) && (x_ < m_radius) && (y_ < m_radius))
         {
            uint32_t stride = m_radius * 4;
            uint8_t r = idr.data[x_ * 4 + y_ * stride + 0];
            uint8_t g = idr.data[x_ * 4 + y_ * stride + 1];
            uint8_t b = idr.data[x_ * 4 + y_ * stride + 2];
            id = (r << 16) | (g << 8) | b;
            uint8_t *depth_ptr = &idr.data[m_radius * m_radius * 4 + x_ * 4 + y_ * stride];
            float *depth_f_ptr = reinterpret_cast<float*>(depth_ptr);
            depth = *depth_f_ptr;
            return true;
         }
      }
   }
   return false;
}

shared_ptr<class model> id_renderer::get(int x, int y)
{
   uint32_t id;
   float depth;

   bool ok = get(x, y, id, depth);

   if (!ok)
      return nullptr;

   for (auto i = m_ranges.cbegin(); i != m_ranges.cend(); ++i)
   {
      auto r = *i;

      if (id >= r.offset && id < (r.offset + r.length))
         return r.model;
   }

   return nullptr;
}



