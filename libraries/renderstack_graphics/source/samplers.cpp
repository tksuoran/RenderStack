#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/samplers.hpp"
#include "renderstack_graphics/uniform.hpp"
#include <sstream>
#include <stdexcept>

namespace renderstack { namespace graphics {

using namespace std;

static char const * const c_str(gl::active_uniform_type::value type);

vector<shared_ptr<uniform> > const& samplers::sampler_uniforms()
{
   return m_samplers;
}

shared_ptr<uniform> samplers::sampler(string const &key) const
{
   auto i = m_sampler_dictionary.find(key);
   if (i != m_sampler_dictionary.cend())
      return i->second;
   return nullptr;
}

void samplers::seal()
{
   m_textures.reserve(m_sampler_dictionary.size());
}

static char const * const c_str(gl::active_uniform_type::value type)
{
   switch (type)
   {
   case gl::active_uniform_type::sampler_1d:                                return "sampler1D";
   case gl::active_uniform_type::sampler_2d:                                return "sampler2D";
   case gl::active_uniform_type::sampler_3d:                                return "sampler3D";
   case gl::active_uniform_type::sampler_cube:                              return "samplerCube";
   case gl::active_uniform_type::sampler_1d_shadow:                         return "sampler1DShadow";
   case gl::active_uniform_type::sampler_2d_shadow:                         return "sampler2DShadow";
   case gl::active_uniform_type::sampler_2d_rect:                           return "sampler2DRect";
   case gl::active_uniform_type::sampler_2d_rect_shadow:                    return "sampler2DRectShadow";
   case gl::active_uniform_type::sampler_1d_array:                          return "sampler1DArray";
   case gl::active_uniform_type::sampler_2d_array:                          return "sampler2DArray";
   case gl::active_uniform_type::sampler_buffer:                            return "samplerBuffer";
   case gl::active_uniform_type::sampler_1d_array_shadow:                   return "sampler1DArrayShadow";
   case gl::active_uniform_type::sampler_2d_array_shadow:                   return "sampler2DArrayShadow";
   case gl::active_uniform_type::sampler_cube_shadow:                       return "samplerCubeShadow";
   case gl::active_uniform_type::int_sampler_1d:                            return "intSampler1D";
   case gl::active_uniform_type::int_sampler_2d:                            return "intSampler2D";
   case gl::active_uniform_type::int_sampler_3d:                            return "intSampler3D";
   case gl::active_uniform_type::int_sampler_cube:                          return "intSamplerCube";
   case gl::active_uniform_type::int_sampler_2d_rect:                       return "intSampler2DRect";
   case gl::active_uniform_type::int_sampler_1d_array:                      return "intSampler1DArray";
   case gl::active_uniform_type::int_sampler_2d_array:                      return "intSampler2DArray";
   case gl::active_uniform_type::int_sampler_buffer:                        return "intSamplerBuffer";
   case gl::active_uniform_type::unsigned_int_sampler_1d:                   return "unsignedIntSampler1D";
   case gl::active_uniform_type::unsigned_int_sampler_2d:                   return "unsignedIntSampler2D";
   case gl::active_uniform_type::unsigned_int_sampler_3d:                   return "unsignedIntSampler3D";
   case gl::active_uniform_type::unsigned_int_sampler_cube:                 return "unsignedIntSamplerCube";
   case gl::active_uniform_type::unsigned_int_sampler_2d_rect:              return "unsignedIntSampler2DRect";
   case gl::active_uniform_type::unsigned_int_sampler_1d_array:             return "unsignedIntSampler1DArray";
   case gl::active_uniform_type::unsigned_int_sampler_2d_array:             return "unsignedIntSampler2DArray";
   case gl::active_uniform_type::unsigned_int_sampler_buffer:               return "unsignedIntSamplerBuffer";
   case gl::active_uniform_type::sampler_cubemap_array:                     return "samplerCubemapArray";
   case gl::active_uniform_type::sampler_cubemap_array_shadow:              return "samplerCubemapArrayShadow";
   case gl::active_uniform_type::int_sampler_cubemap_array:                 return "intSamplerCubemapArray";
   case gl::active_uniform_type::unsigned_int_sampler_cubemap_array:        return "unsignedIntSamplerCubemapArray";
   case gl::active_uniform_type::sampler_2d_multisample:                    return "sampler2DMultisampler";
   case gl::active_uniform_type::int_sampler_2d_multisample:                return "intSampler2DMultisampler";
   case gl::active_uniform_type::unsigned_int_sampler_2d_multisample:       return "unsignedIntSampler2DMultisample";
   case gl::active_uniform_type::sampler_2d_multisample_array:              return "sampler2DMultisampleArray";
   case gl::active_uniform_type::int_sampler_2d_multisample_array:          return "intSampler2DMultisampleArray";
   case gl::active_uniform_type::unsigned_int_sampler_2d_multisample_array: return "unsignedIntSampler2DMultisampleArray";
   default: throw std::runtime_error("unknown sampler type");
   }
}


shared_ptr<uniform> samplers::add(string const &name, gl::active_uniform_type::value type, shared_ptr<class sampler> sampler)
{
   auto uniform = make_shared<class uniform>(name, -1, 1, type);
   uniform->set_sampler(sampler);
   uniform->set_texture_unit_index(static_cast<int>(m_samplers.size()));
   m_samplers.push_back(uniform);
   m_sampler_dictionary[name] = uniform;
   return uniform;
}

string samplers::str() const
{
   stringstream sb;
   for (auto i = m_samplers.cbegin(); i != m_samplers.cend(); ++i)
   {
      auto &uniform = *i;

      sb << "uniform " << c_str(uniform->type()) << " " << uniform->name() << ";\n";
   }
   return sb.str();
}

} }
