#ifndef SC_SMART_PTR_H
#define SC_SMART_PTR_H

#include "typeutils.h"
#include <cstddef>

namespace sc
{

    template <typename T>
    struct default_delete
    {
        constexpr default_delete() noexcept = default;

        /* Constructor from different deleter - unsupported
        template<typename U, typename = enable_if_t<is_convertible_v<U*, T*>>>
        default_delete(const default_delete<U>&) noexcept {}
         */

        void operator()(T* p) const
        {
            delete p;
        }
    };

    template <typename T>
    struct default_delete<T[]>
    {
        constexpr default_delete() noexcept = default;

        void operator()(T* p) const
        {
            delete[] p;
        }
    };

    template <typename T, typename D>
    class smart_ptr
    {
    public:

        constexpr smart_ptr() = default;

        constexpr smart_ptr(T* ptr, D deleter)
            : pair(deleter, ptr)
        { }

        constexpr T* get() const noexcept
        {
            return pair.second();
        }
        constexpr T& operator*() const noexcept
        {
            return *pair.second();
        }
        constexpr T* operator->() const noexcept
        {
            return pair.second();
        }

    protected:
        compressed_pair<D, T*> pair;
    };

    template <typename T, typename D>
    class smart_ptr<T[], D>
    {
    public:
        constexpr smart_ptr(T* ptr, D deleter)
            : pair(deleter, *ptr)
        { }

        constexpr T* get() const noexcept
        {
            return pair.second();
        }
        constexpr T& operator[](size_t i) const noexcept
        {
            return pair.second()[i];
        }

    protected:
        compressed_pair<D, T*> pair;
    };

    /*
    template <>
    struct smart_ptr<void>
    {
        constexpr T* get() const noexcept
        {
            return ptr;
        }

    protected:
        void* ptr;
    };
    */

}

#endif //SC_SMART_PTR_H
