#include "../hashmap.h"
#include "../heap_alloc.h"
#include "../linear_alloc.h"
#include "doctest/doctest.h"
#include <unordered_map>

struct Pod
{
    int a;
    int b;
};

inline bool operator==(const Pod& lhs, const Pod& rhs)
{
    return lhs.a == rhs.a && lhs.b == rhs.b;
}

// char* rand_string(char* str, size_t size)
// {
//     const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
//     if(size) {
//         --size;
//         for(size_t n = 0; n < size; n++) {
//             int key = rand() % (int)(sizeof charset - 1);
//             str[n] = charset[key];
//         }
//         str[size] = '\0';
//     }
//     return str;
// }

TEST_CASE("hash")
{
    SUBCASE("hash-int")
    {
        MESSAGE(sc::hash(1));
        MESSAGE(sc::hash(2));
        MESSAGE(sc::hash(3));
        MESSAGE(sc::hash(4));
    }

    SUBCASE("hash-str")
    {
        MESSAGE(sc::hash("1"));
        MESSAGE(sc::hash("2"));
        MESSAGE(sc::hash("3"));
        MESSAGE(sc::hash("4"));
    }
}

sc::heap_alloc halloc3;
sc::linear_alloc lalloc3(halloc3, sizeof(Pod) * 512, alignof(Pod));

TEST_CASE("hashmap-stress")
{
    constexpr uint32_t LOAD_FACTOR = 85;
    constexpr uint32_t COUNT = 10;
    constexpr uint32_t LIMIT = COUNT * LOAD_FACTOR / 100;

    // my hashmap
    sc::hashmap<uint32_t, Pod> hmap(lalloc3);
    hmap.init(COUNT);

    // stl hashmap
    std::unordered_map<uint32_t, Pod> stlmap;

    // printf("sizeof myhashmap %lu\n", sizeof(sc::hashmap<uint32_t, Pod>));
    // printf("sizeof stdumap %lu\n", sizeof(std::unordered_map<uint32_t, Pod>));

    srand(0);

    // insert
    for(uint32_t i = 0; i < LIMIT; ++i) {
        uint32_t key = static_cast<uint32_t>(rand());
        int a = rand();
        int b = rand();
        Pod pod {a, b};
        CHECK(hmap.put(key, pod));
        stlmap[key] = pod;
    }

    // for(uint32_t i = 0; i < COUNT; ++i) {
    //     printf("KEY: %d, POD: a=%d, b=%d, DIB: %d\n", hmap.kvps[i].key, hmap.kvps[i].value.a,
    //            hmap.kvps[i].value.b, hmap.kvps[i].dib);
    // }

    // printf("------------------------\n");

    REQUIRE_EQ(LIMIT, hmap.len());

    // iterate and check inserted values
    for(auto& i : hmap) {
        std::unordered_map<uint32_t, Pod>::const_iterator citer = stlmap.find(i.key);
        REQUIRE_EQ(citer->first, i.key);
        REQUIRE_EQ(citer->second.a, i.value.a);
    }

    // get and remove each element
    for(auto& [key, value] : stlmap) {

        Pod val = hmap.get(key);
        REQUIRE(val.a != -1);
        REQUIRE_EQ(value, val);
        hmap.remove(key);
    }

    // for(auto& i : hmap) {
    //     printf("AFTER DELETE KEY: %d, POD: a=%d, b=%d, DIB: %d\n", i.key, i.value.a, i.value.b,
    //            i.dib);
    // }

    // for(uint32_t i = 0; i < COUNT; ++i) {
    //     printf("KEY: %d, POD: a=%d, b=%d, DIB: %d\n", hmap.kvps[i].key, hmap.kvps[i].value.a,
    //            hmap.kvps[i].value.b, hmap.kvps[i].dib);
    // }
}

TEST_CASE("hashmap-add-remove-add")
{

    constexpr uint32_t COUNT = 10;

    // my hashmap
    sc::hashmap<uint32_t, Pod> hmap(lalloc3);
    hmap.init(COUNT);

    // stl hashmap
    std::unordered_map<uint32_t, Pod> stlmap;

    srand(0);

    // insert
    for(uint32_t i = 0; i < COUNT; ++i) {
        uint32_t key = static_cast<uint32_t>(rand());
        int a = rand();
        int b = rand();
        Pod pod {a, b};
        CHECK(hmap.put(key, pod));
        stlmap[key] = pod;
    }

    REQUIRE_EQ(stlmap.size(), hmap.len());

    // remove all
    for(auto& [key, value] : stlmap) {

        Pod val = hmap.get(key);
        REQUIRE(val.a != -1);
        REQUIRE_EQ(value, val);
        hmap.remove(key);
    }

    stlmap.clear();

    REQUIRE_EQ(hmap.len(), stlmap.size());

    for(uint32_t i = 0; i < COUNT / 2; ++i) {
        uint32_t key = static_cast<uint32_t>(rand());
        int a = rand();
        int b = rand();
        Pod pod {a, b};
        CHECK(hmap.put(key, pod));
        stlmap[key] = pod;
    }

    REQUIRE_EQ(hmap.len(), 5);

    // compare
    for(auto& [key, value] : stlmap) {
        Pod val = hmap.get(key);
        REQUIRE(val.a != -1);
        REQUIRE_EQ(value, val);
    }

    // for(auto& i : stlmap) {
    //     printf("STL KEY: %d, POD: a=%d, b=%d\n", i.first, i.second.a, i.second.b);
    // }

    // for(uint32_t i = 0; i < COUNT; ++i) {
    //     printf("KEY: %d, POD: a=%d, b=%d, DIB: %d\n", hmap.kvps[i].key, hmap.kvps[i].value.a,
    //            hmap.kvps[i].value.b, hmap.kvps[i].dib);
    // }

    // for(auto& i : hmap) {
    //     printf("AFTER DELETE KEY: %d, POD: a=%d, b=%d, DIB: %d\n", i.key, i.value.a, i.value.b,
    //            i.dib);
    // }
}

TEST_CASE("hashmap-add-get-not-found")
{
    sc::hashmap<uint32_t, Pod> hmap(lalloc3);
    hmap.init(1);
    hmap.put(1, {1, 1});

    const Pod pod {1,1};
    CHECK(pod == hmap.get(1));

    const Pod dpod {};
    CHECK(dpod == hmap.get(2));
}
