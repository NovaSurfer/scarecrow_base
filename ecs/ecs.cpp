//
// Created by max on 5/25/23.
//

#include "ecs.h"
#include "vec.h"
#include "component.h"
#include "entity.h"

namespace sc2d
{

    ecs::ecs(entity_manager* em, sc::heap_alloc& halloc)
        : entt_manager {em}
        , systems{halloc}
        , component_managers{halloc}
        , entity_masks{halloc}
        , alloc{halloc}
    {
        entity_masks.init(2);
    }

    void ecs::init()
    {
        for(size_t i = 0; i < systems.len(); ++i) {
            systems[i]->init();
        }
    }

    void ecs::update(f32 delta_time)
    {
        for(size_t i = 0; i < systems.len(); ++i) {
            systems[i]->update(delta_time);
        }
    }

    void ecs::add_system(system* s)
    {
        systems.push(s);
    }

    entity ecs::create_entity()
    {
        return entt_manager->create();
    }

    void ecs::destroy_entity(entity e)
    {
        for(size_t i = 0; i < systems.len(); ++i) {
            systems[i]->remove_entity(e);
        }

        entt_manager->destroy(e);
    }

    void ecs::update_entity_mask(entity e, component_mask old_mask)
    {
        component_mask new_mask = entity_masks.get(e.id);
        for(size_t i = 0; i < systems.len(); ++i) {
            component_mask sys_signature = systems[i]->get_signarute();
            if(new_mask.is_new_match(old_mask, sys_signature)) {
                systems[i]->add_entity(e);
            } else if(new_mask.no_longer_matched(old_mask, sys_signature)) {
                systems[i]->remove_entity(e);
            }
        }
    }

}
