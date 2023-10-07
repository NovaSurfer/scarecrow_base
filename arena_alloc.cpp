//
// Created by max on 9/24/23.
//

#include "arena_alloc.h"
#include "dbg_asserts.h"
#include "memory.h"
#include <cstring>

namespace sc
{
    void arena_alloc::init(void* backing_buffer, size_t backing_buffer_length)
    {
        buf = static_cast<unsigned char*>(backing_buffer);
        buf_len = backing_buffer_length;
        curr_offset = 0;
        prev_offset = 0;
    }

    void* arena_alloc::allocate(size_t size, size_t align)
    {
        uintptr curr_ptr = (uintptr)buf + curr_offset;
        uintptr offset = mem::align_forward(curr_ptr, align);
        offset -= (uintptr)buf;
        if(offset + size <= buf_len) {
            void* ptr = &buf[offset];
            prev_offset = offset;
            curr_offset = offset + size;

            //            memset(ptr, 0, size);
            return ptr;
        }

        return nullptr;
    }

    void* arena_alloc::resize(void* old_memory, size_t old_size, size_t new_size, size_t align)
    {
        DBG_FAIL_IF(!mem::is_power_of_two(align) || align == 0, "Alignment should be power of 2")

        if(!old_memory || old_size == 0) {
            return allocate(new_size, align);
        } else if(buf <= old_memory && old_memory < buf + buf_len) {
            if(buf + prev_offset == old_memory) {
                curr_offset = prev_offset + new_size;
                if(new_size > old_size) {
                    memset(&buf[curr_offset], 0, new_size - old_size);
                }
                return old_memory;
            } else {
                void* new_memory = allocate(new_size, align);
                size_t copy_size = old_size < new_size ? old_size : new_size;
                memmove(new_memory, old_memory, copy_size);
                return new_memory;
            }
        }
        DBG_FAIL("Memory is out of bounds of the buffer in this arena")
        return nullptr;
    }

    void arena_alloc::deallocate([[maybe_unused]] void* ptr)
    {
        curr_offset = 0;
        prev_offset = 0;
    }

    // TODO: incorrect
    size_t arena_alloc::total_allocated()
    {
        return buf_len - curr_offset;
    }

    arena_memory_scoped_savepoint::arena_memory_scoped_savepoint(arena_alloc* arena)
        : a {arena}
        , prev_offset {a->prev_offset}
        , curr_offset {a->curr_offset}
    { }

    arena_memory_scoped_savepoint::~arena_memory_scoped_savepoint()
    {
        DBG_FAIL_IF(!a, "Arena is NULL")
        a->prev_offset = prev_offset;
        a->curr_offset = curr_offset;
    }

} // namespace sc
