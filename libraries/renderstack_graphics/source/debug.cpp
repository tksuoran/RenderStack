#include "renderstack_toolkit/platform.hpp"

#include "renderstack_graphics/log.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/vertex_array.hpp"
#include "renderstack_graphics/vertex_format.hpp"

#include <algorithm>

#define LOG_CATEGORY &log_renderer

namespace renderstack
{
namespace graphics
{

using namespace std;
using namespace renderstack::toolkit;

void debug_program()
{
    GLint current_program;
    GLint active_attributes;
    GLint link_status;

    gl::get_integer_v(GL_CURRENT_PROGRAM, &current_program);

    GLint p = current_program;

    if (p == 0)
        return;

    gl::get_program_iv(p, GL_ACTIVE_ATTRIBUTES, &active_attributes);
    gl::get_program_iv(p, GL_LINK_STATUS, &link_status);

    printf("Program:           %d\n", p);
    printf("Link status:       %d\n", link_status);
    printf("Active attributes: %d\n", active_attributes);
    for (int i = 0; i < active_attributes; ++i)
    {
        GLsizei buf_size = 1024;
        GLsizei length;
        GLint   size;
        GLenum  type;
        char    buffer[1024];

        gl::get_active_attrib(p, i, buf_size, &length, &size, &type, &buffer[0]);
        GLint location = gl::get_attrib_location(p, buffer);

        printf("Program attribute %d: %s size = %d, type %d, location = %d\n", i, buffer, size, type, location);
    }
}
void debug_buffer(GLenum target, GLint buffer, GLenum index_type)
{
    GLint64 size;
    GLint   usage;
    GLint   access_flags;
    GLint   mapped;
    GLvoid *map_pointer;
    GLint   map_offset;
    GLint   map_length;

    GLint old_binding;

    if (buffer == 0)
        return;

    switch (target)
    {
        case GL_ARRAY_BUFFER: gl::get_integer_v(GL_ARRAY_BUFFER_BINDING, &old_binding); break;
        case GL_ELEMENT_ARRAY_BUFFER: gl::get_integer_v(GL_ELEMENT_ARRAY_BUFFER_BINDING, &old_binding); break;
        case GL_PIXEL_PACK_BUFFER: gl::get_integer_v(GL_PIXEL_PACK_BUFFER_BINDING, &old_binding); break;
        case GL_PIXEL_UNPACK_BUFFER: gl::get_integer_v(GL_PIXEL_UNPACK_BUFFER_BINDING, &old_binding); break;
        case GL_COPY_READ_BUFFER: gl::get_integer_v(GL_COPY_READ_BUFFER_BINDING, &old_binding); break;
        case GL_COPY_WRITE_BUFFER: gl::get_integer_v(GL_COPY_WRITE_BUFFER_BINDING, &old_binding); break;
        case GL_UNIFORM_BUFFER: gl::get_integer_v(GL_UNIFORM_BUFFER_BINDING, &old_binding); break;
        case GL_TRANSFORM_FEEDBACK_BUFFER: gl::get_integer_v(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, &old_binding); break;
        case GL_DRAW_INDIRECT_BUFFER: gl::get_integer_v(GL_DRAW_INDIRECT_BUFFER_BINDING, &old_binding); break;
        default:
            old_binding = -1;
    }

    gl::bind_buffer(target, buffer);

    gl::get_buffer_parameter_i64v(target, GL_BUFFER_SIZE, &size);
    gl::get_buffer_parameter_iv(target, GL_BUFFER_USAGE, &usage);
    gl::get_buffer_parameter_iv(target, GL_BUFFER_ACCESS_FLAGS, &access_flags);
    gl::get_buffer_parameter_iv(target, GL_BUFFER_MAPPED, &mapped);
    gl::get_buffer_pointer_v(target, GL_BUFFER_MAP_POINTER, &map_pointer);
    gl::get_buffer_parameter_iv(target, GL_BUFFER_MAP_OFFSET, &map_offset);
    gl::get_buffer_parameter_iv(target, GL_BUFFER_MAP_LENGTH, &map_length);

    printf("Buffer %d\n", buffer);
    printf("Size:           %lu\n", static_cast<unsigned long>(size));
    printf("Usage:          %d\n", usage);
    printf("Access flags:   %d\n", access_flags);
    printf("Mapped:         %d\n", mapped);
    printf("Map pointer:    %p\n", map_pointer);
    printf("Map offset:     %d\n", map_offset);
    printf("Map length:     %d\n", map_length);

    if (!mapped)
    {
        switch (index_type)
        {
            case GL_NONE:
            {
                size_t count = static_cast<size_t>(size) / sizeof(float);
                float *data  = new float[count];

                gl::get_buffer_sub_data(target, 0, count * 4, data);

                for (size_t i = 0; i < count && i < 16; ++i)
                    printf("%f ", data[i]);

                printf("\n");

                delete[] data;
            }
            break;

            case GL_UNSIGNED_BYTE:
            {
                size_t   count = static_cast<size_t>(size) / sizeof(uint8_t);
                uint8_t *data  = new uint8_t[count];

                gl::get_buffer_sub_data(target, 0, count * sizeof(uint8_t), data);
                for (size_t i = 0; i < count && i < 16; ++i)
                    printf("%u ", data[i]);

                printf("\n");

                delete[] data;
            }
            break;

            case GL_UNSIGNED_SHORT:
            {
                size_t    count = static_cast<size_t>(size) / sizeof(uint16_t);
                uint16_t *data  = new uint16_t[count];

                gl::get_buffer_sub_data(target, 0, count * sizeof(uint16_t), data);
                for (size_t i = 0; i < count && i < 16; ++i)
                    printf("%u ", data[i]);

                printf("\n");

                delete[] data;
            }
            break;

            case GL_UNSIGNED_INT:
            {
                size_t    count = static_cast<size_t>(size) / sizeof(uint32_t);
                uint32_t *data  = new uint32_t[count];

                gl::get_buffer_sub_data(target, 0, count * sizeof(uint32_t), data);
                for (size_t i = 0; i < count && i < 16; ++i)
                    printf("%u ", data[i]);

                printf("\n");

                delete[] data;
            }
            break;

            default:
                assert(0);
        }
    }

    if (old_binding != -1)
        gl::bind_buffer(target, old_binding);
}
void debug_vao()
{
    GLint max_vertex_attribs;
    GLint vertex_array_binding;
    GLint element_array_buffer_binding;

    gl::get_integer_v(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);
    gl::get_integer_v(GL_VERTEX_ARRAY_BINDING, &vertex_array_binding);
    gl::get_integer_v(GL_ELEMENT_ARRAY_BUFFER_BINDING, &element_array_buffer_binding);

    printf("Vertex array binding:          %d\n", vertex_array_binding);
    printf("Element array buffer binding:  %d\n", element_array_buffer_binding);
    debug_buffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_binding, GL_UNSIGNED_SHORT);

