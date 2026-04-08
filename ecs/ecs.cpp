//
// Created by max on 5/25/23.
//

#include "ecs.h"
#include "entity.h"
#include "vec.h"

namespace sc
{

    ecs::ecs(entity_manager* em, heap_alloc& halloc)
        : entities {halloc}
        , component_managers {halloc}
        , entt_manager {em}
        , alloc {halloc}
    { }

    entity ecs::create_entity()
    {
        entities.push(entt_manager->create());
        return entities[entities.len() - 1];
    }

    void ecs::destroy_entity(entity e) const
    {
        entt_manager->destroy(e);
    }

} // namespace sc
