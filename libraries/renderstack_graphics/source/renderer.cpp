#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/logstream.hpp"

#include "renderstack_graphics/vertex_array.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/renderer.hpp"

#define LOG_CATEGORY &log_graphics_renderer

namespace renderstack { namespace graphics {

using namespace std;


texture_unit_state::texture_unit_state()
:  sampler_binding(nullptr)
{
   for (unsigned int target = 0; target < texture_target::count; ++target)
      texture_binding[target].reset();
}

texture_unit_state::texture_unit_state(texture_unit_state const &other)
:  sampler_binding(other.sampler_binding)
{
   for (unsigned int target = 0; target < texture_target::count; ++target)
      texture_binding[target] = other.texture_binding[target];
}


state::state()
:  current_program            (nullptr)
,  vertex_array_binding       (nullptr)
,  transform_feedback_binding (0)
,  active_texture             (0)
,  draw_framebuffer_binding   (0)
,  read_framebuffer_binding   (0)
,  renderbuffer_binding       (0)
,  current_query              (0)
{
   // for (int i = 0; i < RS_TEXTURE_UNIT_COUNT; ++i)
   // texture_unit[i]

   for (int i = 0; i < buffer_target::non_indexed_context_buffer_target_count; ++i)
      buffer_binding[i].reset();

   for (int i = 0; i < RS_UNIFORM_BINDING_POINT_COUNT; ++i)
      uniform_buffer_binding_indexed[i].reset();
}
state::state(state const &other)
:  current_program            (other.current_program)
,  vertex_array_binding       (other.vertex_array_binding)
,  transform_feedback_binding (other.transform_feedback_binding)
,  active_texture             (other.active_texture)
,  draw_framebuffer_binding   (other.draw_framebuffer_binding)
,  read_framebuffer_binding   (other.read_framebuffer_binding)
,  renderbuffer_binding       (other.renderbuffer_binding)
,  current_query              (other.current_query)
{
   for (int i = 0; i < RS_TEXTURE_UNIT_COUNT; ++i)
      texture_unit[i] = other.texture_unit[i];

   for (int i = 0; i < buffer_target::non_indexed_context_buffer_target_count; ++i)
      buffer_binding[i] = other.buffer_binding[i];

   for (int i = 0; i < RS_UNIFORM_BINDING_POINT_COUNT; ++i)
      uniform_buffer_binding_indexed[i] = other.uniform_buffer_binding_indexed[i];
}


renderer::renderer()
{
   // When vertex arrays are not used, we use this to keep track
   // of enabled attributes and set vertex attribute pointers.
   m_default_vertex_array = std::make_shared<class vertex_array>(true);

   for (int i = 0; i < buffer_target::all_buffer_target_count; ++i)
      m_mapped_buffer[i].reset();

   (void)set_vertex_array(m_default_vertex_array);
}
void renderer::push()
{
   m_request_stack.push(m_requested);
}
void renderer::pop()
{
   m_requested = m_request_stack.top();
   m_request_stack.pop();
}

void texture_unit_state::trash(unsigned int unit)
{
   for (int i = 0; i < texture_target::count; ++i)
   {
      texture_target::value target = static_cast<texture_target::value>(i);
      gl::bind_texture(texture_target::gl_texture_target(target), 0);
      texture_binding[i].reset();
   }

   gl::bind_sampler(unit, 0);
   sampler_binding.reset();
}

void renderer::trash()
{
   slog_trace("renderer::trash()");

   m_effective.current_program.reset();
   gl::use_program(0);

   for (int i = 0; i < buffer_target::non_indexed_context_buffer_target_count; ++i)
   {
      buffer_target::value target = static_cast<buffer_target::value>(i);
      m_effective.buffer_binding[i].reset();
      gl::bind_buffer(buffer_target::gl_buffer_target(target), 0);
   }

   m_effective.vertex_array_binding->trash();
   gl::bind_vertex_array(0);

   for (int i = 0; i < RS_TEXTURE_UNIT_COUNT; ++i)
   {
      gl::active_texture(GL_TEXTURE0 + i);
      m_effective.texture_unit[i].trash(i);
   }
   m_effective.active_texture = 0;
   gl::active_texture(GL_TEXTURE0);

   for (int i = 0; i < RS_UNIFORM_BINDING_POINT_COUNT; ++i)
   {
      gl::bind_buffer_range(gl::buffer_target::uniform_buffer, i, 0, 0, 0);
      m_effective.uniform_buffer_binding_indexed[i].reset();
   }
}
std::shared_ptr<class program> renderer::set_program(std::shared_ptr<class program> program)
{
   std::shared_ptr<class program> old = m_effective.current_program;

   m_requested.current_program = program;
   if (m_effective.current_program != m_requested.current_program)
   {
      if (m_requested.current_program)
         gl::use_program(program->gl_name());
      else
         gl::use_program(0);

      m_effective.current_program = m_requested.current_program;
   }

   return old;
}
std::shared_ptr<class texture> renderer::set_texture(
   unsigned int                     unit,
   std::shared_ptr<class texture>   texture,
   unsigned int                     *old_unit
)
{
   auto target = texture->target();
   if (target >= texture_target::count)
      throw runtime_error("invalid texture target");

   unsigned int count = std::min(RS_TEXTURE_UNIT_COUNT, configuration::max_combined_texture_image_units);
   if (unit >= count)
      throw runtime_error("texture unit index out of supported range");

   auto old = m_effective.texture_unit[unit].texture_binding[target];
   if (old_unit)
      *old_unit = m_effective.active_texture;

   m_requested.texture_unit[unit].texture_binding[target] = texture;

   if (
      m_effective.texture_unit[unit].texture_binding[target] !=
      m_requested.texture_unit[unit].texture_binding[target]
   )
   {
      m_requested.active_texture = unit;
      if (m_effective.active_texture != m_requested.active_texture)
      {
         gl::active_texture(gl::texture_unit::texture0 + m_requested.active_texture);
         m_effective.active_texture = m_requested.active_texture;
      }
      unsigned int gl_name = texture ? texture->gl_name() : 0;
      gl::bind_texture(
         texture_target::gl_texture_target(target),
         gl_name
      );

      m_effective.texture_unit[unit].texture_binding[target] = m_requested.texture_unit[unit].texture_binding[target];

      // TODO Do this or not?
      if (texture)
         texture->apply(*this, unit);
   }

   return old;
}
unsigned int renderer::set_texture_unit(unsigned int unit)
{
   unsigned int old_unit = m_effective.active_texture;

   m_requested.active_texture = unit;
   if (m_effective.active_texture != m_requested.active_texture)
   {
      gl::active_texture(gl::texture_unit::texture0 + m_requested.active_texture);
      m_effective.active_texture = m_requested.active_texture;
   }
   return old_unit;
}

// Restores old texture to effective texture unit and then restores effective texture unit to old_unit
void renderer::restore_texture(texture_target::value target, std::shared_ptr<class texture> old_texture, unsigned int old_unit)
{
   unsigned int unit = m_effective.active_texture;

   m_requested.texture_unit[unit].texture_binding[target] = old_texture;
   if (
      m_effective.texture_unit[unit].texture_binding[target] !=
      m_requested.texture_unit[unit].texture_binding[target]
   )
   {
      unsigned int gl_name = old_texture ? old_texture->gl_name() : 0;
      gl::bind_texture(texture_target::gl_texture_target(target), gl_name);

      m_requested.active_texture = old_unit;
      if (m_effective.active_texture != m_requested.active_texture)
      {
         gl::active_texture(gl::texture_unit::texture0 + m_requested.active_texture);
         m_effective.active_texture = m_requested.active_texture;
      }
      m_effective.texture_unit[unit].texture_binding[target] = m_requested.texture_unit[unit].texture_binding[target];
   }
}

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
std::shared_ptr<uniform_buffer_range> renderer::set_uniform_buffer_range(
   unsigned int binding_point,
   shared_ptr<uniform_buffer_range> buffer_range)
{
   unsigned int count = std::min(RS_UNIFORM_BINDING_POINT_COUNT, configuration::max_uniform_buffer_bindings);

   if (binding_point >= count)
      throw runtime_error("uniform buffer binding point index out of supported range");

   if (!buffer_range)
      throw runtime_error("buffer range nullptr");

   if (!configuration::can_use.uniform_buffer_object)
      throw runtime_error("uniform buffer objects not supported by current context");

   std::shared_ptr<uniform_buffer_range> old = m_requested.uniform_buffer_binding_indexed[binding_point];

   m_requested.uniform_buffer_binding_indexed[binding_point] = buffer_range;

   auto buffer = buffer_range->uniform_buffer().lock();

   if (
      (m_effective.uniform_buffer_binding_indexed[binding_point] != m_requested.uniform_buffer_binding_indexed[binding_point]) ||
      (m_effective.buffer_binding[buffer_target::uniform_buffer] != buffer)
   )
   {
      if (m_requested.uniform_buffer_binding_indexed[binding_point])
      {
         // offset and size are assumed not to change once buffer_range is created
         gl::bind_buffer_range(
            gl::buffer_target::uniform_buffer,
            binding_point, 
            buffer_range->uniform_buffer().lock()->gl_name(),
            static_cast<GLsizeiptr>(buffer_range->first_byte()), 
            static_cast<GLsizeiptr>(buffer_range->byte_count())
         );
      }

      auto buffer = buffer_range->uniform_buffer().lock();
      m_effective.buffer_binding[buffer_target::uniform_buffer] = buffer;
      m_effective.uniform_buffer_binding_indexed[binding_point] = m_requested.uniform_buffer_binding_indexed[binding_point];
   }

   return old;
}
#endif

std::shared_ptr<class buffer> renderer::set_buffer(buffer_target::value target, std::shared_ptr<class buffer> buffer)
{
   if (target >= buffer_target::non_indexed_context_buffer_target_count)
      throw runtime_error("invalid buffer target");

   std::shared_ptr<class buffer> old = m_effective.buffer_binding[target];

   m_requested.buffer_binding[target] = buffer;
   if (m_effective.buffer_binding[target] != m_requested.buffer_binding[target])
   {
      if (m_requested.buffer_binding[target])
      {
         unsigned int gl_name = buffer ? buffer->gl_name() : 0;
         gl::bind_buffer(
            buffer_target::gl_buffer_target(target),
            gl_name);
      }

      m_effective.buffer_binding[target] = m_requested.buffer_binding[target];
   }

   return old;
}
std::shared_ptr<class vertex_array> renderer::set_vertex_array(std::shared_ptr<class vertex_array> vertex_array)
{
   std::shared_ptr<class vertex_array> old = m_effective.vertex_array_binding;

   m_requested.vertex_array_binding = vertex_array;
   if (m_effective.vertex_array_binding != m_requested.vertex_array_binding)
   {
      if (m_requested.vertex_array_binding)
      {
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
         bool use_vao =
            configuration::must_use_vertex_array_object || 
            (
               configuration::can_use.vertex_array_object && 
               configuration::use_vertex_array_object
            );

         if (use_vao)
         {
            unsigned int gl_name = vertex_array ? vertex_array->gl_name() : 0;
            gl::bind_vertex_array(gl_name);

            // This helps us validate that VAO is really bound when we try to bind IBO
            if (m_effective.vertex_array_binding)
               m_effective.vertex_array_binding->set_bound(false);
            if (vertex_array)
               vertex_array->set_bound(true);
         }
#endif
      }

      m_effective.vertex_array_binding = m_requested.vertex_array_binding;
   }

   return old;
}
void renderer::reset_vertex_array()
{
   set_vertex_array(m_default_vertex_array);
}

void renderer::setup_attribute_pointers(std::shared_ptr<class vertex_stream> vertex_stream, GLint base_vertex)
{
   if (configuration::use_gl1)
   {
#     if defined(RENDERSTACK_GL_API_OPENGL_WITH_LEGACY)
      {
         setup_attribute_pointers_old(basevertex);
      }
#     else
      {
         throw std::exception();
      }
#     endif
   }
   else
   {
      setup_attribute_pointers_new(vertex_stream, base_vertex);
   }
}
void renderer::setup_attribute_pointers_new(
   std::shared_ptr<class vertex_stream> vertex_stream, GLint base_vertex)
{
   assert(vertex_stream);

   slog_trace(
      "renderer::setup_attribute_pointers_new(base_vertex = %d",
      base_vertex);

   bool use_vao =
      configuration::must_use_vertex_array_object || 
      (
         configuration::can_use.vertex_array_object && 
         configuration::use_vertex_array_object
      );

   if (use_vao)
      set_vertex_array(vertex_stream->vertex_array());

   auto va = m_effective.vertex_array_binding;

   va->clear_enabled_attribs();

   unsigned int count = std::min(RS_ATTRIBUTE_COUNT, configuration::max_vertex_attribs);

   for (auto i = vertex_stream->bindings().cbegin(); i != vertex_stream->bindings().cend(); ++i)
   {
      auto binding   = *i;
      auto vbo       = binding.vertex_buffer().lock();
      auto attribute = binding.vertex_attribute().lock();
      auto mapping   = binding.vertex_stream_mapping().lock();

      assert(vbo);
      assert(attribute);
      assert(mapping);

      if (mapping->dst_index() >= count)
         throw runtime_error("vertex attribute index out of supported range");

      set_buffer(buffer_target::array_buffer, vbo);

      va->enable_attrib(
         mapping->dst_index(),
         attribute->dimension(),
         attribute->shader_type(),
         attribute->data_type(),
         binding.stride(),
         base_vertex * binding.stride() + attribute->offset(),
         attribute->normalized(),
         attribute->divisor()
      );
   }

   va->apply_attrib_enables();
}

void renderer::use_vertex_stream(std::shared_ptr<class vertex_stream> vertex_stream)
{
   assert(vertex_stream);

   bool use_vao =
      configuration::must_use_vertex_array_object || 
      (
         configuration::can_use.vertex_array_object && 
         configuration::use_vertex_array_object
      );

   if (use_vao)
      set_vertex_array(vertex_stream->vertex_array());
}

void renderer::draw_elements_base_vertex(
   std::shared_ptr<class vertex_stream> vertex_stream, 
   GLenum begin_mode, GLsizei count, GLenum index_type, const GLvoid *indices,
   GLint base_vertex)
{
   assert(vertex_stream);

   bool use_vao =
      configuration::must_use_vertex_array_object || 
      (
         configuration::can_use.vertex_array_object && 
         configuration::use_vertex_array_object
      );

   if (use_vao)
      set_vertex_array(vertex_stream->vertex_array());

   auto va = m_effective.vertex_array_binding;

   if (use_vao)
   {
#if defined(RENDERSTACK_GL_API_OPENGL)
      if (configuration::can_use.draw_elements_base_vertex)
      {
         gl::draw_elements_base_vertex(begin_mode, count, index_type, indices, base_vertex);
      }
      else
#endif
      {
         gl::draw_elements(begin_mode, count, index_type, indices);
      }
   }
   else
   {
      setup_attribute_pointers(vertex_stream, base_vertex);
      gl::draw_elements(begin_mode, count, index_type, indices);
   }
}


bool renderer::map_buffer(buffer_target::value target, std::shared_ptr<class buffer> buffer)
{
   assert(target < buffer_target::all_buffer_target_count);
   bool ok = (m_mapped_buffer[target] == nullptr) && (buffer->target() == target);
   m_mapped_buffer[target] = buffer;
   return ok;
}
bool renderer::buffer_is_bound(buffer_target::value target, std::shared_ptr<class buffer> buffer)
{
   assert(target < buffer_target::non_indexed_context_buffer_target_count);
   bool ok = (m_effective.buffer_binding[target] == buffer) && (buffer->target() == target);
   return ok;
}
bool renderer::buffer_is_mapped(buffer_target::value target, std::shared_ptr<class buffer> buffer)
{
   assert(target < buffer_target::all_buffer_target_count);
   bool ok = (m_mapped_buffer[target] == buffer) && (buffer->target() == target);
   return ok;
}
bool renderer::unmap_buffer(buffer_target::value target, std::shared_ptr<class buffer> buffer)
{
   assert(target < buffer_target::all_buffer_target_count);
   bool ok = (m_mapped_buffer[target] == buffer) && (buffer->target() == target);
   m_mapped_buffer[target].reset();
   return ok;
}
bool renderer::texture_is_bound(unsigned int unit, texture_target::value target, std::shared_ptr<class texture> texture)
{
   unsigned int count = std::min(RS_TEXTURE_UNIT_COUNT, configuration::max_combined_texture_image_units);
   if (unit >= count)
      throw runtime_error("texture unit index out of supported range");

   assert(target < texture_target::count);

   bool ok = (m_effective.texture_unit[unit].texture_binding[target] == texture);
   return ok;
}
unsigned int renderer::effective_texture_unit() const
{
   return m_effective.active_texture;
}

} }




