//
// Created by max on 6/7/23.
//

#include "hash.h"

namespace sc
{
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

    template <>
    const char* DEFVAL<const char*> {nullptr};

    template <>
    u32 DEFVAL<u32> {0xffffffffU};
}