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
    u32 hash<>(uint32_t key)
    {
        u32 result[1];
        const u32* ptr = &key;
        MurmurHash3_x86_32(ptr, sizeof(u32), HASH_SEED, result);
        return *result;
    }

    template <>
    u32 hash<>(const char* key)
    {
        u32 result[1];
        MurmurHash3_x86_32(&key, strlen(key), HASH_SEED, result);
        return *result;
    }
}

#endif //SC_BASE_HASH_H
