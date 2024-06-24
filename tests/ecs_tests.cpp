//
// Created by maksim on 13/07/23.
//

#include "doctest/doctest.h"
#include "../ecs/entity.h"
#include "../ecs/ecs.h"
#include "../heap_alloc.h"

TEST_CASE("entity-generation-stress")
{
    sc::heap_alloc ha;
    //    sc::linear_alloc linear_allocator(ha, 5120, alignof(void*));
    sc2d::entity_manager em(ha);
    sc2d::ecs ecs(&em, ha);

    for(size_t i = 0; i < 1024; ++i)
    {
       auto a = ecs.create_entity();
       ecs.destroy_entity(a);
    }
    //    MESSAGE(linear_allocator.total_allocated());
    //    MESSAGE(linear_allocator.get_free_size());
}

TEST_CASE("entity-create-delete")
{
    sc::heap_alloc ha;
    //    sc::linear_alloc linear_allocator(ha, 1024, alignof(void*));
    sc2d::entity_manager em(ha);
    sc2d::ecs ecs(&em, ha);

    sc2d::entity e = ecs.create_entity();
    ecs.destroy_entity(e);

    REQUIRE_EQ(em.alive(e), false);
}
