//
// Created by max on 4/13/23.
//

#ifndef SC_ENTITY_MANAGER_H
#define SC_ENTITY_MANAGER_H

#include "entity.h"
#include "hash.h"
#include "heap_alloc.h"
#include "vec.h"
#include <queue>

namespace sc
{

    class entity_manager
    {
    public:
        explicit entity_manager(heap_alloc& halloc);
        bool alive(entity e);
        entity create();
        void destroy(entity e);

    private:
        std::queue<u32> free_indices;
        vec<u16> generation;
    };
} // namespace sc

// Adds new hash template specialization for an entity struct
template <>
u32 sc::hash<>(sc::entity key);

#endif // SC_ENTITY_MANAGER_H
