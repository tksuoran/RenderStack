#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/constants.hpp>
#include "renderstack_geometry/cone.hpp"
#include <map>
#include <vector>
#include <map>

namespace renderstack { namespace geometry { namespace shapes {

   using namespace glm;
   using namespace std;

   /*  Stack numbering example                                                                 
   with stackDivision = 2                                                                  

   stack                        rel    known                                               
   base0  stack                 stack  as                                                  
   ----------------------------------------------------------------------------------------
   5      3         /\         1.0    top        (singular = single vertex in this case)  
   4      2        /--\                                                                   
   3      1       /----\                                                                  
   2      0      /======\      0.0    equator                                             
   1     -1     /--------\                                                                
   0     -2    /----------\                                                               
   -1     -3   /============\   -1.0    bottom    (not singular in this case)              

   bottom: relStack = -1.0  <=>  stack = -stackDivision - 1                                
   top:    relStack =  1.0  <=>  stack =  stackDivision + 1                                
   */
   struct cone::make_info
   {
      double   min_x;
      double   max_x;
      double   bottom_radius;
      double   top_radius;
      bool     use_bottom;
      bool     use_top;
      int      slice_count;
      int      stack_division;

      int      bottom_not_singular;
      int      top_not_singular;
      int      stack_count;

      double   rel_slice;
      double   rel_stack;

      map<pair<int, int>, class point*>  points;
      class point        *top;
      class point        *bottom;

      shared_ptr<attribute_map<point*, vec3> >   point_locations;
      shared_ptr<attribute_map<point*, vec3> >   point_normals;
      shared_ptr<attribute_map<point*, vec2> >   point_texcoords;
      shared_ptr<attribute_map<corner*, vec3> >  corner_normals;
      shared_ptr<attribute_map<corner*, vec2> >  corner_texcoords;
      shared_ptr<attribute_map<polygon*, vec3> > polygon_centroids;
      shared_ptr<attribute_map<polygon*, vec3> > polygon_normals;

      make_info(
         double  min_x_, 
         double  max_x_, 
         double  bottom_radius_, 
         double  top_radius_, 
         bool    use_bottom_, 
         bool    use_top_, 
         int     slice_count_,
         int     stack_division_
      )
      {
         min_x               = min_x_;
         max_x               = max_x_;
         bottom_radius       = bottom_radius_;
         top_radius          = top_radius_;
         use_bottom          = use_bottom_;
         use_top             = use_top_;
         slice_count         = slice_count_;
         stack_division      = stack_division_;
         stack_count         = (2 * stack_division) + 1 + bottom_not_singular + top_not_singular;

         bottom_not_singular  = (bottom_radius != 0.0) ? 1 : 0;
         top_not_singular     = (top_radius != 0.0) ? 1 : 0;
      }
   };

   point *cone::get_point(make_info &info, int slice, int stack)
   {
      return info.points[make_pair(slice, stack)];
   }

