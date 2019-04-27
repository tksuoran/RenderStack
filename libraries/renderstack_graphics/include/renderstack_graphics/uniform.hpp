#ifndef uniform_hpp_renderstack_graphics
#define uniform_hpp_renderstack_graphics

#include "renderstack_graphics/sampler.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <memory>
#include <string>

namespace renderstack
{
namespace graphics
{

class Uniform_block;

class precision
{
public:
    enum value
    {
        lowp    = 0,
        mediump = 1,
        highp   = 2,
        superp  = 3 // only reserved - not used
    };
    static const char *const desc(value v)
    {
        switch (v)
        {
            case lowp: return "lowp";
            case mediump: return "mediump";
            case highp: return "highp";
            case superp: return "superp";
            default: return "???";
        }
    }
};

// uniforms can be either in a block or in program
// uniforms in program use location
// uniforms in block have index and offset in block
class Uniform
{
public:
    Uniform() = default;

    ~Uniform() = default;

    Uniform(const Uniform &other) = delete;

    Uniform(Uniform &&other) = default;

    Uniform(const std::string &name, Uniform_block *block, size_t count, gl::active_uniform_type::value type);

    Uniform(const std::string &name, int location, size_t count, gl::active_uniform_type::value type);

    Uniform(const std::string &name, Sampler *sampler, int texture_unit, gl::active_uniform_type::value type);

    bool is_array() const;

    void set_is_array(bool value);

    int texture_unit_index() const;

    void set_texture_unit_index(GLint value);

    precision::value precision_qualifier() const;

    void set_precision_qualifier(precision::value value);

    size_t count() const;

    gl::active_uniform_type::value type() const;

    const std::string &name() const;

    Sampler *sampler() const;

    void set_sampler(Sampler *value);

    // Only for uniforms in program, not in block
    int location() const;

    GLint block_index() const;

    // Only for uniforms in block
    int index_in_block() const;

    size_t offset_in_block() const; // Only for uniforms in block

    Uniform_block *block();

    // If uniform is in default block, access is index in block
    // If uniform is in non-default block, access is offset to uniform block / buffer range
    // Note that we do not return location, a
    size_t access() const;

private:
    std::string                    m_name;
    size_t                         m_count;
    gl::active_uniform_type::value m_type;
    precision::value               m_precision{precision::highp};
    bool                           m_is_array{false};
    int                            m_texture_unit_index{0};
    Sampler                        *m_sampler{nullptr};

    // Only used for uniforms in program
    int             m_location{-2};
    int             m_block_index{-1}; // -1 = default block

    // Only for uniforms in block
    Uniform_block *m_block{nullptr};
    int            m_index_in_block{0};
    size_t         m_offset_in_block{0};
};

} // namespace graphics
} // namespace renderstack

#endif
