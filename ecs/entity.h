//
// Created by max on 4/13/23.
//

#ifndef SCARECROW2D_ENTITY_H
#define SCARECROW2D_ENTITY_H

#include "bitset64.h"
#include "sc_types.h"

namespace sc
{
    using entity_id = u32;

    struct entity
    {
        static constexpr u16 INDEX_BITS = 16;
        static constexpr u16 INDEX_MASK = (1 << INDEX_BITS) - 1;
        static constexpr u16 GENERATION_BITS = 1;
        static constexpr u16 GENERATION_MASK = (1 << GENERATION_BITS) - 1;
        static constexpr u16 MINIMUM_FREE_INDES = 1024;

        [[nodiscard]] constexpr u32 index() const
        {
            return id & INDEX_MASK;
        }

        static constexpr u32 index_of_id(entity_id id)
        {
            return id & INDEX_MASK;
        }

        [[nodiscard]] constexpr u32 generation() const
        {
            return (id >> INDEX_BITS) & GENERATION_MASK;
        }

        bitset64 comps_bits;
        entity_id id;
    };
} // namespace sc

#endif // SCARECROW2D_ENTITY_H
