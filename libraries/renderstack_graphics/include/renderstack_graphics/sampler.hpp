#ifndef sampler_hpp_renderstack_graphics
#define sampler_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"

namespace renderstack
{
namespace graphics
{

class sampler
{
public:
    sampler();
    ~sampler();

public:
    void set_min_filter(gl::texture_min_filter::value value);
    void set_mag_filter(gl::texture_mag_filter::value value);

    gl::texture_min_filter::value   min_filter() const;
    gl::texture_mag_filter::value   mag_filter() const;
    gl::texture_wrap_mode::value    wrap() const;
    gl::texture_compare_mode::value compare_mode() const;
    gl::depth_function::value       compare_func() const;

public:
    void apply(unsigned int texture_unit, gl::texture_target::value bind_target);

private:
    unsigned int                    m_gl_name;
    gl::texture_min_filter::value   m_min_filter;
    gl::texture_mag_filter::value   m_mag_filter;
    gl::texture_wrap_mode::value    m_wrap;
    gl::texture_compare_mode::value m_compare_mode;
    gl::depth_function::value       m_compare_func;
};

} // namespace graphics
} // namespace renderstack

#endif
