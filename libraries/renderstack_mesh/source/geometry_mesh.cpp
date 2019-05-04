#include "renderstack_toolkit/math_util.hpp"

#include "renderstack_geometry/geometry.hpp"
#include "renderstack_geometry/property_map.hpp"

#include "renderstack_graphics/buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/vertex_attribute_mappings.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"

#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/log.hpp"
#include "renderstack_mesh/mesh.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/packing.hpp>
#include <glm/gtc/type_precision.hpp>

#include <map>
#include <stdexcept>

#define LOG_CATEGORY &log_geometry_mesh

namespace renderstack
{
namespace mesh
{

using namespace renderstack::toolkit;
using namespace renderstack::geometry;
using namespace renderstack::graphics;
using namespace glm;
using namespace std;

static inline void write(char *data_ptr, gl::vertex_attrib_pointer_type::value type, unsigned int value);
static inline void write(char *data_ptr, gl::vertex_attrib_pointer_type::value type, const vec2 &value);
static inline void write(char *data_ptr, gl::vertex_attrib_pointer_type::value type, const vec3 &value);
static inline void write(char *data_ptr, gl::vertex_attrib_pointer_type::value type, const vec4 &value);

Geometry_mesh::Geometry_mesh(Renderer          &renderer,
                             const Geometry    &geometry,
                             const Format_info &format_info,
                             const Buffer_info &buffer_info)
{
    build_mesh_from_geometry(renderer, geometry, format_info, buffer_info);
}

Geometry_mesh::Geometry_mesh(Renderer &          renderer,
                             const Geometry &    geometry,
                             Normal_style::value normal_style)
{
    Format_info format_info;
    Buffer_info buffer_info;

    format_info.normal_style = normal_style;
    build_mesh_from_geometry(renderer, geometry, format_info, buffer_info);
}

shared_ptr<mesh> Geometry_mesh::get_mesh()
{
    return m_mesh;
}

Geometry_mesh::Property_maps::Property_maps(const renderstack::geometry::Geometry &geometry,
                                            const Format_info &                    format_info)
{
    point_locations = geometry.point_attributes().maybe_find<vec3>("point_locations");

    if (format_info.want_id)
    {
        polygon_ids_vector3 = polygon_attributes.create<vec3>("polygon_ids_vec3");

        if (configuration::use_integer_polygon_ids)
        {
            polygon_ids_uint32 = polygon_attributes.create<unsigned int>("polygon_ids_uint");
        }
    }

    corner_normals  = geometry.corner_attributes().maybe_find<vec3>("corner_normals");
    point_normals   = geometry.point_attributes().maybe_find<vec3>("point_normals");
    polygon_normals = geometry.polygon_attributes().maybe_find<vec3>("polygon_normals");

    if (format_info.want_normal || format_info.want_normal_smooth)
    {
        if (polygon_normals == nullptr && point_locations != nullptr)
        {
            polygon_normals = polygon_attributes.create<vec3>("polygon_normals");
            for (auto polygon : geometry.polygons())
            {
                polygon->compute_normal(*polygon_normals, *point_locations);
            }
        }
    }

    if (format_info.want_normal_smooth)
    {
        point_normals_smooth = geometry.point_attributes().maybe_find<vec3>("point_normals_smooth");
        if (point_normals_smooth == nullptr)
        {
            for (auto point : geometry.points())
            {
                vec3 normal_sum(0.0f, 0.0f, 0.0f);
                for (auto corner : point->corners())
                {
                    normal_sum += polygon_normals->get(corner->polygon);
                }
                point_normals_smooth->put(point, normalize(normal_sum));
            }
        }
    }

    if (format_info.want_tangent)
    {
        corner_tangents = geometry.corner_attributes().maybe_find<vec3>("corner_tangents");
        point_tangents  = geometry.point_attributes().maybe_find<vec3>("point_tangents");
    }

    if (format_info.want_texcoord)
    {
        corner_texcoords = geometry.corner_attributes().maybe_find<vec2>("corner_texcoords");
        point_texcoords  = geometry.point_attributes().maybe_find<vec2>("point_texcoords");
    }

    if (format_info.want_color)
    {
        corner_colors = geometry.corner_attributes().maybe_find<vec4>("corner_colors");
        point_colors  = geometry.point_attributes().maybe_find<vec4>("point_colors");
    }

    if (format_info.want_centroid_points)
    {
        polygon_centroids = geometry.polygon_attributes().maybe_find<vec3>("polygon_centroids");
        if (polygon_centroids == nullptr && point_locations != nullptr)
        {
            polygon_centroids = polygon_attributes.create<vec3>("polygon_centroids");
            for (auto polygon : geometry.polygons())
            {
                polygon->compute_centroid(*polygon_centroids, *point_locations);
            }
        }
    }

    corner_indices = find_or_create<Corner *, unsigned int>(geometry.corner_attributes(),
                                                            corner_attributes,
                                                            "corner_indices");
}

void Geometry_mesh::prepare_vertex_format(const Geometry &   geometry,
                                          const Format_info &format_info,
                                          Buffer_info &      buffer_info)
{
    // TODO Consider case when using multiple vertex formats might work better?
    auto vf = buffer_info.vertex_format;

    // If vertex format is not yet specified, create a base vertex format
    if (!vf)
    {
        vf = make_shared<renderstack::graphics::Vertex_format>();

        buffer_info.vertex_format = vf;

        if (format_info.want_position)
        {
            (void)vf->make_attribute(vertex_attribute_usage::position, format_info.position_type, format_info.position_type, 0, 3);
        }

        if (format_info.want_normal)
        {
            (void)vf->make_attribute(vertex_attribute_usage::normal, format_info.normal_type, format_info.normal_type, 0, 3);
        }

        if (format_info.want_normal_flat)
        {
            (void)vf->make_attribute(vertex_attribute_usage::normal, format_info.normal_flat_type, format_info.normal_flat_type, 1, 3);
        }

        if (format_info.want_normal_smooth)
        {
            (void)vf->make_attribute(vertex_attribute_usage::normal, format_info.normal_smooth_type, format_info.normal_smooth_type, 2, 3);
        }

        if (format_info.want_tangent)
        {
            (void)vf->make_attribute(vertex_attribute_usage::tangent, format_info.tangent_type, format_info.tangent_type, 0, 3);
        }

        if (format_info.want_color)
        {
            (void)vf->make_attribute(vertex_attribute_usage::color, format_info.color_type, format_info.color_type, 0, 4);
        }

        if (format_info.want_id)
        {
            (void)vf->make_attribute(vertex_attribute_usage::id, format_info.id_vec3_type, format_info.id_vec3_type, 0, 3);

            if (configuration::use_integer_polygon_ids)
            {
                (void)vf->make_attribute(vertex_attribute_usage::id,
                                         gl::vertex_attrib_pointer_type::unsigned_int,
                                         gl::vertex_attrib_pointer_type::unsigned_int,
                                         0,
                                         1);
            }
        }
    }

    // If vertex format has no texture coordinate, check if we need to add one
    bool has_tex_coord = vf->has_attribute(vertex_attribute_usage::tex_coord, 0);

    if (!has_tex_coord && format_info.want_texcoord)
    {
        if (geometry.corner_attributes().contains<vec2>("corner_texcoords") || geometry.point_attributes().contains<vec2>("point_texcoords"))
        {
            vf->make_attribute(vertex_attribute_usage::tex_coord,
                               //gl::vertex_attrib_pointer_type::half_float,
                               gl::vertex_attrib_pointer_type::float_,
                               gl::vertex_attrib_pointer_type::float_,
                               0,
                               2);
        }
    }
}

void Geometry_mesh::build_mesh_from_geometry(renderstack::graphics::Renderer &renderer,
                                             const Geometry &                 geometry,
                                             const Format_info &              format_info,
                                             const Buffer_info &              buffer_info)
{
    slog_trace("Geometry_mesh::build_mesh_from_geometry(usage = %s, normal_style = %s, vertex_format = %p) geometry = %s",
               gl::enum_string(buffer_info.usage),
               Normal_style::desc(format_info.normal_style),
               buffer_info.vertex_format.get(),
               geometry.name().c_str());

    m_vertex_format = buffer_info.vertex_format;

    auto attribute_position      = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::position, 0);
    auto attribute_normal        = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::normal, 0); // content normals
    auto attribute_normal_flat   = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::normal, 1); // flat normals
    auto attribute_normal_smooth = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::normal, 2); // smooth normals
    auto attribute_tangent       = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::tangent, 0);
    auto attribute_color         = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::color, 0);
    auto attribute_texcoord      = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::tex_coord, 0);
    auto attribute_id_vec3       = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::id, 0);

    const Vertex_attribute *attribute_id_uint = nullptr;
    if (configuration::use_integer_polygon_ids)
    {
        attribute_id_uint = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::id, 0);
    }

    size_t vertex_stride   = m_vertex_format->stride();
    size_t o_position      = attribute_position ? attribute_position->offset : 0;
    size_t o_normal        = attribute_normal ? attribute_normal->offset : 0;
    size_t o_normal_flat   = attribute_normal_flat ? attribute_normal_flat->offset : 0;
    size_t o_normal_smooth = attribute_normal_smooth ? attribute_normal_smooth->offset : 0;
    size_t o_tangent       = attribute_tangent ? attribute_tangent->offset : 0;
    size_t o_color         = attribute_color ? attribute_color->offset : 0;
    size_t o_texcoord      = attribute_texcoord ? attribute_texcoord->offset : 0;
    size_t o_id_vec3       = attribute_id_vec3 ? attribute_id_vec3->offset : 0;
    size_t o_id_uint       = attribute_id_uint ? attribute_id_uint->offset : 0;

    auto t_position      = attribute_position ? attribute_position->data_type : format_info.position_type;
    auto t_normal        = attribute_normal ? attribute_normal->data_type : format_info.normal_type;
    auto t_normal_flat   = attribute_normal_flat ? attribute_normal_flat->data_type : format_info.normal_flat_type;
    auto t_normal_smooth = attribute_normal_smooth ? attribute_normal_smooth->data_type : format_info.normal_smooth_type;
    auto t_tangent       = attribute_tangent ? attribute_tangent->data_type : format_info.tangent_type;
    auto t_color         = attribute_color ? attribute_color->data_type : format_info.color_type;
    auto t_texcoord      = attribute_texcoord ? attribute_texcoord->data_type : format_info.texcoord_type;
    auto t_id_vec3       = attribute_id_vec3 ? attribute_id_vec3->data_type : format_info.id_vec3_type;
    auto t_id_uint       = attribute_id_uint ? attribute_id_uint->data_type : format_info.id_uint_type;

    m_vertex_stream = make_shared<renderstack::graphics::Vertex_stream>();
    auto va         = m_vertex_stream->vertex_array();
    auto old_va     = renderer.set_vertex_array(va);

    renderstack::geometry::Geometry::Mesh_info mi;
    geometry.info(mi);

    m_mesh = make_shared<mesh>();

    size_t total_vertex_count = 0U;
    size_t total_index_count  = 0U;
    total_vertex_count += mi.vertex_count_corners;
    if (format_info.want_centroid_points)
    {
        total_vertex_count += mi.vertex_count_centroids;
    }

    if (format_info.want_fill_triangles)
    {
        total_index_count += mi.index_count_fill_triangles;
    }

    if (format_info.want_edge_lines)
    {
        total_index_count += mi.index_count_edge_lines;
    }

    if (format_info.want_corner_points)
    {
        total_index_count += mi.index_count_corner_points;
    }

    if (format_info.want_centroid_points)
    {
        total_index_count += mi.index_count_centroid_points;
    }

    if (buffer_info.vertex_buffer)
    {
        // Shared VBO given, allocate space from that
        // TODO: If there is not enough space in the shared VBO,
        //       allocate individual VBO as a fallback.
        m_mesh->allocate_vertex_buffer(buffer_info.vertex_buffer, total_vertex_count);
    }
    else
    {
        // No shared VBO, allocate individual VBO
        m_mesh->allocate_vertex_buffer(renderer, m_vertex_format->stride(), total_vertex_count);
    }

    //auto  old_vbo      = 
    static_cast<void>(renderer.set_buffer(Buffer::Target::array_buffer, m_mesh->vertex_buffer().get()));

    char *vertex_start = reinterpret_cast<char *>(
        m_mesh->vertex_buffer()->map(
            renderer,
            m_mesh->first_vertex(),
            m_mesh->vertex_count(),
            static_cast<gl::buffer_access_mask::value>(
                gl::buffer_access_mask::map_write_bit |
                gl::buffer_access_mask::map_invalidate_range_bit)));
    char *vertex_data = vertex_start;

    if (buffer_info.index_buffer)
    {
        // Shared IBO given, allocate range from that
        // TODO: If there is not enough space in the
        //       shared IBO, allocate an individual
        //       buffer as a fallback.
        m_mesh->allocate_index_buffer(buffer_info.index_buffer, total_index_count);
    }
    else
    {
        // No shared IBO given, allocate individual IBO
        m_mesh->allocate_index_buffer(renderer, 4, total_index_count);
    }

    format_info.vertex_attribute_mappings->add_to_vertex_stream(*(m_vertex_stream.get()),
                                                                m_mesh->vertex_buffer().get(),
                                                                *(m_vertex_format.get()));

    // Setup vertex attribute pointers to VAO. This is not necessary if VAO is not used
    renderer.set_vertex_array(va);
    renderer.setup_attribute_pointers(*(m_vertex_stream.get()), 0);
    va->set_index_buffer(m_mesh->index_buffer().get());

    unsigned int base_vertex = configuration::can_use.draw_elements_base_vertex ? 0 : m_mesh->first_vertex();

    // prepare index buffers
    if (format_info.want_fill_triangles)
    {
        m_mesh->allocate_index_range(gl::begin_mode::triangles, mi.index_count_fill_triangles, m_fill_indices);
    }

    if (format_info.want_edge_lines)
    {
        m_mesh->allocate_index_range(gl::begin_mode::lines, mi.index_count_edge_lines, m_edge_line_indices);
    }

    if (format_info.want_corner_points)
    {
        m_mesh->allocate_index_range(gl::begin_mode::points, mi.index_count_corner_points, m_corner_point_indices);
    }

    if (format_info.want_centroid_points)
    {
        m_mesh->allocate_index_range(gl::begin_mode::points, mi.polygon_count, m_polygon_centroid_indices);
    }

    unsigned int *index_start = reinterpret_cast<unsigned int *>(
        m_mesh->index_buffer()->map(
            renderer,
            m_mesh->first_index(),
            m_mesh->index_count(),
            static_cast<gl::buffer_access_mask::value>(
                gl::buffer_access_mask::map_write_bit |
                gl::buffer_access_mask::map_invalidate_range_bit)));
    unsigned int *corner_point_index_data     = &index_start[m_corner_point_indices.first_index];
    unsigned int *fill_index_data             = &index_start[m_fill_indices.first_index];
    unsigned int *edge_line_index_data        = &index_start[m_edge_line_indices.first_index];
    unsigned int *polygon_centroid_index_data = &index_start[m_polygon_centroid_indices.first_index];

    unsigned int polygon_index = 0;
    unsigned int vertex_index  = 0;
    //unsigned int index_index = 0;

    unsigned int vertices_written             = 0;
    unsigned int corner_point_indices_written = 0;
    unsigned int triangle_indices_written     = 0;
    unsigned int edge_line_indices_written    = 0;
    //unsigned int corner_indices_written             = 0;
    unsigned int polygon_centroid_indices_written = 0;

    Property_maps property_maps(geometry, format_info);

    m_min = vec3(numeric_limits<float>::max());
    m_max = vec3(numeric_limits<float>::lowest());
    if (geometry.points().size() == 0 || property_maps.point_locations == nullptr)
    {
        m_min = m_max = vec3(0.0f);
    }
    else
    {
        for (auto point : geometry.points())
        {
            if (property_maps.point_locations->has(point))
            {
                vec3 position = property_maps.point_locations->get(point);
                m_min         = glm::min(m_min, position);
                m_max         = glm::max(m_max, position);
            }
        }
    }

