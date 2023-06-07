#ifndef SC_BASE_HASH_H
#define SC_BASE_HASH_H

#include "murmur.h"
#include "sc_types.h"
#include <cstring>

namespace sc
{
    static constexpr u32 HASH_SEED = 0x81D;

    template <typename T>
    u32 hash(T);

    template <>
    u32 hash<>(uint32_t key);

    template <>
    u32 hash<>(const char* key);
}

#endif //SC_BASE_HASH_H
