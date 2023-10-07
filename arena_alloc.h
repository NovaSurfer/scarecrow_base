//
// Created by max on 9/24/23.
//

// Based on: https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/

#ifndef SC_BASE_ARENA_ALLOC_H
#define SC_BASE_ARENA_ALLOC_H

#include "allocator.h"
#include "sc_types.h"

namespace sc
{

    struct arena_memory_scoped_savepoint;
    struct arena_alloc : allocator
    {
        friend arena_memory_scoped_savepoint;
        void init(void* backing_buffer, size_t backing_buffer_length);
        void* allocate(size_t size, size_t align) override;
        size_t total_allocated() override;
        void* resize(void* old_memory, size_t old_size, size_t new_size, size_t align);
        void deallocate(void* ptr) override;

    private:
        uchar* buf;
        size_t buf_len;
        size_t prev_offset;
        size_t curr_offset;
    };

    struct arena_memory_scoped_savepoint
    {
        explicit arena_memory_scoped_savepoint(arena_alloc* arena);
        ~arena_memory_scoped_savepoint();

    private:
        arena_alloc* a;
        size_t prev_offset;
        size_t curr_offset;
    };
} // namespace sc

#endif // SC_BASE_ARENA_ALLOC_H
