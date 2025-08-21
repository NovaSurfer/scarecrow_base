//
// Created by novasurfer on 11/30/20.
//
// -- Reference --
// GCC: https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/include/std/type_traits
// LLVM: https://github.com/llvm/llvm-project/blob/master/libcxx/include/type_traits
// MICROSOFT: https://github.com/microsoft/STL/blob/master/stl/inc/type_traits

#ifndef SC_TYPETRAITS_H
#define SC_TYPETRAITS_H

#include "compiler.h"
#include "sc_types.h"

namespace sc
{

    using nullptr_t = decltype(nullptr);

    /**
     * Is Trivial
     * Reference: https://clang.llvm.org/docs/LanguageExtensions.html#type-trait-primitives
     * The compiler intrinsic "__is_is_trivial" provide type information that is otherwise
     * unavailable.
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

    template <bool B, typename T, typename F>
    struct conditional
    {
        typedef T type;
    };

    template <typename T, typename F>
    struct conditional<false, T, F>
    {
        typedef F type;
    };

    /**
     * Remove const volatile from type
     */
    template <typename T>
    struct remove_const_volatile
    {
        using type = T;
    };

    template <typename T>
    struct remove_const_volatile<const T>
    {
        using type = T;
    };

    template <typename T>
    struct remove_const_volatile<volatile T>
    {
        using type = T;
    };

    template <typename T>
    struct remove_const_volatile<const volatile T>
    {
        using type = T;
    };

    template <typename T>
    using remove_const_volatile_t = typename remove_const_volatile<T>::type;

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

    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;

#ifdef SC_COMPILER_GCC_OR_CLANG
    template <typename T1, typename T2>
    constexpr bool is_same_v = __is_same_as(T1, T2);
#else
    template <typename T1, typename T2>
    constexpr bool is_same_v = false;

    template <typename T>
    constexpr bool is_same_v<T, T> = true;
#endif

    template <typename T, typename... Types>
    constexpr bool is_any_of_v = (is_same_v<T, Types> || ...);

    template <typename T>
    constexpr bool is_integral_v =
        is_any_of_v<remove_const_volatile_t<T>, bool, char, signed char, unsigned char, wchar_t,
#ifdef __cpp_char8_t // CXX20
                    char8_t,
#endif
                    char16_t, char32_t, short, unsigned short, int, unsigned int, long,
                    unsigned long, long long, unsigned long long>;

#define SC_requires_unsigned(T)                                                                    \
    static_assert(is_integral_v<T> && T(-1) > T(0), "Expecting integral unsigned type.")

#define SC_requires_signed(T)                                                                      \
    static_assert(is_integral_v<T> && T(-1) < T(0), "Expecting integral signed type.");

    /* Implementing MOVE and FORWARD
     * Reference: https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h
     * https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c
     */

    /**
     * Converts value to rvalue
     * @return the parameter cast to an rvalue-reference to allow moving it.
     */
    template <class T>
    [[nodiscard]] constexpr remove_reference_t<T>&& move(T&& _Arg) noexcept
    {
        return static_cast<remove_reference_t<T>&&>(_Arg);
    }

    /**
     * Forward lvalue
     * @return the parameter cast to specified type.
     */
    template <typename T>
    constexpr T&& forward(remove_reference_t<T>& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    /**
     * Forward rvalue
     * @return the parameter cast to specified type.
     */
    template <typename T>
    constexpr T&& forward(remove_reference_t<T>&& t) noexcept
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

    template <typename Base, typename Member>
    class compressed_pair : private Base
    {
    public:
        constexpr compressed_pair() = default;

        compressed_pair(const Base& b, const Member& m)
            : Base(b)
            , member(m)
        { }

        Base& first()
        {
            return static_cast<Base&>(*this);
        }

        Member& second()
        {
            return this->member;
        }

    private:
        Member member;
    };

    template <typename T, T... Values>
    struct integer_sequence
    {
        static_assert(is_integral_v<T>,
                      "integer_sequence<T, I...> requires T to be an integral type.");

        using value_type = T;

        [[nodiscard]] static constexpr size_t size() noexcept
        {
            return sizeof...(Values);
        }
    };

    template <typename T, T Size>
    using make_integer_sequence
#if __has_builtin(__make_integer_seq)
        = __make_integer_seq<integer_sequence, T, Size>;
#elif defined(_MSC_VER)
        = __make_integer_seq<integer_sequence, T, Size>;
#else
        = integer_sequence<T, __integer_pack(Size)...>;
#endif

    template <size_t... Values>
    using index_sequence = integer_sequence<size_t, Values...>;

    template <size_t Size>
    using make_index_sequence = make_integer_sequence<size_t, Size>;

} // namespace sc

#endif // SC_TYPETRAITS_H
