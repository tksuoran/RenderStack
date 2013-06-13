#ifndef attribute_map_inl_renderstack_geometry
#define attribute_map_inl_renderstack_geometry

namespace renderstack { namespace geometry {

template<typename Key>
inline attribute_map_base<Key>::attribute_map_base()
{
}

template<typename Key>
inline attribute_map_base<Key>::~attribute_map_base()
{
}

template<typename Key, typename Value>
inline attribute_map<Key, Value>::attribute_map()
:  m_is_optimized(true)
,  m_is_inserting(false)
{
}

template<typename Key, typename Value>
inline void attribute_map<Key, Value>::clear()
{
   m_entries.clear();
}

template<typename Key, typename Value>
inline bool attribute_map<Key, Value>::empty() const
{
   return m_entries.empty();
}

template<typename Key, typename Value>
inline index_type attribute_map<Key, Value>::size() const
{
   assert(m_entries.size() < std::numeric_limits<index_type>::max());
   return static_cast<index_type>(m_entries.size());
}

template<typename Key, typename Value>
inline void attribute_map<Key, Value>::begin_insertion(
   index_type estimated_count)
{
   assert(!is_inserting());

   m_entries.clear();
   m_entries.reserve(estimated_count);

   m_is_optimized = false;
   m_is_inserting = true;
}

template<typename Key, typename Value>
inline void attribute_map<Key, Value>::insert(
   Key const   &key,
   Value const &value
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

template<typename Key, typename Value>
inline void attribute_map<Key, Value>::end_insertion()
{
   assert(is_inserting());

   m_is_inserting = false;

   optimize();
}

template<typename Key, typename Value>
inline bool attribute_map<Key, Value>::is_inserting() const
{
   return m_is_inserting;
}

template<typename Key, typename Value>
inline void attribute_map<Key, Value>::set_value(
   Key const   &key,
   Value const &value
)
{
   assert(!is_inserting());

   typename entry_container::iterator i = find(key);
   if (i == m_entries.end())
      insert_entry(key, value);
   else
      i->value = value;
}

template<typename Key, typename Value>
inline Value attribute_map<Key, Value>::value(Key const &key) const
{
   assert(!is_inserting());

   typename entry_container::const_iterator i = find(key);
   if (i != m_entries.end())
      return i->value;
   else
      throw key_not_found_exception<Key>(key);
}

template<typename Key, typename Value>
inline bool attribute_map<Key, Value>::has(Key const& key) const
{
   assert(!is_inserting());

   return find(key) != m_entries.end();
}

template<typename Key, typename Value>
inline void attribute_map<Key, Value>::optimize()
{
   assert(!is_inserting());

   std::sort(m_entries.begin(), m_entries.end());

   m_is_optimized = true;
}

template<typename Key, typename Value>
inline bool attribute_map<Key, Value>::is_optimized() const
{
   return m_is_optimized;
}

template<typename Key, typename Value>
inline std::type_info const &attribute_map<Key, Value>::value_type_id() const
{
   return typeid(Value);
}

template<typename Key, typename Value>
inline typename attribute_map<Key, Value>::entry_container::const_iterator
   attribute_map<Key, Value>::find(Key const &key) const
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

template<typename Key, typename Value>
inline typename attribute_map<Key, Value>::entry_container::iterator
   attribute_map<Key, Value>::find(Key const &key)
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

template<typename Key, typename Value>
inline void attribute_map<Key, Value>::insert_entry(
   Key const   &key,
   Value const &value
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

template<typename Key, typename Value>
inline bool attribute_map<Key, Value>::entry::operator==(entry const &other) const
{
   return key == other.key;
}

template<typename Key, typename Value>
inline bool attribute_map<Key, Value>::entry::operator<(entry const &other) const
{
   return key < other.key;
}

} }

#endif
