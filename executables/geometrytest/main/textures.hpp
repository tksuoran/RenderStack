#ifndef textures_hpp
#define textures_hpp

#include "renderstack_toolkit/platform.hpp"
#include <string>

class textures
{
public:
   textures();

public:
   void on_load();
   void load(unsigned int *texture_object, int texture_unit, std::string const &path);

   unsigned int background_texture_object;
};

#endif