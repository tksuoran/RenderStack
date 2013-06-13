#ifndef mesh_hpp_renderstack_mesh
#define mesh_hpp_renderstack_mesh

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/buffer.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <stdexcept>
#include <memory>

namespace renderstack { namespace mesh {

struct index_range;

class mesh
{
public:
   mesh();

   std::shared_ptr<renderstack::graphics::buffer>  vertex_buffer() const;
   std::shared_ptr<renderstack::graphics::buffer>  index_buffer() const;
   std::size_t first_vertex() const;
   std::size_t first_index () const;
   std::size_t vertex_count() const;
   std::size_t index_count () const;

   void allocate_vertex_buffer  (renderstack::graphics::renderer &renderer, std::size_t vertex_stride, std::size_t vertex_count, gl::buffer_usage_hint::value usage = gl::buffer_usage_hint::static_draw);
   void allocate_vertex_buffer  (std::shared_ptr<renderstack::graphics::buffer> vertex_buffer, std::size_t vertex_count);
   void allocate_index_buffer   (renderstack::graphics::renderer &renderer, std::size_t index_stride, std::size_t index_count, gl::buffer_usage_hint::value usage = gl::buffer_usage_hint::static_draw);
   void allocate_index_buffer   (std::shared_ptr<renderstack::graphics::buffer> index_buffer, std::size_t index_count);
   void allocate_index_range    (gl::begin_mode::value begin_mode, std::size_t index_count, index_range &range);
   void set_first_vertex        (std::size_t value);
   void set_first_index         (std::size_t value);
   void set_vertex_count        (std::size_t value);
   void set_index_count         (std::size_t value);

private:
   std::shared_ptr<renderstack::graphics::buffer>  m_vertex_buffer;
   std::shared_ptr<renderstack::graphics::buffer>  m_index_buffer;

   //  This is the container for index ranges
   std::size_t m_first_vertex;
   std::size_t m_first_index;
   std::size_t m_vertex_count;
   std::size_t m_index_count;
   std::size_t m_next_index_range_start;
};

} }

#endif

