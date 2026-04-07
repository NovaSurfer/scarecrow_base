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

    template <typename...>
    using void_t = void;

    template <typename T, T val>
    struct integral_constant
    {
        static constexpr T value = val;
        using value_type = T;
        using type = integral_constant<T, val>;

        constexpr operator value_type() const noexcept
        {
            return value;
        }
        constexpr value_type operator()() const noexcept
        {
            return value;
        }
    };

    template <typename T, T val>
    constexpr T integral_constant<T, val>::value;

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

    template <typename>
    inline constexpr bool is_reference_v = false;

    template <typename T>
    inline constexpr bool is_reference_v<T&> = true;

    template <typename T>
    inline constexpr bool is_reference_v<T&&> = true;

    template <bool B, typename T, typename F>
    struct conditional
    {
        using type = T;
    };

    template <typename T, typename F>
    struct conditional<false, T, F>
    {
        using type = F;
    };

    template <bool B, class T, class F>
    using conditional_t = typename conditional<B, T, F>::type;

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

    template <typename T>
    inline constexpr bool is_const_v = __is_const(T);

    template <typename T>
    inline constexpr bool is_trivially_destructible_v =
#if __has_builtin(__is_trivially_destructible)
        __is_trivially_destructible(T);
#elif __has_builtin(__has_trivial_destructor)
        __has_trivial_destructor(T);
#else
#    error is_trivially_destructible is not implemented
#endif

#if __has_builtin(__add_pointer)
    template <typename T>
    struct add_pointer
    {
        using type = __add_pointer(T);
    };
#else
    template <typename T, typename = void>
    struct _add_pointer
    {
        using type = T;
    };

    template <typename T>
    struct _add_pointer<T, void_t<T*>>
    {
        using type = T*;
    };

    template <typename T>
    struct add_pointer : _add_pointer<T>
    { };

    template <typename T>
    struct add_pointer<T&>
    {
        using type = T*;
    };

    template <typename T>
    struct add_pointer<T&&>
    {
        using type = T*;
    };
#endif

#if __has_builtin(__decay)
    template <typename T>
    struct decay
    {
        using type = __decay(T);
    };
#else
    template <typename U>
    struct decay_selector
    {
        using type = conditional_t<is_const_v<const U>, // trick: only function types and reference
                                                        // types can't be const qualified
                                   remove_const_volatile_t<U>, add_pointer<U>>;
    };

    template <typename U, size_t N>
    struct decay_selector<U[N]>
    {
        using type = U*;
    };

    template <typename U>
    struct decay_selector<U[]>
    {
        using type = U*;
    };

    template <typename T>
    struct decay
    {
        using type = typename decay_selector<T>::type;
    };

    template <typename T>
    struct decay<T&>
    {
        using type = typename decay_selector<T>::type;
    };

    template <typename T>
    struct decay<T&&>
    {
        using type = typename decay_selector<T>::type;
    };
#endif

    template <typename T>
    using decay_t = typename decay<T>::type;

    template <typename T, typename U, typename... Ts>
    constexpr size_t type_index_v_impl()
    {
        if constexpr(is_same_v<remove_const_volatile_t<T>, remove_const_volatile_t<U>>) {
            return 0;
        } else {
            static_assert(sizeof...(Ts) > 0, "type_index_v: type T is not in the pack");
            return 1 + type_index_v_impl<T, Ts...>();
        }
    }

    template <typename T, typename... Ts>
    constexpr size_t type_index_v()
    {
        static_assert(sizeof...(Ts) > 0, "type_index_v: empty pack");
        return type_index_v_impl<T, Ts...>();
    }

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

    template <typename T>
    class reference_wrapper
    {
    public:
        reference_wrapper(T& v)
            : val {&v} { };

        reference_wrapper(T&&) = delete;
        
        reference_wrapper(const reference_wrapper<T>& o)
            : val {o.val} { };

        reference_wrapper& operator=(const reference_wrapper<T>& o)
        {
            val = o.val;
            return *this;
		}            

        operator T&() const noexcept
        {
            return *val;
        };
        
        T& get() const noexcept
        {
            return *val;
        };

    private:
        T* val;
    };

} // namespace sc

#endif // SC_TYPETRAITS_H
