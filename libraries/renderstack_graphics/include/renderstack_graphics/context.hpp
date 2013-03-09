#ifndef renderstack_graphics__context_hpp
#define renderstack_graphics__context_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/logstream.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/uniform_block.hpp"
#include "renderstack_graphics/uniform_buffer.hpp"
#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace renderstack { namespace graphics {


class context
{
public:
   context();
   ~context();

public:
   static std::shared_ptr<context> current();
   static void make_current(std::shared_ptr<context> context);
   static void deinitialize();

friend class index_buffer;
friend class vertex_buffer;
friend class uniform_buffer;
private:
   void           set_mapped_index_buffer    (index_buffer *value);
   index_buffer   *mapped_index_buffer       () const;
   void           set_mapped_vertex_buffer   (vertex_buffer *value);
   vertex_buffer  *mapped_vertex_buffer      () const;
   void           set_mapped_uniform_buffer  (uniform_buffer *value);
   uniform_buffer *mapped_uniform_buffer     () const;

friend class program;
private:
   typedef std::map<
      std::string,
      std::weak_ptr<uniform_block>
   > uniform_block_collection;

   uniform_block_collection &uniform_block_instances();

public:
   std::shared_ptr<uniform_block>                     make_uniform_block(std::string const &name);
   std::shared_ptr<renderstack::graphics::samplers>   global_samplers();
   std::shared_ptr<vertex_stream_mappings>            make_vertex_stream_mappings(std::string const &name);
   std::shared_ptr<vertex_stream_mappings>            global_vertex_stream_mappings();
   class shader_monitor                               &shader_monitor();

private:
   static std::shared_ptr<context> s_current_context;

   typedef std::map<
      std::string,
      std::shared_ptr<
         vertex_stream_mappings
      >
   > vertex_stream_mappings_collection;

   index_buffer                     *s_mapped_index_buffer;
   vertex_buffer                    *s_mapped_vertex_buffer;
   uniform_buffer                   *s_mapped_uniform_buffer;
   unsigned int                     s_next_uniform_block_binding_point;
   uniform_block_collection         s_uniform_block_instances;

   std::shared_ptr<vertex_stream_mappings>            s_global_vertex_stream_mappings;
   unsigned int                                       s_vertex_stream_mappings_instance_index;
   vertex_stream_mappings_collection                  s_vertex_stream_mappings_collection;

   class shader_monitor                               m_shader_monitor;
   std::shared_ptr<renderstack::graphics::samplers>   s_global_samplers;

};

} }

#endif

