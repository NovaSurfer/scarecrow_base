//
// Created by novasurfer on 11/30/20.
//
// -- Reference --
// GCC: https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/include/std/type_traits
// LLVM: https://github.com/llvm/llvm-project/blob/master/libcxx/include/type_traits
// MICROSOFT: https://github.com/microsoft/STL/blob/master/stl/inc/type_traits

#ifndef SC_TYPETRAITS_H
#define SC_TYPETRAITS_H

namespace sc
{

    /** 
     * Is Trivial
     * Reference: https://clang.llvm.org/docs/LanguageExtensions.html#type-trait-primitives
     * The compiler intrinsic "__is_is_trivial" provide type information that is otherwise unavailable.
     */
    template <typename T>
    inline constexpr bool is_trivial_v = __is_trivial(T);

    /**
     * Determine whether T is a pointer
     */
    template <typename>
    inline constexpr bool is_pointer_v = false; 

    template <typename T>
    inline constexpr bool is_pointer_v<T*> = true;

    template <typename T>
    inline constexpr bool is_pointer_v<T* const> = true;

    template <typename T>
    inline constexpr bool is_pointer_v<T* volatile> = true;

    template <typename T>
    inline constexpr bool is_pointer_v<T* const volatile> = true;

    /**
     * Remove reference
     */
    template <typename T>
    struct remove_reference
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&>
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&&>
    {
        using type = T;
    };

    /* Implementing MOVE and FORWARD
     * Reference: https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h
     * https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c
     */

    /**
     * Converts value to rvalue
     * @return the parameter cast to an rvalue-reference to allow moving it.
     */
    template <class T>
    [[nodiscard]] constexpr typename remove_reference<T>::type&& move(T&& _Arg) noexcept
    {
        return static_cast<typename remove_reference<T>::type&&>(_Arg);
    }

    /**
     * Forward lvalue
     * @return the parameter cast to specified type.
     */
    template <typename T>
    constexpr T&& forward(typename remove_reference<T>::type& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    /**
     * Forward rvalue
     * @return the parameter cast to specified type.
     */
    template <typename T>
    constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    void swap(T& t1, T& t2)
    {
        T temp = move(t1);
        t1 = move(t2);
        t2 = move(temp);
    }

}

#endif //SC_TYPETRAITS_H
