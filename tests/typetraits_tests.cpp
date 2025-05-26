//
// Created by novasurfer on 05/26/25.
//

#include "doctest/doctest.h"
#include "typeutils.h"

TEST_CASE("integer-sequence")
{
    REQUIRE(sc::is_same_v<sc::index_sequence<0, 4, 0, 7, 1, 9, 8, 0>,
                          sc::index_sequence<0, 4, 0, 7, 1, 9, 8, 0>>);
    REQUIRE(sc::is_same_v<sc::make_index_sequence<5>, sc::index_sequence<0, 1, 2, 3, 4>>);
}
