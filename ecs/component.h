//
// Created by max on 4/10/23.
//

#ifndef SCARECROW2D_COMPONENT_H
#define SCARECROW2D_COMPONENT_H

#include "allocator.h"
#include "dbg_asserts.h"
#include "entity.h"
#include "hashmap.h"
#include "sc_types.h"
#include "vec.h"
#include <cassert>

namespace sc2d
{

    static constexpr const size_t MAX_COMPONENETS_PER_ENTITY = 16;

    struct component_id
    {
        static inline u16 id = 0;
    };

    template <typename T>
    struct component
    {
        static u16 id()
        {
            DBG_FAIL_IF((u16)-1 == component_id::id, "Maximum number of components is reached")
            static u16 my_id = component_id::id++;
            return my_id;
        }
    };

    struct component_mask
    {
        template <typename Component>
        constexpr void add_component()
        {
            mask |= (1 << Component::id());
        }

        template <typename Component>
        constexpr void remove_component()
        {
            mask &= ~(1 << Component::id());
        }

        constexpr bool is_new_match(component_mask old_mask, component_mask sys_mask) const
        {
            return matches(sys_mask) && !old_mask.matches(sys_mask);
        }

        constexpr bool no_longer_matched(component_mask old_mask, component_mask sys_mask) const
        {
            return (old_mask.matches(sys_mask) && !matches(sys_mask));
        }

    private:
        constexpr bool matches(component_mask system_mask) const
        {
            return (mask & system_mask.mask) == system_mask.mask;
        }

        u16 mask = 0;
    };

    /*
        struct any_component
        {
            template <typename AnyComponent>
            explicit constexpr any_component(AnyComponent* any_type)
                : type {any_type}
                , component_id {any_type->id()}
            { }

            template <typename AnyComponent>
            constexpr AnyComponent* get()
            {
                DBG_FAIL_IF(AnyComponent::id() != component_id,
                            "Stored component ID != Component ID to cast")
                return static_cast<AnyComponent*>(type);
            }

        private:
            void* type;
            u16 component_id;
        };
    */

    class any_component_manager
    { };

    template <typename Component>
    class component_manager : public any_component_manager
    {

        using component_id = u32;

    public:
        explicit component_manager(sc::allocator& alloc)
            : component_instancies {alloc}
            , entity_to_component {alloc}
        {
            entity_to_component.init(MAX_COMPONENETS_PER_ENTITY);
        }

        u16 add(entity e, Component c)
        {
            log_info_cmd("Adding component: %d to an entity: %d", Component::id(), e.id);
            const u16 instance = component_instancies.len();
            component_instancies.push(c);
            entity_to_component.put(e, instance);
            return instance;
        }

        void destroy(entity e)
        {
            log_info_cmd("Destroying component: %d from an entity: %d", Component::id(), e.id);
            const component_id instance = entity_to_component.get(e);
            entity_to_component.remove(e);

            const size_t instancies_size = component_instancies.len();
            sc::swap(component_instancies[instancies_size - 1], component_instancies[instance]);
            component_instancies.pop_back();
        }

        constexpr bool is_empty() const
        {
            return component_instancies.empty();
        }

        //        Component& get(entity e)
        //        {
        //            Component instance = entity_to_component[e.id];
        //            return components[instance];
        //        }

    private:
        // TODO: SoA
        sc::vec<Component> component_instancies;
        // <entity_id, component_id>
        sc::hashmap<entity, component_id> entity_to_component;
    };
} // namespace sc2d

#endif // SCARECROW2D_COMPONENT_H
