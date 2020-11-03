#include <cassert>
#include <cstdint>
#include <iostream>
#include "memory.h"

int main()
{

    using namespace sc;
    mem::set_header(memhead_type::SIZE | memhead_type::ALIGN);
    long* l = (long*)mem::alloc_aligned(sizeof(long) * 5, 16);
    assert(reinterpret_cast<uintptr_t>(l) % 16 == 0);
    std::cout << mem::allocated_size(l) << '\n';
    std::cout << mem::allocated_alignment(l) << '\n';
    mem::dealloc_aligned(l);
    return 0;
}

