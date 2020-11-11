//
// Created by novasurfer on 11/10/20.
//

#include "heap_alloc.h"
#include "memory.h"

namespace sc
{
    void* heap_alloc::allocate(size_t size, size_t align)
    {
        ++allocs_count;
        void* p = mem::alloc_aligned(size, align);
#if defined(SC_ALLOC_WITH_HEADER)
        allocated_size += mem::allocated_size(p);
#endif
            return p;
    }

    void heap_alloc::deallocate(void* ptr)
    {
        --allocs_count;
#if defined(SC_ALLOC_WITH_HEADER)
        allocated_size -= mem::allocated_size(p);
#endif
        mem::dealloc_aligned(ptr);
    }

    size_t heap_alloc::total_allocated()
    {
	return allocs_count;
    }

}
