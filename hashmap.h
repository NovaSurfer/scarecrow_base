#include "allocator.h"
#include "hash.h"
#include <bits/stdint-uintn.h>
#include <cstdio>

namespace sc
{

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
            uint32_t dib;

        private:
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
                        if(kvps[i].dib != 0xffffffffU) {
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
                    if(kvps[id].dib != 0xffffffffU) {
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

    public:
        using iter = kv_iter;

        hashmap(allocator& default_alloc);
        ~hashmap();
        void init(size_t capacity);
        bool put(K key, const V& value);
        const V get(const K& key) const;
        void remove(const K& key);
        uint32_t len() const;
        iter begin();
        iter end();

        kv_pair* kvps;

    private:
        allocator* alloc;
        uint32_t size;
        uint32_t capacity;
    };

    template <typename K, typename V>
    hashmap<K, V>::hashmap(allocator& default_alloc)
        : alloc(&default_alloc)
        , kvps(nullptr)
        , size(0)
        , capacity(0)
    {}

    template <typename K, typename V>
    hashmap<K, V>::~hashmap()
    {
        capacity = 0;
        size = 0;
        alloc->deallocate(kvps);
    }

    template <typename K, typename V>
    void hashmap<K, V>::init(size_t capacity)
    {
        this->capacity = capacity;
        kvps = static_cast<kv_pair*>(alloc->allocate(capacity * sizeof(kv_pair), alignof(kv_pair)));
        memset(kvps, 0xffffffffU, sizeof(kv_pair) * capacity);
    }

    template <typename K, typename V>
    bool hashmap<K, V>::put(K key, const V& value)
    {
        if(size == capacity) {
            return false;
        }

        const uint32_t index = hash(key) % capacity;
        kv_pair to_insert;
        to_insert.key = key;
        to_insert.value = value;
        to_insert.dib = 0;

        for(uint32_t i = index;; i = (i + 1) % capacity) {
            if(kvps[i].key == 0xffffffffU) {
                kvps[i] = to_insert;
                ++size;
                printf("ON INSERT: kvps[%d].dib=%d\n", i, to_insert.dib);
                return true;
            } else if(kvps[i].dib < to_insert.dib) {
                sc::swap(to_insert, kvps[i]);
            }
            ++to_insert.dib;
        }
        return false;
    }

    template <typename K, typename V>
    const V hashmap<K, V>::get(const K& key) const
    {
        uint32_t key_hash = hash(key);
        uint32_t index = key_hash % capacity;
        kv_pair kv = kvps[index];
        while(kv.key != key) {
            index = (index + 1) % capacity;
            kv = kvps[index];
            if(index == key_hash % capacity) {
                return V();
            }
        }
        return kv.value;
    }

    template <typename K, typename V>
    void hashmap<K, V>::remove(const K& key)
    {
        // get the index for the intial key that will be deleted
        uint32_t index = hash(key) % capacity;
        //printf("index:%d\n", index);
        for(uint32_t i = 0; i <= capacity; ++i) {
            index = (index + 1) % capacity;
            if(kvps[index].key == key) {
                break;
            }
        }
        // 1) Find "stop" bucket
        //    empty bucket or DIB that equal to 0.
        // 2) Shift elements backward between deleted and stop buckets.
        uint32_t prev_index = index;
        uint32_t swap_index;
        for(uint32_t i = 1; i < capacity; ++i) {
            uint32_t prev_index = (index + i - 1) % capacity;
            uint32_t swap_index = (index + i) % capacity;
            if(kvps[swap_index].key == 0xffffffffU || kvps[swap_index].dib == 0) {
                kvps[prev_index].key = 0xffffffffU;
                --size;
                return;
            }
            printf("BEFORE SWAP: kvps[%d].dib=%d\n", swap_index, kvps[swap_index].dib);
            --kvps[swap_index].dib;
            kvps[prev_index] = kvps[swap_index];
        }
    }

    template <typename K, typename V>
    uint32_t hashmap<K, V>::len() const
    {
        return size;
    }

    template <typename K, typename V>
    typename hashmap<K, V>::iter hashmap<K, V>::begin()
    {
        return kv_iter(kvps, capacity, 0);
    }

    template <typename K, typename V>
    typename hashmap<K, V>::iter hashmap<K, V>::end()
    {
        return kv_iter(kvps, capacity, 0xffffffffU);
    }
}
