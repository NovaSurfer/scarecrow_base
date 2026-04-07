//
//  Created by novasurfer on 05/26/25
//

/**
 * Implements a tuple based on this post (rus): https://habr.com/ru/articles/835176/
 * but written in C++17 instead of C++26.
 * The main idea behind this implementation is that instead of using multiple recursive inheritance
 * we allocate a byte array (storage) large enough to hold all tuple members.
 * Unlike the basic implementation, this version takes member alignment into account.
 * Members are stored in the array based on their alignment, starting with the one that has the
 * largest alignment. This helps avoid inefficient memory layout and improves overall storage
 * efficiency.
 */

#ifndef SC_TUPLE_H
#define SC_TUPLE_H

#include "algo.h"
#include "arr.h"
#include "sc_types.h"
#include "typeutils.h"
#include <new>

namespace sc
{

    template <size_t I, typename... T>
    struct type_at;

    template <typename First, typename... Rest>
    struct type_at<0, First, Rest...>
    {
        using type = First;
    };

    template <size_t I, typename First, typename... Rest>
    struct type_at<I, First, Rest...>
    {
        static_assert(I < sizeof...(Rest) + 1, "Index out of bounds");
        using type = typename type_at<I - 1, Rest...>::type;
    };

    template <size_t I, typename... Ts>
    using type_at_t = typename type_at<I, Ts...>::type;

    template <size_t I, typename Arg0, typename... Args>
    decltype(auto) get_arg_by_index(Arg0&& arg0, Args&&... args)
    {
        if constexpr(I == 0) {
            return sc::forward<Arg0>(arg0);
        } else {
            return get_arg_by_index<I - 1>(sc::forward<Args>(args)...);
        }
    }

    template <typename... T>
    class tuple
    {
        static_assert((sc::is_trivially_destructible_v<T> && ...),
                      "sc::tuple works only with trivially destructible types");

    public:
        template <typename... U>
        constexpr tuple(U&&... args)
        {
            static_assert(sizeof...(U) == sizeof...(T));
            initialize_storage(make_index_sequence<sizeof...(T)>(), sc::forward<U>(args)...);
        }

        template <size_t I>
        constexpr type_at_t<I, T...>& get() & noexcept
        {
            static_assert(I < sizeof...(T), "get<I>() index out of range");

            constexpr size_t off = offset_for<I>();
            return *reinterpret_cast<type_at_t<I, T...>*>(storage + off);
        }

        template <size_t I>
        constexpr const type_at_t<I, T...>& get() const& noexcept
        {
            static_assert(I < sizeof...(T), "get<I>() index out of range");

            constexpr size_t off = offset_for<I>();
            return *reinterpret_cast<const type_at_t<I, T...>*>(storage + off);
        }

        template <size_t I>
        constexpr type_at_t<I, T...>&& get() && noexcept
        {
            static_assert(I < sizeof...(T), "get<I>() index out of range");

            constexpr size_t off = offset_for<I>();
            return static_cast<type_at_t<I, T...>&&>(
                *reinterpret_cast<type_at_t<I, T...>*>(storage + off));
        }

        template <size_t I>
        constexpr const type_at_t<I, T...>&& get() const&& noexcept
        {
            static_assert(I < sizeof...(T), "get<I>() index out of range");

            constexpr size_t off = offset_for<I>();
            return static_cast<const type_at_t<I, T...>&&>(
                *reinterpret_cast<const type_at_t<I, T...>*>(storage + off));
        }

    private:
        template <size_t>
        static constexpr inline bool tuple_always_false = false;

        struct member_info
        {
            size_t original_index;
            size_t align;
            size_t sizeof_;
            size_t offset;
        };

        template <size_t... I, typename... U>
        constexpr auto initialize_storage(index_sequence<I...> idx, U&&... args)
        {
            constexpr auto info = get_members_info();
            (void(::new(storage + info[I].offset) type_at_t<info[I].original_index, T...>(
                 get_arg_by_index<info[I].original_index>(sc::forward<U>(args)...))),
             ...);
        }

        template <size_t... I>
        static constexpr auto get_members_info_impl(index_sequence<I...>)
        {
            // for each T, store its original index relative to the sequence of T..., its sizeof and
            // alignof
            sc::arr<member_info, sizeof...(T)> res = {
                {member_info {I, alignof(type_at_t<I, T...>), sizeof(type_at_t<I, T...>), 0}...}};

            // sort array by alignment to get the optimal placement
            for(size_t i = 0; i < res.size(); ++i) {
                for(size_t j = 1; j < res.size() - i; ++j) {
                    if(res[j - 1].align < res[j].align) {
                        auto temp = res[j - 1];
                        res[j - 1] = res[j];
                        res[j] = temp;
                    }
                }
            }

            // calculate offsets
            for(size_t idx = 1, size = res.size(); idx != size; ++idx) {
                res[idx].offset = res[idx - 1].offset + res[idx - 1].sizeof_;
            }

            return res;
        }

        static constexpr auto get_members_info()
        {
            return get_members_info_impl(make_index_sequence<sizeof...(T)> {});
        }

        template <size_t Orig, size_t Idx = 0>
        static constexpr size_t offset_for()
        {
            constexpr auto info = get_members_info();
            if constexpr(Idx == info.size()) {
                static_assert(tuple_always_false<Orig>, "get<I>() index out of range");
                return 0;
            } else if constexpr(info[Idx].original_index == Orig) {
                return info[Idx].offset;
            } else {
                return offset_for<Orig, Idx + 1>();
            }
        }

