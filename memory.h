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

namespace sc
{ 
}

#endif //S_MEMORY_H
