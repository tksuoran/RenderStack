#ifndef uniform_hpp_renderstack_graphics
#define uniform_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_graphics/sampler.hpp"
#include <string>
#include <memory>

namespace renderstack { namespace graphics {

class precision
{
public:
   enum value
   {
      lowp     = 0,
      mediump  = 1,
      highp    = 2,
      superp   = 3   // only reserved - not used
   };
   static const char * const desc(value v)
   {
      switch (v)
      {
      case lowp:     return "lowp";
      case mediump:  return "mediump";
      case highp:    return "highp";
      case superp:   return "superp";
      default:       return "???";
      }
   }
};

class uniform
{
public:
   uniform(std::string const &name, GLint index, std::size_t count, gl::active_uniform_type::value type, std::size_t offset);
   uniform(std::string const &name, GLint index, std::size_t count, gl::active_uniform_type::value type);

   bool                             is_array() const;
   void                             set_is_array(bool value);
   int                              index() const;
   int                              texture_unit_index() const;
   void                             set_texture_unit_index(GLint value);
   std::size_t                      count() const;
   gl::active_uniform_type::value   type() const;
   precision::value                 precision_qualifier() const;
   void                             set_precision_qualifier(precision::value value);
   std::string const                &name() const;
   GLint                            block() const;
   std::size_t                      offset() const;
   std::shared_ptr<class sampler>   sampler() const;
   void                             set_sampler(std::shared_ptr<class sampler> value);

   void                             program_set_index(GLint index);

private:
   std::string                      m_name;
   std::size_t                      m_count;
   gl::active_uniform_type::value   m_type;
   precision::value                 m_precision;
   int                              m_index;
   int                              m_block; // -1 = default block
   std::size_t                      m_offset;
   int                              m_texture_unit_index;
   std::shared_ptr<class sampler>   m_sampler;
   bool                             m_is_array;
};

} }

#endif