        alignas(sc::max_value(alignof(T)...)) uchar storage[(sizeof(T) + ...)];
    };

    // template <size_t I, typename T>
    // struct tuple_leaf
    // {
    //     T value;

    //     constexpr tuple_leaf() = default;

    //     template <class U>
    //     constexpr tuple_leaf(U&& v)
    //         : value(sc::forward<U>(v))
    //     { }

    //     constexpr T& get() & noexcept
    //     {
    //         return value;
    //     }

    //     constexpr const T& get() const& noexcept
    //     {
    //         return value;
    //     }

    //     constexpr T&& get() && noexcept
    //     {
    //         return static_cast<T&&>(value);
    //     }

    //     constexpr const T&& get() const&& noexcept
    //     {
    //         return static_cast<const T&&>(value);
    //     }
    // };

    // template <size_t I, typename T>
    // struct tuple_leaf<I, T&>
    // {
    //     T* ptr = nullptr;

    //     tuple_leaf() = default;
    //     constexpr tuple_leaf(T& ref)
    //         : ptr(&ref)
    //     { }

    //     constexpr T& get() const noexcept
    //     {
    //         return *ptr;
    //     }
    // };

    // template <class Seq, class... Ts>
    // struct tuple_impl;

    // template <size_t... Is, typename... Ts>
    // struct tuple_impl<index_sequence<Is...>, Ts...> : tuple_leaf<Is, Ts>...
    // {
    //     constexpr tuple_impl() = default;

    //     template <class... Us>
    //     constexpr tuple_impl(Us&&... args)
    //         : tuple_leaf<Is, Ts>(sc::forward<Us>(args))...
    //     { }
    // };

    // template <class... Ts>
    // class tuple_leaf_based : public tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>
    // {
    //     using base = tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;

    // public:
    //     using base::base;

    //     template <std::size_t I>
    //     constexpr decltype(auto) get() & noexcept
    //     {
    //         using Leaf = tuple_leaf<I, type_at_t<I, Ts...>>;
    //         return static_cast<Leaf&>(*this).get();
    //     }

    //     template <size_t I>
    //     constexpr decltype(auto) get() const& noexcept
    //     {
    //         using Leaf = tuple_leaf<I, type_at_t<I, Ts...>>;
    //         return static_cast<const Leaf&>(*this).get();
    //     }

    //     template <size_t I>
    //     constexpr decltype(auto) get() && noexcept
    //     {
    //         using Leaf = tuple_leaf<I, type_at_t<I, Ts...>>;
    //         return sc::move(static_cast<Leaf&>(*this)).get();
    //     }

    //     template <std::size_t I>
    //     constexpr decltype(auto) get() const&& noexcept
    //     {
    //         using Leaf = tuple_leaf<I, type_at_t<I, Ts...>>;
    //         return sc::move(static_cast<const Leaf&>(*this)).get();
    //     }
    // };

    // choosing sc::tuple implementation

    template <typename... Ts>
    inline constexpr bool tuple_has_ref_v = (sc::is_reference_v<Ts> || ...);

    // template <typename... T>
    // class tuple
    //     : public conditional_t<tuple_has_ref_v<T...>, tuple_leaf_based<T...>, tuple_packed<T...>>
    // {
    //     using base =
    //         conditional_t<tuple_has_ref_v<T...>, tuple_leaf_based<T...>, tuple_packed<T...>>;

    // public:
    //     using base::base;
    // };

    template <>
    class tuple<>
    {
    public:
        constexpr tuple() noexcept = default;
    };

    template <class... U>
    tuple(U&&...) -> tuple<decay_t<U>...>;

    template <size_t I, class T>
    struct tuple_element;

    template <size_t I, class... Ts>
    struct tuple_element<I, tuple<Ts...>>
    {
        using type = type_at_t<I, Ts...>;
    };

    template <size_t I, class T>
    using tuple_element_t = typename tuple_element<I, T>::type;

    // todo: add get_type_index_v
    // https://github.com/electronicarts/EASTL/blob/9d2e8a090bceae2bb658bc45c3d4ee2d796cdf48/include/EASTL/meta.h#L26

    template <size_t I, class... Ts>
    constexpr decltype(auto) get(tuple<Ts...>& t) noexcept
    {
        return t.template get<I>();
    }
    template <size_t I, class... Ts>
    constexpr decltype(auto) get(const tuple<Ts...>& t) noexcept
    {
        return t.template get<I>();
    }
    template <size_t I, class... Ts>
    constexpr decltype(auto) get(tuple<Ts...>&& t) noexcept
    {
        return sc::move(t).template get<I>();
    }
    template <size_t I, class... Ts>
    constexpr decltype(auto) get(const tuple<Ts...>&& t) noexcept
    {
        return sc::move(t).template get<I>();
    }

} // namespace sc

namespace std
{
    template <typename T>
    struct tuple_size;

	template<size_t I, typename T>
    struct tuple_element;

    template <class... Ts>
    struct tuple_size<sc::tuple<Ts...>> : sc::integral_constant<size_t, sizeof...(Ts)>
    { };

    template <size_t I, class... Ts>
    struct tuple_element<I, sc::tuple<Ts...>>
    {
        using type = sc::type_at_t<I, Ts...>;
    };
} // namespace std

#endif // SC_TUPLE_H