#if defined(RENDERSTACK_GL_API_OPENGL_WITH_LEGACY)
// This path is outdated
void vertex_stream::setup_attribute_pointers_old(GLint basevertex)
{
   for (auto i = m_vertex_stream_bindings.begin(); i != m_vertex_stream_bindings.end(); ++i)
   {
      vertex_stream_binding  *binding   = *i;
      class vertex_attribute *attribute = binding->vertex_attribute();
      vertex_stream_mapping  *mapping   = binding->vertex_stream_mapping();

      switch (mapping->dst_usage())
      {
      case vertex_attribute_usage::position:
         {
            gl::enable_client_state(gl::array_cap::vertex_array);
            gl::vertex_pointer(
               attribute->dimension(), 
               attribute->type(), 
               binding->stride(), 
               reinterpret_cast<char*>(basevertex * binding->stride() + attribute->offset())
            );
         }
         break;
      case vertex_attribute_usage::normal:
         {
            gl::enable_client_state(gl::array_cap::normal_array);
            gl::normal_pointer(
               attribute->type(),
               binding->stride(),
               reinterpret_cast<char*>(basevertex * binding->stride() + attribute->offset())
            );
         }
         break;
      case vertex_attribute_usage::color:
         {
            if ((attribute->dimension() != 3) && (attribute->dimension() != 4)) {
               throw std::exception();
               continue;
            }
            gl::enable_client_state(gl::array_cap::color_array);
            gl::color_pointer(
               attribute->dimension(), 
               attribute->type(), 
               binding->stride(), 
               reinterpret_cast<char*>(basevertex * binding->stride() + attribute->offset())
            );
         }
         break;
      case vertex_attribute_usage::tex_coord:
         {
            gl::client_active_texture(gl::texture_unit::texture0 + binding->vertex_stream_mapping()->dst_index());
            gl::enable_client_state(gl::array_cap::texture_coord_array);
            gl::texcoord_pointer(
               attribute->dimension(), 
               attribute->type(), 
               binding->stride(), 
               reinterpret_cast<char*>(basevertex * binding->stride() + attribute->offset())
            );
         }
         break;
      default:
         break;
      }
   }
}
void vertex_stream::disable_attributes_old()
{
   for (auto i = vertex_stream_bindings().begin(); i != vertex_stream_bindings().end(); ++i)
   {
      vertex_stream_binding  *binding   = *i;
      class vertex_attribute *attribute = binding->vertex_attribute();
      vertex_stream_mapping  *mapping   = binding->vertex_stream_mapping();

      switch (mapping->dst_usage())
      {
      case vertex_attribute_usage::position:
         gl::disable_client_state(gl::array_cap::vertex_array);
         break;
      case vertex_attribute_usage::normal:
         gl::disable_client_state(gl::array_cap::normal_array);
         break;
      case vertex_attribute_usage::color:
         gl::disable_client_state(gl::array_cap::color_array);
         break;
      case vertex_attribute_usage::tex_coord:
         gl::client_active_texture(gl::texture_unit::texture0 + binding->vertex_stream_mapping()->dst_index());
         gl::disable_client_state(gl::array_cap::texture_coord_array);
         break;
      default:
         break;
      }
   }
}
#endif
