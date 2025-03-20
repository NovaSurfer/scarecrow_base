// Created by novasurfer on 05/03/25.

#include "../heap_alloc.h"
#include "doctest/doctest.h"
#include "non_pod_struct.h"
#include "sc_types.h"
#include "sparse_set.h"

TEST_CASE("sparse_set-operations-pod")
{
    sc::heap_alloc halloc;

    SUBCASE("default constructor")
    {
        sc::sparse_set<u32> ss(halloc);
        CHECK(ss.capacity() == 0);
    }

    SUBCASE("add has remove")
    {
        u32 value = 5;
        constexpr u32 iters = 1024;
        sc::sparse_set<u32> ss(halloc);

        ss.add(value);
        REQUIRE(ss.has(value));

        const sc::vec<u32>& dense_vec = ss.get_dense();
        CHECK(dense_vec[0] == 5);

        ss.remove(value);
        REQUIRE_FALSE(ss.has(value));

        REQUIRE(ss.empty());

        ss.clear();

        for(u32 i = 0; i < iters; ++i) {
            ss.add(i);
        }

        for(u32 i = 0; i < iters; ++i) {
            REQUIRE(ss.has(i));
        }

        for(u32 i = 0; i < iters; ++i) {
            ss.remove(i);
        }

        REQUIRE(ss.empty());
        for(u32 i = 0; i < iters; ++i) {
            REQUIRE_FALSE(ss.has(i));
        }

		// TODO: 
        //ss.shrink_to_fit();
    }
}
