#ifndef SC_BASE_QUEUE_H
#define SC_BASE_QUEUE_H

#include "allocator.h"
#include "vec.h"

namespace sc
{
    template <typename T>
    class queue
    {
    public:
        constexpr queue(allocator& alloc);
        constexpr void push(const T& val);
        constexpr T pop();
        constexpr size_t len() const;

    private:
        vec<T> arr;
        size_t out;
    };

    template <typename T>
    constexpr queue<T>::queue(allocator& alloc)
        : arr(alloc)
        , out(0)
    {}

    template <typename T>
    constexpr void queue<T>::push(const T& val)
    {
        arr.push(val);
    }

    template <typename T>
    constexpr T queue<T>::pop()
    {
        T val = arr[out];
        out = (out + 1) % arr.size();
        arr.pop_back();
        return val;
    }

    template <typename T>
    constexpr size_t queue<T>::len() const
    {
        return arr.size();
    }

}

#endif //SC_BASE_QUEUE_H
