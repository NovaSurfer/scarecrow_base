//
// Created by maksim on 13/07/23.
//

#include "../ecs/ecs.h"
#include "../ecs/entity.h"
#include "../heap_alloc.h"
#include "doctest/doctest.h"
#include "ecs/component.h"
#include "ecs/entity_manager.h"
#include "ecs/view.h"

struct first_cmpt : sc::component<first_cmpt>
{ };
struct second_cmpt : sc::component<second_cmpt>
{
	bool b = false;
};
struct third_cmpt : sc::component<third_cmpt>
{ };
struct fourth_cmpt : sc::component<fourth_cmpt>
{ };

TEST_CASE("entity-generation-stress")
{
    sc::heap_alloc ha;
    sc::entity_manager em(ha);
    sc::ecs ecs(&em, ha);

    for(size_t i = 0; i < 1024; ++i) {
        auto a = ecs.create_entity();
        REQUIRE_EQ(a.index(), i);
        REQUIRE_EQ(em.alive(a), true);
        ecs.destroy_entity(a);
    }
}

TEST_CASE("entity-generation-free-indices-overrun")
{
    sc::heap_alloc ha;
    sc::entity_manager em(ha);
    sc::ecs ecs(&em, ha);

    for(size_t i = 0; i < 2048; ++i) {
        auto a = ecs.create_entity();
        ecs.destroy_entity(a);
    }
}

TEST_CASE("entity-create-delete")
{
    sc::heap_alloc ha;
    sc::entity_manager em(ha);
    sc::ecs ecs(&em, ha);

    sc::entity e = ecs.create_entity();
    ecs.destroy_entity(e);

    REQUIRE_EQ(em.alive(e), false);
}

TEST_CASE("single-entity-multiple-components")
{
    sc::heap_alloc ha;
    sc::entity_manager em(ha);
    sc::ecs ecs(&em, ha);

    sc::entity e = ecs.create_entity();

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
    sc::entity_manager em(ha);
    sc::ecs ecs(&em, ha);

    sc::entity e1 = ecs.create_entity();
    sc::entity e2 = ecs.create_entity();
    sc::entity e3 = ecs.create_entity();

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

TEST_CASE("get-component")
{
    sc::heap_alloc ha;
    sc::entity_manager em(ha);
    sc::ecs ecs(&em, ha);

    sc::entity e1 = ecs.create_entity();
    sc::entity e2 = ecs.create_entity();

    first_cmpt cmpt_1;

    ecs.add_component(e1, cmpt_1);
    ecs.add_component(e2, cmpt_1);

    const first_cmpt& first_cmp_instance_01 = ecs.get_component<first_cmpt>(e1);
    const first_cmpt& first_cmp_isntance_02 = ecs.get_component<first_cmpt>(e2);

    REQUIRE_EQ(&first_cmp_isntance_02 - &first_cmp_instance_01, 1);

    ecs.remove_component<first_cmpt>(e1);
    ecs.remove_component<first_cmpt>(e2);

    ecs.destroy_entity(e1);
    ecs.destroy_entity(e2);
}

TEST_CASE("ecs-view")
{
    sc::heap_alloc ha;
    sc::entity_manager em(ha);
    sc::ecs ecs(&em, ha);

    sc::entity e1 = ecs.create_entity();
    sc::entity e2 = ecs.create_entity();
    sc::entity e3 = ecs.create_entity();

    first_cmpt cmpt_1;
    second_cmpt cmpt_2;
    third_cmpt cmpt_3;

	cmpt_2.b = true;

    ecs.add_component(e1, cmpt_1);
    ecs.add_component(e2, cmpt_1);
    ecs.add_component(e3, cmpt_1);

    ecs.add_component(e1, cmpt_2);
    ecs.add_component(e2, cmpt_2);
    ecs.add_component(e3, cmpt_2);

    ecs.add_component(e1, cmpt_3);
    ecs.add_component(e2, cmpt_3);
    ecs.add_component(e3, cmpt_3);

    auto view = ecs.view<first_cmpt, second_cmpt, third_cmpt>();
    for(auto [e, f, s, t] : view) {
        if(s.get().b == true) {
			MESSAGE("============== TRUE =============");
		}            
            
    }

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
