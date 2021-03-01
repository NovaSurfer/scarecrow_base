#include "MurmurHash3.h"
#include <cstdint>
#include <cstring>

namespace sc
{
    static constexpr uint32_t HASH_SEED = 0x81D;

    uint32_t hash(uint32_t key)
    {
        uint32_t result[1];
	    const uint32_t* ptr = &key;
        MurmurHash3_x86_32(ptr, sizeof(uint32_t), HASH_SEED, result);
        return *result;
    }

    uint32_t hash(const char* key)
    {
        uint32_t result[1];
        MurmurHash3_x86_32(&key, strlen(key), HASH_SEED, result);
        return *result;
    }
}
