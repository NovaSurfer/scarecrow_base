//
// Created by novasurfer on 11/15/20.
//

#include "temp_alloc.h"
#include <cstddef>

namespace sc
{
    template <size_t STACK_BUFF_SIZE>
    temp_alloc<STACK_BUFF_SIZE>::temp_alloc(allocator& default_alloc)
        : alloc(&default_alloc)
    {
        current = start = stackbuff;
        end = start + STACK_BUFF_SIZE;
        *reinterpret_cast<void**>(start) = nullptr;
        current += sizeof(void*);
    }

    template <size_t STACK_BUFF_SIZE>
    temp_alloc<STACK_BUFF_SIZE>::~temp_alloc()
    {
        char* first = stackbuff;
        void* p = *reinterpret_cast<void**>(first);
        while(p) {
            void* next = *reinterpret_cast<void**>(p);
            alloc->deallocate(p);
            p = next;
        }
    }

    template <size_t STACK_BUFF_SIZE>
    void* temp_alloc<STACK_BUFF_SIZE>::allocate(size_t size, size_t align)
    {
        if((ptrdiff_t)size > end - current) {
            // allocate size + void*, so we can store the void* that
            // will be used for deallocation.
            size_t to_allocate = sizeof(void*) + size;
            void* p = alloc->allocate(to_allocate, align);
            // set the address of allocation
            // to the first (void*)element of the previuos allocation
            *reinterpret_cast<void**>(start) = p;
            // set the begining of allocation current allocation
            start = static_cast<char*>(p);
            end = start + to_allocate;
            *(void**)start = nullptr;
            current = start + sizeof(void*);
        }

        void* result = current;
        current += size;
        return result;
    }

    template <size_t STACK_BUFF_SIZE>
    [[maybe_unused]] void temp_alloc<STACK_BUFF_SIZE>::deallocate(void*)
    {}

    template <size_t STACK_BUFF_SIZE>
    [[maybe_unused]] size_t temp_alloc<STACK_BUFF_SIZE>::total_allocated()
    {
        return 0;
    }

    template class temp_alloc<64>;
    template class temp_alloc<128>;
    template class temp_alloc<256>;
    template class temp_alloc<512>;
    template class temp_alloc<1024>;
    template class temp_alloc<2048>;
    template class temp_alloc<4096>;
}
