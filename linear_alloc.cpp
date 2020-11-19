//
// Created by novasurfer on 11/13/20.
//

#include "linear_alloc.h"
#include <cstdint>

namespace sc
{

    linear_alloc::linear_alloc(allocator& default_alloc, size_t size, size_t align)
        : total_size(size)
        , free_size(size)
        , alloc(&default_alloc)
        , membuff(default_alloc.allocate(size, align))

    {}

    linear_alloc::~linear_alloc()
    {
        alloc->deallocate(membuff);
	membuff = nullptr;
    }

    // warn: align is not needed here
    void* linear_alloc::allocate(size_t size, [[maybe_unused]] size_t align = 0)
    {
        if(free_size <= 0) {
            return nullptr;
        }

        void* ptr =
            static_cast<void*>((static_cast<uintptr_t*>(membuff) + (total_size - free_size)));
        free_size -= size;

        return ptr;
    }

    [[maybe_unused]] void linear_alloc::deallocate(void*) {}

    size_t linear_alloc::total_allocated() 
    {
        return total_size;
    }

    void linear_alloc::reset()
    {
	free_size = total_size;
    }
}