    for (int i = 0; i < max_vertex_attribs; ++i)
    {
        GLint   buffer_binding;
        GLint   enabled;
        GLint   size;
        GLint   stride;
        GLint   type;
        GLint   normalized;
        GLint   integer;
        GLint   divisor;
        GLvoid *pointer;

        gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &buffer_binding);
        gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
        gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
        gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
        gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized);
        gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_INTEGER, &integer);
        gl::get_vertex_attrib_iv(i, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, &divisor);

        gl::get_vertex_attrib_pointer_v(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);

        if (buffer_binding || enabled)
        {
            printf("Vertex attrib %d: buffer = %d, enabled = %d, size = %d, stride = %d, type = %d, normalized = %d, integer = %d, divisor = %d, pointer = %u\n",
                   i,
                   buffer_binding,
                   enabled,
                   size,
                   stride,
                   type,
                   normalized,
                   integer,
                   divisor,
                   static_cast<unsigned int>(
                       reinterpret_cast<uintptr_t>(pointer) & 0xffffffff));

            debug_buffer(GL_ARRAY_BUFFER, buffer_binding, GL_NONE);
        }
    }
}
void debug_gl_state()
{
    debug_program();
    debug_vao();

    GLint active_texture;
    gl::get_integer_v(GL_ACTIVE_TEXTURE, &active_texture);
}

} // namespace graphics
} // namespace renderstack