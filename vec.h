//
// Created by novasurfer on 11/26/20.
//

#ifndef SC_VEC_H
#define SC_VEC_H

#include "typeutils.h"
#include <cstddef>

struct allocator;

namespace sc
{
    template <typename T>
    class vec
    {
    public:
        constexpr vec(allocator* alc);
        constexpr explicit vec(allocator* alc, size_t len);
        constexpr vec(size_t len, const T& item);
        constexpr vec(const vec& other);
        constexpr vec(vec&& other) noexcept;
        ~vec();

        constexpr vec<T>& operator=(const vec<T>& v);
        constexpr vec<T>& operator=(vec<T>&& v);

        void reserve(size_t capacity);
        void resize(size_t new_size);
        void shrink_to_fit();

        void push(const T& item);
        template <typename... Args>
        void emplace(Args&... args);

        constexpr T* raw() noexcept;
        constexpr const T* raw() const noexcept;

        constexpr size_t size() const;
        constexpr size_t capacity() const;
        constexpr bool empty() const;

        T& operator[](size_t index);
        const T& operator[](size_t index) const;

    private:
        T* data;
        allocator* alloc;
        size_t length;
        size_t space;
    };

    template <typename T>
    constexpr vec<T>::vec(allocator* alc)
        : alloc(alc)
        , length(2)
        , space(2)
    {
        data = static_cast<T*>(alloc->allocate(sizeof(T) * 2, alignof(T)));
    }
    
    template <typename T>
    constexpr vec<T>::vec(allocator* alc, size_t len)
        : length(len)
        , space(len)
    {
        data = static_cast<T*>(alloc->allocate(sizeof(T) * length, alignof(T)));
    }

    template <typename T>
    constexpr vec<T>::vec(const vec& other)
	: alloc(other.alloc)
        , length(other.length)
        , space(other.space)
    {
        data = static_cast<T*>(alloc->allocate(sizeof(T) * length, alignof(T)));
    }

    template <typename T>
    constexpr vec<T>::vec(vec&& other) noexcept
	: alloc(other.alloc)
        , length(other.length)
        , space(other.space)
    {
        data = move(other);
    }

    template <typename T>
    vec<T>::~vec()
    {
        if constexpr(!is_trivial_v<T>) {
            for(size_t i = 0; i < length; ++i) {
                data[i]->~T();
            }
        }

        alloc->deallocate(data);
        data = nullptr;
        length = 0;
        space = 0;
    }

    template <typename T>
    constexpr vec<T>& vec<T>::operator=(const vec<T>& v)
    {
        // check for self-assignment?
        alloc = v.alloc;

        if(v.length > space) {
            ~vec();
            data = static_cast<T*>(alloc->allocate(sizeof(T) * v.length, alignof(T)));
            space = v.length;
        }

        length = v.length;

        for(size_t i = 0; i < v.length; ++i) {
            data[i] = v[i];
        }

        return *this;
    }

    template <typename T>
    constexpr vec<T>& vec<T>::operator=(vec<T>&& v)
    {
        // check for self-assignment?
        alloc = v.alloc;

        if(v.length > space) {
            ~vec();
            data = static_cast<T*>(alloc->allocate(sizeof(T) * v.length, alignof(T)));
            space = v.length;
        }

        length = v.length;

        for(size_t i = 0; i < v.length; ++i) {
            data[i] = move(v[i]);
        }

        return *this;
    }

    template <typename T>
    void vec<T>::reserve(size_t capacity)
    {
        if(capacity > space) {
            T* new_data = (T*)alloc->allocate(sizeof(T) * capacity, alignof(T));

            for(size_t i = 0; i < length; ++i) {
                new_data[i] = data[i];
            }

            if constexpr(!is_trivial_v<T>) {
                for(size_t i = 0; i < length; ++i) {
                    data[i]->~T();
                }
            }

            alloc->deallocate(data);

            data = new_data;
            space = capacity;
        }
    }

    template <typename T>
    void vec<T>::resize(size_t new_size)
    {
        if(new_size > length) {
            if(new_size > space) {
                reserve(new_size);
            }

            if constexpr(!is_trivial_v<T>) {
                for(size_t i = length; i != new_size; ++i) {
                    new(&data[i]) T();
                }
            }

            length = new_size;

        } else if(length > new_size) {
            // if T is non-POD
            if constexpr(!is_trivial_v<T>) {
                for(size_t i = length; i != new_size; --i) {
                    data[i]->~T();
                }
            }

            length = new_size;
        }
    }

    template <typename T>
    void vec<T>::shrink_to_fit()
    {
        if(space > length) {
            T* new_data = (T*)alloc->allocate(sizeof(T) * length, alignof(T));

            for(size_t i = 0; i < length; ++i) {
                new_data[i] = data[i];
            }

            if constexpr(!is_trivial_v<T>) {
                for(size_t i = 0; i < length; ++i) {
                    data[i]->~T();
                }
            }

            alloc->deallocate(data);

            data = new_data;
            space = length;
        }
    }

    template <typename T>
    void vec<T>::push(const T& item)
    {
        if(!space) {
            reserve(8);
        } else if(length == space) {
            reserve(space * 2);
        }

        data[length] = item;
        ++length;
    }

    template <typename T>
    template <typename... Args>
    void vec<T>::emplace(Args&... args)
    {
        if(!space) {
            reserve(8);
        } else if(length == space) {
            reserve(space * 2);
        }

        data[length] = T(forward<Args>(args)...);
        ++length;
    }

    template <typename T>
    constexpr T* vec<T>::raw() noexcept
    {
        return data;
    }

    template <typename T>
    constexpr const T* vec<T>::raw() const noexcept
    {
        return data;
    }

    template <typename T>
    constexpr size_t vec<T>::size() const
    {
        return length;
    }

    template <typename T>
    constexpr size_t vec<T>::capacity() const
    {
        return space;
    }

    template <typename T>
    constexpr bool vec<T>::empty() const
    {
        length == 0;
    }

    template <typename T>
    T& vec<T>::operator[](size_t index)
    {
        return data[index];
    }

    template <typename T>
    const T& vec<T>::operator[](size_t index) const
    {
        return data[index];
    }

}

#endif //SC_VEC_H
