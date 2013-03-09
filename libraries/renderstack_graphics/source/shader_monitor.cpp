#include "renderstack_toolkit/platform.hpp"
#include "renderstack_graphics/shader_monitor.hpp"
#include "renderstack_graphics/program.hpp"
#include <vector>
#include <stdexcept>
#include <fstream>

namespace renderstack { namespace graphics {

using namespace std;

void shader_monitor::set_src_path(string const &src_path)
{
   m_src_path = src_path;
}

void shader_monitor::set_dst_path(string const &dst_path)
{
   m_dst_path = dst_path;
}

void shader_monitor::add(std::string const &path, class program *program)
{
   // bool is_src_path = path.find_first_of(m_src_path) == 0;
   // bool is_dst_path = path.find_first_of(m_dst_path) == 0;

   struct stat st;
   int res = ::stat(path.c_str(), &st);
   if (res != 0)
      throw std::runtime_error("file not found");

   file f;
   f.last_time = st.st_mtime;

#if 0
   if (path.find_first_of(m_src_path) == 0)
   {
      auto path_pos = path.find_first_not_of(m_src_path);
      size_t len = path.length() - path_pos;
      f.src_path = path;
      f.dst_path = m_dst_path + "/" + path.substr(path_pos, len);
   }
   else if (path.find_first_of(m_dst_path) == 0)
#endif
   {
      auto path_pos = path.find_first_not_of(m_dst_path);
      size_t len = path.length() - path_pos;
      f.src_path = m_src_path + "/" + path.substr(path_pos, len);
      f.dst_path = path;
   } 
#if 0
   else
   {
      f.src_path = "";
      f.dst_path = path;
   }
#endif
   f.program = program;

   m_files.push_back(f);
}

void shader_monitor::poll()
{
   for (auto i = m_files.begin(); i != m_files.end(); ++i)
   {
      file &f = *i;
      struct stat st;
      int res = stat(f.src_path.c_str(), &st);
      if (res == 0)
      {
         if (f.last_time != st.st_mtime)
         {
            f.program->reload();
            f.last_time = st.st_mtime;
            continue;
         }
      }
      res = stat(f.dst_path.c_str(), &st);
      if (res == 0)
      {
         if (f.last_time != st.st_mtime)
         {
            f.program->reload();
            f.last_time = st.st_mtime;
            continue;
         }
      }
   }
}

} }
