#ifndef property_map_collection_hpp_renderstack_geometry
#define property_map_collection_hpp_renderstack_geometry

#include "renderstack_geometry/exception.hpp"
#include "renderstack_geometry/property_map.hpp"
//#include "renderstack_toolkit/platform.hpp"
#include <map>
#include <memory>

namespace renderstack
{
namespace geometry
{

template <typename T, T prime, T offset>
struct fnv1ah {
	using key_type = T;
	static constexpr T hash(const char *p, size_t len, T seed = offset) {
		T hash = seed;
		for (size_t i = 0; i < len; ++i) {
			hash = (hash ^ static_cast<unsigned char>(p[i])) * prime;
		}
		return hash;
	}
	template <size_t N>
	static constexpr T hash(const char(&s)[N], T seed = offset) {
		return hash(s, N - 1, seed);
	}
	static T hash(const std::string& str, T seed = offset) {
		return hash(str.data(), str.size(), seed);
	}
	template <typename... Args>
	constexpr T operator()(Args&&... args) const {
		return hash(std::forward<Args>(args)...);
	}
};
using fnv1ah32 = fnv1ah<std::uint32_t, 0x1000193UL, 2166136261UL>;
using fnv1ah64 = fnv1ah<std::uint64_t, 0x100000001b3ULL, 14695981039346656037ULL>;

constexpr std::uint32_t operator"" _fnv1a(const char* s, size_t size) {
	return fnv1ah64::hash(s, size);
}

template <typename Key_type>
class Property_map_collection
{
private:
    using Hash = fnv1ah32;
    struct Entry
    {
        Entry() = default;
        ~Entry() = default;
        Entry(const std::string &key, Property_map_base<Key_type> *p)
        : key(key)
        , value(p)
        {
        }
        Entry(const Entry &) = delete;
        Entry& operator=(const Entry &) = delete;

        Entry(Entry &&other) 
        {
            key = other.key;
            value = std::move(other.value);
        }
        Entry& operator=(Entry &&other)
        {
            key = other.key;
            value = std::move(other.value);
        }

        std::string key;
        std::unique_ptr<Property_map_base<Key_type>> value;
    };

    struct Chain
    {
        Chain() = default;
        ~Chain() = default;
        Chain(Hash::key_type hash, const std::string &name, Property_map_base<Key_type> *p) : hash(hash) {
            values.emplace_back(name, p);
        }
        Chain(const Chain&) = delete;
        Chain& operator=(const Chain &) = delete;        
        Chain(Chain &&other)
        {
            hash = other.hash;
            values = std::move(other.values);
        }
        Chain& operator=(Chain &&other)
        {
            hash = other.hash;
            values = std::move(other.values);
            return *this;
        }

        Hash::key_type hash{0U};
        std::vector<Entry> values;
    };

    struct Chain_comparator
    {
        // for lower_bound() and upper_bound
        bool operator()(const Chain &chain, const Hash::key_type hash_value) const
        {
            return chain.hash < hash_value;
        }
        // for sort()
        bool operator()(const Chain &lhs, const Chain &rhs) const
        {
            return lhs.hash < rhs.hash;
        }        
    };
    using Collection_type = std::vector<Chain>;

public:
    void clear();

    size_t size() const;

    template <typename Value_type>
    Property_map<Key_type, Value_type> *create(const std::string &name);

    // Inserts map to collection
#if 0
    template <typename Value_type>
    void insert(const std::string &name, Property_map<Key_type, Value_type> *map);
#endif

    void insert(const std::string &name, Property_map_base<Key_type> *map);
    

    // Removes map from collections
    void remove(const std::string &name);

    // not const because calls ensure_optimized()
    template <typename Value_type>
    bool contains(const std::string &name);

    template <typename Value_type>
    bool contains(const std::string &name) const;

    Property_map_base<Key_type> *find_any(const std::string &name);

    template <typename Value_type>
    Property_map<Key_type, Value_type> *find(const std::string &name);

    template <typename Value_type>
    Property_map<Key_type, Value_type> *maybe_find(const std::string &name);

    template <typename Value_type>
    Property_map<Key_type, Value_type> *maybe_find(const std::string &name) const;

    template <typename Value_type>
    Property_map<Key_type, Value_type> *find_or_create(const std::string &name);

    template <typename Value_type>
    void replace(const std::string &name, const std::string &replacement);

    void interpolate(Property_map_collection<Key_type> &                         destination,
                     std::map<Key_type, std::vector<std::pair<float, Key_type>>> key_new_to_olds);

    void ensure_optimized();

private:
    Collection_type m_collection;
    bool m_is_optimized{false};
};

} // namespace geometry
} // namespace renderstack

#include "property_map_collection.inl"

#endif
