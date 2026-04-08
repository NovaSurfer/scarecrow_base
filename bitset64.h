//
// Created by novasurfer on 20/01/25
//

#ifndef SC_BITSET_H
#define SC_BITSET_H

#include "sc_types.h"

namespace sc
{

    struct bitset64
    {
        constexpr bool operator==(bitset64 other) const;
        constexpr bool is_zero() const;
        constexpr bool test_bit(u64 pos) const;
        constexpr bitset64 and_bits(bitset64 other) const;
        constexpr bitset64 or_bits(bitset64 other) const;
        constexpr bitset64 xor_bits(bitset64 other) const;
        constexpr u64 not_bits() const;
        constexpr u64 flip_bit(u64 pos) const;

        void toggle_bit(u64 pos, bool on);

        u64 set;
    };
} // namespace sc

#endif // SC_BITSET_H
