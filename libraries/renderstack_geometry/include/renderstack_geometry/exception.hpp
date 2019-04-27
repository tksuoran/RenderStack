#ifndef exception_hpp_renderstack_geometry
#define exception_hpp_renderstack_geometry

#include <cstdint>
#include <stdexcept>
#include <string>

namespace renderstack
{
namespace geometry
{

struct Property_map_not_found_exception
{
    Property_map_not_found_exception(const std::string &name) : name(name) {}

    std::string name;
};

struct Incompatible_attribute_type_exception
{
    Incompatible_attribute_type_exception(const std::string &name,
                                          const std::string &type_name,
                                          const std::string &expected_type_name) 
        : name(name)
        , type_name(type_name)
        , expected_type_name(expected_type_name) {}

    std::string name;
    std::string type_name;
    std::string expected_type_name;
};

struct Invalid_index_exception
{
    Invalid_index_exception(uint32_t index, uint32_t max_index) 
        : index(index)
        , max_index(max_index) {}

    uint32_t index;
    uint32_t max_index;
};

template <typename Key_type>
struct Key_not_found_exception
{
    Key_not_found_exception(Key_type key) : key (key) {}

    Key_type key;
};

} // namespace geometry
} // namespace renderstack

#endif
