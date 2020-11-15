//
// Created by novasurfer on 10/21/20.
//

#ifndef SC_MEMORY_H
#define SC_MEMORY_H

#include <cstddef>

namespace sc
{

    // TODO: Implement type name info.
    
#if defined(SC_ALLOC_WITH_HEADER)

    enum class memhead_type : unsigned
    {
	NONE = 0x00,
        SIZE = 0x01,
        ALIGN = 0x02,
        NAME = 0x04
    };

    constexpr memhead_type operator&(memhead_type lhs, memhead_type rhs)
    {
        return static_cast<memhead_type>(static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs));
    }

    constexpr memhead_type operator|(memhead_type lhs, memhead_type rhs)
    {
        return static_cast<memhead_type>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs));
    }

#endif

    struct mem
    {
	static void* alloc(size_t size);
	static void dealloc(void* ptr);
        static void* alloc_aligned(size_t size, size_t align);
        static void dealloc_aligned(void* ptr);
#if defined(SC_ALLOC_WITH_HEADER)
        static size_t allocated_size(void* ptr);
        static size_t allocated_alignment(void* ptr);
	static void set_header(memhead_type header_flags);
    private:
	static memhead_type HEADER_FLAGS;
	static size_t HEADER_SIZE;
#endif
	mem() = delete;
	~mem() = delete;
	mem(const mem&) = delete;
	mem(mem&&) = delete;
	mem& operator=(const mem&) = delete;
	mem& operator=(mem&&) = delete;
    };
}

#if defined(SC_ALLOC_WITH_HEADER)
void* operator new(size_t size);
void* operator new[] (size_t size);
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, size_t size) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, size_t size) noexcept;
#endif

#endif //SC_MEMORY_H
