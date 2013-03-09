#ifndef renderstack_geometry__attribute_map_hpp
#define renderstack_geometry__attribute_map_hpp

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

template<typename Key>
class attribute_map_base
{
public:
   typedef std::vector<Key>            key_array;
   typedef std::map<Key,index_type>    key_index_map;

   virtual ~attribute_map_base();

public:
   virtual std::type_info const& value_type_id() const = 0;

protected:
   attribute_map_base();
};

/**  \brief contains attributes (like positions, normals, texcoords) for keys (polygon, point, corner)  */ 
template<typename Key, typename Value>
class attribute_map 
:  public attribute_map_base<Key>
{
public:
   typedef Key   key_type;
   typedef Value value_type;

   attribute_map();

   void        clear          ();
   bool        empty          () const;
   index_type  size           () const;
   void        begin_insertion(index_type estimated_count = 0);
   void        insert         (Key const &key, Value const &value);
   void        end_insertion  ();
   bool        is_inserting   () const;
   void        set_value      (Key const &key, Value const &value);
   Value       value          (Key const &key) const;
   bool        has            (Key const &key) const;
   void        optimize       ();
   bool        is_optimized   () const;

public:
   std::type_info const &value_type_id() const;

private:
   struct entry {
      bool operator==(entry const &other) const;
      bool operator< (entry const &other) const;

      Key   key;
      Value value;
   };

   typedef std::vector<entry> entry_container;

private:
   typename entry_container::const_iterator find(Key const &key) const;
   typename entry_container::iterator       find(Key const &key);

   void insert_entry(Key const &key, Value const &value);

private:
   entry_container   m_entries;
   bool              m_is_optimized;
   bool              m_is_inserting;
};

} }

#include "attribute_map.inl"

#endif
