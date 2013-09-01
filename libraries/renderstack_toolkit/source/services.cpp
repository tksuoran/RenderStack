#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/services.hpp"
#include "renderstack_toolkit/log.hpp"

#define LOG_CATEGORY &log_services

namespace renderstack { namespace toolkit {

using namespace std;

services::services()
{
}

void services::add(shared_ptr<renderstack::toolkit::service>s)
{
   // Silently ignores nullptr services
   if (s)
      m_services.insert(s);   
}
void services::cleanup_services()
{
   m_services.clear();
}
void services::initialize_services()
{
   set<shared_ptr<renderstack::toolkit::service> > uninitialized(m_services);
   set<shared_ptr<renderstack::toolkit::service> > remove_set;

   size_t total_count = uninitialized.size();
   size_t uninitialized_count = total_count;
   size_t initialized_count = 0;

   log_info("Initializing %d services:", uninitialized_count);

   while (uninitialized_count > 0)
   {
      remove_set.clear();

      for (auto i = uninitialized.begin(); i != uninitialized.end();)
      {
         auto s = *i;

         if (!s)
            throw runtime_error("service is nullptr");

         if (s->ready())
         {
            ++initialized_count;
            log_info("Initializing service %d / %d: %s...", initialized_count, total_count, s->name().c_str());
            try
            {
               s->initialize();  
            }
            catch (...)
            {
               log_info("Fatal error: Initializing service %d / %d: %s failed", initialized_count, total_count, s->name().c_str());
               throw;
            }
            remove_set.insert(s);
            --uninitialized_count;
            i = uninitialized.erase(i);
         }
         else
         {
            ++i;
         }
      }

      if (remove_set.size() == 0)
      {
         log_error("circular service dependenciers detected");
         throw runtime_error("Circular dependencies detected");
      }

      for (auto i = uninitialized.begin(); i != uninitialized.end(); ++i)
      {
         auto s = *i;
         s->remove_dependencies(remove_set);
      }
   }
   log_info("Done initializing services");
}


} }



#if 0
template <class T>
inline void hash_combine(size_t & seed, const T & v)
{
   hash<T> hasher;
   seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T>
size_t hash(const shared_ptr<T> & ptr)
{
   return ((size_t) ptr.get()) / sizeof(T);
}

template<typename T>
bool equal_to(const shared_ptr<T> & left, const shared_ptr<T> & right)
{
   return left.get() == right.get();
}
#endif
