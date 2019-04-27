#ifndef file_hpp_renderstack_toolkit
#define file_hpp_renderstack_toolkit

#include "renderstack_toolkit/platform.hpp"
#include <string>

namespace renderstack
{
namespace toolkit
{

extern bool        exists(const std::string &fname);
extern std::string read(const std::string &fname);
extern std::string get_current_working_directory();

} // namespace toolkit
} // namespace renderstack

#endif
