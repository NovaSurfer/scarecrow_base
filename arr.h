#ifndef SC_BASE_ARR_H
#define SC_BASE_ARR_H

#include <cstddef>
#include <cstring>

namespace sc
{

    template <typename T, size_t SIZE>
    class arr
    {
    public:
        constexpr size_t size() const
        {
            return SIZE;
        }

        constexpr T& operator[](size_t index);
        constexpr const T& operator[](size_t index) const;
        constexpr void insert_at(const T* item, size_t index, size_t size);
        constexpr const T* get() const;

    private:
        T data[SIZE];
    };

    template <typename T, size_t SIZE>
    constexpr T& arr<T, SIZE>::operator[](size_t index)
    {
        return data[index];
    }

    template <typename T, size_t SIZE>
    constexpr const T& arr<T, SIZE>::operator[](size_t index) const
    {
        return data[index];
    }

    template <typename T, size_t SIZE>
    constexpr void arr<T, SIZE>::insert_at(const T* item, size_t index, size_t size)
    {
        //DBG_CONSTEXPR_FAIL_IF(!IS_T_TRIVIAL::value, "insert_at for non-POD array is not supported");
        memcpy(&data[index], item, sizeof(T) * size);
    }

    template <typename T, size_t SIZE>
    constexpr const T* arr<T, SIZE>::get() const
    {
        return data;
    }

}

#endif //SC_BASE_ARR_H
