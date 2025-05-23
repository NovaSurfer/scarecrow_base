//
// Created by novasurfer on 14/12/20.
//

#include "../heap_alloc.h"
#include "../vec.h"
#include "doctest/doctest.h"
#include "non_pod_struct.h"

TEST_CASE("vector-operations-pods")
{
    sc::heap_alloc halloc;

    SUBCASE("default constructor")
    {
        sc::vec<double> v(halloc);
        CHECK(v.capacity() == 0);
    }

    SUBCASE("constructor with size")
    {
        sc::vec<double> v(halloc, 4);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("constructor with size, filled with some value")
    {
        double value = 5.799;
        sc::vec<double> v(halloc, 4, value);

        CHECK(v.capacity() == 4);
        CHECK(v[0] == 5.799);
        CHECK(v[1] == 5.799);
        CHECK(v[2] == 5.799);
        CHECK(v[3] == 5.799);
    }

    SUBCASE("constructor with ref to other vector")
    {
        sc::vec<double> v(halloc, 10, 1.99);
        sc::vec<double> v2(v);
        CHECK(v2.capacity() == 10);
        CHECK(v2[0] == 1.99);
        CHECK(v2[1] == 1.99);
        CHECK(v2[2] == 1.99);
        CHECK(v2[3] == 1.99);
        CHECK(v2[4] == 1.99);
    }

    SUBCASE("constructor with lvalue ref to other vector")
    {
        sc::vec<double> v(halloc, 10, 1.99);
        sc::vec<double> v2(sc::move(v));
        CHECK(v2.capacity() == 10);
        CHECK(v2[0] == 1.99);
        CHECK(v2[1] == 1.99);
        CHECK(v2[2] == 1.99);
        CHECK(v2[3] == 1.99);
        CHECK(v2[4] == 1.99);
    }

    SUBCASE("copy assignment operator")
    {
        sc::vec<double> v(halloc, 5, 1.99);
        sc::vec<double> v2(halloc, 2, 5.0);
        v2 = v;
        CHECK(v2.capacity() == 5);
        CHECK(v2[0] == 1.99);
        CHECK(v2[1] == 1.99);
        CHECK(v2[2] == 1.99);
        CHECK(v2[3] == 1.99);
        CHECK(v2[4] == 1.99);
    }

    SUBCASE("move assignment operator")
    {
        sc::vec<double> v(halloc, 5, 1.99);
        sc::vec<double> v2(halloc, 2, 5.0);
        v2 = sc::move(v);
        CHECK(v2.capacity() == 5);
        CHECK(v2[0] == 1.99);
        CHECK(v2[1] == 1.99);
        CHECK(v2[2] == 1.99);
        CHECK(v2[3] == 1.99);
        CHECK(v2[4] == 1.99);
    }

    SUBCASE("len()")
    {
        sc::vec<double> v(halloc, 1, 1.1);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.len() == 3);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("relen()")
    {
        sc::vec<double> v(halloc, 1, 1.1);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.len() == 3);

        // Increasing size
        v.resize(5);

        CHECK(v.len() == 5);
        CHECK(v[0] == 1.1);
        CHECK(v[1] == 2.2);
        CHECK(v[2] == 3.3);

        v.push(4.4);
        v.push(5.5);
        v.push(6.6);

        CHECK(v.len() == 8);

        // Decreasing size
        v.resize(2);
        CHECK(v[0] == 1.1);
        CHECK(v[1] == 2.2);

        // Resizing with the same size
        v.resize(2);
        CHECK(v.len() == 2);
        CHECK(v.capacity() == 10);
        CHECK(v[0] == 1.1);
        CHECK(v[1] == 2.2);
    }

    SUBCASE("reserve()")
    {
        sc::vec<double> v(halloc, 1, 1.1);
        v.reserve(5);
        CHECK(v.len() == 1);
        CHECK(v.capacity() == 5);
    }

    SUBCASE("shrink_to_fit()")
    {
        sc::vec<double> v(halloc, 1, 1.1);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.len() == 3);
        CHECK(v.capacity() == 4);
        v.shrink_to_fit();

        CHECK(v.len() == 3);
        CHECK(v.capacity() == 3);
    }

    SUBCASE("raw()")
    {
        sc::vec<double> v(halloc, 1, 1.1);
        CHECK(v[0] == *v.raw());
    }

    SUBCASE("push_back()")
    {
        sc::vec<double> v(halloc);
        v.push(1.1);
        v.push(2.1);
        v.push(3.1);

        CHECK(v[0] == 1.1);
        CHECK(v[1] == 2.1);
        CHECK(v[2] == 3.1);
    }

    SUBCASE("emplace_back()")
    {
        sc::vec<double> v(halloc);
        v.emplace(1.1);
        v.emplace(2.1);
        v.emplace(3.1);

        CHECK(v[0] == 1.1);
        CHECK(v[1] == 2.1);
        CHECK(v[2] == 3.1);
    }

    SUBCASE("pop_back()")
    {
        sc::vec<double> v(halloc, 1, 1.1);
        v.push(2.2);
        v.push(3.3);
        v.push(4.4);
        v.pop_back();
        CHECK(v[v.len() - 1] == v[2]);
    }
}

/*************** non-pod types ***************/

