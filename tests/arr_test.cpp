//
// Created by novasurfer on 28/02/21.
//

#include "doctest/doctest.h"
#include "../arr.h"


TEST_CASE("stack-array-operations")
{
    sc::arr<int, 100> array;
    array[88] = 88;

    CHECK(array[88] == 88);
    CHECK(array.size() == 100);
}

