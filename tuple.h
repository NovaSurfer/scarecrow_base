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
        static_assert((sc::is_trivial_v<T> && ...),
                      "sc::tuple works only with trivially constructible types");

    public:
        constexpr tuple(T&&... args)
        {
            initialize_storage(make_index_sequence<sizeof...(T)>(), sc::forward<T>(args)...);
        }

        template <size_t I>
        constexpr type_at_t<I, T...>& get() noexcept
        {
            static_assert(I < sizeof...(T), "get<I>() index out of range");

            constexpr size_t off = offset_for<I>();
            return *std::launder(reinterpret_cast<type_at_t<I, T...>*>(storage + off));
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

        template <size_t... I>
        constexpr auto initialize_storage(index_sequence<I...> idx, T&&... args)
        {
            constexpr auto info = get_members_info();
            (void(::new(storage + info[I].offset) type_at_t<info[I].original_index, T...>(
                 get_arg_by_index<info[I].original_index>(sc::forward<T>(args)...))),
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
} // namespace sc

#endif // SC_TUPLE_H
