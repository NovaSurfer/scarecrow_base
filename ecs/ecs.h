//
// Created by max on 5/25/23.
//

#ifndef SCARECROW2D_ECS_H
#define SCARECROW2D_ECS_H

#include "allocator.h"
#include "arr.h"
#include "component.h"
#include "entity.h"
#include "entity_manager.h"
#include "heap_alloc.h"
#include "logout.h"
#include "sparse_set.h"
#include "view.h"
#include "tuple.h"
#include "typeutils.h"

namespace sc
{

    class ecs
    {
    public:
        explicit ecs(entity_manager* em, heap_alloc& halloc);

        void init();

        void update(f32 delta_time);

        entity create_entity();

        void destroy_entity(entity e) const;

        template <typename Component>
        void add_component(entity e, const Component& c);

        template <typename Component>
        void remove_component(entity e);

        template <typename Component>
        Component& get_component(entity e);

        template <typename... Cmps>
        basic_view<Cmps...> view()
        {
            arr<const sparse_set<entity_id>*, sizeof...(Cmps)> pools {
                &get_component_manager<remove_const_volatile_t<Cmps>>()->entities...};

            return basic_view<Cmps...>(pools, tuple {get_component_manager<Cmps>()...});
        };

    private:
        template <typename Component>
        component_manager<Component>* get_component_manager();

        vec<entity> entities;
        vec<any_component_manager*> component_managers;
        entity_manager* entt_manager;
        allocator& alloc;
    };

    template <typename Component>
    void ecs::add_component(entity e, const Component& c)
    {
        log_info_cmd("Adding component: %d to an entity: %d", Component::id(), e.index());
        component_manager<Component>* cm = get_component_manager<Component>();

        // load entity
        entity& entity_instance = entities[e.index()];

        // add component to an entity, via proper component manager
        cm->add(entity_instance, c);

        // add or remove entity from the systems
        //update_systems_registry(entity_instance);
    }

    template <typename Component>
    void ecs::remove_component(entity e)
    {
        log_info_cmd("Removing component: %d from an entity: %d", Component::id(), e.index());
        component_manager<Component>* cm = get_component_manager<Component>();
        cm->destroy(e);

        if(cm->is_empty()) {
            alloc.make_delete(cm);
        }

        // component_mask old_mask = entity_masks.get(e.id);
        // entity_masks.get(e.id).remove_component<Component>();
        // update_entity_mask(e, old_mask);
    }

    template <typename Component>
    Component& ecs::get_component(entity e)
    {
        component_manager<Component>* cm = get_component_manager<Component>();
        return cm->get_instance(e);
    }

    template <typename Component = component_id>
    component_manager<Component>* ecs::get_component_manager()
    {
        const auto id = static_cast<size_t>(Component::id());
        if(id >= component_managers.len()) {
            component_managers.resize(id + 1);

            component_manager<Component>* new_manager =
                alloc.make_new<component_manager<Component>>(alloc);
            component_managers[id] = new_manager;
        }

        return static_cast<component_manager<Component>*>(component_managers[id]);
    }
} // namespace sc

#endif // SCARECROW2D_ECS_H