   //  relStackIn is in range -1..1
   //  relStack is in range 0..1
   point *cone::cone_point(make_info &info, double rel_slice, double rel_stack_in)
   {
      //var pointLocations      = PointAttributes.FindOrCreate<Vector3>("point_locations");
      //var pointNormals        = PointAttributes.FindOrCreate<Vector3>("point_normals");
      //var pointTexCoords      = PointAttributes.FindOrCreate<Vector2>("point_texcoords");

      double phi                  = pi<double>() * 2.0 * rel_slice;
      double sin_phi              = sin(phi);
      double cos_phi              = cos(phi);
      double rel_stack            = 0.5 + 0.5 * rel_stack_in;
      double one_minus_rel_stack  = 1.0 - rel_stack;

      vec3 position(
         (float)(one_minus_rel_stack * (info.min_x)                   + rel_stack * (info.max_x)),
         (float)(one_minus_rel_stack * (info.bottom_radius * sin_phi) + rel_stack * (info.top_radius * sin_phi)),
         (float)(one_minus_rel_stack * (info.bottom_radius * cos_phi) + rel_stack * (info.top_radius * cos_phi))
      );
      vec3 bottom(
         (float)(info.min_x),
         (float)(info.bottom_radius * sin_phi),
         (float)(info.bottom_radius * cos_phi)
      );
      vec3 top(
         (float)(info.max_x),
         (float)(info.top_radius * sin_phi),
         (float)(info.top_radius * cos_phi)
      );

      vec3 bitangent  = normalize(top - bottom);  /*  generatrix  */ 
      vec3 tangent(
         0.0f,
         (float)sin(phi + (pi<double>() * 0.5)),
         (float)cos(phi + (pi<double>() * 0.5))
      );
      vec3 normal = cross(bitangent, tangent);
      normal = normalize(normal);

      double s = info.rel_slice;
      double t = 0.5 + 0.5 * info.rel_stack;

      point *point = make_point();

      info.point_locations->set_value(point, vec3((float)position.x,  (float)position.y,  (float)position.z));
      info.point_normals  ->set_value(point, vec3((float)normal.x,    (float)normal.y,    (float)normal.z));
      info.point_texcoords->set_value(point, vec2((float)s,           (float)t));

      return point;
   }
   corner *cone::make_corner(make_info &info, polygon *polygon, int slice, int stack)
   {
      return make_corner(info, polygon, slice, stack, false);
   }
   corner *cone::make_corner(make_info &info, polygon *polygon, int slice, int stack, bool cap)
   {
      double rel_slice = (double)slice / (double)info.slice_count;
      double rel_stack = (double)stack / (double)(info.stack_division + 1);
      // centroid: double relSlice = ((double)slice + 0.5) / (double)info.sliceCount;
      //double relStack = -1.0 + (0.5 / (double)(info.stackDivision + 1));

      bool    slice_seam   = (slice == 0) || (slice == info.slice_count);
      bool    bottom      = stack == -info.stack_division - 1;
      bool    top         = stack == info.stack_division + 1;
      bool    uv_discontinuity = slice_seam || bottom || top;
      point *point;

      if (top && (info.top_radius == 0.0))
         point = info.top;
      else if (bottom && (info.bottom_radius == 0.0))
         point = info.bottom;
      else
      {
         if (slice == info.slice_count)
            point = info.points[make_pair(0, stack)];
         else
            point = info.points[make_pair(slice, stack)];
      }

      corner *corner = polygon->make_corner(point);

      if (uv_discontinuity)
      {
         float s = (float)(rel_slice);
         float t = (float)(0.5 + 0.5 * rel_stack);

         info.corner_texcoords->set_value(corner, vec2(s, t));
      }

      if (top || bottom)
      {
         if (cap && bottom && (info.bottom_radius != 0.0) && info.use_bottom)
            info.corner_normals->set_value(corner, vec3(-1.0f, 0.0f, 0.0f));

         if (cap && top && (info.top_radius != 0.0) && info.use_top)
            info.corner_normals->set_value(corner, vec3(1.0f, 0.0f, 0.0f));
      }
      return corner;
   }

