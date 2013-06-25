#ifndef attribute_map_inl_renderstack_geometry
#define attribute_map_inl_renderstack_geometry

namespace renderstack { namespace geometry {

template<typename key_type>
inline attribute_map_base<key_type>::attribute_map_base()
{
}

template<typename key_type>
inline attribute_map_base<key_type>::~attribute_map_base()
{
}

template<typename key_type, typename value_type>
inline attribute_map<key_type, value_type>::attribute_map()
:  m_is_optimized(true)
,  m_is_inserting(false)
,  m_usage(usage::none)
{
}

template<typename key_type, typename value_type>
inline void attribute_map<key_type, value_type>::clear()
{
   m_entries.clear();
}

template<typename key_type, typename value_type>
inline bool attribute_map<key_type, value_type>::empty() const
{
   return m_entries.empty();
}

template<typename key_type, typename value_type>
inline index_type attribute_map<key_type, value_type>::size() const
{
   assert(m_entries.size() < std::numeric_limits<index_type>::max());
   return static_cast<index_type>(m_entries.size());
}

template<typename key_type, typename value_type>
inline void attribute_map<key_type, value_type>::begin_insertion(
   index_type estimated_count)
{
   assert(!is_inserting());

   m_entries.clear();
   m_entries.reserve(estimated_count);

   m_is_optimized = false;
   m_is_inserting = true;
}

template<typename key_type, typename value_type>
inline void attribute_map<key_type, value_type>::insert(
   key_type const   &key,
   value_type const &value
)
{
   assert(is_inserting());
   assert(m_entries.size() + 1 < std::numeric_limits<index_type>::max());

   entry e;
   e.key = key;
   e.value = value;

#if _HAS_ITERATOR_DEBUGGING
   assert(
      std::find(m_entries.begin(), m_entries.end(), e) == m_entries.end()
   );
#endif

   m_entries.push_back(e);
}

template<typename key_type, typename value_type>
inline void attribute_map<key_type, value_type>::end_insertion()
{
   assert(is_inserting());

   m_is_inserting = false;

   optimize();
}

template<typename key_type, typename value_type>
inline bool attribute_map<key_type, value_type>::is_inserting() const
{
   return m_is_inserting;
}

template<typename key_type, typename value_type>
inline void attribute_map<key_type, value_type>::set_value(
   key_type const   &key,
   value_type const &value
)
{
   assert(!is_inserting());

   typename entry_container::iterator i = find(key);
   if (i == m_entries.end())
      insert_entry(key, value);
   else
      i->value = value;
}

template<typename key_type, typename value_type>
inline value_type attribute_map<key_type, value_type>::value(key_type const &key) const
{
   assert(!is_inserting());

   typename entry_container::const_iterator i = find(key);
   if (i != m_entries.end())
      return i->value;
   else
      throw key_not_found_exception<key_type>(key);
}

template<typename key_type, typename value_type>
inline bool attribute_map<key_type, value_type>::has(key_type const& key) const
{
   assert(!is_inserting());

   return find(key) != m_entries.end();
}

template<typename key_type, typename value_type>
inline void attribute_map<key_type, value_type>::optimize()
{
   assert(!is_inserting());

   std::sort(m_entries.begin(), m_entries.end());

   m_is_optimized = true;
}

template<typename key_type, typename value_type>
inline bool attribute_map<key_type, value_type>::is_optimized() const
{
   return m_is_optimized;
}

template<typename key_type, typename value_type>
inline std::type_info const &attribute_map<key_type, value_type>::value_type_id() const
{
   return typeid(value_type);
}

template<typename key_type, typename value_type>
inline typename attribute_map<key_type, value_type>::entry_container::const_iterator
   attribute_map<key_type, value_type>::find(key_type const &key) const
{
   entry e;
   e.key = key;

   if (m_is_optimized)
   {
      std::pair<
         typename entry_container::const_iterator,
         typename entry_container::const_iterator
      > p = std::equal_range(m_entries.begin(), m_entries.end(), e);
      return p.first != p.second ? p.first : m_entries.end();
   }
   else
      return std::find(m_entries.begin(), m_entries.end(), e);
}

template<typename key_type, typename value_type>
inline typename attribute_map<key_type, value_type>::entry_container::iterator
   attribute_map<key_type, value_type>::find(key_type const &key)
{
   entry e;
   e.key = key;

   if (m_is_optimized)
   {
      std::pair<
         typename entry_container::iterator,
         typename entry_container::iterator
      > p = std::equal_range(m_entries.begin(), m_entries.end(), e); 
      return p.first != p.second ? p.first : m_entries.end();
   } else
      return std::find(m_entries.begin(), m_entries.end(), e);
}

template<typename key_type, typename value_type>
inline void attribute_map<key_type, value_type>::insert_entry(
   key_type const   &key,
   value_type const &value
)
{
   if (
      !m_entries.empty() && 
      (key < m_entries.back().key)
   )
      m_is_optimized = false;

   entry e;
   e.key = key;
   e.value = value;
   m_entries.push_back(e);
}

template<typename key_type, typename value_type>
inline bool attribute_map<key_type, value_type>::entry::operator==(entry const &other) const
{
   return key == other.key;
}

template<typename key_type, typename value_type>
inline bool attribute_map<key_type, value_type>::entry::operator<(entry const &other) const
{
   return key < other.key;
}

} }

#endif
