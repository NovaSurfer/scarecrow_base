//
// Created by max on 5/25/23.
//

#include "entity_manager.h"
#include <cassert>

namespace sc
{
    entity_manager::entity_manager(heap_alloc& halloc)
        : generation {halloc}
    { }

    bool entity_manager::alive(entity e)
    {
        return generation[e.index()] == e.generation();
    }

    entity entity_manager::create()
    {
        u32 id;
        if(free_indices.size() > entity::MINIMUM_FREE_INDES) {
            id = free_indices.front();
            free_indices.pop();
        } else {
            generation.push(0u);
            id = generation.len() - 1;
            assert(id < (1 << entity::INDEX_BITS));
        }

        return {{0}, id | generation[id] << entity::INDEX_BITS};
    }

    void entity_manager::destroy(entity e)
    {
        const u32 idx = e.index();
        ++generation[idx];
        free_indices.push(idx);
    }
} // namespace sc

template <>
u32 sc::hash<>(entity key)
{
    u32 result[1];
    const u32* ptr = &key.id;
    MurmurHash3_x86_32(ptr, sizeof(u32), HASH_SEED, result);
    return *result;
}

template <>
sc::entity sc::DEFVAL<sc::entity> {entity {0xffffffffU}};
