#ifndef shader_monitor_hpp_renderstack_graphics
#define shader_monitor_hpp_renderstack_graphics

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"
#include <string>
#include <vector>
#include <memory>
#include <sys/stat.h>

namespace renderstack { namespace graphics {

class program;

class shader_monitor : public renderstack::toolkit::service
{
public:
   shader_monitor();
   /*virtual*/ ~shader_monitor();

   /*virtual*/ void initialize_service();

   void add(std::string const &path, std::shared_ptr<program> program_);
   void poll();
   void set_src_path(std::string const &src_path);
   void set_dst_path(std::string const &dst_path);

private:
   struct file
   {
      time_t                           last_time;
      std::string                      src_path;
      std::string                      dst_path;
      std::set<
         std::shared_ptr<class program>
      >                                programs;
   };

   std::vector<file> m_files;
   std::string       m_src_path;
   std::string       m_dst_path;
};


} }

#endif

