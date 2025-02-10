//
// Created by novasurfer on 20/01/25
//

#ifndef SC_BITSET_H
#define SC_BITSET_H

#include "sc_types.h"

namespace sc
{

    class bitset64
    {
    public:
        constexpr bool is_zero() const;
        constexpr bool test_bit(u64 pos) const;
        constexpr u64 and_bits(u64 other) const;
        constexpr u64 or_bits(u64 other) const;
        constexpr u64 xor_bits(u64 other) const;
        constexpr u64 not_bits() const;
        constexpr u64 flip_bit(u64 pos) const;

        constexpr bool operator==(u64 other) const;

        u64 set;

    private:
    };
} // namespace sc

#endif // SC_BITSET_H
