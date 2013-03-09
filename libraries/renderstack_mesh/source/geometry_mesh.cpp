#include "renderstack_toolkit/platform.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/constants.hpp>

#include "renderstack_mesh/geometry_mesh.hpp"
#include "renderstack_mesh/mesh.hpp"
#include "renderstack_toolkit/math_util.hpp"
#include "renderstack_toolkit/logstream.hpp"

#include "renderstack_geometry/attribute_map.hpp"
#include "renderstack_geometry/geometry.hpp"
#include "renderstack_geometry/corner.hpp"
#include "renderstack_geometry/polygon.hpp"
#include "renderstack_geometry/point.hpp"

#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_graphics/configuration.hpp"
#include "renderstack_graphics/vertex_attribute.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_graphics/vertex_stream_mappings.hpp"

#include <stdexcept>
#include <map>

#define LOG_CATEGORY &graphics_geometry_mesh

namespace renderstack { namespace mesh {

using namespace renderstack::geometry;
using namespace renderstack::graphics;
using namespace glm;
using namespace std;

static inline void write(char *data_ptr, gl::vertex_attrib_pointer_type::value type, unsigned int value);
static inline void write(char *data_ptr, gl::vertex_attrib_pointer_type::value type, vec2 const &value);
static inline void write(char *data_ptr, gl::vertex_attrib_pointer_type::value type, vec3 const &value);
static inline void write(char *data_ptr, gl::vertex_attrib_pointer_type::value type, vec4 const &value);

geometry_mesh::geometry_mesh(
   shared_ptr<renderstack::geometry::geometry>  geometry,
   geometry_mesh_format_info const              &format_info,
   geometry_mesh_buffer_info const              &buffer_info
)
:  m_geometry(geometry)
{
   build_mesh_from_geometry(format_info, buffer_info);
}

geometry_mesh::geometry_mesh(
   shared_ptr<renderstack::geometry::geometry> geometry,
   normal_style::value                         normal_style
)
:  m_geometry(geometry)
{
   geometry_mesh_format_info format_info;
   geometry_mesh_buffer_info buffer_info;

   format_info.set_normal_style(normal_style);
   build_mesh_from_geometry(format_info, buffer_info);
}

shared_ptr<mesh> geometry_mesh::get_mesh()
{
   return m_mesh;
}

void geometry_mesh::prepare_vertex_format(
   std::shared_ptr<renderstack::geometry::geometry> geometry,
   geometry_mesh_format_info const &format_info,
   geometry_mesh_buffer_info &buffer_info
)
{
   // TODO Consider case when using multiple vertex formats might work better?

   auto vf = buffer_info.vertex_format();
   // If vertex format is not yet specified, create a base vertex format
   if (!vf)
   {
      vf = make_shared<renderstack::graphics::vertex_format>();
      buffer_info.set_vertex_format(vf);

      if (format_info.want_position())
         (void)vf->make_attribute(vertex_attribute_usage::position, format_info.position_type(), format_info.position_type(), 0, 3);

      if (format_info.want_normal())
         (void)vf->make_attribute(vertex_attribute_usage::normal, format_info.normal_type(), format_info.normal_type(), 0, 3);

      if (format_info.want_normal_flat())
         (void)vf->make_attribute(vertex_attribute_usage::normal, format_info.normal_flat_type(), format_info.normal_flat_type(), 1, 3);

      if (format_info.want_normal_smooth())
         (void)vf->make_attribute(vertex_attribute_usage::normal, format_info.normal_smooth_type(), format_info.normal_smooth_type(), 2, 3);

      if (format_info.want_color())
         (void)vf->make_attribute(vertex_attribute_usage::color, format_info.color_type(), format_info.color_type(), 0, 4);

      if (format_info.want_id())
      {
         (void)vf->make_attribute(vertex_attribute_usage::id, format_info.id_vec3_type(), format_info.id_vec3_type(), 0, 3);

         if (configuration::use_integer_polygon_ids)
         {
            (void)vf->make_attribute(
               vertex_attribute_usage::id,
               gl::vertex_attrib_pointer_type::unsigned_int,
               gl::vertex_attrib_pointer_type::unsigned_int,
               0,
               1
            );
         }
      }
   }

   // If vertex format has no texture coordinate, check if we need to add one
   bool has_tex_coord = vf->has_attribute(vertex_attribute_usage::tex_coord, 0);

   if (!has_tex_coord && format_info.want_texcoord())
   {
      shared_ptr<attribute_map<corner*, vec2> > corner_texcoords;
      shared_ptr<attribute_map<point*, vec2> >  point_texcoords;

      if (geometry->corner_attributes().contains<vec2>("corner_texcoords"))
         corner_texcoords = geometry->corner_attributes().find<vec2>("corner_texcoords");

      if (geometry->point_attributes().contains<vec2>("point_texcoords"))
         point_texcoords = geometry->point_attributes().find<vec2>("point_texcoords");

      if (corner_texcoords || point_texcoords)
      {
         shared_ptr<vertex_attribute> attribute_texcoord = vf->make_attribute(
            vertex_attribute_usage::tex_coord,
            //gl::vertex_attrib_pointer_type::half_float,
            gl::vertex_attrib_pointer_type::float_,
            gl::vertex_attrib_pointer_type::float_,
            0,
            2
         );
      }
   }
}

void geometry_mesh::build_mesh_from_geometry(
   geometry_mesh_format_info const &format_info,
   geometry_mesh_buffer_info const &buffer_info
)
{
   slog_trace(
      "geometry_mesh::build_mesh_from_geometry(usage = %s, normal_style = %s, vertex_format = %p) geometry = %s",
      gl::enum_string(buffer_info.usage()),
      normal_style::desc(format_info.normal_style()),
      buffer_info.vertex_format().get(),
      m_geometry->name().c_str()
   );

   m_vertex_format = buffer_info.vertex_format();

   shared_ptr<attribute_map<polygon*,  vec3> >           polygon_ids_vector3;
   shared_ptr<attribute_map<polygon*,  unsigned int> >   polygon_ids_uint32;
   shared_ptr<attribute_map<polygon*,  vec3> >           polygon_normals;
   shared_ptr<attribute_map<polygon*,  vec3> >           polygon_centroids;
   shared_ptr<attribute_map<corner*,   vec3> >           corner_normals;
   shared_ptr<attribute_map<corner*,   vec2> >           corner_texcoords;
   shared_ptr<attribute_map<corner*,   vec4> >           corner_colors;
   shared_ptr<attribute_map<corner*,   unsigned int> >   corner_indices;
   shared_ptr<attribute_map<point*,    vec3> >           point_locations;
   shared_ptr<attribute_map<point*,    vec3> >           point_normals;
   shared_ptr<attribute_map<point*,    vec3> >           point_normals_smooth;
   shared_ptr<attribute_map<point*,    vec2> >           point_texcoords;
   shared_ptr<attribute_map<point*,    vec4> >           point_colors;

   if (!m_geometry->polygon_attributes().contains<vec3>("polygon_normals"))
      m_geometry->compute_polygon_normals();

   if (format_info.want_centroid_points())
      if (!m_geometry->point_attributes().contains<vec3>("polygon_centroids"))
         m_geometry->compute_polygon_centroids();

   if (format_info.want_id())
   {
      polygon_ids_vector3 = m_geometry->polygon_attributes().find_or_create<vec3>("polygon_ids_vec3");

      if (configuration::use_integer_polygon_ids)
         polygon_ids_uint32 = m_geometry->polygon_attributes().find_or_create<unsigned int>("polygon_ids_uint");
   }

   if (format_info.want_normal())
   {
      bool normals_found = false;
      if (m_geometry->corner_attributes().contains<vec3>("corner_normals"))
      {
         corner_normals = m_geometry->corner_attributes().find<vec3>("corner_normals");
         normals_found = true;
      }
      if (m_geometry->point_attributes().contains<vec3>("point_normals"))
      {
         point_normals = m_geometry->point_attributes().find<vec3>("point_normals");
         normals_found = true;
      }
      if (normals_found == false)
      {
         m_geometry->smooth_normalize("corner_normals", "polygon_normals", 0.0f * pi<float>());
         corner_normals = m_geometry->corner_attributes().find<vec3>("corner_normals");
      }
   }

   if (format_info.want_normal_smooth())
   {
      m_geometry->compute_point_normals("point_normals_smooth");
      point_normals_smooth = m_geometry->point_attributes().find<vec3>("point_normals_smooth");
   }

   if (format_info.want_texcoord())
   {
      if (m_geometry->corner_attributes().contains<vec2>("corner_texcoords"))
         corner_texcoords = m_geometry->corner_attributes().find<vec2>("corner_texcoords");

      if (m_geometry->point_attributes().contains<vec2>("point_texcoords"))
         point_texcoords = m_geometry->point_attributes().find<vec2>("point_texcoords");
   }

   if (format_info.want_color())
   {
      if (m_geometry->corner_attributes().contains<vec4>("corner_colors"))
         corner_colors = m_geometry->corner_attributes().find<vec4>("corner_colors");

      if (m_geometry->point_attributes().contains<vec4>("point_colors"))
         point_colors = m_geometry->point_attributes().find<vec4>("point_colors");
   }

   polygon_normals = m_geometry->polygon_attributes().find<vec3>("polygon_normals");

   if (format_info.want_centroid_points())
      polygon_centroids = m_geometry->polygon_attributes().find<vec3>("polygon_centroids");

   if (format_info.want_position())
      point_locations = m_geometry->point_attributes().find<vec3>("point_locations");

   corner_indices = m_geometry->corner_attributes().find_or_create<unsigned int>("corner_indices");

   auto attribute_position      = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::position,     0);
   auto attribute_normal        = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::normal,       0);   /*  content normals     */
   auto attribute_normal_flat   = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::normal,       1);   /*  flat normals        */
   auto attribute_normal_smooth = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::normal,       2);   /*  smooth normals      */
   auto attribute_color         = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::color,        0);
   auto attribute_texcoord      = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::tex_coord,    0);
   auto attribute_id_vec3       = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::id,           0);

   shared_ptr<vertex_attribute> attribute_id_uint = nullptr;
   if (configuration::use_integer_polygon_ids)
      attribute_id_uint = m_vertex_format->find_attribute_maybe(vertex_attribute_usage::id, 0);

   size_t vertex_stride = m_vertex_format->stride();
   size_t o_position      = (attribute_position     ) ? attribute_position     ->offset() : 0;
   size_t o_normal        = (attribute_normal       ) ? attribute_normal       ->offset() : 0;
   size_t o_normal_flat   = (attribute_normal_flat  ) ? attribute_normal_flat  ->offset() : 0;
   size_t o_normal_smooth = (attribute_normal_smooth) ? attribute_normal_smooth->offset() : 0;
   size_t o_color         = (attribute_color        ) ? attribute_color        ->offset() : 0;
   size_t o_texcoord      = (attribute_texcoord     ) ? attribute_texcoord     ->offset() : 0;
   size_t o_id_vec3       = (attribute_id_vec3      ) ? attribute_id_vec3      ->offset() : 0;
   size_t o_id_uint       = (attribute_id_uint      ) ? attribute_id_uint      ->offset() : 0;

   auto t_position      = (attribute_position     ) ? attribute_position     ->data_type() : format_info.position_type();
   auto t_normal        = (attribute_normal       ) ? attribute_normal       ->data_type() : format_info.normal_type();
   auto t_normal_flat   = (attribute_normal_flat  ) ? attribute_normal_flat  ->data_type() : format_info.normal_flat_type();
   auto t_normal_smooth = (attribute_normal_smooth) ? attribute_normal_smooth->data_type() : format_info.normal_smooth_type();
   auto t_color         = (attribute_color        ) ? attribute_color        ->data_type() : format_info.color_type();
   auto t_texcoord      = (attribute_texcoord     ) ? attribute_texcoord     ->data_type() : format_info.texcoord_type();
   auto t_id_vec3       = (attribute_id_vec3      ) ? attribute_id_vec3      ->data_type() : format_info.id_vec3_type();
   auto t_id_uint       = (attribute_id_uint      ) ? attribute_id_uint      ->data_type() : format_info.id_uint_type();

   renderstack::geometry::geometry::mesh_info mi;
   m_geometry->info(mi);

   m_mesh = make_shared<mesh>();

   size_t total_vertex_count = 0;
   size_t total_index_count = 0;
   total_vertex_count += mi.vertex_count_corners;
   if (format_info.want_centroid_points())
      total_vertex_count += mi.vertex_count_centroids;
      
   if (format_info.want_fill_triangles())
      total_index_count += mi.index_count_fill_triangles;

   if (format_info.want_edge_lines())
      total_index_count += mi.index_count_edge_lines;

   if (format_info.want_corner_points())
      total_index_count += mi.index_count_corner_points;

   if (format_info.want_centroid_points())
      total_index_count += mi.index_count_centroid_points;

   if (buffer_info.vertex_buffer())
   {
      // Shared VBO given, allocate space from that
      // TODO: If there is not enough space in the shared VBO,
      //       allocate individual VBO as a fallback.
      m_mesh->allocate_vertex_buffer(buffer_info.vertex_buffer(), total_vertex_count);
   }
   else
   {
      // No shared VBO, allocate individual VBO
      m_mesh->allocate_vertex_buffer(m_vertex_format->stride(), total_vertex_count);
   }

   m_mesh->vertex_buffer()->bind();
   char *vertex_start = reinterpret_cast<char *>(
      m_mesh->vertex_buffer()->map_vertices(
         m_mesh->first_vertex(), 
         m_mesh->vertex_count(), 
         static_cast<gl::buffer_access_mask::value>(
            gl::buffer_access_mask::map_write_bit | 
            gl::buffer_access_mask::map_invalidate_range_bit
         )
      )
   );
   char *vertex_data = vertex_start;

   if (buffer_info.index_buffer())
   {
      // Shared IBO given, allocate range from that
      // TODO: If there is not enough space in the
      //       shared IBO, allocate an individual
      //       buffer as a fallback.
      m_mesh->allocate_index_buffer(buffer_info.index_buffer(), total_index_count);
   }
   else
   {
      // No shared IBO given, allocate individual IBO
      m_mesh->allocate_index_buffer(4, total_index_count);
   }
   m_mesh->index_buffer()->bind();

   // prepare index buffers
   if (format_info.want_fill_triangles())
      m_mesh->allocate_index_range(gl::begin_mode::triangles, mi.index_count_fill_triangles, m_fill_indices);

   if (format_info.want_edge_lines())
      m_mesh->allocate_index_range(gl::begin_mode::lines, mi.index_count_edge_lines, m_edge_line_indices);

   if (format_info.want_corner_points())
      m_mesh->allocate_index_range(gl::begin_mode::points, mi.index_count_corner_points, m_corner_point_indices);

   if (format_info.want_centroid_points())
      m_mesh->allocate_index_range(gl::begin_mode::points, mi.polygon_count, m_polygon_centroid_indices);

   unsigned int *index_start = reinterpret_cast<unsigned int *>(
      m_mesh->index_buffer()->map_indices(
         m_mesh->first_index(), 
         m_mesh->index_count(), 
         static_cast<gl::buffer_access_mask::value>(
            gl::buffer_access_mask::map_write_bit | 
            gl::buffer_access_mask::map_invalidate_range_bit
         )
      )
   );
   unsigned int *corner_point_index_data       = &index_start[m_corner_point_indices.first_index];
   unsigned int *fill_index_data               = &index_start[m_fill_indices.first_index];
   unsigned int *edge_line_index_data          = &index_start[m_edge_line_indices.first_index];
   unsigned int *polygon_centroid_index_data   = &index_start[m_polygon_centroid_indices.first_index];

   unsigned int polygon_index = 0;
   unsigned int vertex_index = 0;
   //unsigned int index_index = 0;

   unsigned int vertices_written                   = 0;
   unsigned int corner_point_indices_written       = 0;
   unsigned int triangle_indices_written           = 0;
   unsigned int edge_line_indices_written          = 0;
   //unsigned int corner_indices_written             = 0;
   unsigned int polygon_centroid_indices_written   = 0;

