// TODO: remove futex for MacOS build, cuz it's not supported on MacOS.
#if SC_OS_LINUX || SC_OS_WINDOWS
#include "mutex.h"
#include "futex.h"
#include "atomic_utils.h"

namespace sc
{
    mutex::mutex()
        : val(0)
    {}

    void mutex::lock()
    {
        mtx_val_type c;

        if((c = cmp_n_swap(&val, UNLOCKED, LOCKED)) != UNLOCKED)
            do {
                if(c == LOCKED_WITH_WAITERS ||
                   cmp_n_swap(&val, LOCKED, LOCKED_WITH_WAITERS) != UNLOCKED) {
                    futex_wait(&val, LOCKED_WITH_WAITERS);
                }
            } while((c = cmp_n_swap(&val, UNLOCKED, LOCKED_WITH_WAITERS)) != UNLOCKED);
    }

    void mutex::unlock()
    {
        if(atomic_decrement(&val) != 1) {
            val = 0;
            futex_wake_one(&val);
        }
    }
}
#endif