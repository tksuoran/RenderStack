#ifndef Renderstack_Geometry_attribute_map_collection_hpp
#define Renderstack_Geometry_attribute_map_collection_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/attribute_map.hpp"
#include "renderstack_geometry/exception.hpp"
#include <memory>
#include <map>

namespace renderstack { namespace geometry {

template<typename Key>
class attribute_map_collection
{
private: 
   typedef std::map<std::string, std::shared_ptr<attribute_map_base<Key> > > map_map;

public:
   typedef typename map_map::const_iterator const_iterator;

   attribute_map_collection();

   void clear();

   size_t size() const;

   template<typename Value>
   std::shared_ptr<attribute_map<Key, Value> > create(std::string const &name);

   void insert(std::string const &name, std::shared_ptr<attribute_map_base<Key> > const &map);
   void remove(std::string const &name);
   template<typename Value> bool contains(std::string const &name) const;

   std::shared_ptr<attribute_map_base<Key> > find_any(std::string const &name) const;

   template<typename Value> std::shared_ptr<attribute_map<Key, Value> > find(std::string const &name) const;
   template<typename Value> std::shared_ptr<attribute_map<Key, Value> > find_or_create(std::string const &name);
   void replace(std::string const &name, std::string const &temp);

   const_iterator begin() const;
   const_iterator end  () const;

private:
   map_map m_maps;
};

} }

#include "attribute_map_collection.inl"

#endif
