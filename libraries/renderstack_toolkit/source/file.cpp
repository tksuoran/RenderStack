#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/file.hpp"
#include "renderstack_toolkit/log.hpp"

#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <stdexcept>

namespace renderstack { namespace toolkit {

#define LOG_CATEGORY &log_file

using namespace std;

bool exists(std::string const &fname)
{
   ifstream file(fname, ios::binary);
   struct stat st;
   int res = stat(fname.c_str(), &st);
   return res == 0;
}

string read(string const &fname)
{
   ifstream file(fname, ios::binary);
   struct stat st;
   int res = stat(fname.c_str(), &st);
   if (res != 0)
   {
      stringstream ss;
      ss << "file not found: " << fname;
      log_error(ss.str().c_str());
      throw runtime_error(ss.str().c_str());
   }

   string result(static_cast<size_t>(st.st_size), 0);
   file.read(&result[0], static_cast<streamsize>(st.st_size));

   return result;
}

} }
