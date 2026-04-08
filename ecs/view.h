//
// Created by max on 5/4/23.
//

#ifndef SCARECROW2D_VIEW_H
#define SCARECROW2D_VIEW_H

#include "component.h"
#include "ecs/entity.h"
#include "sparse_set.h"
#include "tuple.h"
#include "typeutils.h"

namespace sc
{

    template <typename... Cs>
    class basic_view
    {
        static_assert(sizeof...(Cs) > 0, "basic_view needs at least one component");

        template <typename T>
        using base_t = remove_const_volatile_t<remove_reference_t<T>>;

        class iter
        {
            const basic_view* v;
            size_t i{};

            constexpr void next()
            {
                const auto& d = v->pools[v->leading]->get_dense();
                while(i < d.len()) {
                    entity_id e {d[i]};
                    if(v->has_all(e)) {
                        break;
                    }
                    ++i;
                }
            }

        public:
            using value_type = tuple<entity_id, reference_wrapper<Cs>...>;
            iter() = default;
            iter(const basic_view* view, const size_t idx)
                : v {view}
                , i {idx}
            {
                next();
            }

            iter& operator++()
            {
                ++i;
                next();
                return *this;
            }

            bool operator!=(const iter& lhs) const
            {
                return i != lhs.i;
            }

            value_type operator*() const
            {
                entity_id e {v->pools[v->leading]->get_dense()[i]};
                return {e, v->template comp<Cs>(e)...};
            }
        };

    public:
        constexpr basic_view(arr<const sparse_set<entity_id>*, sizeof...(Cs)> pools,
                             tuple<component_manager<base_t<Cs>>*...> stores)
            : pools {pools}
            , stores {sc::move(stores)}
            , leading {pick_leading()}
        { }

        iter begin() const
        {
            return {this, 0};
        }

        iter end() const
        {
            return {this, pools[leading]->get_dense().len()};
        }

    private:
        [[nodiscard]] size_t pick_leading() const
        {
            std::size_t best {0};
            for(size_t i = 1; i < sizeof...(Cs); ++i) {
                if(pools[i]->get_dense().len() < pools[best]->get_dense().len()) {
                    best = i;
                }
            }
            return best;
        }

        [[nodiscard]] constexpr bool has_all(entity_id e) const
        {
            for(size_t i = 0; i < pools.size(); ++i) {
                if(!pools[i]->has(e)) {
                    return false;
                }
            }
            return true;
        }

        template <typename T>
        constexpr reference_wrapper<T> comp(entity_id e) const
        {
            constexpr size_t I = type_index_v<T, Cs...>();
            auto store = stores.template get<I>();
            return {store->get_instance(e)};
        }

        arr<const sparse_set<entity_id>*, sizeof...(Cs)> pools;
        tuple<component_manager<base_t<Cs>>*...> stores;
        size_t leading;
    };

} // namespace sc

#endif // SCARECROW2D_VIEW_H