#if 1 // polygons
    property_maps.corner_indices->clear();
    vec3 unit_y(0.0f, 1.0f, 0.0f);
    for (auto polygon : geometry.polygons())
    {
        if (format_info.want_id)
        {
            if (property_maps.polygon_ids_uint32 != nullptr)
            {
                property_maps.polygon_ids_uint32->put(polygon, polygon_index);
            }

            property_maps.polygon_ids_vector3->put(polygon, vec3_from_uint(polygon_index));
        }

        vec3 polygon_normal(0.0f, 1.0f, 0.0f);

        if (property_maps.polygon_normals)
        {
            property_maps.polygon_normals->maybe_get(polygon, polygon_normal);
        }

        unsigned int first_index    = vertex_index;
        unsigned int previous_index = first_index;

#    if 1 /* corners */
        for (auto corner : polygon->corners())
        {
            auto point = corner->point;

            //  Position
            if (format_info.want_position)
            {
                assert(property_maps.point_locations != nullptr);
                vec3 position = property_maps.point_locations->get(corner->point);
                write(&vertex_data[o_position], t_position, position);
            }

            //  Normal
            vec3 normal(0.0f, 1.0f, 0.0f);
            if (property_maps.corner_normals && property_maps.corner_normals->has(corner))
            {
                normal = property_maps.corner_normals->get(corner);
            }
            else if (property_maps.point_normals && property_maps.point_normals->has(point))
            {
                normal = property_maps.point_normals->get(point);
            }
            else if (property_maps.point_normals_smooth && property_maps.point_normals_smooth->has(point))
            {
                normal = property_maps.point_normals_smooth->get(point);
            }

            vec3 point_normal(0.0f, 1.0f, 0.0f);
            if (property_maps.point_normals && property_maps.point_normals->has(point))
            {
                point_normal = property_maps.point_normals->get(point);
            }
            else if (property_maps.point_normals_smooth && property_maps.point_normals_smooth->has(point))
            {
                point_normal = property_maps.point_normals_smooth->get(point);
            }

            if (format_info.want_normal)
            {
                switch (format_info.normal_style)
                {
                    case Normal_style::corner_normals:
                    {
                        write(&vertex_data[o_normal], t_normal, normal);
                        break;
                    }

                    case Normal_style::point_normals:
                    {
                        write(&vertex_data[o_normal], t_normal, point_normal);
                        break;
                    }

                    case Normal_style::polygon_normals:
                    {
                        write(&vertex_data[o_normal], t_normal, polygon_normal);
                        break;
                    }
                }
            }

            if (format_info.want_normal_flat && attribute_normal_flat)
            {
                write(&vertex_data[o_normal_flat], t_normal_flat, polygon_normal);
            }

            if (format_info.want_normal_smooth && attribute_normal_smooth)
            {
                vec3 smooth_point_normal(0.0f, 1.0f, 0.0f);
                if (property_maps.point_normals_smooth && property_maps.point_normals_smooth->has(point))
                {
                    smooth_point_normal = property_maps.point_normals_smooth->get(point);
                }
                write(&vertex_data[o_normal_smooth], t_normal_smooth, smooth_point_normal);
            }

            //  Tangent
            if (format_info.want_tangent && attribute_tangent)
            {
                vec3 tangent(1.0f, 0.0f, 0.0f);
                if (property_maps.corner_tangents && property_maps.corner_tangents->has(corner))
                {
                    tangent = property_maps.corner_tangents->get(corner);
                }
                else if (property_maps.point_tangents && property_maps.point_tangents->has(point))
                {
                    tangent = property_maps.point_tangents->get(point);
                }

                write(&vertex_data[o_tangent], t_tangent, tangent);
            }

            //  Texcoord
            if (format_info.want_texcoord && attribute_texcoord)
            {
                vec2 texcoord(0.0f, 0.0f);
                if (property_maps.corner_texcoords && property_maps.corner_texcoords->has(corner))
                {
                    texcoord = property_maps.corner_texcoords->get(corner);
                }
                else if (property_maps.point_texcoords && property_maps.point_texcoords->has(point))
                {
                    texcoord = property_maps.point_texcoords->get(point);
                }

                write(&vertex_data[o_texcoord], t_texcoord, texcoord);
            }

            //  Vertex Color
            if (format_info.want_color && attribute_color)
            {
                if (property_maps.corner_colors && property_maps.corner_colors->has(corner))
                {
                    write(&vertex_data[o_color], t_color, property_maps.corner_colors->get(corner));
                }
                else if (property_maps.point_colors && property_maps.point_colors->has(point))
                {
                    write(&vertex_data[o_color], t_color, property_maps.point_colors->get(point));
                }
                else
                {
                    write(&vertex_data[o_color], t_color, format_info.constant_color);
                }
            }

            //  PolygonId
            if (format_info.want_id)
            {
                if (configuration::use_integer_polygon_ids && attribute_id_uint)
                {
                    write(&vertex_data[o_id_uint], t_id_uint, polygon_index);
                }

                if (attribute_id_vec3)
                {
                    vec3 v = vec3_from_uint(polygon_index);
                    write(&vertex_data[o_id_vec3], t_id_vec3, v);
                }
            }

            ++vertices_written;

            // Indices
            if (format_info.want_corner_points)
            {
                *corner_point_index_data++ = vertex_index + base_vertex;
                ++corner_point_indices_written;
            }

            // TODO Where is this used? With or without + base_vertex?
            property_maps.corner_indices->put(corner, vertex_index /* + base_vertex*/);

            if (format_info.want_fill_triangles)
            {
                if (previous_index != first_index)
                {
                    *fill_index_data++ = first_index + base_vertex;
                    *fill_index_data++ = vertex_index + base_vertex;
                    *fill_index_data++ = previous_index + base_vertex;
                    triangle_indices_written += 3;
                }
            }

            previous_index = vertex_index;

            vertex_data = &vertex_data[vertex_stride];
            ++vertex_index;
        }
#    endif // corners

        ++polygon_index;
    }
