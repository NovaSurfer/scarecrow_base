//
// Created by novasurfer on 7/25/21.
//

#ifndef SC_BASE_SHARED_H
#define SC_BASE_SHARED_H

#include "atomic_utils.h"
#include "compiler.h"
#include "smart_ptr.h"
#include "typeutils.h"
#include <cstdint>

namespace sc
{

    enum class CONTROL_BLOCK_TYPE
    {
        simple,
        atomic
    };

    struct atomic_counter
    {
        sc_forceinline constexpr void inc()
        {
            atomic_increment(&counter);
        }

        sc_forceinline constexpr uint32_t dec()
        {
            atomic_decrement(&counter);
            return counter;
        }

        sc_forceinline constexpr uint32_t value() const
        {
            return counter;
        }

    private:
        uint32_t counter;
    };

    struct shared_control_block
    {
        atomic_counter uses;
        atomic_counter weaks;
    };

    struct simple_counter
    {
        sc_forceinline constexpr void inc()
        {
            ++counter;
        }

        sc_forceinline constexpr void dec()
        {
            --counter;
        }

    private:
        uint32_t counter;
    };

    struct simple_shared_control_block
    {
        simple_counter uses;
        simple_counter weaks;
    };

    // clang-format off
    template <CONTROL_BLOCK_TYPE BLOCK_T>
    using get_control_block = typename sc::conditional<
                                        BLOCK_T == CONTROL_BLOCK_TYPE::simple,
                                        simple_shared_control_block,
                                        shared_control_block
                                        >::type;
    // clang-format on

    template <typename T, CONTROL_BLOCK_TYPE BLOCK_T = CONTROL_BLOCK_TYPE::simple,
              typename D = default_delete<T>>
    class shared : public smart_ptr<T, D>
    {
        using base = smart_ptr<T, D>;

    public:
        constexpr shared()
            : base(nullptr, nullptr)
            , control_block(nullptr)
        { }
        constexpr shared(nullptr_t) noexcept { }

        template <typename U>
        constexpr shared(U* ptr)
        {
            base::pair.second() = ptr;
        }

        template <typename U>
        constexpr shared(const shared<U>& rhs, T* ptr) noexcept
            : control_block(rhs.control_block)
        {
            base::pair.second() = ptr;
            increment_use_count();
        }

        constexpr shared(const shared& rhs) noexcept
            : shared(rhs, rhs.get())
        { }
        template <typename Y>
        constexpr shared(const shared<Y>& rhs) noexcept
            : shared(rhs, rhs.get())
        { }

        ~shared()
        {
            decrement_use_count();
        }

    private:
        get_control_block<BLOCK_T>* control_block;

        constexpr void increment_use_count()
        {
            if(control_block) {
                control_block->uses.inc();
            }
        }

        constexpr void decrement_use_count()
        {
            if(control_block) {
                uint32_t new_use_count = control_block->uses.dec();
                if(new_use_count == 0) {
                    // control_block->delete_controlled_object
                    T* ptr = base::pair.second();
                    base::pair.first()(ptr);

                    uint32_t new_weak_count = control_block->weaks.dec();
                    if(new_weak_count == 0) {
                        delete control_block;
                    }
                }
            }
        }
    };
}

#endif //SC_BASE_SHARED_H
