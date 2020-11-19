//
// Created by novasurfer on 11/15/20.
//

#ifndef SC_TEMPALLOC_H
#define SC_TEMPALLOC_H

#include "allocator.h"

namespace sc
{
    template<size_t STACK_BUFF_SIZE>
    struct temp_alloc : allocator
    {
	temp_alloc(allocator& default_alloc);
	~temp_alloc();
	void* allocate(size_t size, size_t align) override;
        void deallocate(void* ptr) override;
        size_t total_allocated() override;

    private:
	char stackbuff[STACK_BUFF_SIZE];
	allocator* alloc;
	char* start;
	char* current;
	char* end;
    };

    using temp_alloc64 = temp_alloc<64>;
    using temp_alloc128 = temp_alloc<128>;
    using temp_alloc256 = temp_alloc<256>;
    using temp_alloc512 = temp_alloc<512>;
    using temp_alloc1024 = temp_alloc<1024>;
    using temp_alloc2048 = temp_alloc<2048>;
    using temp_alloc4096 = temp_alloc<4096>;
}

#endif //SC_TEMPALLOC_H