#endif // polygons

    if (format_info.want_edge_lines)
    {
        for (auto i : geometry.edges())
        {
            auto &edge = i.first;

            Point *pa = edge.a();
            Point *pb = edge.b();

            Corner *ca = pa->corners().front();
            Corner *cb = pb->corners().front();

            if (property_maps.corner_indices->has(ca) && property_maps.corner_indices->has(cb))
            {
                unsigned int i0         = property_maps.corner_indices->get(ca);
                unsigned int i1         = property_maps.corner_indices->get(cb);
                *edge_line_index_data++ = i0 + base_vertex;
                *edge_line_index_data++ = i1 + base_vertex;
                edge_line_indices_written += 2;
            }
        }
    }

    if (format_info.want_centroid_points)
    {
        for (auto polygon : geometry.polygons())
        {
            vec3 &normal = unit_y;

            if (property_maps.polygon_normals->has(polygon))
            {
                normal = property_maps.polygon_normals->get(polygon);
            }

            if (format_info.want_position)
            {
                write(&vertex_data[o_position], t_position, property_maps.polygon_centroids->get(polygon));
            }

            if (format_info.want_normal && attribute_normal)
            {
                write(&vertex_data[o_normal], t_normal, normal);
            }

            if (format_info.want_normal_flat && attribute_normal_flat)
            {
                write(&vertex_data[o_normal_flat], t_normal_flat, normal);
            }

            *polygon_centroid_index_data++ = vertex_index + base_vertex;
            ++polygon_centroid_indices_written;

            vertex_data = &vertex_data[vertex_stride];
            ++vertices_written;
            ++vertex_index;
        }
    }

    m_mesh->vertex_buffer()->unmap(renderer);
    m_mesh->index_buffer()->unmap(renderer);

    (void)renderer.set_vertex_array(old_va);

    if (vertices_written != vertex_index)
    {
        throw runtime_error("written vertex count does not match written indices");
    }

    if (vertices_written != total_vertex_count)
    {
        throw runtime_error("written vertex count does not match expected vertex count");
    }

    check_memory_system();
}

