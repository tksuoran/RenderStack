#ifndef Property_map_collection_inl_renderstack_geometry
#define Property_map_collection_inl_renderstack_geometry

#include "renderstack_geometry/exception.hpp"

namespace renderstack
{
namespace geometry
{

namespace detail
{

void throw_incompatible_attribute_type(const std::string &name,
                                       char const *       type_name,
                                       char const *       expected_type_name);
}

template <typename Key_type>
inline void Property_map_collection<Key_type>::clear()
{
    m_collection.clear();
}

template <typename Key_type>
inline size_t Property_map_collection<Key_type>::size() const
{
    return m_collection.size();
}

#if 0
template <typename Key_type>
template <typename Value_type>
inline void Property_map_collection<Key_type>::insert(const std::string &name, Property_map<Key_type, Value_type> *map)
{
    Hash::key_type hash = Hash::hash(name.c_str());

    // Look for existing chain
    ensure_optimized();
    auto lower = std::lower_bound(m_collection.begin(), m_collection.end(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        Chain &chain = *lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                // Key collision - bad
                abort();
            }
        }

        // Hash collision - add to chain
        chain.values.emplace_back(name, map);
    }

    // New chain
    m_collection.emplace_back(hash, name, map);

    m_is_optimized = false;
}
#endif

template <typename Key_type>
inline void Property_map_collection<Key_type>::insert(const std::string &name, Property_map_base<Key_type> *map)
{
    Hash::key_type hash = Hash::hash(name.c_str());

    // Look for existing chain
    ensure_optimized();
    auto lower = std::lower_bound(m_collection.begin(), m_collection.end(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        Chain &chain = *lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                // Key collision - bad
                abort();
            }
        }

        // Hash collision - add to chain
        chain.values.emplace_back(name, map);
    }

    // New chain
    m_collection.emplace_back(hash, name, map);

    m_is_optimized = false;
}

template <typename Key_type>
inline void Property_map_collection<Key_type>::remove(const std::string &name)
{
    Hash::key_type hash = Hash::hash(name.c_str());
    auto lower = std::lower_bound(m_collection.begin(), m_collection.end(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        Chain &chain = *lower;
        for (auto i = chain.values.begin(); chain.values.end();)
        {
            Entry &entry = *i;
            if (entry.key == name)
            {
                i = chain.erase(i);
            }
            else
            {
                ++i;
            }
        }
        if (chain.values.size() == 0)
        {
            m_collection.erase(lower);
            m_is_optimized = false; // TODO is sorted vector optimized after erase?
        }
    }
}

template <typename Key_type>
template <typename Value_type>
inline bool Property_map_collection<Key_type>::contains(const std::string &name)
{
    Hash::key_type hash = Hash::hash(name.c_str());
    ensure_optimized();
    auto lower = std::lower_bound(m_collection.cbegin(), m_collection.cend(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        const Chain &chain = *lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                auto p = entry.value.get();
                auto t = dynamic_cast<Property_map<Key_type, Value_type>*>(p);
                return (t != nullptr);
            }
        }
    }

    return false;
}

template <typename Key_type>
template <typename Value_type>
inline bool Property_map_collection<Key_type>::contains(const std::string &name) const
{
    Hash::key_type hash = Hash::hash(name.c_str());
    if (!m_is_optimized)
    {
        // not optimized
        abort();
    }

    auto lower = std::lower_bound(m_collection.cbegin(), m_collection.cend(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        const Chain &chain = *lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                auto p = entry.value.get();
                auto t = dynamic_cast<Property_map<Key_type, Value_type>*>(p);
                return (t != nullptr);
            }
        }
    }

    return false;
}

template <typename Key_type>
inline Property_map_base<Key_type> *
Property_map_collection<Key_type>::find_any(const std::string &name)
{
    Hash::key_type hash = Hash::hash(name.c_str());
    ensure_optimized();
    auto lower = std::lower_bound(m_collection.begin(), m_collection.end(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        Chain &chain = *lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                auto p = entry.value.get();
                return p;
            }
        }
    }
    return nullptr;
}

template <typename Key_type>
template <typename Value_type>
inline Property_map<Key_type, Value_type> *
Property_map_collection<Key_type>::create(const std::string &name)
{
    Hash::key_type hash = Hash::hash(name.c_str());

    // Look for existing chain
    ensure_optimized();
    auto lower = std::lower_bound(m_collection.begin(), m_collection.end(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        Chain &chain = *lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                // Key collision - bad
                abort();
            }
        }

        // Hash collision - add to chain
        auto p = new Property_map<Key_type, Value_type>();
        chain.values.emplace_back(name, p);
    }

    // New chain
    auto p = new Property_map<Key_type, Value_type>();
    m_collection.emplace_back(hash, name, p);

    m_is_optimized = false;
    return p;
}

template <typename Key_type>
template <typename Value_type>
inline Property_map<Key_type, Value_type> *
Property_map_collection<Key_type>::find(const std::string &name)
{
    Hash::key_type hash = Hash::hash(name.c_str());
    ensure_optimized();
    auto lower = std::lower_bound(m_collection.begin(), m_collection.end(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        Chain &chain = *lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                auto p = entry.value.get();
                auto t = dynamic_cast<Property_map<Key_type, Value_type>*>(p);
                if (t != nullptr)
                {
                    return t;
                }
                else
                {
                    throw std::runtime_error("Incompatible_attribute_type_exception" + name);
;
                }
            }
        }
    }
    return nullptr;
}

