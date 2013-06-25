#ifndef attribute_map_hpp_renderstack_geometry
#define attribute_map_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/exception.hpp"
#include <map>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <cassert>
#include <stdint.h>

namespace renderstack { namespace geometry {

typedef size_t  index_type;

template<typename key_type>
class attribute_map_base
{
public:
   typedef std::vector<key_type>          key_array;
   typedef std::map<key_type, index_type> key_index_map;

   virtual ~attribute_map_base();

public:
   virtual std::type_info const &value_type_id() const = 0;

protected:
   attribute_map_base();
};

namespace usage
{
   enum value
   {
      direction,
      position,
      none
   };
};

/**  \brief contains attributes (like positions, normals, texcoords) for keys (polygon, point, corner)  */ 
template<typename key_type, typename value_type>
class attribute_map 
:  public attribute_map_base<key_type>
{
public:
   attribute_map();

   void        clear          ();
   bool        empty          () const;
   index_type  size           () const;
   void        begin_insertion(index_type estimated_count = 0);
   void        insert         (key_type const &key, value_type const &value);
   void        end_insertion  ();
   bool        is_inserting   () const;
   void        set_value      (key_type const &key, value_type const &value);
   value_type  value          (key_type const &key) const;
   bool        has            (key_type const &key) const;
   void        optimize       ();
   bool        is_optimized   () const;

   usage::value usage() const { return m_usage; }
   void                 set_usage(usage::value value) { m_usage = value; }

public:
   std::type_info const &value_type_id() const;

private:
   struct entry
   {
      bool operator==(entry const &other) const;
      bool operator< (entry const &other) const;

      key_type   key;
      value_type value;
   };

   typedef std::vector<entry> entry_container;

private:
   typename entry_container::const_iterator find(key_type const &key) const;
   typename entry_container::iterator       find(key_type const &key);

   void insert_entry(key_type const &key, value_type const &value);

private:
   entry_container   m_entries;
   bool              m_is_optimized;
   bool              m_is_inserting;
   usage::value      m_usage;
};

} }

#include "attribute_map.inl"

#endif
