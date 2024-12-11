//
// Created by maksim on 13/07/23.
//

#include "../ecs/ecs.h"
#include "../ecs/entity.h"
#include "../heap_alloc.h"
#include "doctest/doctest.h"
#include "ecs/component.h"

struct first_cmpt : sc2d::component<first_cmpt>
{ };
struct second_cmpt : sc2d::component<second_cmpt>
{ };
struct third_cmpt : sc2d::component<third_cmpt>
{ };
struct fourth_cmpt : sc2d::component<fourth_cmpt>
{ };

TEST_CASE("entity-generation-stress")
{
    sc::heap_alloc ha;
    sc2d::entity_manager em(ha);
    sc2d::ecs ecs(&em, ha);

    for(size_t i = 0; i < 1024; ++i) {
        auto a = ecs.create_entity();
        REQUIRE_EQ(em.alive(a), true);
        ecs.destroy_entity(a);
    }
}

TEST_CASE("entity-generation-free-indices-overrun")
{
    sc::heap_alloc ha;
    sc2d::entity_manager em(ha);
    sc2d::ecs ecs(&em, ha);

    for(size_t i = 0; i < 2048; ++i) {
        auto a = ecs.create_entity();
        ecs.destroy_entity(a);
    }
}

TEST_CASE("entity-create-delete")
{
    sc::heap_alloc ha;
    sc2d::entity_manager em(ha);
    sc2d::ecs ecs(&em, ha);

    sc2d::entity e = ecs.create_entity();
    ecs.destroy_entity(e);

    REQUIRE_EQ(em.alive(e), false);
}

TEST_CASE("single-entity-multiple-components")
{
    sc::heap_alloc ha;
    sc2d::entity_manager em(ha);
    sc2d::ecs ecs(&em, ha);

    sc2d::entity e = ecs.create_entity();

    first_cmpt cmpt_1;
    second_cmpt cmpt_2;
    third_cmpt cmpt_3;

    ecs.add_component(e, cmpt_1);
    ecs.remove_component<first_cmpt>(e);

    ecs.add_component(e, cmpt_2);
    ecs.remove_component<second_cmpt>(e);

    ecs.add_component(e, cmpt_3);
    ecs.remove_component<third_cmpt>(e);

    ecs.destroy_entity(e);
}

TEST_CASE("multiple-entities-multiple-components")
{
    sc::heap_alloc ha;
    sc2d::entity_manager em(ha);
    sc2d::ecs ecs(&em, ha);

    sc2d::entity e1 = ecs.create_entity();
    sc2d::entity e2 = ecs.create_entity();
    sc2d::entity e3 = ecs.create_entity();

    first_cmpt cmpt_1;
    second_cmpt cmpt_2;
    third_cmpt cmpt_3;

    ecs.add_component(e1, cmpt_1);
    ecs.add_component(e2, cmpt_1);
    ecs.add_component(e3, cmpt_1);

    ecs.add_component(e1, cmpt_2);
    ecs.add_component(e2, cmpt_2);
    ecs.add_component(e3, cmpt_2);

    ecs.add_component(e1, cmpt_3);
    ecs.add_component(e2, cmpt_3);
    ecs.add_component(e3, cmpt_3);

    ecs.remove_component<first_cmpt>(e1);
    ecs.remove_component<first_cmpt>(e2);
    ecs.remove_component<first_cmpt>(e3);
    
    ecs.remove_component<second_cmpt>(e1);
    ecs.remove_component<second_cmpt>(e2);
    ecs.remove_component<second_cmpt>(e3);
    
    ecs.remove_component<third_cmpt>(e1);
    ecs.remove_component<third_cmpt>(e2);
    ecs.remove_component<third_cmpt>(e3);

    ecs.destroy_entity(e1);
    ecs.destroy_entity(e2);
    ecs.destroy_entity(e3);
}
