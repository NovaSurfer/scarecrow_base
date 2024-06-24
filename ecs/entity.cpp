//
// Created by max on 5/25/23.
//

#include "entity.h"
#include "heap_alloc.h"
//#include "base/hashmap.h"

namespace sc2d
{

    static constexpr u16 ENTITY_INDEX_BITS = 16;
    static constexpr u16 ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;
    static constexpr u16 ENTITY_GENERATION_BITS = 1;
    static constexpr u16 ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;
    static constexpr u16 MINIMUM_FREE_INDES = 1024;

    constexpr u32 entity::index() const
    {

        return id & ENTITY_INDEX_MASK;
    }

    constexpr u32 entity::generation() const
    {

        return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK;
    }

    constexpr bool operator<(const entity& l, const entity& r)
    {
        return l.id < r.id;
    }

    entity_manager::entity_manager(sc::heap_alloc& halloc)
        : generation {halloc}
    { }

    bool entity_manager::alive(entity e)
    {
        return generation[e.index()] == e.generation();
    }

    entity entity_manager::create()
    {
        u32 id;
        if(free_indices.size() > MINIMUM_FREE_INDES) {
            id = free_indices.front();
            free_indices.pop();
        } else {
            generation.push(0u);
            id = generation.len() - 1;
            assert(id < (1 << ENTITY_INDEX_BITS));
        }

        return {id | generation[id] << ENTITY_INDEX_BITS};
    }

    void entity_manager::destroy(entity e)
    {
        const u32 idx = e.id;
        ++generation[idx];
        free_indices.push(idx);
    }
}

template <>
u32 sc::hash<>(sc2d::entity key)
{
    u32 result[1];
    const u32* ptr = &key.id;
    MurmurHash3_x86_32(ptr, sizeof(u32), HASH_SEED, result);
    return *result;
}

template<>
sc2d::entity sc::DEFVAL<sc2d::entity> {sc2d::entity{0xffffffffU}};
