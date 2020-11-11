//
// Created by novasurfer on 11/10/20.
//

#ifndef SC_HEAPALLOC_H
#define SC_HEAPALLOC_H

#include "allocator.h"

namespace sc
{

    struct heap_alloc : allocator
    {
        void* allocate(size_t size, size_t align) override;
        void deallocate(void* ptr) override;
        size_t total_allocated() override;

    private:
	static size_t allocs_count;
	static size_t allocs_size;
    };
}


#endif //SC_HEAPALLOC_H
