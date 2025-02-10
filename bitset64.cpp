//
// Created by novasurfer on 20/01/25
//

#include "bitset64.h"

namespace sc
{

    constexpr bool bitset64::is_zero() const
    {
        return set == 0U;
    }

    constexpr bool bitset64::test_bit(u64 pos) const
    {
        return set & (static_cast<u64>(1U) << pos);
    }

    constexpr u64 bitset64::and_bits(u64 other) const
    {
        return set & other;
    }

    constexpr u64 bitset64::or_bits(u64 other) const
    {
        return set | other;
    }

    constexpr u64 bitset64::xor_bits(u64 other) const
    {
        return set ^ other;
    }

    constexpr u64 bitset64::not_bits() const
    {
        return ~set;
    }

    constexpr u64 bitset64::flip_bit(u64 pos) const
    {
        return set ^ (static_cast<u64>(1U) << pos);
    }

    constexpr bool bitset64::operator==(u64 other) const
    {
        return set == other;
    }

} // namespace sc

