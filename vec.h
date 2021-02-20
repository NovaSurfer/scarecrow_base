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
        constexpr vec() = delete;
        constexpr vec(allocator& alc);
        constexpr explicit vec(allocator& alc, size_t len);
        constexpr vec(allocator& alc, size_t len, const T& item);
        constexpr vec(const vec& other);
        constexpr vec(vec&& other) noexcept;
        ~vec();

        // TODO:
        constexpr vec<T>& operator=(const vec<T>& v);
        constexpr vec<T>& operator=(vec<T>&& v);

        void reserve(size_t capacity);
        void resize(size_t new_size);
        void shrink_to_fit();
        void pop_back();
        void clear();

        void push(const T& item);
        template <typename... Args>
        void emplace(Args&&... args);

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
        size_t sz;
        size_t space;

        constexpr static size_t GROWTH_FACTOR = 2;
        constexpr static size_t MIN_CAPACITY = 8;
    };

    template <typename T>
    constexpr vec<T>::vec(allocator& alc)
        : data(nullptr)
        , alloc(&alc)
        , sz(0)
        , space(0)
    {}

    template <typename T>
    constexpr vec<T>::vec(allocator& alc, size_t len)
        : alloc(&alc)
        , sz(len)
        , space(len)
    {
        data = static_cast<T*>(alloc->allocate(sizeof(T) * space, alignof(T)));
        if constexpr(!is_trivial_v<T>) {
            for(size_t i = 0; i < space; ++i) {
                new(data + i) T();
            }
        }
    }

    template <typename T>
    constexpr vec<T>::vec(allocator& alc, size_t len, const T& item)
        : alloc(&alc)
        , sz(len)
        , space(len)
    {
        data = static_cast<T*>(alloc->allocate(sizeof(T) * space, alignof(T)));
        for(size_t i = 0; i < space; ++i) {
            new(data + i) T(item);
        }
    }

    template <typename T>
    constexpr vec<T>::vec(const vec& other)
        : alloc(other.alloc)
        , sz(other.sz)
        , space(other.space)
    {
        data = static_cast<T*>(alloc->allocate(sizeof(T) * space, alignof(T)));
        for(size_t i = 0; i < sz; ++i) {
            new(data + i) T(other.data[i]);
        }
    }

    template <typename T>
    constexpr vec<T>::vec(vec&& other) noexcept
        : alloc(move(other.alloc))
        , sz(move(other.sz))
        , space(move(other.space))
        , data(move(other.data))
    {}

    template <typename T>
    vec<T>::~vec()
    {
        clear();
    }

    template <typename T>
    constexpr vec<T>& vec<T>::operator=(const vec<T>& o)
    {
        if(this != &o) {
            // if alloc differs
            clear();
            if(alloc != o.alloc) {
                alloc = o.alloc;
            }
            data = static_cast<T*>(alloc->allocate(sizeof(T) * o.space, alignof(T)));
            for(size_t i = 0; i < o.sz; ++i) {
                data[i] = o.data[i];
            }
            space = o.space;
            sz = o.sz;
        }

        return *this;
    }

    template <typename T>
    constexpr vec<T>& vec<T>::operator=(vec<T>&& o)
    {
        clear();
        space = o.space;
        sz = o.sz;
        alloc = o.alloc;
        data = o.data;
        o.data = nullptr;
        o.sz = o.space = 0;

        return *this;
    }

    template <typename T>
    void vec<T>::reserve(size_t capacity)
    {
        if(capacity > space) {

            T* new_data = static_cast<T*>(alloc->allocate(sizeof(T) * capacity, alignof(T)));

            if(data) {
                for(size_t i = 0; i < sz; ++i) {
                    new(new_data + i) T(data[i]);
                }

                if constexpr(!is_trivial_v<T>) {
                    for(size_t i = 0; i < sz; ++i) {
                        data[i].~T();
                    }
                }

                alloc->deallocate(data);
            }

            data = new_data;
            space = capacity;
        }
    }

    template <typename T>
    void vec<T>::resize(size_t new_size)
    {
        if(new_size > sz) {
            reserve(new_size);
            if constexpr(!is_trivial_v<T>) {
                for(size_t i = sz; i < new_size; ++i) {
                    new(data + i) T();
                }
            }

        } else {
            if constexpr(!is_trivial_v<T>) {
                for(size_t i = new_size; i < sz; ++i) {
                    data[i].~T();
                }
            }
        }

        sz = new_size;
    }

    template <typename T>
    void vec<T>::shrink_to_fit()
    {
        if(space > sz) {
            T* new_data = (T*)alloc->allocate(sizeof(T) * sz, alignof(T));

            if constexpr(!is_trivial_v<T>) {
                for(size_t i = 0; i < sz; ++i) {
                    new(new_data + i) T(data[i]);
                }

                for(size_t i = 0; i < sz; ++i) {
                    data[i].~T();
                }
            } else {
                for(size_t i = 0; i < sz; ++i) {
                    new_data[i] = data[i];
                }
            }

            alloc->deallocate(data);

            data = new_data;
            space = sz;
        }
    }

    template <typename T>
    void vec<T>::pop_back()
    {
        --sz;
        if constexpr(!is_trivial_v<T>) {
            data[sz].~T();
        }
    }

    template <typename T>
    void vec<T>::clear()
    {
        if constexpr(!is_trivial_v<T>) {
            for(size_t i = 0; i < sz; ++i) {
                data[i].~T();
            }
        }

        alloc->deallocate(data);
        sz = space = 0;
    }

    template <typename T>
    void vec<T>::push(const T& item)
    {
        if(!space) {
            // TODO: test that branch
            reserve(vec::MIN_CAPACITY);
        } else if(sz == space) {
            reserve(space * vec::GROWTH_FACTOR);
        }

        data[sz] = item;
        ++sz;
    }

    template <typename T>
    template <typename... Args>
    void vec<T>::emplace(Args&&... args)
    {
        if(!space) {
            reserve(vec::MIN_CAPACITY);
        } else if(sz == space) {
            reserve(space * vec::GROWTH_FACTOR);
        }

        data[sz] = T(forward<Args>(args)...);
        ++sz;
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
        return sz;
    }

    template <typename T>
    constexpr size_t vec<T>::capacity() const
    {
        return space;
    }

    template <typename T>
    constexpr bool vec<T>::empty() const
    {
        sz == 0;
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
