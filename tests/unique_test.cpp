//
// Created by novasurfer on 7/4/21.
//

#include "../unique.h"
#include "doctest/doctest.h"

TEST_CASE("unique-ptr-operations")
{
    SUBCASE("default constructor")
    {
        MESSAGE(sizeof(sc::unique<int*>));
    }

    SUBCASE("make-unique")
    {
        sc::unique<int> un = sc::make_unique<int>(47);
    }
}
