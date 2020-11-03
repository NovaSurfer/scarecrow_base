//
// Created by novasurfer on 10/21/20.
//

#ifndef SC_MEMORY_H
#define SC_MEMORY_H

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

#ifdef SC_ALLOC_WITH_HEADER
#    define sc_alloc_aligned(size, align) sc::mem<SC_ALLOC_WITH_HEADER>::alloc_aligned(size, align);
#    define sc_allocated_size(ptr) sc::mem<SC_ALLOC_WITH_HEADER>::allocated_size(ptr)
#    define sc_allocated_alignment(ptr) sc::mem<SC_ALLOC_WITH_HEADER>::allocated_align(ptr)
#    define sc_dealloc_aligned(ptr) sc::mem<SC_ALLOC_WITH_HEADER>::dealloc_aligned(ptr);

#else
#    define sc_alloc_aligned(size, align) sc::mem::alloc_aligned(size, align)
#    define sc_dealloc_aligned(ptr) sc::mem::dealloc_aligned(ptr);
#    endif


namespace sc
{

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
    };


}


#endif //SC_MEMORY_H
