#ifndef renderstack_mesh__build_info_hpp
#define renderstack_mesh__build_info_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/gl.hpp"
#include "renderstack_toolkit/strong_gl_enums.hpp"
#include "renderstack_graphics/index_buffer.hpp"
#include "renderstack_graphics/vertex_buffer.hpp"
#include "renderstack_graphics/vertex_format.hpp"
#include "renderstack_graphics/vertex_stream.hpp"
#include "renderstack_mesh/normal_style.hpp"
#include <cstddef>
#include <memory>

namespace renderstack { namespace mesh {

class normal_style;

// Controls what kind of mesh should be built from geometry
class geometry_mesh_format_info
{
public:
   geometry_mesh_format_info()
   :  m_want_fill_triangles   (false)
   ,  m_want_edge_lines       (false)
   ,  m_want_corner_points    (false)
   ,  m_want_centroid_points  (false)
   ,  m_want_position         (false)
   ,  m_want_normal           (false)
   ,  m_want_normal_flat      (false)
   ,  m_want_normal_smooth    (false)
   ,  m_want_color            (false)
   ,  m_want_texcoord         (false)
   ,  m_want_id               (false)
   ,  m_position_type         (gl::vertex_attrib_pointer_type::float_)
   ,  m_normal_type           (gl::vertex_attrib_pointer_type::half_float)
   ,  m_normal_flat_type      (gl::vertex_attrib_pointer_type::half_float)
   ,  m_normal_smooth_type    (gl::vertex_attrib_pointer_type::half_float)
   ,  m_color_type            (gl::vertex_attrib_pointer_type::half_float)
   ,  m_texcoord_type         (gl::vertex_attrib_pointer_type::half_float)
   ,  m_id_vec3_type          (gl::vertex_attrib_pointer_type::half_float) // TODO: Check
   ,  m_id_uint_type          (gl::vertex_attrib_pointer_type::float_)     // TODO: Check
   ,  m_keep_geometry         (false)
   ,  m_normal_style          (normal_style::corner_normals)
   {
   }

   bool want_fill_triangles   () const { return m_want_fill_triangles;   }
   bool want_edge_lines       () const { return m_want_edge_lines;       }
   bool want_corner_points    () const { return m_want_corner_points;    }
   bool want_centroid_points  () const { return m_want_centroid_points;  }
   bool want_position         () const { return m_want_position;         }
   bool want_normal           () const { return m_want_normal;           }
   bool want_normal_flat      () const { return m_want_normal_flat;      }
   bool want_normal_smooth    () const { return m_want_normal_smooth;    }
   bool want_color            () const { return m_want_color;            }
   bool want_texcoord         () const { return m_want_texcoord;         }
   bool want_id               () const { return m_want_id;               }
   bool keep_geometry         () const { return m_keep_geometry;         }

   void set_want_fill_triangles   (bool value) { m_want_fill_triangles  = value; }
   void set_want_edge_lines       (bool value) { m_want_edge_lines      = value; }
   void set_want_corner_points    (bool value) { m_want_corner_points   = value; }
   void set_want_centroid_points  (bool value) { m_want_centroid_points = value; }
   void set_want_position         (bool value) { m_want_position        = value; }
   void set_want_normal           (bool value) { m_want_normal          = value; }
   void set_want_normal_flat      (bool value) { m_want_normal_flat     = value; }
   void set_want_normal_smooth    (bool value) { m_want_normal_smooth   = value; }
   void set_want_color            (bool value) { m_want_color           = value; }
   void set_want_texcoord         (bool value) { m_want_texcoord        = value; }
   void set_want_id               (bool value) { m_want_id              = value; }

   void set_keep_geometry         (bool value) { m_keep_geometry        = value; }

   void set_normal_style(normal_style::value value) { m_normal_style = value; }

   gl::vertex_attrib_pointer_type::value position_type      () const { return m_position_type;      }
   gl::vertex_attrib_pointer_type::value normal_type        () const { return m_normal_type;        }
   gl::vertex_attrib_pointer_type::value normal_flat_type   () const { return m_normal_flat_type;   }
   gl::vertex_attrib_pointer_type::value normal_smooth_type () const { return m_normal_smooth_type; }
   gl::vertex_attrib_pointer_type::value color_type         () const { return m_color_type;         }
   gl::vertex_attrib_pointer_type::value texcoord_type      () const { return m_texcoord_type;      }
   gl::vertex_attrib_pointer_type::value id_vec3_type       () const { return m_id_vec3_type;       }
   gl::vertex_attrib_pointer_type::value id_uint_type       () const { return m_id_uint_type;       }

   normal_style::value normal_style() const { return m_normal_style; }

private:
   bool m_want_fill_triangles;
   bool m_want_edge_lines;
   bool m_want_corner_points;
   bool m_want_centroid_points;
   bool m_want_position;
   bool m_want_normal;
   bool m_want_normal_flat;
   bool m_want_normal_smooth;
   bool m_want_color;
   bool m_want_texcoord;
   bool m_want_id;

   bool m_keep_geometry;

   gl::vertex_attrib_pointer_type::value  m_position_type;
   gl::vertex_attrib_pointer_type::value  m_normal_type;
   gl::vertex_attrib_pointer_type::value  m_normal_flat_type;
   gl::vertex_attrib_pointer_type::value  m_normal_smooth_type;
   gl::vertex_attrib_pointer_type::value  m_color_type;
   gl::vertex_attrib_pointer_type::value  m_texcoord_type;
   gl::vertex_attrib_pointer_type::value  m_id_vec3_type;
   gl::vertex_attrib_pointer_type::value  m_id_uint_type;
   normal_style::value                    m_normal_style;
};

class geometry_mesh_buffer_info
{
public:
   geometry_mesh_buffer_info()   
   :  m_usage        (gl::buffer_usage_hint::static_draw)
   ,  m_normal_style (normal_style::corner_normals)
   ,  m_index_buffer (nullptr)
   ,  m_vertex_buffer(nullptr)
   ,  m_vertex_format(nullptr)
   {
   }

   gl::buffer_usage_hint::value                          usage        () const { return m_usage;         }
   normal_style::value                                   normal_style () const { return m_normal_style;  }
   std::shared_ptr<renderstack::graphics::index_buffer>  index_buffer () const { return m_index_buffer;  }
   std::shared_ptr<renderstack::graphics::vertex_buffer> vertex_buffer() const { return m_vertex_buffer; }
   std::shared_ptr<renderstack::graphics::vertex_format> vertex_format() const { return m_vertex_format; }

   void set_usage        (gl::buffer_usage_hint::value                          value) { m_usage = value; }
   void set_normal_style (normal_style::value                                   value) { m_normal_style = value; }
   void set_index_buffer (std::shared_ptr<renderstack::graphics::index_buffer>  value) { m_index_buffer = value; }
   void set_vertex_buffer(std::shared_ptr<renderstack::graphics::vertex_buffer> value) { m_vertex_buffer = value; }
   void set_vertex_format(std::shared_ptr<renderstack::graphics::vertex_format> value) { m_vertex_format = value; }

private:
   normal_style::value                                   m_normal_style;
   gl::buffer_usage_hint::value                          m_usage;
   std::shared_ptr<renderstack::graphics::index_buffer>  m_index_buffer;
   std::shared_ptr<renderstack::graphics::vertex_buffer> m_vertex_buffer;
   std::shared_ptr<renderstack::graphics::vertex_format> m_vertex_format;
};

} }

#endif
