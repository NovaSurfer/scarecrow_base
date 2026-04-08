//
// Created by novasurfer on 20/01/25
//

#include "bitset64.h"

namespace sc
{

    constexpr bool bitset64::operator==(bitset64 other) const
    {
        return set == other.set;
    }

    constexpr bool bitset64::is_zero() const
    {
        return set == 0U;
    }

    constexpr bool bitset64::test_bit(u64 pos) const
    {
        return set & (static_cast<u64>(1U) << pos);
    }

     constexpr bitset64 bitset64::and_bits(bitset64 other) const
     {
         return {set & other.set};
     }

    constexpr sc::bitset64 bitset64::or_bits(bitset64 other) const
    {
        return {set | other.set};
    }

    constexpr bitset64 bitset64::xor_bits(bitset64 other) const
    {
        return {set ^ other.set};
    }

    constexpr u64 bitset64::not_bits() const
    {
        return ~set;
    }

    constexpr u64 bitset64::flip_bit(u64 pos) const
    {
        return set ^ (static_cast<u64>(1U) << pos);
    }

     void bitset64::toggle_bit(u64 pos, bool on)
     {
         if(on) {
             set |= (static_cast<u64>(1U) << pos);
         } else {
             set &= (static_cast<u64>(1U) << pos);
         }
     }

} // namespace sc
