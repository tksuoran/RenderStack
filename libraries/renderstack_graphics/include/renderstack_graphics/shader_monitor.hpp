#ifndef renderstack_graphics__shader_monitor_hpp
#define renderstack_graphics__shader_monitor_hpp

#include "renderstack_toolkit/platform.hpp"
#include <string>
#include <vector>
#include <memory>
#include <sys/stat.h>

namespace renderstack { namespace graphics {

class program;

class shader_monitor
{
public:
   void add(std::string const &path, class program *program);
   void poll();
   void set_src_path(std::string const &src_path);
   void set_dst_path(std::string const &dst_path);

private:
   struct file
   {
      time_t         last_time;
      std::string    src_path;
      std::string    dst_path;
      class program  *program;
   };

   std::vector<file> m_files;
   std::string       m_src_path;
   std::string       m_dst_path;
};


} }

#endif

