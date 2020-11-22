//
// Created by novasurfer on 11/10/20.
//

#include "heap_alloc.h"
#include "memory.h"

// TODO: Make it thread-safe
// TODO: unused alloc_count

namespace sc
{
    size_t heap_alloc::allocs_size;
    size_t heap_alloc::allocs_count;

    void* heap_alloc::allocate(size_t size, size_t align)
    {
        ++allocs_count;
        void* p = mem::alloc_aligned(size, align);
#if defined(SC_ALLOC_WITH_HEADER)
        allocs_size += mem::allocated_size(p);
#endif
        return p;
    }

    void heap_alloc::deallocate(void* ptr)
    {
        --allocs_count;
#if defined(SC_ALLOC_WITH_HEADER)
        allocs_size -= mem::allocated_size(ptr);
#endif
        mem::dealloc_aligned(ptr);
    }

    size_t heap_alloc::total_allocated()
    {
        return allocs_size;
    }

}
