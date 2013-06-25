#ifndef exception_hpp_renderstack_geometry
#define exception_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include <stdexcept>
#include <string>
#include <stdint.h>
//#include "renderstack_geometry/types.hpp"

namespace renderstack { namespace geometry {

struct attribute_map_not_found_exception
{
   attribute_map_not_found_exception(std::string const &name_)
   {
      this->name = name_;
   }
   std::string name;
};

struct incompatible_attribute_type_exception
{
   incompatible_attribute_type_exception(
      std::string name_,
      std::string type_name_,
      std::string expected_type_name_
   )
   {
      this->name               = name_;
      this->type_name          = type_name_;
      this->expected_type_name = expected_type_name_;
   }
   std::string name;
   std::string type_name;
   std::string expected_type_name;
};

struct invalid_index_exception
{
   invalid_index_exception(uint32_t index_, uint32_t max_index_)
   {
      this->index = index_;
      this->max_index = max_index_;
   }
   uint32_t index;
   uint32_t max_index;
};

template<typename key_type>
struct key_not_found_exception
{
   key_not_found_exception(key_type key_)
   {
      this->key = key_;
   }
   key_type key;
};

} }

#endif