   cone::cone(
      double  min_x, 
      double  max_x, 
      double  bottom_radius, 
      double  top_radius, 
      bool    use_bottom, 
      bool    use_top, 
      int     slice_count,
      int     stack_division
   )
   :  geometry("cone")
   {
      make_info info(min_x, max_x, bottom_radius, top_radius, use_bottom, use_top, slice_count, stack_division);

      info.point_locations    = point_attributes().find_or_create<vec3>("point_locations");
      info.point_normals      = point_attributes().find_or_create<vec3>("point_normals");
      info.point_texcoords    = point_attributes().find_or_create<vec2>("point_texcoords");
      info.polygon_centroids  = polygon_attributes().find_or_create<vec3>("polygon_centroids");
      info.polygon_normals    = polygon_attributes().find_or_create<vec3>("polygon_normals");
      info.corner_normals     = corner_attributes().find_or_create<vec3>("corner_normals");
      info.corner_texcoords   = corner_attributes().find_or_create<vec2>("corner_texcoords");

      //  Points
      for (int slice = 0; slice <= slice_count; ++slice)
      {
         double rel_slice = (double)slice / (double)slice_count;

         for (
            int stack = -stack_division - info.bottom_not_singular; 
            stack <= stack_division + info.top_not_singular; 
            ++stack
         )
         {
            double rel_stack = (double)stack / (double)(stack_division + 1);

            info.points[make_pair(slice, stack)] = cone_point(info, rel_slice, rel_stack);
         }
      }

      //  Bottom parts
      if (bottom_radius == 0.0)
      {
         info.bottom = make_point(min_x, 0.0, 0.0);  //  apex
         for (int slice = 0; slice < slice_count; ++slice)
         {
            int     stack               =  -stack_division;  //  second last stack, bottom is -stackDivision - 1
            double  rel_slice_centroid  = ((double)slice + 0.5) / (double)slice_count;
            double  rel_stack_centroid  = -1.0 + (0.5 / (double)(stack_division + 1));
            point   *centroid           = cone_point(info, rel_slice_centroid, rel_stack_centroid);
            polygon *polygon            = make_polygon();

            make_corner(info, polygon, slice + 1, stack);
            make_corner(info, polygon, slice,     stack);
            corner *tip = make_corner(info, polygon, slice, -stack_division - 1);

            vec3 n1 = info.point_normals->value(get_point(info, slice,     stack));
            vec3 n2 = info.point_normals->value(get_point(info, slice + 1, stack));
            vec3 average_normal = normalize(n1 + n2);
            info.corner_normals->set_value(tip, average_normal);

            vec2 t1 = info.point_texcoords->value(get_point(info, slice,     stack));
            vec2 t2 = info.point_texcoords->value(get_point(info, slice + 1, stack));
            vec2 average_texcoord = (t1 + t2) / 2.0f;
            info.corner_texcoords->set_value(tip, average_texcoord);

            info.polygon_centroids->set_value(polygon, info.point_locations->value(centroid));
            info.polygon_normals  ->set_value(polygon, info.point_normals  ->value(centroid));
         }
      }
      else
      {
         if (use_bottom)
         {
            polygon *polygon = make_polygon();
            info.polygon_centroids->set_value(polygon, vec3((float)min_x, 0.0f, 0.0f));
            info.polygon_normals  ->set_value(polygon, vec3(-1.0f,        0.0f, 0.0f));

            for (int slice = 0; slice < slice_count; ++slice)
            {
               int reverse_slice = slice_count - 1 - slice;

               make_corner(info, polygon, reverse_slice, -stack_division - 1, true);
            }
         }
      }

      //  Middle quads, bottom up
      for (
         int stack = -stack_division - info.bottom_not_singular; 
         stack < stack_division + info.top_not_singular; 
         ++stack
      )
      {
         double rel_stack_centroid = ((double)stack + 0.5) / (double)(stack_division + 1);

         for (int slice = 0; slice < slice_count; ++slice)
         {
            double rel_slice_centroid = ((double)(slice) + 0.5) / (double)(slice_count);

            point *centroid = cone_point(info, rel_slice_centroid, rel_stack_centroid);

            polygon *polygon = make_polygon();
            make_corner(info, polygon, slice + 1, stack + 1);
            make_corner(info, polygon, slice,     stack + 1);
            make_corner(info, polygon, slice,     stack    );
            make_corner(info, polygon, slice + 1, stack    );

            info.polygon_centroids->set_value(polygon, info.point_locations->value(centroid));
            info.polygon_normals  ->set_value(polygon, info.point_normals  ->value(centroid));
         }
      }

      //  Top parts
      if (top_radius == 0.0)
      {
         info.top = make_point(max_x, 0.0, 0.0);  //  apex

         for (int slice = 0; slice < slice_count; ++slice)
         {
            int     stack               = stack_division;
            double  rel_slice_centroid  = ((double)(slice) + 0.5) / (double)(slice_count);
            double  rel_stack_centroid  = 1.0 - (0.5 / (double)(stack_division + 1));
            point   *centroid           = cone_point(info, rel_slice_centroid, rel_stack_centroid);
            polygon *polygon            = make_polygon();

            corner *tip = make_corner(info, polygon, slice, stack_division + 1);
            make_corner(info, polygon, slice, stack);
            make_corner(info, polygon, slice + 1, stack);

            vec3 n1 = info.point_normals->value(get_point(info, slice, stack));
            vec3 n2 = info.point_normals->value(get_point(info, slice + 1, stack));
            vec3 average_normal = normalize(n1 + n2);
            info.corner_normals->set_value(tip, average_normal);

            vec2 t1 = info.point_texcoords->value(get_point(info, slice, stack));
            vec2 t2 = info.point_texcoords->value(get_point(info, slice + 1, stack));
            vec2 average_texcoord = (t1 + t2) / 2.0f;
            info.corner_texcoords->set_value(tip, average_texcoord);

            info.polygon_centroids->set_value(polygon, info.point_locations->value(centroid));
            info.polygon_normals  ->set_value(polygon, info.point_normals  ->value(centroid));
         }
      }
      else
      {
         if (use_top)
         {
            polygon *polygon = make_polygon();
            info.polygon_centroids->set_value(polygon, vec3((float)max_x, 0.0f, 0.0f));
            info.polygon_normals  ->set_value(polygon, vec3(1.0f, 0.0f, 0.0f));

            for (int slice = 0; slice < slice_count; ++slice)
               make_corner(info, polygon, slice, stack_division + 1, true);
         }
      }
   }

} } }
