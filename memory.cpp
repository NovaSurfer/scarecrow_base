//
// Created by novasurfer on 10/21/20.
//

#if defined(__GNUC__)
#    define SC_COMPILER_GCC 1
#endif

#if defined(__clang__)
#    define SC_COMPILER_CLANG 1
#endif

#if defined(_MSC_VER)
#    define SC_COMPILER_MVC 1
#endif

#ifdef SC_COMPILER_MVC
#    define sc_forceinline __forceinline
#elif defined(SC_COMPILER_GCC)
#    define sc_forceinline inline __attribute__((__always_inline__))
#elif defined(SC_COMPILER_CLANG)
#    if __has_attribute(__always_inline__)
#        define forceinline inline __attribute__((__always_inline__))
#    else
#        define sc_forceinline inline
#    endif
#else
#    define sc_forceinline inline
#endif

#if SC_COMPILER_GCC || SC_COMPILER_CLANG
#    include <cstdlib>
#    define malloc_aligned(bytes, alignment) aligned_alloc(alignment, bytes)
#    define free_aligned(ptr) free(ptr)
#elif SC_COMPILER_MVC
#    include <malloc.h>
#    define malloc_aligned(bytes, alignment) _aligned_malloc(bytes, alignment)
#    define free_aligned(ptr) _aligned_free(ptr)
#    define realloc_aligned(ptr, bytes, alignment) _aligned_realloc(ptr, bytes, alignment)
#endif

#include "memory.h"
#include <cstdint>
#include <new>

namespace sc
{

#if defined(SC_ALLOC_WITH_HEADER) /* ------------------------------------------ */

    memhead_type mem::HEADER_FLAGS;
    size_t mem::HEADER_SIZE;

    using memhead_size_type = size_t;
    using memhead_align_type = size_t;

    constexpr void construct_header(void* allocated_raw_buffer, size_t size, size_t align,
                                    memhead_type header_flags)
    {
        // just size
        if((header_flags & (memhead_type::SIZE)) == memhead_type::SIZE) {
            new(allocated_raw_buffer) size_t {size};
        }

        // size & alignment
        if((header_flags & (memhead_type::SIZE | memhead_type::ALIGN)) ==
           (memhead_type::SIZE | memhead_type::ALIGN)) {
            new(allocated_raw_buffer) size_t {size};
            void* align_buff = static_cast<size_t*>(allocated_raw_buffer) + 1;
            new(align_buff) size_t {align};
        }
    }

    constexpr size_t total_mem_header_size(memhead_type header_flags)
    {
        // just size
        if((header_flags & (memhead_type::SIZE)) == memhead_type::SIZE)
            return sizeof(memhead_size_type);

        // size & alignment
        if((header_flags & (memhead_type::SIZE | memhead_type::ALIGN)) ==
           (memhead_type::SIZE | memhead_type::ALIGN))
            return sizeof(memhead_size_type) + sizeof(memhead_align_type);

        return 0;
    }

    constexpr sc_forceinline void* user_address(void* header_ptr, size_t header_size)
    {
        uintptr_t* p_header = static_cast<uintptr_t*>(header_ptr);
        uintptr_t* p_user = p_header + header_size;
        return static_cast<void*>(p_user);
    }

    constexpr sc_forceinline void* header_address(void* user_ptr, size_t header_size)
    {
        uintptr_t* p_user = static_cast<uintptr_t*>(user_ptr);
        uintptr_t* p_header = p_user - header_size;
        return static_cast<void*>(p_header);
    }

    void* mem::alloc_aligned(size_t size, size_t align)
    {
        void* p_header_raw = malloc_aligned(mem::HEADER_SIZE + size, align);

        // construct mem_header
        construct_header(p_header_raw, size, align, mem::HEADER_FLAGS);

        // get & return user memory
        void* p_user = user_address(p_header_raw, mem::HEADER_SIZE);
        return p_user;
    }

    void mem::dealloc_aligned(void* ptr)
    {
        if(!ptr)
            return;

        void* p_header = header_address(ptr, mem::HEADER_SIZE);
        free_aligned(p_header);
    }

    void* mem::alloc(size_t size)
    {
        void* p_header_raw = malloc(mem::HEADER_SIZE + size);

        // construct mem_header
        construct_header(p_header_raw, size, 0, mem::HEADER_FLAGS);

        // get & return user memory
        void* p_user = user_address(p_header_raw, mem::HEADER_SIZE);
        return p_user;
    }

    void mem::dealloc(void* ptr)
    {
        if(!ptr)
            return;

        void* p_header = header_address(ptr, mem::HEADER_SIZE);
        free(p_header);
    }

    size_t mem::allocated_size(void* ptr)
    {
        if(!ptr)
            return 0;

        size_t size = *static_cast<size_t*>(header_address(ptr, mem::HEADER_SIZE));
        return size;
    }

    size_t mem::allocated_alignment(void* ptr)
    {
        if(!ptr)
            return 0;

        void* align_size_address = static_cast<size_t*>(header_address(ptr, mem::HEADER_SIZE)) + 1;
        size_t align_size = *static_cast<size_t*>(align_size_address);
        return align_size;
    }

    void mem::set_header(memhead_type header_flags)
    {
        HEADER_FLAGS = header_flags;
        HEADER_SIZE = total_mem_header_size(HEADER_FLAGS);
    }

#else /* ------------------------------------------------------------- */

    void* mem::alloc_aligned(size_t size, size_t align)
    {
        return malloc_aligned(size, align);
    }

    void mem::dealloc_aligned(void* ptr)
    {
        free_aligned(ptr);
    }

    void* mem::alloc(size_t size)
    {
        return malloc(size);
    }

    void mem::dealloc(void* ptr)
    {
        free(ptr);
    }

#endif

}

#if defined(SC_ALLOC_WITH_HEADER)
void* operator new(size_t size)
{
    return sc::mem::alloc(size);
}

void* operator new[](size_t size)
{
    return sc::mem::alloc(size);
}

void operator delete(void* ptr) noexcept
{
    sc::mem::dealloc(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] size_t size) noexcept
{
    operator delete(ptr);
}

void operator delete[](void* ptr) noexcept
{
    sc::mem::dealloc(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] size_t size) noexcept
{
    operator delete(ptr);
}

#endif
