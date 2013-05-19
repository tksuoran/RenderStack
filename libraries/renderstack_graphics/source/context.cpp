#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/context.hpp"

namespace renderstack { namespace graphics {

using namespace std;

shared_ptr<context> context::s_current_context;

shared_ptr<context> context::current()
{
   return s_current_context;
}
void context::make_current(std::shared_ptr<context> context)
{
   s_current_context = context;
}
void context::deinitialize()
{
   s_current_context.reset();
}

void context::set_mapped_index_buffer(index_buffer *value)
{
   s_mapped_index_buffer = value;
}
index_buffer *context::mapped_index_buffer() const
{
   return s_mapped_index_buffer;
}
void context::set_mapped_vertex_buffer(vertex_buffer *value)
{
   s_mapped_vertex_buffer = value;
}
vertex_buffer *context::mapped_vertex_buffer() const
{
   return s_mapped_vertex_buffer;
}
void context::set_mapped_uniform_buffer(uniform_buffer *value)
{
   s_mapped_uniform_buffer = value;
}
uniform_buffer *context::mapped_uniform_buffer() const
{
   return s_mapped_uniform_buffer;
}

context::context()
:  s_mapped_index_buffer(nullptr)
,  s_mapped_vertex_buffer(nullptr)
,  s_mapped_uniform_buffer(nullptr)
,  s_next_uniform_block_binding_point(0)
{
}
context::~context()
{
}

#if 0
shared_ptr<index_buffer> context::get_index_buffer(
   size_t                        index_stride, 
   gl::buffer_usage_hint::value  usage, 
   size_t                        index_capacity
)
{
#if 0
   auto key = std::make_pair(index_stride, usage);
   ibo_pool_collection::iterator fi = s_pools.find(key);
   if (fi != s_pools.end())
   {
      auto index_buffer = fi->second.back();
      if (index_buffer->free_capacity_indices() >= index_capacity)
         return index_buffer;
   }
   auto index_buffer = make_shared<class index_buffer>(
      max(index_capacity, s_default_allocation_size), 
      index_stride, 
      usage
   );
   s_index_buffers[key].push_back(index_buffer);
   return index_buffer;
#else
   auto index_buffer = make_shared<class index_buffer>(
      index_capacity,
      index_stride, 
      usage
   );
   return index_buffer;
#endif
}

shared_ptr<vertex_buffer> context::get_vertex_buffer(
   size_t                        vertex_stride, 
   gl::buffer_usage_hint::value  usage, 
   size_t                        vertex_capacity
)
{
#if 0
   auto key = make_pair(vertex_stride, usage);
   vbo_pool_collection::iterator fi = s_pools.find(key);
   if (fi != s_pools.end())
   {
      auto pool = fi->second.back();
      if (pool->free_capacity_vertices() >= vertex_capacity)
         return pool;
   }
   auto pool = make_shared<vbo_pool>(
      max(vertex_capacity, s_default_allocation_size), 
      vertex_stride, 
      usage
   );
   s_pools[key].push_back(pool);
   return pool;
#else
   auto vertex_buffer = make_shared<class vertex_buffer>(
      vertex_capacity,
      vertex_stride, 
      usage
   );
   return vertex_buffer;
#endif
}

shared_ptr<uniform_buffer> context::get_uniform_buffer(
   gl::buffer_usage_hint::value  usage, 
   size_t                        byte_capacity
)
{
#if 0
   auto fi = s_uniform_buffers.find(usage);
   if (fi != s_uniform_buffers.end())
   {
      auto b = fi->second.back();
      if (b->free_capacity_bytes() >= byte_capacity)
         return b;
   }
   auto b = make_shared<uniform_buffer>(byte_capacity, usage);
   s_uniform_buffers[usage].push_back(b);
   return b;
#else
   auto b = make_shared<uniform_buffer>(byte_capacity, usage);
   return b;
#endif
}

context::uniform_block_collection &context::uniform_block_instances()
{
   return s_uniform_block_instances; 
}
#endif

std::shared_ptr<uniform_block> context::make_uniform_block(std::string const &name)
{
   std::string block_name = name + "_block";
   if (s_uniform_block_instances.find(block_name) != s_uniform_block_instances.end())
      throw std::runtime_error("uniform_block already exists");

   std::shared_ptr<uniform_block> u(
      new uniform_block(
         name, 
         block_name, 
         s_next_uniform_block_binding_point++
      )
   );
   s_uniform_block_instances[block_name] = u;
   return u;
}

std::shared_ptr<renderstack::graphics::samplers> context::global_samplers()
{
   if (!s_global_samplers)
      s_global_samplers = std::make_shared<renderstack::graphics::samplers>();
   return s_global_samplers;
}

class shader_monitor &context::shader_monitor()
{
   return m_shader_monitor;
}

} }