#  if 1  // polygons
   corner_indices->clear();
   vec3 unit_y(0.0f, 1.0f, 0.0f);
   for (auto i = m_geometry->polygons().begin(); i != m_geometry->polygons().end(); ++i)
   {
      class polygon *polygon = *i;

      if (format_info.want_id())
      {
         if (polygon_ids_uint32)
            polygon_ids_uint32->set_value(polygon, polygon_index);

         polygon_ids_vector3->set_value(polygon, vec3_from_uint(polygon_index));
      }

      vec3 polygon_normal(0.0f, 1.0f, 0.0f);

      if (
         polygon_normals &&
         polygon_normals->has(polygon)
      )
      {
         polygon_normal = polygon_normals->value(polygon);
      }

      unsigned int first_index    = vertex_index;
      unsigned int previous_index = first_index;

#     if 1 /* corners */
      for (auto j = polygon->corners().cbegin(); j != polygon->corners().cend(); ++j)
      {
         class corner *corner = *j;

         //  Position
         if (format_info.want_position())
         {
            vec3 position = point_locations->value(corner->point());
            write(&vertex_data[o_position], t_position, position);
         }

         //  Normal
         vec3 normal(0.0f, 1.0f, 0.0f);
         if (corner_normals && corner_normals->has(corner))
            normal = corner_normals->value(corner);
         else if (point_normals && point_normals->has(corner->point()))
            normal = point_normals->value(corner->point());
         else if (point_normals_smooth && point_normals_smooth->has(corner->point()))
            normal = point_normals_smooth->value(corner->point());

         vec3 point_normal(0.0f, 1.0f, 0.0f);
         if (point_normals && point_normals->has(corner->point()))
            point_normal = point_normals->value(corner->point());
         else if (point_normals_smooth && point_normals_smooth->has(corner->point()))
            point_normal = point_normals_smooth->value(corner->point());

         if (format_info.want_normal())
         {
            switch (format_info.normal_style())
            {
            case normal_style::corner_normals:
               write(&vertex_data[o_normal], t_normal, normal);
               break;
            case normal_style::point_normals:
               write(&vertex_data[o_normal], t_normal, point_normal);
               break;
            case normal_style::polygon_normals:
               write(&vertex_data[o_normal], t_normal, polygon_normal);
               break;
            }
         }

         if (format_info.want_normal_flat() && attribute_normal_flat)
            write(&vertex_data[o_normal_flat], t_normal_flat, polygon_normal);

         if (format_info.want_normal_smooth() && attribute_normal_smooth)
            write(&vertex_data[o_normal_smooth], t_normal_smooth, point_normals_smooth->value(corner->point()));

         //  Texcoord
         if (format_info.want_texcoord() && attribute_texcoord)
         {
            vec2 texcoord(0.0f, 0.0f);
            if (corner_texcoords && corner_texcoords->has(corner))
               texcoord = corner_texcoords->value(corner);
            else if (point_texcoords && point_texcoords->has(corner->point()))
               texcoord = point_texcoords->value(corner->point());

            write(&vertex_data[o_texcoord], t_texcoord, texcoord);
         }

         //  Vertex Color
         if (format_info.want_color() && attribute_color)
         {
            if (corner_colors && corner_colors->has(corner))
               write(&vertex_data[o_color], t_color, corner_colors->value(corner));
            else if (point_colors && point_colors->has(corner->point()))
               write(&vertex_data[o_color], t_color, point_colors->value(corner->point()));
            else
               write(&vertex_data[o_color], t_color, vec4(1.0f, 1.0f, 1.0f, 1.0f));
         }

         //  PolygonId
         if (format_info.want_id())
         {
            if (configuration::use_integer_polygon_ids && attribute_id_uint)
               write(&vertex_data[o_id_uint], t_id_uint, polygon_index);

            if (attribute_id_vec3)
            {
               vec3 v = vec3_from_uint(polygon_index);
               write(&vertex_data[o_id_vec3], t_id_vec3, v);
            }
         }

         ++vertices_written;

         if (format_info.want_corner_points())
         {
            *corner_point_index_data++ = vertex_index;
            ++corner_point_indices_written;
         }

         corner_indices->set_value(corner, vertex_index);

         if (format_info.want_fill_triangles())
         {
            if (previous_index != first_index)
            {
               *fill_index_data++ = first_index;
               *fill_index_data++ = vertex_index;
               *fill_index_data++ = previous_index;
               triangle_indices_written += 3;
            }
         }

         previous_index = vertex_index;

         vertex_data = &vertex_data[vertex_stride];
         ++vertex_index;
      }
#     endif /* corners */

      ++polygon_index;
   }
