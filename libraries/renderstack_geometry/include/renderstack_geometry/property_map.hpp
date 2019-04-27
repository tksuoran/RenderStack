#ifndef property_map_hpp_renderstack_geometry
#define property_map_hpp_renderstack_geometry

#include "renderstack_geometry/exception.hpp"
#include "renderstack_toolkit/platform.hpp"
#include <algorithm>
#include <cassert>
#include <map>
#include <stdint.h>
#include <typeinfo>
#include <vector>

namespace renderstack
{
namespace geometry
{

template <typename Key_type>
class Property_map_base
{
public:
    virtual ~Property_map_base() = default;

public:
    virtual Property_map_base *constructor() const = 0;

    virtual void clear() = 0;

    virtual bool empty() const = 0;

    virtual size_t size() const = 0;

    //virtual bool is_inserting() const = 0;

    virtual bool has(Key_type key) const = 0;

    virtual void optimize() = 0;

    //virtual bool is_optimized() const = 0;

    //virtual std::type_info const &value_type_id() const = 0;

    virtual void interpolate(Property_map_base<Key_type> *                               destination,
                             std::map<Key_type, std::vector<std::pair<float, Key_type>>> key_new_to_olds) const = 0;

protected:
    Property_map_base() = default;
};

template <typename Key_type, typename Value_type>
class Property_map
    : public Property_map_base<Key_type>
{
public:
    Property_map() = default;

    void put(Key_type key, Value_type value);

    Value_type get(Key_type key) const;

    bool maybe_get(Key_type key, Value_type &value) const;

    void clear() override;

    bool empty() const override;

    size_t size() const override;

    void begin_insertion(size_t estimated_count = 0);

    void insert(Key_type key, Value_type value);

    void end_insertion();

    bool has(Key_type key) const override;

    void optimize() override;

    void interpolate(Property_map_base<Key_type> *                               destination,
                     std::map<Key_type, std::vector<std::pair<float, Key_type>>> key_new_to_olds) const override;

    Property_map_base<Key_type> *constructor() const override;

    // std::type_info const &value_type_id() const;

private:
    struct Entry
    {
        Entry(Key_type key) : key(key) {}
        Entry(Key_type key, Value_type value) : key(key), value(value) {}

        bool operator==(Entry const &other) const;
        bool operator<(Entry const &other) const;

        Key_type   key;
        Value_type value;
    };

    struct Entry_comparator
    {
        // for equal_range()
        bool operator()(const Entry &lhs, const Entry &rhs) const
        {
            return lhs.key < rhs.key;
        }
    };

    typedef std::vector<Entry> Entry_container;

    typename Entry_container::const_iterator find(Key_type key) const;

    typename Entry_container::iterator find(Key_type key);

    void insert_entry(Key_type key, Value_type value);

    Entry_container m_entries;
    bool            m_is_optimized{true};
    bool            m_is_inserting{false};
};

} // namespace geometry
} // namespace renderstack

#include "property_map.inl"

#endif
