//
// Created by novasurfer on 11/26/20.
//

#ifndef SC_VEC_H
#define SC_VEC_H

#include <cstddef>

struct allocator;

namespace sc
{
    template <typename T, class Allocator = allocator>
    class vec
    {
    public:
        constexpr vec() noexcept;
        constexpr explicit vec(size_t len);
        constexpr vec(size_t len, const T& item);
        constexpr vec(const vec& other);
        constexpr vec(vec&& other) noexcept;
        ~vec();

        constexpr vec<T, Allocator>& operator=(const vec<T, Allocator>& v);
        constexpr vec<T, Allocator>& operator=(vec<T, Allocator>&& v);

        void reserve(size_t capacity);
        void resize(size_t new_size);
        void shrink_to_fit();

        void push(const T& item);
        template <typename... Args>
        void emplace(Args&... args);

        T* raw() noexcept;
        const T* raw() const noexcept;

        void clear() noexcept;

	constexpr size_t size() const;
	constexpr size_t capacity() const;
	constexpr bool empty() const;

        T& operator[](size_t index);
        const T& operator[](size_t index) const;

    private:
        T* data;
        Allocator* alloc;
        size_t length;
	size_t space;
    };

    template <typename T, typename Allocator>
    constexpr vec<T, Allocator>::vec() noexcept
        : length(2)
	, space(2)
    {
        data = static_cast<T*>(alloc.allocate(sizeof(T) * 2, alignof(T)));
    }

    template <typename T, typename Allocator>
    constexpr vec<T, Allocator>::vec(size_t len)
        : length(len)
	, space(len)
    {
        data = static_cast<T*>(alloc.allocate(sizeof(T) * length, alignof(T)));
    }

    template <typename T, typename Allocator>
    constexpr vec<T, Allocator>::vec(const vec& other)
        : length(other.length)
	, space(other.space)
    {
        data = static_cast<T*>(alloc.allocate(sizeof(T) * length, alignof(T)));
    }

    template <typename T, typename Allocator>
    constexpr vec<T, Allocator>::vec(vec&& other) noexcept
	: length(other.length)
	, space(other.space)  
    {
        // std::move
    }

    template <typename T, typename Allocator>
    vec<T, Allocator>::~vec()
    {
        alloc.deallocate(data);
        data = nullptr;
        length = 0;
	space = 0;
    }

    template <typename T, typename Allocator>
    constexpr vec<T, Allocator>& vec<T, Allocator>::operator=(const vec<T, Allocator>& v)
    {
	// check for self-assignment?
	alloc = v.alloc;

	if(v.length > space){
	    ~vec();
	    data = static_cast<T*>(alloc.allocate(sizeof(T) * v.length, alignof(T)));
	    space = v.length;
	}
	
	length = v.length;
	
	for(size_t i = 0; i < v.length; ++i){
	    data[i] = v[i];
	}
	
	return *this;
    }

    template <typename T, typename Allocator>
    constexpr vec<T, Allocator>& vec<T, Allocator>::operator=(vec<T, Allocator>&& v)
    {
		// check for self-assignment?
	alloc = v.alloc;

	if(v.length > space){
	    ~vec();
	    data = static_cast<T*>(alloc.allocate(sizeof(T) * v.length, alignof(T)));
	    space = v.length;
	}
	
	length = v.length;
	
	for(size_t i = 0; i < v.length; ++i){
	    // data[i] = v[i];
	    // move
	}
	
	return *this;

    }


    template <typename T, typename Allocator>
    void vec<T, Allocator>::reserve(size_t capacity)
    {
	if(capacity > space) {
	    T* new_data = (T*)alloc.allocate(sizeof(T) * capacity, alignof(T));

	    for(size_t i = 0; i < length; ++i) {
		new_data[i] = data[i];
	    }

	    alloc.deallocate(data);

	    data = new_data;
	    space = capacity;
	}
    }

    template <typename T, typename Allocator>
    void vec<T, Allocator>::resize(size_t new_size)
    {
	if(new_size > length) {
	    if(new_size > space) {
		reserve(new_size);
	    }

	    // if T is non-POD
	    /*
	    for(size_t i = length; i != new_size; ++i) {
		new(&data[i]) T();
	    }
	    */

	    length = new_size;

	} else if(length > new_size) {
	    // if T is non-POD
        /*
	    for(size_t i = length; i != new_size; --i) {
		data[i]->~T();
	    }
	*/
	    length = new_size;
	}

    }

    template <typename T, typename Allocator>
    void vec<T, Allocator>::shrink_to_fit()
    {
	if(space > length)
	{
	    T* new_data = (T*)alloc.allocate(sizeof(T) * length, alignof(T));

	    for(size_t i = 0; i < length; ++i) {
		new_data[i] = data[i];
	    }

	    alloc.deallocate(data);

	    data = new_data;
	    space = length;

	}
    }


}

#endif //SC_VEC_H
