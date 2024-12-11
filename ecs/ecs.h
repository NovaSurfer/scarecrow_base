//
// Created by max on 5/25/23.
//

#ifndef SCARECROW2D_ECS_H
#define SCARECROW2D_ECS_H

#include "allocator.h"
#include "component.h"
#include "entity.h"
#include "hashmap.h"
#include "heap_alloc.h"
#include "system.h"
#include <climits>

namespace sc2d
{

    class ecs
    {
    public:
        explicit ecs(entity_manager* em, sc::heap_alloc& halloc);

        void init();

        void update(f32 delta_time);

        entity create_entity();

        void destroy_entity(entity e);

        template <typename Component>
        void add_component(entity e, Component c);

        template <typename Component>
        void remove_component(entity e);

        void add_system(system* s);

    private:
        template <typename Component>
        component_manager<Component>* get_component_manager();

        void update_entity_mask(entity e, component_mask old_mask);

        sc::vec<system*> systems;
        sc::vec<any_component_manager*> component_managers;
        sc::hashmap<u32, component_mask> entity_masks;
        entity_manager* entt_manager;
        sc::allocator& alloc;
    };

    template <typename Component>
    void ecs::add_component(entity e, Component c)
    {
        component_manager<Component>* cm = get_component_manager<Component>();
        log_info_cmd("Adding component: %d to an entity: %d", Component::id(), e.id);
        cm->add(e, c);

        component_mask old_mask = entity_masks.get(e.id);
        entity_masks.get(e.id).add_component<Component>();
        update_entity_mask(e, old_mask);
    }

    template <typename Component>
    void ecs::remove_component(entity e)
    {
        component_manager<Component>* cm = get_component_manager<Component>();
        cm->destroy(e);

        if(cm->is_empty()) {
            alloc.make_delete(cm);
        }

        component_mask old_mask = entity_masks.get(e.id);
        entity_masks.get(e.id).remove_component<Component>();
        update_entity_mask(e, old_mask);
    }

    template <typename Component = component_id>
    component_manager<Component>* ecs::get_component_manager()
    {
        const auto id = (size_t)Component::id();
        if(id >= component_managers.len()) {
            component_managers.resize(id + 1);

            component_manager<Component>* new_manager =
                alloc.make_new<component_manager<Component>>(alloc);
            component_managers[id] = new_manager;
        }

        return static_cast<component_manager<Component>*>(component_managers[id]);
    }
} // namespace sc2d

#endif // SCARECROW2D_ECS_H
