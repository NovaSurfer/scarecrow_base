//
// Created by novasurfer on 9/3/21.
//

#ifndef SC_BASE_TEST_ATOMIC_UTILS_H
#define SC_BASE_TEST_ATOMIC_UTILS_H

#include "compiler.h"

#if SC_OS_WINDOWS
#    include <windows.h>
#endif

template <typename T>
constexpr static T atomic_decrement(T* val)
{
#if defined(SC_COMPILER_GCC_OR_CLANG)
    return __sync_sub_and_fetch(val, 1);
#elif defined(SC_COMPILER_MVC)
    return _InterlockedDecrement(val);
#endif
}

template <typename T>
constexpr static T atomic_increment(T* val)
{
#if defined(SC_COMPILER_GCC_OR_CLANG)
    return __sync_add_and_fetch(val, 1);
#elif defined(SC_COMPILER_MVC)
    return _InterlockedIncrement(val);
#endif
}

#endif //SC_BASE_TEST_ATOMIC_UTILS_H
