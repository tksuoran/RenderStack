#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_toolkit/gl.hpp"

namespace renderstack { namespace graphics {

sampler::sampler()
:  m_gl_name      (~0u)
,  m_min_filter   (gl::texture_min_filter::nearest)
,  m_mag_filter   (gl::texture_mag_filter::nearest)
,  m_wrap         (gl::texture_wrap_mode::clamp_to_edge)
,  m_compare_mode (gl::texture_compare_mode::none)
,  m_compare_func (gl::depth_function::less)
{
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   if (configuration::can_use.sampler_object)
      gl::gen_samplers(1, &m_gl_name);
#endif
}

sampler::~sampler()
{
#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   if (configuration::can_use.sampler_object)
      gl::delete_samplers(1, &m_gl_name);
#endif
}

void sampler::set_min_filter(gl::texture_min_filter::value value)
{
   m_min_filter = value;
}

void sampler::set_mag_filter(gl::texture_mag_filter::value value)
{
   m_mag_filter = value;
}

gl::texture_min_filter::value sampler::min_filter() const 
{ 
   return m_min_filter; 
}

gl::texture_mag_filter::value sampler::mag_filter() const 
{ 
   return m_mag_filter; 
}

gl::texture_wrap_mode::value sampler::wrap() const 
{ 
   return m_wrap; 
}

gl::texture_compare_mode::value sampler::compare_mode() const 
{ 
   return m_compare_mode; 
}

gl::depth_function::value sampler::compare_func() const 
{ 
   return m_compare_func; 
}

void sampler::apply(unsigned int texture_unit, gl::texture_target::value bind_target)
{
   //  Workarounds for AMD driver bug
   //  http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=298749#Post298749

#if defined(RENDERSTACK_GL_API_OPENGL) || defined(RENDERSTACK_GL_API_OPENGL_ES_3)
   if (configuration::can_use.sampler_object)
   {
      gl::sampler_parameter_i(m_gl_name, gl::sampler_parameter::texture_min_filter, m_min_filter);
      gl::sampler_parameter_i(m_gl_name, gl::sampler_parameter::texture_mag_filter, m_mag_filter);
      gl::sampler_parameter_i(m_gl_name, gl::sampler_parameter::texture_compare_mode, m_compare_mode);
      gl::sampler_parameter_i(m_gl_name, gl::sampler_parameter::texture_compare_func, m_compare_func);
      gl::sampler_parameter_i(m_gl_name, gl::sampler_parameter::texture_wrap_s, m_wrap);
      gl::sampler_parameter_i(m_gl_name, gl::sampler_parameter::texture_wrap_t, m_wrap);
      gl::sampler_parameter_i(m_gl_name, gl::sampler_parameter::texture_wrap_r, m_wrap);
      gl::tex_parameter_i(bind_target, gl::texture_parameter_name::texture_min_filter, m_min_filter);
      gl::bind_sampler(texture_unit, m_gl_name);
   }
   else 
#endif
   {
      (void)texture_unit;
      
      //gl::bind_texture(BindTarget, TextureObject);
      gl::tex_parameter_i(bind_target, gl::texture_parameter_name::texture_min_filter, m_min_filter);
      gl::tex_parameter_i(bind_target, gl::texture_parameter_name::texture_mag_filter, m_mag_filter);
      gl::tex_parameter_i(bind_target, gl::texture_parameter_name::texture_wrap_s, m_wrap);
      gl::tex_parameter_i(bind_target, gl::texture_parameter_name::texture_wrap_t, m_wrap);
      gl::tex_parameter_i(bind_target, gl::texture_parameter_name::texture_wrap_r, m_wrap);

      //  \todo Apply() needs to be called somewhere anyway :P

      //gl::bind_sampler(texture_unit, m_gl_name);
   }
}

} }
