//
// Created by novasurfer on 05/26/25.
//

#include "doctest/doctest.h"
#include "tuple.h"
#include <tuple>

TEST_CASE("tuple_test")
{
    sc::tuple<int, double> x1(1, 2.0);
    REQUIRE_EQ(sizeof(x1), sizeof(std::tuple<int, double> {}));
    REQUIRE(x1.get<0>() == 1);
    REQUIRE(x1.get<1>() == 2.0);

    sc::tuple<double, int> x2(2.0, 1);
    REQUIRE_EQ(sizeof(x2), sizeof(std::tuple<double, int> {}));
    REQUIRE(x2.get<0>() == 2.0);
    REQUIRE(x2.get<1>() == 1);
    // error: static assertion failed: Index out of bounds
    // REQUIRE(x2.get<2>() == 1);

    REQUIRE_EQ(sizeof(sc::tuple<char, double, int>), 16);
    REQUIRE_EQ(sizeof(std::tuple<char, double, int>), 24);

    REQUIRE_EQ(sizeof(sc::tuple<int32_t, char>), 8);
    REQUIRE_EQ(sizeof(std::tuple<int32_t, char>), 8);

    REQUIRE_EQ(sizeof(sc::tuple<char>), 1);
    REQUIRE_EQ(sizeof(std::tuple<char>), 1);
}