#if 0
void Geometry_mesh::setup_vertex_stream(
   geometry_mesh_buffer_info const &buffer_info,
   shared_ptr<renderstack::graphics::Vertex_stream_mappings> mappings)
{
   mappings->add_to_vertex_stream(
      m_vertex_stream,
      buffer_info.vertex_buffer(),
      buffer_info.vertex_format());
}
#endif

static inline void write(char *                                data_ptr,
                         gl::vertex_attrib_pointer_type::value type,
                         unsigned int                          value)
{
    switch (type)
    {
        case gl::vertex_attrib_pointer_type::unsigned_byte:
        {
            GLubyte *ptr = reinterpret_cast<GLubyte *>(data_ptr);
            assert(value <= 0xffU);
            ptr[0] = value & 0xffU;
            break;
        }

        case gl::vertex_attrib_pointer_type::unsigned_short:
        {
            GLushort *ptr = reinterpret_cast<GLushort *>(data_ptr);
            assert(value <= 0xffffU);
            ptr[0] = value & 0xffffU;
            break;
        }

        case gl::vertex_attrib_pointer_type::unsigned_int:
        {
            GLuint *ptr = reinterpret_cast<GLuint *>(data_ptr);
            ptr[0]      = value;
            break;
        }

        default:
        {
            throw runtime_error("bad index type");
        }
    }
}
static inline void write(
    char *                                data_ptr,
    gl::vertex_attrib_pointer_type::value type,
    const vec2 &                          value)
{
    switch (type)
    {
        case gl::vertex_attrib_pointer_type::float_:
        {
            float *ptr = reinterpret_cast<float *>(data_ptr);
            ptr[0]     = value.x;
            ptr[1]     = value.y;
            break;
        }

        case gl::vertex_attrib_pointer_type::half_float:
        {
            // TODO Would this be safe even if we are not aligned?
            // uint *ptr = reinterpret_cast<uint*>(data_ptr);
            // *ptr = glm::packHalf2x16(value);
            uint16 *ptr = reinterpret_cast<uint16 *>(data_ptr);
            ptr[0]      = glm::packHalf1x16(value.x);
            ptr[1]      = glm::packHalf1x16(value.y);
            break;
        }

        default:
        {
            throw runtime_error("unsupported attribute type");
        }
    }
}
static inline void write(
    char *                                data_ptr,
    gl::vertex_attrib_pointer_type::value type,
    const vec3 &                          value)
{
    switch (type)
    {
        case gl::vertex_attrib_pointer_type::float_:
        {
            float *ptr = reinterpret_cast<float *>(data_ptr);
            ptr[0]     = value.x;
            ptr[1]     = value.y;
            ptr[2]     = value.z;
            break;
        }

        case gl::vertex_attrib_pointer_type::half_float:
        {
            uint16 *ptr = reinterpret_cast<uint16 *>(data_ptr);
            ptr[0]      = glm::packHalf1x16(value.x);
            ptr[1]      = glm::packHalf1x16(value.y);
            ptr[2]      = glm::packHalf1x16(value.z);
            break;
        }

        default:
        {
            throw runtime_error("unsupported attribute type");
        }
    }
}

static void write(
    char *                                data_ptr,
    gl::vertex_attrib_pointer_type::value type,
    const vec4 &                          value)
{
    switch (type)
    {
        case gl::vertex_attrib_pointer_type::float_:
        {
            float *ptr = reinterpret_cast<float *>(data_ptr);
            ptr[0]     = value.x;
            ptr[1]     = value.y;
            ptr[2]     = value.z;
            ptr[3]     = value.w;
            break;
        }

        case gl::vertex_attrib_pointer_type::half_float:
        {
            uint16 *ptr = reinterpret_cast<uint16 *>(data_ptr);
            // TODO glm::packHalf4x16() - but what if we are not aligned?
            ptr[0] = glm::packHalf1x16(value.x);
            ptr[1] = glm::packHalf1x16(value.y);
            ptr[2] = glm::packHalf1x16(value.z);
            ptr[3] = glm::packHalf1x16(value.w);
            break;
        }

        default:
        {
            throw runtime_error("unsupported attribute type");
        }
    }
}

} // namespace mesh
} // namespace renderstack