#  endif /* polygons */

   if (format_info.want_edge_lines())
   {
      for (auto i = m_geometry->edges().cbegin(); i != m_geometry->edges().cend(); ++i)
      {
         struct edge const &edge = i->first;
         point *pa = edge.a();
         point *pb = edge.b();
         corner *ca = pa->corners().front();
         corner *cb = pb->corners().front();

         if (corner_indices->has(ca) && corner_indices->has(cb))
         {
            unsigned int i0 = corner_indices->value(ca);
            unsigned int i1 = corner_indices->value(cb);
            *edge_line_index_data++ = i0;
            *edge_line_index_data++ = i1;
            edge_line_indices_written += 2;
         }
      }
   }

   if (format_info.want_centroid_points())
   {
      for (auto i = m_geometry->polygons().cbegin(); i != m_geometry->polygons().cend(); ++i)
      {
         polygon *polygon = *i;
         vec3 &normal = unit_y;

         if (polygon_normals->has(polygon))
            normal = polygon_normals->value(polygon);

         if (format_info.want_position())
            write(&vertex_data[o_position], t_position, polygon_centroids->value(polygon));

         if (format_info.want_normal() && attribute_normal)
            write(&vertex_data[o_normal], t_normal, normal);

         if (format_info.want_normal_flat() && attribute_normal_flat)
            write(&vertex_data[o_normal_flat], t_normal_flat, normal);

         *polygon_centroid_index_data++ = vertex_index;
         ++polygon_centroid_indices_written;

         vertex_data = &vertex_data[vertex_stride];
         ++vertices_written;
         ++vertex_index;
      }
   }

   m_mesh->vertex_buffer()->unmap_vertices();
   m_mesh->index_buffer()->unmap_indices();

   if (vertices_written != vertex_index)
      throw runtime_error("written vertex count does not match written indices");

   if (vertices_written != total_vertex_count)
      throw runtime_error("written vertex count does not match expected vertex count");

   if (!format_info.keep_geometry())
      m_geometry.reset();

   check_memory_system();
}

