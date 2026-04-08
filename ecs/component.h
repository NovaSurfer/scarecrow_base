//
// Created by max on 4/10/23.
//

#ifndef SC_COMPONENT_H
#define SC_COMPONENT_H

#include "allocator.h"
#include "dbg_asserts.h"
#include "entity.h"
#include "sc_types.h"
#include "sparse_set.h"
#include "typeutils.h"
#include "vec.h"

namespace sc
{

    class ecs;

    struct component_id
    {
        static inline u16 id = 0;
    };

    template <typename T>
    struct component
    {
        static u16 id()
        {
            DBG_FAIL_IF(static_cast<u16>(-1) == component_id::id, "Maximum number of components is reached")
            static u16 my_id = component_id::id++;
            return my_id;
        }
    };

    class any_component_manager
    { };

    template <typename Component>
    class component_manager : public any_component_manager
    {

        friend ecs;
        using component_id = u32;

    public:
        explicit component_manager(allocator& alloc)
            : component_instancies {alloc}
            , entities {alloc}
        { }

        void add(entity e, const Component& c)
        {
            log_info_cmd("Adding component: %d to an entity: %d", Component::id(), e.index());
            entities.add(e.index());

            component_instancies.push(c);
            e.comps_bits.toggle_bit(Component::id(), true);
        }

        void destroy(entity e)
        {
            log_info_cmd("Destroying component: %d from an entity: %d", Component::id(), e.index());

            const size_t instance_index = entities.get_item_index(e.index());
            DBG_FAIL_IF(instance_index == size_t(-1), "Component is not attached to an entity");
            const size_t instancies_size = component_instancies.len();

            // turn off "that" component for entity
            e.comps_bits.toggle_bit(Component::id(), false);

            // destroy component instance
            sc::swap(component_instancies[instancies_size - 1],
                     component_instancies[instance_index]);
            component_instancies.pop_back();
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return component_instancies.empty();
        }

        Component& get_instance(entity e)
        {
            const size_t instance_index = entities.get_item_index(e.index());
            DBG_FAIL_IF(instance_index == size_t(-1), "Component is not attached to an entity");
            return component_instancies[instance_index];
        }        

        Component& get_instance(entity_id e)
        {
            const size_t instance_index = entities.get_item_index(entity::index_of_id(e));
            DBG_FAIL_IF(instance_index == size_t(-1), "Component is not attached to an entity");
            return component_instancies[instance_index];
        }

        const Component& get_instance(entity_id e) const
        {
            const size_t instance_index = entities.get_item_index(entity::index_of_id(e));
            DBG_FAIL_IF(instance_index == size_t(-1), "Component is not attached to an entity");
            return component_instancies[instance_index];
        }        
        
    private:
        // TODO: SoA
        vec<Component> component_instancies;
        // TODO: Pagination
        sparse_set<entity_id> entities;
    };
} // namespace sc

#endif // SC_COMPONENT_H
