#ifndef SC_BASE_UNIQUE_H
#define SC_BASE_UNIQUE_H

#include "smart_ptr.h"
#include "typeutils.h"
#include "compiler.h"

struct allocator;

namespace sc
{

    template <typename T, typename D = default_delete<T>>
    class unique : public smart_ptr<T, D>
    {
        using base = smart_ptr<T, D>;

    public:
        constexpr unique() noexcept = default;

        constexpr unique(nullptr_t) noexcept {};

        constexpr unique(T* ptr)
        {
            base::pair.second() = ptr;
        }

        constexpr unique(T* ptr, D deleter) noexcept
            : base(ptr, sc::move(deleter)) {};

        constexpr unique(unique&&) = default;

        ~unique()
        {
            reset();
        }

        constexpr unique& operator=(unique&& other) noexcept
        {
            reset(other.release());
            base::pair.second() = sc::move(other.pair.second());
        }

        /* Move assignment operator for unique ptrs with different deleters - not implemented
        template< class U, class E >
        unique& operator=( unique<U,E>&& r ) noexcept;
        */

        constexpr void reset(T* ptr = nullptr)
        {
            T* old = base::pair.second();
            base::pair.second() = ptr;
            if(old) {
                // delete
                base::pair.first()(old);
            }
        }

        constexpr T* release()
        {
            T* tmp = smart_ptr<T, D>::pair.second();
            base::pair.second() = nullptr;
            return tmp;
        };
    };

    template <typename T, typename D>
    constexpr sc_forceinline bool operator==(const unique<T, D>& a, const unique<T, D>& b) noexcept
    {
        return a.get() == b.get();
    }

    template <typename T, typename D>
    constexpr sc_forceinline bool operator!=(const unique<T, D>& a, const unique<T, D>& b) noexcept
    {
        return a.get() != b.get();
    }

    template <typename T, typename D>
    constexpr sc_forceinline bool operator==(const unique<T, D>& a, nullptr_t) noexcept
    {
        return a.get() == nullptr;
    }

    template <typename T, typename D>
    constexpr sc_forceinline bool operator!=(const unique<T, D>& a, nullptr_t) noexcept
    {
        return a.get() != nullptr;
    }

    template <typename T, typename D>
    constexpr sc_forceinline bool operator==(nullptr_t, const unique<T, D>& a) noexcept
    {
        return a.get() == nullptr;
    }

    template <typename T, typename D>
    constexpr sc_forceinline bool operator!=(nullptr_t, const unique<T, D>& a) noexcept
    {
        return a.get() != nullptr;
    }
}

#endif //SC_BASE_UNIQUE_H