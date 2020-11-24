//
// Created by novasurfer on 11/23/20.
//
//Pool allocator reference: http://www.thinkmind.org/download.php?articleid=computation_tools_2012_1_10_80006

#include "pool_alloc.h"

namespace sc
{

    // warn: p_start can be initialized with nullptr, maybe move to create() method ?!
    pool_alloc::pool_alloc(allocator& default_alloc, size_t block_size, size_t blocks_num,
                           size_t alignment)
        : size_of_block(block_size)
        , num_of_blocks(blocks_num)
        , free_blocks(blocks_num)
        , num_of_initialized(0)
        , alloc(default_alloc)
        , p_start(static_cast<unsigned char*>(alloc.allocate(block_size * blocks_num, alignment)))
        , p_next(p_start)
    {}

    pool_alloc::~pool_alloc()
    {
	alloc.deallocate(p_start);
	p_start = nullptr;
    }

    void* pool_alloc::allocate([[maybe_unused]] size_t size, [[maybe_unused]] size_t align)
    {
        if(num_of_initialized < num_of_blocks) {
            size_t* ptr = reinterpret_cast<size_t*>(addr_from_index(num_of_initialized));
            *ptr = num_of_initialized + 1;
            ++num_of_initialized;
        }

        void* result = nullptr;

        if(free_blocks > 0) {
            result = static_cast<void*>(p_next);
            --free_blocks;
            if(free_blocks > 0) {
                p_next = addr_from_index(*reinterpret_cast<size_t*>(p_next));
            } else {
		p_next = nullptr;
	    }
        }

	return result;
    }

    void pool_alloc::deallocate(void* ptr)
    {
	if(!p_next) {
	    *static_cast<size_t*>(ptr) = index_from_addr(p_next);
	    p_next = static_cast<unsigned char*>(ptr);
	} else {
	    *static_cast<size_t*>(ptr) = num_of_blocks;
	    p_next = static_cast<unsigned char*>(ptr);
	}
	++free_blocks;
    }

    size_t pool_alloc::total_allocated()
    {
	return num_of_blocks * size_of_block;
    }

    constexpr unsigned char* pool_alloc::addr_from_index(size_t index) const
    {
        return p_start + index * size_of_block;
    }

    constexpr size_t pool_alloc::index_from_addr(const unsigned char* ptr) const
    {
        return ((size_t)(ptr - p_start) / size_of_block);
    }

}
