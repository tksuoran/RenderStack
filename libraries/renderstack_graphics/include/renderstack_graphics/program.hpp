#ifndef program_hpp_renderstack_graphics
#define program_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/log.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"
#include <map>
#include <string>
#include <vector>
#include <memory>

namespace renderstack { namespace graphics {

class renderer;

class program : public std::enable_shared_from_this<program>
{
public:
   program(
      std::string const                               &name, 
      int                                             glsl_version, 
      std::shared_ptr<class samplers>                 samplers,
      std::shared_ptr<class vertex_stream_mappings>   mappings
   );

   std::string const &name() const { return m_name; }
   std::shared_ptr<class samplers> samples() { return m_samplers; }
   std::shared_ptr<class vertex_stream_mappings> mappings () { return m_mappings; }

   void dump_shaders() const;

   void map_uniform(size_t key, std::string const &name);
   int uniform_at(std::size_t index) const;

   void                             bind_attrib_location    (int location, std::string const name);

   void                             define              (std::string const &key, std::string const &value);
   int                              get_uniform_location(const char *name);
   void                             load_shader         (gl::shader_type::value type, std::string const &path);
   void                             set_shader          (gl::shader_type::value type, std::string const &source);

   program                          &load_vs            (std::string const &path);
   program                          &load_tcs           (std::string const &path);
   program                          &load_tes           (std::string const &path);
   program                          &load_gs            (std::string const &path);
   program                          &load_fs            (std::string const &path);
   program                          &set_vs             (std::string const &source);
   program                          &set_tcs            (std::string const &source);
   program                          &set_tes            (std::string const &source);                              
   program                          &set_gs             (std::string const &source);
   program                          &set_fs             (std::string const &source);
   std::shared_ptr<class uniform>   uniform             (std::string const &name);
   void                             transform_feedback  (std::vector<std::string> varyings, GLenum buffer_mode);
   void                             add                 (std::weak_ptr<class uniform_block> uniform_block);
   void                             link                ();
   void                             reload              ();

friend class renderer;
private:
   unsigned int gl_name() const { return m_gl_name; }

private:
   struct loaded_shader
   {
      gl::shader_type::value  type;
      unsigned int            shader;
      unsigned int            last_good_shader;
      std::string             path;
      std::string             source;
      std::string             compiled_src;
   };

   std::string                                                 m_name;
   int                                                         m_glsl_version;
   unsigned int                                                m_gl_name;
   std::map<std::string, std::weak_ptr<class uniform_block > > m_uniform_blocks;
   std::map<std::string, std::shared_ptr<class uniform > >     m_uniforms;
   std::shared_ptr<class samplers>                             m_samplers;
   std::shared_ptr<class vertex_stream_mappings>               m_mappings;
   std::vector<loaded_shader>                                  m_loaded_shaders;
   std::vector<std::pair<std::string, std::string> >           m_defines;
   std::vector<GLint>                                          m_uniform_map;

private:
   static std::string format(std::string const &source);

private:
   unsigned int make_shader(
      gl::shader_type::value type,
      std::string source_str,
      std::string &compiled_src
   );
};

} } 

#endif
