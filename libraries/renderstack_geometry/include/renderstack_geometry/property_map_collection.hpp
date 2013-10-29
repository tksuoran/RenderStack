#ifndef property_map_collection_hpp_renderstack_geometry
#define property_map_collection_hpp_renderstack_geometry

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_geometry/property_map.hpp"
#include "renderstack_geometry/exception.hpp"
#include <memory>
#include <map>

namespace renderstack { namespace geometry {


template<typename key_type>
class property_map_collection
{
private: 
   typedef std::map<std::string, std::shared_ptr<property_map_base<key_type> > > map_map;

public:
   typedef typename map_map::const_iterator const_iterator;

   property_map_collection();

   void clear();

   size_t size() const;

   template<typename value_type>
   std::shared_ptr<property_map<key_type, value_type> > create(std::string const &name);

   void insert(std::string const &name, std::shared_ptr<property_map_base<key_type> > const &map);
   void remove(std::string const &name);
   template<typename value_type> bool contains(std::string const &name) const;

   std::shared_ptr<property_map_base<key_type> > find_any(std::string const &name) const;
   std::shared_ptr<property_map_base<key_type> > maybe_find_any(std::string const &name) const;

   template<typename value_type> std::shared_ptr<property_map<key_type, value_type> > find(std::string const &name) const;
   template<typename value_type> std::shared_ptr<property_map<key_type, value_type> > maybe_find(std::string const &name) const;
   template<typename value_type> std::shared_ptr<property_map<key_type, value_type> > find_or_create(std::string const &name);
   void replace(std::string const &name, std::string const &temp);

   const_iterator begin() const;
   const_iterator end  () const;

   void interpolate(
      property_map_collection<key_type>                              &destination,
      std::map<key_type, std::vector<std::pair<float, key_type> > >  key_new_to_olds
   ) const;

private:
   map_map m_maps;
};

} }

#include "property_map_collection.inl"

#endif
