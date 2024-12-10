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
#include <algorithm>
#include <cassert>
#include <queue>

namespace sc2d
{

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

        constexpr bool is_new_match(component_mask old_mask, component_mask sys_mask)
        {
            return matches(sys_mask) && !old_mask.matches(sys_mask);
        }

        constexpr bool no_longer_matched(component_mask old_mask, component_mask sys_mask)
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

    struct any_component_manager
    { };

    template <typename Component>
    struct component_manager : any_component_manager
    {
        explicit component_manager(sc::allocator& alloc)
            : components {alloc}
            , entity_to_component {alloc}
        {
            entity_to_component.init(2);
        }

        using component_id = u32;
        // TODO: SoA
        sc::vec<Component> components;
        // <entity_id, component_id>
        sc::hashmap<entity, component_id> entity_to_component;

        u16 add(entity e, Component c)
        {
            log_info_cmd("Adding component: %d to an entity: %d", Component::id(), e.id);
            const u16 instance = c.id();
            components.push(c);
            entity_to_component.put(e, instance);
            return instance;
        }

        void destroy(entity e)
        {
            log_info_cmd("Destroying component: %d from an entity: %d", Component::id(), e.id);
            component_id instance = entity_to_component.get(e);
            entity_to_component.remove(e);

            for(size_t i = 0, len = components.len(); i < len; ++i) {
                if(components[i].id() == instance) {
                    log_info_cmd("Component removed");
                    if(len > 1) {
                        sc::swap(components[len - 1], components[i]);
                        components.pop_back();
                        --len;
                    } else {
                        components.pop_back();
                        --len;
                    }
                }
            }
        }

        //        Component& get(entity e)
        //        {
        //            Component instance = entity_to_component[e.id];
        //            return components[instance];
        //        }
    };
} // namespace sc2d

#endif // SCARECROW2D_COMPONENT_H
