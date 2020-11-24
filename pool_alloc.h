//
// Created by novasurfer on 11/23/20.
//

#ifndef SC_POOLALLOC_H
#define SC_POOLALLOC_H

#include "allocator.h"

namespace sc
{
    
    struct pool_alloc : allocator
    {
	pool_alloc(allocator& default_alloc, size_t block_size, size_t blocks_num, size_t alignment);
	~pool_alloc();
	void* allocate(size_t size = 0, size_t align = 0) override;
        void deallocate(void* ptr) override;
        size_t total_allocated() override;

    private:
	constexpr unsigned char* addr_from_index(size_t index) const;
	constexpr size_t index_from_addr(const unsigned char* ptr) const;

        size_t size_of_block;
	size_t num_of_blocks;
	size_t free_blocks;
        size_t num_of_initialized;
	allocator& alloc;
        unsigned char* p_start;
        unsigned char* p_next;
    };
}

#endif //SC_POOLALLOC_H
