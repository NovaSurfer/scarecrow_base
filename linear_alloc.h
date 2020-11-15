//
// Created by novasurfer on 11/13/20.
//

#ifndef SC_LINEARALLOC_H
#define SC_LINEARALLOC_H

#include "allocator.h"

namespace sc
{

    struct linear_alloc : allocator
    {
	linear_alloc(allocator& default_alloc, size_t size, size_t align);
	~linear_alloc();
	void* allocate(size_t size, size_t align) override;
        void deallocate(void* ptr) override;
        size_t total_allocated() override;
	void reset();

    private:
	size_t total_size;
	size_t free_size;
	allocator* alloc;
	void* membuff;
	
    };
}

#endif //SC_LINEARALLOC_H
