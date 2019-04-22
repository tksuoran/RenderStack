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

class uniform_block;

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
class uniform
{
public:
    uniform(std::string const &name, std::shared_ptr<uniform_block> block, std::size_t count, gl::active_uniform_type::value type);
    uniform(std::string const &name, int location, std::size_t count, gl::active_uniform_type::value type);

    bool                           is_array() const;
    void                           set_is_array(bool value);
    int                            texture_unit_index() const;
    void                           set_texture_unit_index(GLint value);
    precision::value               precision_qualifier() const;
    void                           set_precision_qualifier(precision::value value);
    std::size_t                    count() const;
    gl::active_uniform_type::value type() const;
    std::string const &            name() const;
    std::shared_ptr<class sampler> sampler() const;
    void                           set_sampler(std::shared_ptr<class sampler> value);

    // Only for uniforms in program, not in block
    int   location() const;
    GLint block_index() const;

    // Only for uniforms in block
    int                                index_in_block() const;
    std::size_t                        offset_in_block() const; // Only for uniforms in block
    std::weak_ptr<class uniform_block> block();

    // If uniform is in default block, access is index in block
    // If uniform is in non-default block, access is offset to uniform block / buffer range
    // Note that we do not return location, a
    std::size_t access() const;

private:
    std::string                    m_name;
    std::size_t                    m_count;
    gl::active_uniform_type::value m_type;
    precision::value               m_precision;
    bool                           m_is_array;
    int                            m_texture_unit_index;
    std::shared_ptr<class sampler> m_sampler;

    // Only used for uniforms in program
    int m_location;
    int m_block_index; // -1 = default block

    // Only for uniforms in block
    std::weak_ptr<class uniform_block> m_block;
    int                                m_index_in_block;
    std::size_t                        m_offset_in_block;
};

} // namespace graphics
} // namespace renderstack

#endif