template <typename Key_type>
template <typename Value_type>
inline Property_map<Key_type, Value_type> *
Property_map_collection<Key_type>::maybe_find(const std::string &name)
{
    Hash::key_type hash = Hash::hash(name.c_str());
    ensure_optimized();
    auto lower = std::lower_bound(m_collection.begin(), m_collection.end(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        Chain &chain = *lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                auto p = entry.value.get();
                auto t = dynamic_cast<Property_map<Key_type, Value_type>*>(p);
                if (t != nullptr)
                {
                    return t;
                }
                else
                {
                    return nullptr;
                }
            }
        }
    }
    return nullptr;
}


template <typename Key_type>
template <typename Value_type>
inline Property_map<Key_type, Value_type> *
Property_map_collection<Key_type>::maybe_find(const std::string &name) const
{    
    Hash::key_type hash = Hash::hash(name.c_str());

    if (m_is_optimized)
    {
        auto i = std::lower_bound(m_collection.cbegin(), m_collection.cend(), hash, Chain_comparator());
        if (i != m_collection.end())
        {
            const Chain &chain = *i;
            for (auto &entry : chain.values)
            {
                if (entry.key == name)
                {
                    auto p = entry.value.get();
                    auto t = dynamic_cast<Property_map<Key_type, Value_type>*>(p);
                    if (t != nullptr)
                    {
                        return t;
                    }
                    else
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    else
    {
        for (auto &chain : m_collection)
        {
            for (auto &entry : chain.values)
            {
                if (entry.key == name)
                {
                    auto p = entry.value.get();
                    auto t = dynamic_cast<Property_map<Key_type, Value_type>*>(p);
                    if (t != nullptr)
                    {
                        return t;
                    }
                    else
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    
    return nullptr;
}

template <typename Key_type>
template <typename Value_type>
inline Property_map<Key_type, Value_type> *
Property_map_collection<Key_type>::find_or_create(const std::string &name)
{
    Hash::key_type hash = Hash::hash(name.c_str());
    ensure_optimized();
    // Look for existing chain
    auto lower = std::lower_bound(m_collection.begin(), m_collection.end(), hash, Chain_comparator());
    if (lower != m_collection.end())
    {
        Chain &chain = *lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                auto p = entry.value.get();
                auto t = dynamic_cast<Property_map<Key_type, Value_type>*>(p);
                if (t != nullptr)
                {
                    return t;
                }
                else
                {
                    throw std::runtime_error("Incompatible attribute type");
                }
            }
        }

        // Hash collision - add to chain
        auto p = new Property_map<Key_type, Value_type>();
        chain.values.emplace_back(name, p);
        return p;
    }

    // New chain
    auto p = new Property_map<Key_type, Value_type>();
    m_collection.emplace_back(hash, name, p);
    m_is_optimized = false;
    return p;
}

template <typename Key_type>
template <typename Value_type>
inline void Property_map_collection<Key_type>::replace(const std::string &name, const std::string &replacement)
{
    Hash::key_type name_hash = Hash::hash(name.c_str());
    Hash::key_type replacement_hash = Hash::hash(replacement.c_str());
    ensure_optimized();

    // Look for existing chain
    auto lower   = std::lower_bound(m_collection.begin(), m_collection.end(), name_hash, Chain_comparator());
    auto r_lower = std::lower_bound(m_collection.begin(), m_collection.end(), replacement_hash, Chain_comparator());
    if (lower != m_collection.end() && r_lower != m_collection.end())
    {
        Chain &chain = *lower;
        Chain &r_chain = *r_lower;
        for (auto &entry : chain.values)
        {
            if (entry.key == name)
            {
                auto p = entry.value.get();
                auto t = dynamic_cast<Property_map<Key_type, Value_type>*>(p);
                if (t != nullptr)
                {
                    // Found old chain and entry with correct type
                    for (auto &r_entry : r_chain.values)
                    {
                        if (r_entry.key == replacement)
                        {
                            auto r_p = entry.value.get();
                            auto r_t = dynamic_cast<Property_map<Key_type, Value_type>*>(r_p);
                            if (r_t != nullptr)
                            {
                                // Found new chain and entry with correct type
                                // Rename new entry
                                r_entry.key = name;
                                // Throw away old entry
                                m_collection.erase(lower);
                                // Need to sort
                                m_is_optimized = false;
                                return;
                            }
                            else
                            {
                                throw Incompatible_attribute_type_exception(name,
                                                                            p->value_type_id().name(),
                                                                            typeid(Value_type).name());
                            }
                        }
                    }
                }
                else
                {
                    throw Incompatible_attribute_type_exception(name,
                                                                p->value_type_id().name(),
                                                                typeid(Value_type).name());
                }
            }
        }
    }
    abort(); // not found
}

template <typename Key_type>
inline void Property_map_collection<Key_type>::ensure_optimized()
{
    if (m_is_optimized)
    {
        return;
    }
    std::sort(m_collection.begin(), m_collection.end(), Chain_comparator());
    m_is_optimized = true;
}

template <typename Key_type>
inline void Property_map_collection<Key_type>::interpolate(
    Property_map_collection<Key_type> &                         destination,
    std::map<Key_type, std::vector<std::pair<float, Key_type>>> key_new_to_olds)
{
    // For all original attribute maps
    for (auto &chain : m_collection)
    {
        for (auto &entry : chain.values)
        {
            const std::string &map_name = entry.key;
            Property_map_base<Key_type> *src_map = entry.value.get();
            Property_map_base<Key_type> *destination_map = src_map->constructor();

            src_map->optimize();
            src_map->interpolate(destination_map, key_new_to_olds);

            destination.insert(map_name, destination_map);
        }
    }
}

} // namespace geometry
} // namespace renderstack

#endif
