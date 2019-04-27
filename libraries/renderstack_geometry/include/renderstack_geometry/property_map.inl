#ifndef property_map_inl_renderstack_geometry
#define property_map_inl_renderstack_geometry

#include <algorithm>

namespace renderstack
{
namespace geometry
{


template <typename Key_type, typename Value_type>
inline void Property_map<Key_type, Value_type>::clear()
{
    m_entries.clear();
}

template <typename Key_type, typename Value_type>
inline bool Property_map<Key_type, Value_type>::empty() const
{
    return m_entries.empty();
}

template <typename Key_type, typename Value_type>
inline size_t Property_map<Key_type, Value_type>::size() const
{
    assert(m_entries.size() < std::numeric_limits<size_t>::max());
    return static_cast<size_t>(m_entries.size());
}

template <typename Key_type, typename Value_type>
inline void Property_map<Key_type, Value_type>::begin_insertion(size_t estimated_count)
{
    assert(!m_is_inserting);

    m_entries.clear();
    m_entries.reserve(estimated_count);

    m_is_optimized = false;
    m_is_inserting = true;
}

template <typename Key_type, typename Value_type>
inline void Property_map<Key_type, Value_type>::insert(Key_type key, Value_type value)
{   
    assert(m_is_inserting);
    assert(m_entries.size() + 1 < std::numeric_limits<size_t>::max());

#if _HAS_ITERATOR_DEBUGGING
    assert(std::find(m_entries.begin(), m_entries.end(), e) == m_entries.end());
#endif

    m_entries.emplace_back(key, value);
}

template <typename Key_type, typename Value_type>
inline void Property_map<Key_type, Value_type>::end_insertion()
{
    assert(m_is_inserting);

    m_is_inserting = false;

    optimize();
}

template <typename Key_type, typename Value_type>
inline void Property_map<Key_type, Value_type>::put(Key_type key, Value_type value)
{
    assert(!m_is_inserting);

    typename Entry_container::iterator i = find(key);
    if (i == m_entries.end())
    {
        insert_entry(key, value);
    }
    else
    {
        i->value = value;
    }
}

template <typename Key_type, typename Value_type>
inline Value_type Property_map<Key_type, Value_type>::get(Key_type key) const
{
    assert(!m_is_inserting);

    typename Entry_container::const_iterator i = find(key);
    if (i != m_entries.end())
    {
        return i->value;
    }
    else
    {
        throw Key_not_found_exception<Key_type>(key);
    }
}

template <typename Key_type, typename Value_type>
inline bool Property_map<Key_type, Value_type>::maybe_get(Key_type key, Value_type &value) const
{
    assert(!m_is_inserting);

    typename Entry_container::const_iterator i = find(key);
    if (i != m_entries.end())
    {
        value = i->value;
        return true;
    }
    else
    {
        return false;
    }
}

template <typename Key_type, typename Value_type>
inline bool Property_map<Key_type, Value_type>::has(Key_type key) const
{
    assert(!m_is_inserting);

    return find(key) != m_entries.end();
}

template <typename Key_type, typename Value_type>
inline void Property_map<Key_type, Value_type>::optimize()
{
    assert(!m_is_inserting);

    std::sort(m_entries.begin(), m_entries.end());

    m_is_optimized = true;
}

template <typename Key_type, typename Value_type>
inline Property_map_base<Key_type> *Property_map<Key_type, Value_type>::constructor() const
{
    Property_map<Key_type, Value_type> *instance = new Property_map<Key_type, Value_type>();
    Property_map_base<Key_type> *       base_ptr = dynamic_cast<Property_map_base<Key_type> *>(instance);
    return base_ptr;
}

// template <typename Key_type, typename Value_type>
// inline std::type_info const &Property_map<Key_type, Value_type>::value_type_id() const
// {
//     return typeid(Value_type);
// }

template <typename Key_type, typename Value_type>
inline typename Property_map<Key_type, Value_type>::Entry_container::const_iterator
Property_map<Key_type, Value_type>::find(Key_type key) const
{
    Entry compare_entry(key);

    if (m_is_optimized)
    {
        std::pair<typename Entry_container::const_iterator,
                  typename Entry_container::const_iterator>
            p = std::equal_range(m_entries.begin(), m_entries.end(), compare_entry, Entry_comparator());
        return p.first != p.second ? p.first : m_entries.end();
    }
    else
    {
        return std::find(m_entries.begin(), m_entries.end(), compare_entry);
    }
}

template <typename Key_type, typename Value_type>
inline typename Property_map<Key_type, Value_type>::Entry_container::iterator
Property_map<Key_type, Value_type>::find(Key_type key)
{
    Entry compare_key(key);

    if (m_is_optimized)
    {
        std::pair<typename Entry_container::iterator,
                  typename Entry_container::iterator>
            p = std::equal_range(m_entries.begin(), m_entries.end(), compare_key, Entry_comparator());
        return p.first != p.second ? p.first : m_entries.end();
    }
    else
    {
        return std::find(m_entries.begin(), m_entries.end(), compare_key);
    }
}

template <typename Key_type, typename Value_type>
inline void Property_map<Key_type, Value_type>::insert_entry(Key_type key, Value_type value)
{
    if (!m_entries.empty() && (key < m_entries.back().key))
    {
        m_is_optimized = false;
    }

    m_entries.emplace_back(key, value);
}

template <typename Key_type, typename Value_type>
inline bool Property_map<Key_type, Value_type>::Entry::operator==(Entry const &other) const
{
    return key == other.key;
}

template <typename Key_type, typename Value_type>
inline bool Property_map<Key_type, Value_type>::Entry::operator<(Entry const &other) const
{
    return key < other.key;
}

template <typename Key_type, typename Value_type>
inline void Property_map<Key_type, Value_type>::interpolate(
    Property_map_base<Key_type> *destination_base,
    std::map<Key_type, std::vector<std::pair<float, Key_type>>> key_new_to_olds) const
{
    Property_map<Key_type, Value_type> *destination = dynamic_cast<Property_map<Key_type, Value_type> *>(destination_base);
    assert(destination != nullptr);

    for (auto i : key_new_to_olds)
    {
        Key_type                                new_key  = i.first;
        std::vector<std::pair<float, Key_type>> old_keys = i.second;

        float sum_weights = 0.0f;
        for (auto j : old_keys)
        {
            Key_type old_key = j.second;
            auto     fi      = find(old_key);
            if (fi != m_entries.cend())
            {
                sum_weights += j.first;
            }
        }

        if (sum_weights == 0.0f)
        {
            continue;
        }

        Value_type new_value(0);
        for (auto j : old_keys)
        {
            float      weight  = j.first;
            Key_type   old_key = j.second;
            Value_type old_value;

            auto fi = find(old_key);
            if (fi != m_entries.cend())
            {
                auto &entry = *fi;
                old_value   = entry.value;
                new_value += (weight / sum_weights) * old_value;
            }
        }

        destination->put(new_key, new_value);
    }
}

} // namespace geometry
} // namespace renderstack

#endif
