#ifndef renderstack_mesh__normal_style_hpp
#define renderstack_mesh__normal_style_hpp

namespace renderstack { namespace mesh {

class normal_style
{
public:
   enum value
   {
      corner_normals,
      polygon_normals,
      point_normals
   };
   static const char *desc(enum value a);
};

} }

#endif
