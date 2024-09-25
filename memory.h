//
// Created by novasurfer on 10/21/20.
//

#ifndef SC_MEMORY_H
#define SC_MEMORY_H

#include "compiler.h"
#include "sc_types.h"
#include <cstddef>

namespace sc
{

    // TODO: Implement type name info.

#if defined(SC_ALLOC_WITH_HEADER)

    /**
     * Memory header types
     */
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

#if defined(SC_x64)
    static constexpr const size_t DEFAULT_ALIGNMENT = 16U;
#elif defined(SC_x32)
    static constexpr const size_t DEFAULT_ALIGNMENT = 8U;
#endif

    /**
     * Main memory allocation structure.
     * Can be used for tracking memory size & alignment when SC_ALLOC_WITH_HEADER is defined.
     */
    struct mem
    {
        /**
         * Allocates @param size bytes of uninitialized storage.
         */
        static void* alloc(size_t size);

        /**
         * Deallocates the space previously allocated by alloc().
         */
        static void dealloc(void* ptr);

        /**
         * Allocate size bytes of uninitialized storage whose alignment is specified by alignment.
         * The size parameter must be an integral multiple of alignment.
         */
        static void* alloc_aligned(size_t size, size_t align = DEFAULT_ALIGNMENT);

        /**
         * Deallocates the space previously allocated by alloc_aligned().
         */
        static void dealloc_aligned(void* ptr);

        static constexpr bool is_power_of_two(uintptr val)
        {
            return (val & (val - 1)) == 0;
        }

        static uintptr align_forward(uintptr ptr, size_t align);

#if defined(SC_ALLOC_WITH_HEADER)
        /**
         * Sets memory header flags.
         * Can be read with allocated_size(), allocated_alignment().
         */
        static void set_header(memhead_type header_flags);

        /**
         * Returns the previously allocated size by alloc() or alloc_aligned().
         */
        static size_t allocated_size(void* ptr);

        /**
         * Returns the alignment of the allocation made by alloc_aligned().
         */
        static size_t allocated_alignment(void* ptr);

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
} // namespace sc

#if defined(SC_ALLOC_WITH_HEADER)
void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, size_t size) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, size_t size) noexcept;
#endif

#endif // SC_MEMORY_H