TEST_CASE("vector-operations-non-pods")
{
    sc::heap_alloc halloc2;

    SUBCASE("default constructor")
    {
        sc::vec<NonPod> v(halloc2);
        CHECK(v.capacity() == 0);
    }

    SUBCASE("constructor with size")
    {
        sc::vec<NonPod> v(halloc2, 4);
        CHECK(v[0].val == 88.8);
        CHECK(v[1].val == 88.8);
        CHECK(v[2].val == 88.8);
        CHECK(v[3].val == 88.8);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("constructor with size, filled with some value")
    {
        const NonPod& value = 5.799;
        sc::vec<NonPod> v(halloc2, 4, value);
        CHECK(v[0].val == 5.799);
        CHECK(v[1].val == 5.799);
        CHECK(v[2].val == 5.799);
        CHECK(v[3].val == 5.799);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("constructor with ref to other vector")
    {
        const NonPod value {1.99};
        sc::vec<NonPod> v(halloc2, 10, value);
        sc::vec<NonPod> v2(v);
        CHECK(v2.capacity() == 10);
        CHECK(v2[0].val == 1.99);
        CHECK(v2[1].val == 1.99);
        CHECK(v2[2].val == 1.99);
        CHECK(v2[3].val == 1.99);
        CHECK(v2[4].val == 1.99);
    }

    SUBCASE("constructor with lvalue ref to other vector")
    {
        const NonPod value {1.99};
        sc::vec<NonPod> v(halloc2, 10, value);
        sc::vec<NonPod> v2(sc::move(v));
        CHECK(v2.capacity() == 10);
        CHECK(v2[0].val == 1.99);
        CHECK(v2[1].val == 1.99);
        CHECK(v2[2].val == 1.99);
        CHECK(v2[3].val == 1.99);
        CHECK(v2[4].val == 1.99);
    }

    SUBCASE("copy assignment operator")
    {
        const NonPod value {1.99};
        const NonPod value2 {5.0};
        sc::vec<NonPod> v(halloc2, 5, value);
        sc::vec<NonPod> v2(halloc2, 2, value2);
        v2 = v;
        CHECK(v2.capacity() == 5);
        CHECK(v2[0].val == 1.99);
        CHECK(v2[1].val == 1.99);
        CHECK(v2[2].val == 1.99);
        CHECK(v2[3].val == 1.99);
        CHECK(v2[4].val == 1.99);
    }

    SUBCASE("move assignment operator")
    {
        const NonPod value {1.99};
        const NonPod value2 {5.0};
        sc::vec<NonPod> v(halloc2, 5, value);
        sc::vec<NonPod> v2(halloc2, 2, value2);
        v2 = sc::move(v);
        CHECK(v2.capacity() == 5);
        CHECK(v2[0].val == 1.99);
        CHECK(v2[1].val == 1.99);
        CHECK(v2[2].val == 1.99);
        CHECK(v2[3].val == 1.99);
        CHECK(v2[4].val == 1.99);
    }

    SUBCASE("len()")
    {
        const NonPod value {1.1};
        NonPod value2 {2.2};
        NonPod value3 {3.3};
        sc::vec<NonPod> v(halloc2, 1, value);
        v.push(value2);
        v.push(value3);
        CHECK(v.len() == 3);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("relen()")
    {
        const NonPod value {1.1};
        sc::vec<NonPod> v(halloc2, 1, value);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.len() == 3);

        // Increasing size
        v.resize(5);

        CHECK(v.len() == 5);
        CHECK(v.capacity() == 5);
        CHECK(v[0].val == 1.1);
        CHECK(v[1].val == 2.2);
        CHECK(v[2].val == 3.3);

        v.push(4.4);
        v.push(5.5);
        v.push(6.6);

        CHECK(v.len() == 8);

        // Decreasing size
        v.resize(2);
        CHECK(v.len() == 2);
        CHECK(v.capacity() == 10);
        CHECK(v[0].val == 1.1);
        CHECK(v[1].val == 2.2);

        // Resizing with the same size
        v.resize(2);
        CHECK(v.len() == 2);
        CHECK(v.capacity() == 10);
        CHECK(v[0].val == 1.1);
        CHECK(v[1].val == 2.2);
    }

    SUBCASE("reserve()")
    {
        const NonPod value {1.1};
        sc::vec<NonPod> v(halloc2, 1, value);
        v.reserve(5);
        CHECK(v.len() == 1);
        CHECK(v.capacity() == 5);
    }

    SUBCASE("shrink_to_fit()")
    {
        const NonPod value {1.1};
        sc::vec<NonPod> v(halloc2, 1, value);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.len() == 3);
        CHECK(v.capacity() == 4);
        v.shrink_to_fit();

        CHECK(v.len() == 3);
        CHECK(v.capacity() == 3);
    }

    SUBCASE("raw()")
    {
        sc::vec<NonPod> v(halloc2, 1, NonPod(1.1));
        CHECK(v[0].bytes == v.raw()->bytes);
    }

    SUBCASE("push_back()")
    {
        sc::vec<NonPod> v(halloc2);
        v.push(1.1);
        v.push(2.1);
        v.push(3.1);

        CHECK(v[0].val == 1.1);
        CHECK(v[1].val == 2.1);
        CHECK(v[2].val == 3.1);
    }

    SUBCASE("emplace_back()")
    {
        sc::vec<NonPod> v(halloc2);
        v.emplace(1.1);
        v.emplace(2.1);
        v.emplace(3.1);

        CHECK(v[0].val == 1.1);
        CHECK(v[1].val == 2.1);
        CHECK(v[2].val == 3.1);
    }

    SUBCASE("pop_back()")
    {
        const NonPod value {1.1};
        sc::vec<NonPod> v(halloc2, 1, value);
        v.push(2.2);
        v.push(3.3);
        v.push(4.4);
        v.pop_back();
        CHECK(v[v.len() - 1].val == v[2].val);
    }
}
