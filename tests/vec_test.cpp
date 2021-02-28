//
// Created by novasurfer on 14/12/20.
//

#include "../heap_alloc.h"
#include "../linear_alloc.h"
#include "../vec.h"
#include "doctest/doctest.h"
#include <algorithm>
#include <cstdlib>
#include <vector>

sc::heap_alloc halloc;
sc::linear_alloc lalloc(halloc, sizeof(double) * 256, alignof(double));

TEST_CASE("vector-operations-pods")
{

    SUBCASE("default constructor")
    {
        sc::vec<double> v(lalloc);
        CHECK(v.capacity() == 0);
    }

    SUBCASE("constructor with size")
    {
        sc::vec<double> v(lalloc, 4);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("constructor with size, filled with some value")
    {
        double value = 5.799;
        sc::vec<double> v(lalloc, 4, value);

        CHECK(v.capacity() == 4);
        CHECK(v[0] == 5.799);
        CHECK(v[1] == 5.799);
        CHECK(v[2] == 5.799);
        CHECK(v[3] == 5.799);
    }

    SUBCASE("constructor with ref to other vector")
    {
        sc::vec<double> v(lalloc, 10, 1.99);
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
        sc::vec<double> v(lalloc, 10, 1.99);
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
        sc::vec<double> v(lalloc, 5, 1.99);
        sc::vec<double> v2(lalloc, 2, 5.0);
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
        sc::vec<double> v(lalloc, 5, 1.99);
        sc::vec<double> v2(lalloc, 2, 5.0);
        v2 = sc::move(v);
        CHECK(v2.capacity() == 5);
        CHECK(v2[0] == 1.99);
        CHECK(v2[1] == 1.99);
        CHECK(v2[2] == 1.99);
        CHECK(v2[3] == 1.99);
        CHECK(v2[4] == 1.99);
    }

    SUBCASE("size()")
    {
        sc::vec<double> v(lalloc, 1, 1.1);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.size() == 3);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("resize()")
    {
        sc::vec<double> v(lalloc, 1, 1.1);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.size() == 3);

        // Increasing size
        v.resize(5);

        CHECK(v.size() == 5);
        CHECK(v[0] == 1.1);
        CHECK(v[1] == 2.2);
        CHECK(v[2] == 3.3);

        v.push(4.4);
        v.push(5.5);
        v.push(6.6);

        CHECK(v.size() == 8);

        // Decreasing size
        v.resize(2);
        CHECK(v[0] == 1.1);
        CHECK(v[1] == 2.2);
    }

    SUBCASE("reserve()")
    {
        sc::vec<double> v(halloc, 1, 1.1);
        v.reserve(5);
        CHECK(v.size() == 1);
        CHECK(v.capacity() == 5);
    }

    SUBCASE("shrink_to_fit()")
    {
        sc::vec<double> v(halloc, 1, 1.1);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.size() == 3);
        CHECK(v.capacity() == 4);
        v.shrink_to_fit();

        CHECK(v.size() == 3);
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
        CHECK(v[v.size() - 1] == v[2]);
    }
}

/*************** non-pod types ***************/
struct NonPod
{

    NonPod()
        : val(88.8)
        , bytes((char*)malloc(256))

    {
        MESSAGE("default ctr");
    }

    NonPod(double dval)
        : val(dval)
        , bytes((char*)malloc(256))
    {
        MESSAGE("ctr");
    }

    NonPod(const NonPod& o)
        : val(o.val)
        , bytes((char*)malloc(256))
    {
        MESSAGE("copy ctr");
        std::copy(o.bytes, o.bytes + 256, bytes);
    }

    NonPod& operator=(const NonPod& o)
    {
        MESSAGE("assignment operator");
        if(this != &o) {
            // free(bytes);
            // bytes = nullptr;

            bytes = (char*)malloc(256);
            val = o.val;
            std::copy(o.bytes, o.bytes + 256, bytes);
        }
        return *this;
    }

    //NonPod(NonPod&&) = default;

    ~NonPod()
    {
        MESSAGE("dstr");
        free(bytes);
        bytes = nullptr;
    }

    char* bytes;
    double val;
};

sc::heap_alloc halloc2;
sc::linear_alloc lalloc2(halloc2, sizeof(NonPod) * 1024, alignof(NonPod));

TEST_CASE("vector-operations-non-pods")
{

    SUBCASE("default constructor")
    {
        sc::vec<NonPod> v(lalloc2);
        CHECK(v.capacity() == 0);
    }

    SUBCASE("constructor with size")
    {
        sc::vec<NonPod> v(lalloc2, 4);
        CHECK(v[0].val == 88.8);
        CHECK(v[1].val == 88.8);
        CHECK(v[2].val == 88.8);
        CHECK(v[3].val == 88.8);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("constructor with size, filled with some value")
    {
        const NonPod& value = 5.799;
        sc::vec<NonPod> v(lalloc2, 4, value);
        CHECK(v[0].val == 5.799);
        CHECK(v[1].val == 5.799);
        CHECK(v[2].val == 5.799);
        CHECK(v[3].val == 5.799);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("constructor with ref to other vector")
    {
        const NonPod value {1.99};
        sc::vec<NonPod> v(lalloc2, 10, value);
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
        sc::vec<NonPod> v(lalloc2, 10, value);
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
        sc::vec<NonPod> v(lalloc2, 5, value);
        sc::vec<NonPod> v2(lalloc2, 2, value2);
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

    SUBCASE("size()")
    {
        const NonPod value {1.1};
        NonPod value2 {2.2};
        NonPod value3 {3.3};
        sc::vec<NonPod> v(lalloc2, 1, value);
        v.push(value2);
        v.push(value3);
        CHECK(v.size() == 3);
        CHECK(v.capacity() == 4);
    }

    SUBCASE("resize()")
    {
        const NonPod value {1.1};
        sc::vec<NonPod> v(lalloc2, 1, value);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.size() == 3);

        // Increasing size
        v.resize(5);

        CHECK(v.size() == 5);
        CHECK(v.capacity() == 5);
        CHECK(v[0].val == 1.1);
        CHECK(v[1].val == 2.2);
        CHECK(v[2].val == 3.3);

        v.push(4.4);
        v.push(5.5);
        v.push(6.6);

        CHECK(v.size() == 8);

        // Decreasing size
        v.resize(2);
        CHECK(v.size() == 2);
        CHECK(v.capacity() == 10);
        CHECK(v[0].val == 1.1);
        CHECK(v[1].val == 2.2);
    }

    SUBCASE("reserve()")
    {
        const NonPod value {1.1};
        sc::vec<NonPod> v(halloc2, 1, value);
        v.reserve(5);
        CHECK(v.size() == 1);
        CHECK(v.capacity() == 5);
    }

    SUBCASE("shrink_to_fit()")
    {
        const NonPod value {1.1};
        sc::vec<NonPod> v(halloc2, 1, value);
        v.push(2.2);
        v.push(3.3);
        CHECK(v.size() == 3);
        CHECK(v.capacity() == 4);
        v.shrink_to_fit();

        CHECK(v.size() == 3);
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
        CHECK(v[v.size() - 1].val == v[2].val);
    }
}
