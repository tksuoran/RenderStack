#ifndef property_map_hpp_renderstack_geometry
#define property_map_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/exception.hpp"
#include <map>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <cassert>
#include <stdint.h>
#include "boost/any.hpp"

namespace renderstack { namespace geometry {


typedef size_t index_type;

template<typename key_type>
class property_map_base
{
public:
   typedef std::vector<key_type>          key_array;
   typedef std::map<key_type, index_type> key_index_map;

   virtual ~property_map_base();

public:
   virtual property_map_base *constructor() const = 0;

   virtual void                  clear          () = 0;
   virtual bool                  empty          () const = 0;
   virtual index_type            size           () const = 0;
   virtual void                  begin_insertion(index_type estimated_count) = 0;
   virtual bool                  is_inserting   () const = 0;
   virtual bool                  has            (key_type const &key) const = 0;
   virtual void                  optimize       () = 0;
   virtual bool                  is_optimized   () const = 0;
   virtual std::type_info const  &value_type_id () const = 0;
   virtual void interpolate(
      property_map_base<key_type> *destination,
      std::map<key_type, std::vector<std::pair<float, key_type>>> key_new_to_olds
   ) const = 0;

protected:
   property_map_base();
};


/**  \brief contains attributes (like positions, normals, texcoords) for keys (polygon, point, corner)  */ 
template<typename key_type, typename value_type>
class property_map
:  public property_map_base<key_type>
{
public:
   property_map();

   void        put            (key_type const &key, value_type const &value);
   value_type  get            (key_type const &key) const;

   void        clear          ();
   bool        empty          () const;
   index_type  size           () const;
   void        begin_insertion(index_type estimated_count = 0);
   void        insert         (key_type const &key, value_type const &value);
   void        end_insertion  ();
   bool        is_inserting   () const;
   bool        has            (key_type const &key) const;
   void        optimize       ();
   bool        is_optimized   () const;

   /*virtual*/ void interpolate(
      property_map_base<key_type> *destination,
      std::map<key_type, std::vector<std::pair<float, key_type>>> key_new_to_olds
   ) const;

   property_map_base<key_type> *constructor() const;

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
};

} }

#include "property_map.inl"

#endif
