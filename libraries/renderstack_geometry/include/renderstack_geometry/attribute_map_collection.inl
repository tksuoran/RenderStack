#ifndef Renderstack_Geometry_attribute_map_collection_inl
#define Renderstack_Geometry_attribute_map_collection_inl

#include "renderstack_geometry/exception.hpp"

namespace renderstack { namespace geometry {

namespace detail {
   void throw_incompatible_attribute_type(
      std::string const   &name,
      char const          *type_name, 
      char const          *expected_type_name
   );
}

template<typename Key>
inline attribute_map_collection<Key>::attribute_map_collection()
{
}

template<typename Key>
inline void attribute_map_collection<Key>::clear()
{
   m_maps.clear();
}


template<typename Key>
inline size_t attribute_map_collection<Key>::size() const
{
   return m_maps.size();
}

template<typename Key>
inline void attribute_map_collection<Key>::insert(
   std::string const &name,
   std::shared_ptr< attribute_map_base<Key> > const &map_
)
{
   assert(m_maps.find(name) == m_maps.end());

   m_maps.insert(
      // This works:
      //std::map<std::string, std::shared_ptr<attribute_map_base<Key> > >::value_type(name, map_)

      // I have this:
      // private: typedef std::map<std::string, std::shared_ptr<attribute_map_base<Key> > > map_map;
      // so this should work, right?
      typename map_map::value_type(name, map_)
   );
}

template<typename Key>
inline void attribute_map_collection<Key>::remove(std::string const &name)
{
   m_maps.erase(name);
}

template<typename Key>
template<typename Value>
inline bool attribute_map_collection<Key>::contains(std::string const &name) const
{
   const_iterator i = m_maps.find(name);

   if (i == m_maps.end())
      return false;

   std::shared_ptr<attribute_map_base<Key> > p = i->second;

   if (
      std::shared_ptr<attribute_map<Key, Value> > 
      t = std::dynamic_pointer_cast<attribute_map<Key, Value> >(p)
   )
      return true;
   else
      return false;
}

template<typename Key>
inline std::shared_ptr<attribute_map_base<Key> >
   attribute_map_collection<Key>::find_any(std::string const &name) const
{
   const_iterator i = m_maps.find(name);

   if (i == m_maps.end())
      throw attribute_map_not_found_exception(name);

   assert(i != m_maps.end());
   return i->second;
}

template<typename Key>
inline typename attribute_map_collection<Key>::const_iterator
   attribute_map_collection<Key>::begin() const
{
   return m_maps.begin();
}

template<typename Key>
inline typename attribute_map_collection<Key>::const_iterator
   attribute_map_collection<Key>::end() const
{
   return m_maps.end();
}

template<typename Key>
template<typename Value>
inline std::shared_ptr<attribute_map<Key, Value> > attribute_map_collection<Key>::create(std::string const &name)
{
   std::shared_ptr<attribute_map<Key, Value> > p(new attribute_map<Key, Value>());
   insert(name, p);
   //insert(name, std::make_shared<attribute_map<Key, Value> >(Key, Value));
   return p;
}

template<typename Key>
template<typename Value>
inline std::shared_ptr<attribute_map<Key, Value> >
   attribute_map_collection<Key>::find(std::string const &name) const
{
   std::shared_ptr<attribute_map_base<Key> > p = find_any(name);

   if (
      std::shared_ptr<attribute_map<Key, Value> > 
      t = std::dynamic_pointer_cast<attribute_map<Key, Value> >(p)
   )
      return t;
   else
      throw incompatible_attribute_type_exception(
         name, 
         p->value_type_id().name(),
         typeid(Value).name()
      );

      //return std::shared_ptr<attribute_map<Key, Value> >();
}

template<typename Key>
template<typename Value>
inline std::shared_ptr<attribute_map<Key, Value> >
   attribute_map_collection<Key>::find_or_create(std::string const &name)
{
   typename map_map::iterator i = m_maps.find(name);
   if (i != m_maps.end())
      if (
         std::shared_ptr<attribute_map<Key, Value> > 
         t = std::dynamic_pointer_cast<attribute_map<Key, Value> >(i->second)
      )
         return t;
      else
         throw incompatible_attribute_type_exception(
            name, 
            t->value_type_id().name(),
            typeid(Value).name()
         );

         //return std::shared_ptr<attribute_map<Key, Value> >();
   else
      return create<Value>(name);
}

template<typename Key>
inline void attribute_map_collection<Key>::replace(std::string const &name, std::string const &temp)
{
   typename map_map::iterator i = m_maps.find(name);
   typename map_map::iterator j = m_maps.find(temp);
   if (
      (i != m_maps.end()) &&
      (j != m_maps.end())
   )
   {
      i->second = j->second;
      m_maps.erase(j);
   }
   else
      throw std::runtime_error("map not found");
}

} }

#endif 
