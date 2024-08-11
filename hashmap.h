#ifndef SC_BASE_HASHMAP_H
#define SC_BASE_HASHMAP_H

/**
 * Hashmap with open addressing and Robin Hood reordering scheme.
 * Backward shift deleteion.
 *
 * - Cannot grow
 * - Only for POD types
 * - Supports custom allocator
 * - Supports range-based for loop
 *
 * Usage:
 * \code{.cpp}
 *     sc::hashmap<uint32_t Pod> hmap(allocator);
 *     hmap.init(size);
 *     hmap.put(2077, {1, 2, 3});
 *     Pod val = hmap.get(2077);
 *     for(auto& i : hmap) {...}
 * \endcode
 */

#include "allocator.h"
#include "dbg_asserts.h"
#include "hash.h"
#include "logout.h"
#include <cassert>

namespace sc
{

    /**
     * Hashmap with open addressing, Robin Hood reordering scheme, backward shift deleteion
     * \tparam K key's type.
     * \tparam V value's type.
     */
    template <typename K, typename V>
    class hashmap
    {
        struct kv_iter;
        struct kv_pair
        {
            friend hashmap;
            friend kv_iter;

            K key;
            V value;

        private:
            uint32_t dib;
        };

        struct kv_iter
        {
            constexpr kv_iter(kv_pair* kv_pairs, uint32_t cap, uint32_t id_to_return)
                : kvps(kv_pairs)
                , capacity(cap)
                , id(id_to_return)
            {
                // if it's a begin() iterator
                if(id == 0) {
                    // find first non-empty bucket
                    for(uint32_t i = 0; i < capacity; ++i) {
                        if(kvps[i].key != DEFVAL<K>) {
                            id = i;
                            return;
                        }
                    }
                }
            };

            constexpr inline const kv_iter& operator++()
            {
                ++id;
                for(; id < capacity; ++id) {
                    if(kvps[id].key != DEFVAL<K>) {
                        return *this;
                    }
                }
                id = 0xffffffffU;
                return *this;
            }

            constexpr inline bool operator==(const kv_iter& lhs) const
            {
                return lhs.capacity == capacity && lhs.kvps == kvps && lhs.id == id;
            }

            constexpr inline bool operator!=(const kv_iter& lhs) const
            {
                return !(lhs == *this);
            }

            constexpr inline kv_pair& operator*() const
            {
                return kvps[id];
            }

        private:
            kv_pair* kvps;
            const uint32_t capacity;
            uint32_t id;
        };

        using iter = kv_iter;

    public:
        /**
	 * Sets allocator.
	 */
        constexpr explicit hashmap(allocator& default_alloc);

        /**
	 * Destroys map, sets size and capacity to 0.
	 */
        ~hashmap();

        /**
	 * Allocates array of key-value pairs with a given @param capacity.
	 */
        constexpr void init(size_t capacity);

        /**
	 * Puts key & value.
	 * @return true on success.
	 */
        constexpr bool put(const K& key, const V& value);

        /**
	 * Gets value by key.
	 * @return default constructed value on fail.
	 */
        V& get(const K& key) const;
        constexpr void remove(const K& key);
        [[nodiscard]] constexpr uint32_t len() const;
        constexpr iter begin() const;
        constexpr iter end() const;

    private:
        allocator* alloc;
        kv_pair* kvps;
        uint32_t size;
        uint32_t capacity;
    };

    template <typename K, typename V>
    constexpr hashmap<K, V>::hashmap(allocator& default_alloc)
        : alloc(&default_alloc)
        , kvps(nullptr)
        , size(0)
        , capacity(0)
    { }

    template <typename K, typename V>
    hashmap<K, V>::~hashmap()
    {
        capacity = 0;
        size = 0;
        alloc->deallocate(kvps);
    }

    template <typename K, typename V>
    constexpr void hashmap<K, V>::init(size_t cap)
    {
        capacity = cap;
        kvps = static_cast<kv_pair*>(alloc->allocate(capacity * sizeof(kv_pair), alignof(kv_pair)));
        if constexpr(is_pointer_v<K>) {
            memset(kvps, 0, sizeof(kv_pair) * capacity);
        } else {
            memset(kvps, 0xffU, sizeof(kv_pair) * capacity);
        }
    }

    template <typename K, typename V>
    constexpr bool hashmap<K, V>::put(const K& key, const V& value)
    {
        DBG_FAIL_IF(size == capacity, "can't insert anymore")
        if(size == capacity) {
            return false;
        }

        const uint32_t init_index = hash(key) % capacity;
        kv_pair to_insert;
        to_insert.key = key;
        to_insert.value = value;
        to_insert.dib = 0;

        for(uint32_t i = 0, current_index = init_index; i < capacity;
            ++i, current_index = (init_index + i) % capacity) {
            // if slot is empty
            if(kvps[current_index].key == DEFVAL<K>) {
                kvps[current_index] = to_insert;
                ++size;
                return true;
                // if key is already inserted
            } else if(kvps[current_index].key == to_insert.key) {
                sc::swap(to_insert, kvps[current_index]);
                return true;
                // if DIB of value to insert is greater
            } else if(kvps[current_index].dib < to_insert.dib) {
                sc::swap(to_insert, kvps[current_index]);
            }
            ++to_insert.dib;
        }

        return false;
    }

    template <typename K, typename V>
    V& hashmap<K, V>::get(const K& key) const
    {
        uint32_t key_hash = hash(key);
        uint32_t index = key_hash % capacity;
        kv_pair kv = kvps[index];
        while(kv.key != key) {
            index = (index + 1) % capacity;
            kv = kvps[index];
            if(index == key_hash % capacity) {
                static V default_value = V();
                return default_value;
            }
        }
        return kvps[index].value;
    }

    template <typename K, typename V>
    constexpr void hashmap<K, V>::remove(const K& key)
    {
        // get the index for the intial key that will be deleted
        uint32_t index = hash(key) % capacity;
        for(uint32_t i = 0; i <= capacity; ++i) {
            index = (index + 1) % capacity;
            if(kvps[index].key == key) {
                break;
            }
        }
        // 1) Find "stop" bucket
        //    empty bucket or DIB that equal to 0.
        // 2) Shift elements backward between deleted and stop buckets.
        for(uint32_t i = 1; i < capacity; ++i) {
            uint32_t prev_index = (index + i - 1) % capacity;
            uint32_t swap_index = (index + i) % capacity;
            if(kvps[swap_index].key == DEFVAL<K> || kvps[swap_index].dib == 0) {
                kvps[prev_index].key = DEFVAL<K>;
                --size;
                return;
            }
            --kvps[swap_index].dib;
            kvps[prev_index] = kvps[swap_index];
        }
    }

    template <typename K, typename V>
    constexpr uint32_t hashmap<K, V>::len() const
    {
        return size;
    }

    template <typename K, typename V>
    constexpr typename hashmap<K, V>::iter hashmap<K, V>::begin() const
    {
        return kv_iter(kvps, capacity, 0);
    }

    template <typename K, typename V>
    constexpr typename hashmap<K, V>::iter hashmap<K, V>::end() const
    {
        return kv_iter(kvps, capacity, 0xffffffffU);
    }
}

#endif //SC_BASE_HASHMAP_H
