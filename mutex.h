#ifndef SC_BASE_MUTEX_H
#define SC_BASE_MUTEX_H

#include "compiler.h"
#ifdef SC_COMPILER_MVC
#    include <windows.h>
#endif

namespace sc
{

    class mutex
    {
#ifdef SC_COMPILER_MVC
        using mtx_val_type = LONG;
#elif defined(SC_COMPILER_GCC) || defined(SC_COMPILER_CLANG)
        using mtx_val_type = int;
#endif
        constexpr static mtx_val_type UNLOCKED = 0;
        constexpr static mtx_val_type LOCKED = 1;
        constexpr static mtx_val_type LOCKED_WITH_WAITERS = 2;

    public:
        mutex();
        mutex& operator=(const mutex&) = delete;
        mutex& operator=(mutex&&) = delete;
        void lock();
        void unlock();

    private:
        mtx_val_type val;
    };
}

#endif //SC_BASE_MUTEX_H
