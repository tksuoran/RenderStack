#ifndef renderer_hpp_renderstack_graphics
#define renderer_hpp_renderstack_graphics

#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/color_mask_state.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/program.hpp"
#include "renderstack_graphics/sampler.hpp"
#include "renderstack_graphics/state_trackers.hpp"
#include "renderstack_graphics/texture.hpp"
#include "renderstack_graphics/uniform_buffer_range.hpp"
#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_toolkit/service.hpp"
#include <memory>
#include <stack>

namespace renderstack
{
namespace graphics
{

class Renderer : public renderstack::toolkit::service
{
private:
    Renderer(Renderer const &other);

public:
    Renderer();
    virtual ~Renderer();

    void initialize_service() override;

    State_trackers &track()
    {
        return m_track;
    }

    void push();

    void pop();

    void trash();

    Program *set_program(Program *program);

    // Set specified texture to specified texture unit.
    // Returns what was previously active texture unit, and what texture was bound to the specified unit
    Texture *set_texture(unsigned int unit, Texture *texture, unsigned int *old_unit = nullptr);

    Texture *reset_texture(unsigned int unit, Texture::Target target, unsigned int *old_unit = nullptr);

    // Switch active texture unit without touching bound texture
    unsigned int set_texture_unit(unsigned int unit);

    // Restores old texture to effective texture unit and then restores effective texture unit to old_unit
    void restore_texture(Texture::Target target, Texture *old_texture, unsigned int old_unit);

    Buffer *set_buffer(Buffer::Target target, Buffer *buffer);

    Vertex_array *set_vertex_array(Vertex_array *Vertex_array);

    void reset_vertex_array();

    void setup_attribute_pointers(Vertex_stream &vertex_stream, GLint basevertex);

    void use_vertex_stream(Vertex_stream &vertex_stream);

    void draw_elements_base_vertex(Vertex_stream &vertex_stream,
                                   GLenum begin_mode, GLsizei count, GLenum index_type, const GLvoid *indices,
                                   GLint base_vertex);

    Vertex_array *vertex_array() const
    {
        return m_effective.vertex_array_binding;
    }

    friend class Buffer;
    friend class Uniform_buffer_range;
    friend class Texture;

private:
    bool cache_enabled() const
    {
        return m_cache_enabled;
    }

    bool map_buffer(Buffer::Target target, Buffer *buffer);

    bool buffer_is_bound(Buffer::Target target, Buffer *buffer);

    bool buffer_is_mapped(Buffer::Target target, Buffer *buffer);

    bool unmap_buffer(Buffer::Target target, Buffer *buffer);

    bool texture_is_bound(unsigned int unit, Texture::Target target, Texture *texture);

    unsigned int effective_texture_unit() const;

private:
    void setup_attribute_pointers_new(Vertex_stream &vertex_stream, GLint basevertex);

    bool m_cache_enabled;

    State_trackers    m_track;
    std::stack<State> m_request_stack;
    State             m_requested;
    State             m_effective;

    std::array<Buffer *, Buffer::all_buffer_target_count> m_mapped_buffer;

    // When vertex array objects are used, we use this to track which attributes are enabled,
    // and set vertex attribute pointers through
    renderstack::graphics::Vertex_array m_default_vertex_array{true};
};

} // namespace graphics
} // namespace renderstack

#endif