void geometry_mesh::use_vertex_stream()
{
   m_vertex_stream.use();
}

void geometry_mesh::setup_vertex_stream(shared_ptr<renderstack::graphics::vertex_stream_mappings> mappings)
{
   mappings->bind_attributes(m_vertex_stream, *(vertex_format().get()));
}

void geometry_mesh::render(
   gl::begin_mode::value   begin_mode,
   index_range const       &index_range, 
   normal_style::value     normal_style
)
{
   (void)normal_style; // TODO
   m_vertex_stream.draw_elements_base_vertex(
      begin_mode,
      static_cast<GLsizei>(index_range.index_count),
      gl::draw_elements_type::unsigned_int,
      (
         index_range.first_index + get_mesh()->first_index()
      ) * sizeof(unsigned int),
      static_cast<GLint>(get_mesh()->first_vertex())
   );
}

static inline void write(
   char                                   *data_ptr, 
   gl::vertex_attrib_pointer_type::value  type,
   unsigned int                           value
)
{
   switch (type)
   {
   case gl::vertex_attrib_pointer_type::unsigned_byte:
      {
         GLubyte *ptr = reinterpret_cast<GLubyte*>(data_ptr);
         assert(value <= 0xff);
         ptr[0] = value & 0xff;
      }
      break;
   case gl::vertex_attrib_pointer_type::unsigned_short:
      {
         GLushort *ptr = reinterpret_cast<GLushort*>(data_ptr);
         assert(value <= 0xffff);
         ptr[0] = value & 0xffff;
      }
      break;
   case gl::vertex_attrib_pointer_type::unsigned_int:
      {
         GLuint *ptr = reinterpret_cast<GLuint*>(data_ptr);
         ptr[0] = value;
      }
      break;
   default:
      throw std::runtime_error("bad index type");
   }
}
static inline void write(
   char                                   *data_ptr, 
   gl::vertex_attrib_pointer_type::value  type,
   vec2 const                             &value
)
{
   switch (type)
   {
   case gl::vertex_attrib_pointer_type::float_:
      {
         float *ptr = reinterpret_cast<float*>(data_ptr);
         ptr[0] = value.x;
         ptr[1] = value.y;
      }
      break;
   case gl::vertex_attrib_pointer_type::half_float:
      {
         short *ptr = reinterpret_cast<short*>(data_ptr);
         ptr[0] = glm::half(value.x)._data();
         ptr[1] = glm::half(value.y)._data();
      }
      break;
   default:
      throw std::runtime_error("unsupported attribute type");
   }
}
static inline void write(
   char                                   *data_ptr, 
   gl::vertex_attrib_pointer_type::value  type,
   vec3 const                             &value
)
{
   switch (type)
   {
   case gl::vertex_attrib_pointer_type::float_:
      {
         float *ptr = reinterpret_cast<float*>(data_ptr);
         ptr[0] = value.x;
         ptr[1] = value.y;
         ptr[2] = value.z;
      }
      break;
   case gl::vertex_attrib_pointer_type::half_float:
      {
         short *ptr = reinterpret_cast<short*>(data_ptr);
         ptr[0] = glm::half(value.x)._data();
         ptr[1] = glm::half(value.y)._data();
         ptr[2] = glm::half(value.z)._data();
      }
      break;
   default:
      throw std::runtime_error("unsupported attribute type");
   }
}
static void write(
   char                                    *data_ptr, 
   gl::vertex_attrib_pointer_type::value   type,
   vec4 const                              &value
);
static void write(
   char                                    *data_ptr, 
   gl::vertex_attrib_pointer_type::value   type,
   vec4 const                              &value
)
{
   switch (type)
   {
   case gl::vertex_attrib_pointer_type::float_:
      {
         float *ptr = reinterpret_cast<float*>(data_ptr);
         ptr[0] = value.x;
         ptr[1] = value.y;
         ptr[2] = value.z;
         ptr[3] = value.w;
      }
      break;
   case gl::vertex_attrib_pointer_type::half_float:
      {
         short *ptr = reinterpret_cast<short*>(data_ptr);
         ptr[0] = glm::half(value.x)._data();
         ptr[1] = glm::half(value.y)._data();
         ptr[2] = glm::half(value.z)._data();
         ptr[3] = glm::half(value.w)._data();
      }
      break;
   default:
      throw std::runtime_error("unsupported attribute type");
   }
}


} }
