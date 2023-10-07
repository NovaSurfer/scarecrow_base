//
// Created by novasurfer on 28/02/21.
//

#include "../heap_alloc.h"
#include "../queue.h"
#include "doctest/doctest.h"

TEST_CASE("queue-operations-pods")
{
    sc::heap_alloc halloc;

    SUBCASE("default constructor")
    {
        sc::queue<double> v(halloc);

        CHECK(v.len() == 0);
    }

    SUBCASE("push-pop-pods")
    {
        sc::queue<double> v(halloc);

        v.push(2.2);
        v.push(3.3);
        v.push(4.4);

        CHECK(v.len() == 3);
        CHECK(2.2 == v.pop());
        CHECK(v.len() == 2);
    }
}
