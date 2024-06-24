//
// Created by max on 4/13/23.
//

#ifndef SCARECROW2D_ENTITY_H
#define SCARECROW2D_ENTITY_H

#include "hash.h"
#include "hashmap.h"
#include "heap_alloc.h"
#include "queue.h"
#include "sc_types.h"
#include <cassert>
#include <functional>
#include <queue>
#include <vector>

namespace sc2d
{

    struct entity
    {
        u32 id;
        [[nodiscard]] constexpr u32 index() const;
        [[nodiscard]] constexpr u32 generation() const;
        constexpr friend bool operator<(const entity& l, const entity& r);
        constexpr bool operator!=(const entity& other) const
        {
            return !(id == other.id);
        }
        constexpr bool operator==(const entity& other) const
        {
            return id == other.id;
        }
    };

    class entity_manager
    {
    public:
        entity_manager(sc::heap_alloc& halloc);
        bool alive(entity e);
        entity create();
        void destroy(entity e);

    private:
        std::queue<u32> free_indices;
        sc::vec<u16> generation;
    };
}

// Adds new hash template specialization for an entity struct
template <>
u32 sc::hash<>(sc2d::entity key);

#endif //SCARECROW2D_ENTITY_H
