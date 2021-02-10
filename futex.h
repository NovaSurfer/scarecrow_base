#ifndef SC_BASE_FUTEX_H
#define SC_BASE_FUTEX_H

#include "compiler.h"

#if SC_OS_LINUX
#    include <linux/futex.h>
#    include <syscall.h>
#    include <unistd.h>
#elif SC_OS_WINDOWS
#    include <synchapi.h>
#    include <windows.h>
#endif

template <typename T>
constexpr static T cmp_n_swap(T* ptr, T old, T fresh)
{
#if defined(SC_COMPILER_GCC_OR_CLANG)
    return __sync_val_compare_and_swap(ptr, old, fresh);
#elif defined(SC_COMPILER_MVC)
    return _InterlockedCompareExchange(ptr, old, fresh);
#endif
}

template <typename T>
constexpr static T atomic_decrement(T* val)
{
#if defined(SC_COMPILER_GCC_OR_CLANG)
    return __sync_add_and_fetch(val, -1);
#elif defined(SC_COMPILER_MVC)
    return _InterlockedDecrement(val);
#endif
}

namespace sc
{

    sc_forceinline int futex_wait(void* addr, int block_val)
    {
#if SC_OS_LINUX
        return syscall(SYS_futex, addr, FUTEX_WAIT, block_val, nullptr, nullptr, 0);
#elif SC_OS_WINDOWS
        return WaitOnAddress(addr, &block_val, sizeof(int), INFINITE);
#endif
    }

    sc_forceinline int futex_wake_one(void* addr)
    {
#if SC_OS_LINUX
        return syscall(SYS_futex, addr, FUTEX_WAKE, 1, nullptr, nullptr, 0);
#elif SC_OS_WINDOWS
        WakeByAddressSingle(addr);
        // number of woken waiters
        return 1;
#endif
    }
}

#endif //SC_BASE_FUTEX_H
