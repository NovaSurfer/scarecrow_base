//
// Created by novasurfer on 11/13/20.
//

#ifndef SC_LINEARALLOC_H
#define SC_LINEARALLOC_H

#include "allocator.h"

namespace sc
{
    /**
     * Linear allocator, allocates one chunk of memory.
     * On each allocate() gives a piece that chunk.
     */
    struct linear_alloc : allocator
    {
	/**
	 * @param default_alloc backing allocator.
	 * @param size size of the linear allocator chunk.
	 * @param align alignment of the linear allocator chunk.
	 */
	linear_alloc(allocator& default_alloc, size_t size, size_t align);
	~linear_alloc();

	/**
	 * Gives @param size of space from allocated chunk.
	 */
	void* allocate(size_t size, size_t align) override;
        void deallocate(void* ptr) override;
        size_t total_allocated() override;
	void reset();

    private:
	size_t total_size;
	size_t free_size;
	allocator& alloc;
	void* membuff;
    };
}

#endif //SC_LINEARALLOC_H
