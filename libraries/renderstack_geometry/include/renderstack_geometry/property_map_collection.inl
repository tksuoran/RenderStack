#ifndef property_map_collection_inl_renderstack_geometry
#define property_map_collection_inl_renderstack_geometry

#include "renderstack_geometry/exception.hpp"

namespace renderstack { namespace geometry {

namespace detail
{

void throw_incompatible_attribute_type(
   std::string const   &name,
   char const          *type_name, 
   char const          *expected_type_name
);

}

template<typename key_type>
inline property_map_collection<key_type>::property_map_collection()
{
}

template<typename key_type>
inline void property_map_collection<key_type>::clear()
{
   m_maps.clear();
}


template<typename key_type>
inline size_t property_map_collection<key_type>::size() const
{
   return m_maps.size();
}

template<typename key_type>
inline void property_map_collection<key_type>::insert(
   std::string const &name,
   std::shared_ptr< property_map_base<key_type> > const &map_
)
{
   assert(m_maps.find(name) == m_maps.end());

   m_maps.insert(
      // This works:
      //std::map<std::string, std::shared_ptr<property_map_base<key_type> > >::value_type(name, map_)

      // I have this:
      // private: typedef std::map<std::string, std::shared_ptr<property_map_base<key_type> > > map_map;
      // so this should work, right?
      typename map_map::value_type(name, map_)
   );
}

template<typename key_type>
inline void property_map_collection<key_type>::remove(std::string const &name)
{
   m_maps.erase(name);
}

template<typename key_type>
template<typename value_type>
inline bool property_map_collection<key_type>::contains(std::string const &name) const
{
   const_iterator i = m_maps.find(name);

   if (i == m_maps.end())
      return false;

   std::shared_ptr<property_map_base<key_type> > p = i->second;

   if (
      std::shared_ptr<property_map<key_type, value_type> > 
      t = std::dynamic_pointer_cast<property_map<key_type, value_type> >(p)
   )
      return true;
   else
      return false;
}

template<typename key_type>
inline std::shared_ptr<property_map_base<key_type> >
   property_map_collection<key_type>::find_any(std::string const &name) const
{
   const_iterator i = m_maps.find(name);

   if (i == m_maps.end())
      throw property_map_not_found_exception(name);

   return i->second;
}

template<typename key_type>
inline std::shared_ptr<property_map_base<key_type> >
   property_map_collection<key_type>::maybe_find_any(std::string const &name) const
{
   const_iterator i = m_maps.find(name);

   if (i == m_maps.end())
      return nullptr;;

   return i->second;
}

template<typename key_type>
inline typename property_map_collection<key_type>::const_iterator
   property_map_collection<key_type>::begin() const
{
   return m_maps.begin();
}

template<typename key_type>
inline typename property_map_collection<key_type>::const_iterator
   property_map_collection<key_type>::end() const
{
   return m_maps.end();
}

template<typename key_type>
template<typename value_type>
inline std::shared_ptr<property_map<key_type, value_type> >
   property_map_collection<key_type>::create(std::string const &name)
{
   std::shared_ptr<property_map<key_type, value_type> > p(new property_map<key_type, value_type>());
   insert(name, p);
   //insert(name, std::make_shared<property_map<key_type, value_type> >(key_type, value_type));
   return p;
}

template<typename key_type>
template<typename value_type>
inline std::shared_ptr<property_map<key_type, value_type> >
   property_map_collection<key_type>::find(std::string const &name) const
{
   std::shared_ptr<property_map_base<key_type> > p = find_any(name);

   if (
      std::shared_ptr<property_map<key_type, value_type> > 
      t = std::dynamic_pointer_cast<property_map<key_type, value_type> >(p)
   )
      return t;
   else
      throw incompatible_attribute_type_exception(
         name, 
         p->value_type_id().name(),
         typeid(value_type).name()
      );

      //return std::shared_ptr<property_map<key_type, value_type> >();
}

template<typename key_type>
template<typename value_type>
inline std::shared_ptr<property_map<key_type, value_type> >
   property_map_collection<key_type>::maybe_find(std::string const &name) const
{
   std::shared_ptr<property_map_base<key_type> > p = maybe_find_any(name);

   if (!p)
      return nullptr;

   if (
      std::shared_ptr<property_map<key_type, value_type> > 
      t = std::dynamic_pointer_cast<property_map<key_type, value_type> >(p)
   )
      return t;
   else
      return nullptr;
}

template<typename key_type>
template<typename value_type>
inline std::shared_ptr<property_map<key_type, value_type> >
   property_map_collection<key_type>::find_or_create(std::string const &name)
{
   typename map_map::iterator i = m_maps.find(name);

   if (i != m_maps.end())
   {
      if (
         std::shared_ptr<property_map<key_type, value_type> > 
         t = std::dynamic_pointer_cast<property_map<key_type, value_type> >(i->second)
      )
         return t;
      else
         throw incompatible_attribute_type_exception(
            name, 
            t->value_type_id().name(),
            typeid(value_type).name()
         );

         //return std::shared_ptr<property_map<key_type, value_type> >();
   }
   else
   {
      //shared_ptr<property_map<key_type, value_type> > res = create<value_type>(name);
      auto res = create<value_type>(name);
      return res;
   }
}

template<typename key_type>
inline void property_map_collection<key_type>::replace(std::string const &name, std::string const &temp)
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

template<typename key_type>
inline void property_map_collection<key_type>::interpolate(
   property_map_collection<key_type>                              &destination,
   std::map<key_type, std::vector<std::pair<float, key_type> > >  key_new_to_olds
) const
{
   /*  For all original attribute maps  */ 
   for (auto i = m_maps.cbegin(); i != m_maps.cend(); ++i)
   {
      std::string                                     map_name             = i->first;
      std::shared_ptr<property_map_base<key_type> >   src_map              = i->second;
      property_map_base<key_type>                     *destination_map_ptr = src_map->constructor();
      std::shared_ptr<property_map_base<key_type> >   destination_map(destination_map_ptr);

      src_map->optimize();
      src_map->interpolate(destination_map_ptr, key_new_to_olds);

      destination.insert(map_name, destination_map);
   }
}

} }

#endif 
