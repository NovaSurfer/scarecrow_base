//
// Created by novasurfer on 11/10/20.
//

#ifndef SC_ALLOCATOR_H
#define SC_ALLOCATOR_H

#include "typeutils.h"
#include <cstddef>
#include <new>

namespace sc
{

    /**
     * Base allocator structure
     */
    struct allocator
    {
        virtual void* allocate(size_t size, size_t align) = 0;
        virtual void deallocate(void* ptr) = 0;
        virtual size_t total_allocated() = 0;
        virtual ~allocator() {};

        template <typename T, typename... TArgs>
        T* make_new(TArgs&&... args);
        template <typename T>
        void make_delete(const T* ptr);

        allocator() = default;
        allocator(const allocator&) = delete;
        allocator(allocator&&) = delete;
        allocator& operator=(const allocator&) = delete;
        allocator& operator=(allocator&&) = delete;
    };

    template <typename T, typename... TArgs>
    T* allocator::make_new(TArgs&&... args)
    {
        return new(allocate(sizeof(T), alignof(T))) T(sc::forward<TArgs>(args)...);
    }

    template <typename T>
    void allocator::make_delete(const T* ptr)
    {
        if(ptr) {
            ptr->~T();
            deallocate(ptr);
        }
    }

}

#endif //SC_ALLOCATOR_H
