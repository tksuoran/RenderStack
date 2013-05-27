#ifndef renderstack_graphics__texture_hpp
#define renderstack_graphics__texture_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <cassert>
#include <memory>

namespace renderstack { namespace graphics {

   
namespace texture_target
{
   enum value
   {
      texture_1d                    = 0,
      texture_2d                    = 1,
      texture_3d                    = 2,
      texture_rectangle             = 3,
      texture_cubemap               = 4,
      texture_1d_array              = 5,
      texture_2d_array              = 6,
      texture_buffer                = 7,
      texture_cubemap_array         = 8,
      texture_2d_multisample        = 9,
      texture_2d_multisample_array  = 10,
      count                         = 11
   };

   gl::texture_target::value gl_texture_target(value rs_target);
   unsigned int dimensions(value target);
   const char * const desc(value target);

}

class buffer;

class texture : public std::enable_shared_from_this<texture>
{
public:
   texture(
      texture_target::value   target,
      unsigned int            internal_format,
      bool                    mipmaps,
      unsigned int            width,
      unsigned int            height = 0,
      unsigned int            depth = 0
   );
   texture(
      std::shared_ptr<class buffer> buffer,
      unsigned int                  internal_format
   );
   ~texture();

public:
   void allocate_storage(class renderer &renderer);

   void set_swizzle     (unsigned int i, unsigned int value);
   void set_min_filter  (gl::texture_min_filter::value value);
   void set_mag_filter  (gl::texture_mag_filter::value value);
   void set_wrap        (unsigned int i, gl::texture_wrap_mode::value value);
   void set_compare_mode(gl::texture_compare_mode::value value);
   void set_compare_func(gl::depth_function::value value);

   texture_target::value            target      () const;
   unsigned int                     width       () const;
   unsigned int                     height      () const;
   unsigned int                     depth       () const;
   unsigned int                     swizzle     (unsigned int i) const;
   gl::texture_min_filter::value    min_filter  () const;
   gl::texture_mag_filter::value    mag_filter  () const;
   gl::texture_wrap_mode::value     wrap        (unsigned int i) const;
   gl::texture_compare_mode::value  compare_mode() const;
   gl::depth_function::value        compare_func() const;

public:
   void apply(class renderer &renderer, unsigned int unit);

   static unsigned int levels(unsigned int size);

   static void format_type(unsigned int internal_format, unsigned int &format, unsigned int &type);

friend class renderer;
private:
   unsigned int gl_name() const { return m_gl_name; }


private:
   unsigned int                     m_gl_name;
   texture_target::value            m_target;
   unsigned int                     m_internal_format;

   unsigned int                     m_levels;
   unsigned int                     m_width;
   unsigned int                     m_height;
   unsigned int                     m_depth;

   std::shared_ptr<class buffer>    m_buffer;

   bool                             m_dirty;
   unsigned int                     m_swizzle[4];  // rgba
   gl::texture_min_filter::value    m_min_filter;
   gl::texture_mag_filter::value    m_mag_filter;
   gl::texture_wrap_mode::value     m_wrap[3];     // str
   int                              m_min_lod;
   int                              m_max_lod;
   int                              m_base_level;
   int                              m_max_level;
   gl::texture_compare_mode::value  m_compare_mode;
   gl::depth_function::value        m_compare_func;
};

} }

#endif
