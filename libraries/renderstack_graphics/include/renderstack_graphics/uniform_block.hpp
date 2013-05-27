#ifndef renderstack_graphics__uniform_block_hpp
#define renderstack_graphics__uniform_block_hpp

#include "renderstack_toolkit/platform.hpp"
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace renderstack { namespace graphics {

class uniform;

class uniform_block : public std::enable_shared_from_this<uniform_block>
{
public:
   typedef std::vector<std::shared_ptr<uniform> > uniform_collection;

   uniform_block(unsigned int binding_point, std::string const &name);
   uniform_block(unsigned int binding_point, std::string const &name, std::string const &block_name);
   ~uniform_block()
   {
   }

public:
   uniform_collection const   &uniforms      () const;
   uniform_collection         &uniforms      ();
   std::string const          &name          () const;
   std::string const          &block_name    () const;
   void                       set_name       (std::string const &value);
   unsigned int               binding_point  () const;
   std::string                source         (int glsl_shader) const;
   std::size_t                size           () const;
   std::size_t                offset         () const;

public:
   void                     seal();
   std::shared_ptr<uniform> add_float(std::string const &name);
   std::shared_ptr<uniform> add_float(std::string const &name, unsigned int dimension);
   std::shared_ptr<uniform> add_vec2(std::string const &name);
   std::shared_ptr<uniform> add_vec2(std::string const &name, unsigned int dimension);
   std::shared_ptr<uniform> add_vec3(std::string const &name);
   std::shared_ptr<uniform> add_vec3(std::string const &name, unsigned int dimension);
   std::shared_ptr<uniform> add_vec4(std::string const &name);
   std::shared_ptr<uniform> add_vec4(std::string const &name, unsigned int dimension);
   std::shared_ptr<uniform> add_mat4(std::string const &name);
   std::shared_ptr<uniform> add_mat4(std::string const &name, unsigned int dimension);
   std::shared_ptr<uniform> add_int(std::string const &name);
   std::shared_ptr<uniform> add_int(std::string const &name, unsigned int dimension);
   // TODO add_ivecN(), add_uvecN()
   std::shared_ptr<uniform> add_uint(std::string const &name);
   std::shared_ptr<uniform> add_uint(std::string const &name, unsigned int dimension);

private:
   std::string    m_name;
   std::string    m_block_name;
   unsigned int   m_binding_point;
   std::size_t    m_offset;

   std::vector<std::shared_ptr<uniform> > m_uniforms;
};

} }

#endif
