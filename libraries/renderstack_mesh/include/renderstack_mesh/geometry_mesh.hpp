#ifndef renderstack_mesh__geometry_mesh_hpp
#define renderstack_mesh__geometry_mesh_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_mesh/build_info.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_mesh/normal_style.hpp"
#include "renderstack_mesh/index_range.hpp"
#include "renderstack_geometry/geometry.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/renderer.hpp"
#include <string>
#include <memory>

namespace renderstack { namespace graphics {

   class index_buffer;
   class vertex_format;
   class vertex_stream_mappings;

} }

namespace renderstack { namespace mesh {

class geometry_mesh
{
public:
   geometry_mesh(
      class renderstack::graphics::renderer &renderer,
      std::shared_ptr<renderstack::geometry::geometry> geometry,
      normal_style::value normal_style
   );

   geometry_mesh(
      class renderstack::graphics::renderer &renderer,
      std::shared_ptr<renderstack::geometry::geometry> geometry,
      geometry_mesh_format_info const &format_info,
      geometry_mesh_buffer_info const &buffer_info
   );

public:
   std::shared_ptr<mesh> get_mesh();

public:
   void build_mesh_from_geometry(
      class renderstack::graphics::renderer &renderer,
      geometry_mesh_format_info const &format_info,
      geometry_mesh_buffer_info const &buffer_info
   );

   std::shared_ptr<renderstack::graphics::vertex_format> vertex_format() { return m_vertex_format; }
   std::shared_ptr<renderstack::graphics::vertex_stream> vertex_stream() { return m_vertex_stream; }
   
   void setup_vertex_stream(
      geometry_mesh_buffer_info const &buffer_info,
      std::shared_ptr<renderstack::graphics::vertex_stream_mappings> mappings);

   index_range const &fill_indices             () const { return m_fill_indices; }
   index_range const &edge_line_indices        () const { return m_edge_line_indices; }
   index_range const &corner_point_indices     () const { return m_corner_point_indices; }
   index_range const &polygon_centroid_indices () const { return m_polygon_centroid_indices; }

   static void prepare_vertex_format(
      std::shared_ptr<renderstack::geometry::geometry> geometry,
      geometry_mesh_format_info const &format_info,
      geometry_mesh_buffer_info &buffer_info
   );

private:
   std::shared_ptr<renderstack::geometry::geometry>      m_geometry;
   std::shared_ptr<renderstack::graphics::vertex_format> m_vertex_format;
   std::shared_ptr<renderstack::graphics::vertex_stream> m_vertex_stream;
   std::shared_ptr<mesh>                                 m_mesh;
   index_range                                           m_fill_indices;
   index_range                                           m_edge_line_indices;
   index_range                                           m_corner_point_indices;
   index_range                                           m_polygon_centroid_indices;
};

} }

#endif
