#ifndef geometry_mesh_hpp_renderstack_mesh
#define geometry_mesh_hpp_renderstack_mesh

#include "renderstack_geometry/geometry.hpp"
#include "renderstack_graphics/renderer.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_mesh/build_info.hpp"
#include "renderstack_mesh/index_range.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_mesh/normal_style.hpp"
#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include <memory>
#include <string>

namespace renderstack
{
namespace graphics
{

class Index_buffer;
class Vertex_format;
class Vertex_stream_mappings;

} // namespace graphics
} // namespace renderstack

namespace renderstack
{
namespace mesh
{

class Geometry_mesh
{
public:
    // Controls what kind of mesh should be built from geometry
    struct Format_info
    {
        bool want_fill_triangles {false};
        bool want_edge_lines     {false};
        bool want_corner_points  {false};
        bool want_centroid_points{false};
        bool want_position       {false};
        bool want_normal         {false};
        bool want_normal_flat    {false};
        bool want_normal_smooth  {false};
        bool want_tangent        {false};
        bool want_color          {false};
        bool want_texcoord       {false};
        bool want_id             {false};

        gl::vertex_attrib_pointer_type::value position_type     {gl::vertex_attrib_pointer_type::float_};
        gl::vertex_attrib_pointer_type::value normal_type       {gl::vertex_attrib_pointer_type::float_};
        gl::vertex_attrib_pointer_type::value normal_flat_type  {gl::vertex_attrib_pointer_type::float_};
        gl::vertex_attrib_pointer_type::value normal_smooth_type{gl::vertex_attrib_pointer_type::float_};
        gl::vertex_attrib_pointer_type::value tangent_type      {gl::vertex_attrib_pointer_type::float_};
        gl::vertex_attrib_pointer_type::value color_type        {gl::vertex_attrib_pointer_type::float_};
        gl::vertex_attrib_pointer_type::value texcoord_type     {gl::vertex_attrib_pointer_type::float_};
        gl::vertex_attrib_pointer_type::value id_vec3_type      {gl::vertex_attrib_pointer_type::float_};
        gl::vertex_attrib_pointer_type::value id_uint_type      {gl::vertex_attrib_pointer_type::float_};

        glm::vec4                                         constant_color{1.0f};
        bool                                              keep_geometry{false};
        Normal_style::value                               normal_style{Normal_style::corner_normals};
        renderstack::graphics::Vertex_attribute_mappings *vertex_attribute_mappings;
    };

    struct Buffer_info
    {
        gl::buffer_usage_hint::value                          usage{gl::buffer_usage_hint::static_draw};
        Normal_style::value                                   normal_style{Normal_style::corner_normals};
        std::shared_ptr<renderstack::graphics::Buffer>        index_buffer;
        std::shared_ptr<renderstack::graphics::Buffer>        vertex_buffer;
        std::shared_ptr<renderstack::graphics::Vertex_format> vertex_format;
    };

    struct Property_maps
    {
        Property_maps(const renderstack::geometry::Geometry &geometry, const Format_info &format_info);

        template <typename Key_type, typename Value_type>
        renderstack::geometry::Property_map<Key_type, Value_type> *
        find_or_create(const renderstack::geometry::Property_map_collection<Key_type> &geometry_attributes, 
                       renderstack::geometry::Property_map_collection<Key_type>       &attributes,
                       const std::string                                              &name)
        {
            renderstack::geometry::Property_map<Key_type, Value_type> *map = geometry_attributes.template maybe_find<Value_type>(name);
            if (map == nullptr)
            {
                map = attributes.template create<Value_type>(name);
            }
            return map;
        }

        renderstack::geometry::Property_map_collection<renderstack::geometry::Point *>   point_attributes;
        renderstack::geometry::Property_map_collection<renderstack::geometry::Corner *>  corner_attributes;
        renderstack::geometry::Property_map_collection<renderstack::geometry::Polygon *> polygon_attributes;
        renderstack::geometry::Property_map_collection<renderstack::geometry::Edge *>    edge_attributes;

        renderstack::geometry::Property_map<renderstack::geometry::Polygon *, glm::vec3>    *polygon_ids_vector3{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Polygon *, unsigned int> *polygon_ids_uint32{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Polygon *, glm::vec3>    *polygon_normals{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Polygon *, glm::vec3>    *polygon_centroids{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Corner *, glm::vec3>     *corner_normals{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Corner *, glm::vec3>     *corner_tangents{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Corner *, glm::vec2>     *corner_texcoords{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Corner *, glm::vec4>     *corner_colors{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Corner *, unsigned int>  *corner_indices{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Point *, glm::vec3>      *point_locations{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Point *, glm::vec3>      *point_normals{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Point *, glm::vec3>      *point_normals_smooth{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Point *, glm::vec3>      *point_tangents{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Point *, glm::vec2>      *point_texcoords{nullptr};
        renderstack::geometry::Property_map<renderstack::geometry::Point *, glm::vec4>      *point_colors{nullptr};
    };

    Geometry_mesh(renderstack::graphics::Renderer &renderer,
                  const renderstack::geometry::Geometry &geometry, // cannot modify geometry
                  Normal_style::value normal_style);

    Geometry_mesh(renderstack::graphics::Renderer &renderer,
                  const renderstack::geometry::Geometry &geometry,
                  const Format_info       &format_info,
                  const Buffer_info       &buffer_info);

    void get_property_maps(const renderstack::geometry::Geometry &geometry, Property_maps &maps) const;

    glm::vec3 min() const
    {
        return m_min;
    }

    glm::vec3 max() const
    {
        return m_max;
    }

    std::shared_ptr<mesh> get_mesh();

    void build_mesh_from_geometry(renderstack::graphics::Renderer &renderer,
                                  const renderstack::geometry::Geometry &geometry,
                                  const Format_info                     &format_info,
                                  const Buffer_info                     &buffer_info);

    std::shared_ptr<renderstack::graphics::Vertex_format> vertex_format()
    {
        return m_vertex_format;
    }

    std::shared_ptr<renderstack::graphics::Vertex_stream> vertex_stream()
    {
        return m_vertex_stream;
    }

    void setup_vertex_stream(const Buffer_info &buffer_info,
                             std::shared_ptr<renderstack::graphics::Vertex_stream_mappings> mappings);

    Index_range fill_indices() const
    {
        return m_fill_indices;
    }

    Index_range edge_line_indices() const
    {
        return m_edge_line_indices;
    }

    Index_range corner_point_indices() const
    {
        return m_corner_point_indices;
    }

    Index_range polygon_centroid_indices() const
    {
        return m_polygon_centroid_indices;
    }

    static void prepare_vertex_format(const renderstack::geometry::Geometry &geometry,
                                      const Format_info                     &format_info,
                                      Buffer_info                           &buffer_info);

private:
    std::shared_ptr<renderstack::graphics::Vertex_format> m_vertex_format;
    std::shared_ptr<renderstack::graphics::Vertex_stream> m_vertex_stream;
    glm::vec3                                             m_min; // bounding box
    glm::vec3                                             m_max;
    std::shared_ptr<mesh>                                 m_mesh;
    Index_range                                           m_fill_indices;
    Index_range                                           m_edge_line_indices;
    Index_range                                           m_corner_point_indices;
    Index_range                                           m_polygon_centroid_indices;
};

} // namespace mesh
} // namespace renderstack

#endif
