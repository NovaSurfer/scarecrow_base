//
// Created by max on 10/1/23.
//

#include "../arena_alloc.h"
#include "doctest/doctest.h"
#include <cstdio>
#include <cstring>

TEST_CASE("arena-alloc-stress?")
{

    static constexpr size_t DEFAULT_ALIGNMENT = sizeof(void*);

    unsigned char backing_buffer[256];
    sc::arena_alloc arena;
    arena.init(backing_buffer, 256);

    for(int i = 0; i < 10; i++) {
        int* x;
        float* f;
        char* str;

        // Reset all arena offsets for each loop
        arena.deallocate(nullptr);

        x = (int*)arena.allocate(sizeof(int), DEFAULT_ALIGNMENT);
        f = (float*)arena.allocate(sizeof(float), DEFAULT_ALIGNMENT);
        str = static_cast<char*>(arena.allocate(10, DEFAULT_ALIGNMENT));

        *x = 123;
        *f = 987;
        memmove(str, "Hellope", 7);

        printf("%p: %d\n", x, *x);
        printf("%p: %f\n", f, *f);
//        printf("%p: %s\n", str, str);

        str = static_cast<char*>(arena.resize(str, 10, 16, DEFAULT_ALIGNMENT));
        memmove(str + 7, " world!", 7);
        //        printf("%p: %s\n", str, str);
    }

    arena.deallocate(nullptr);
}

TEST_CASE("arena_memory_scoped_savepoint")
{
    static constexpr size_t DEFAULT_ALIGNMENT = sizeof(void*);

    unsigned char backing_buffer[256];
    sc::arena_alloc arena;
    arena.init(backing_buffer, 256);

    int* integer = static_cast<int*>(arena.allocate(sizeof(int), DEFAULT_ALIGNMENT));
    REQUIRE(integer);
    *integer = 8;
    REQUIRE_EQ(*integer, 8);
    {
        sc::arena_memory_scoped_savepoint arena_savepoint(&arena);
        // 256 - sizeof_int(4) = 252, but the maximum, at that point is 248, because of the alignment
        unsigned char* chars = static_cast<unsigned char*>(arena.allocate(sizeof(unsigned char) * 248, DEFAULT_ALIGNMENT));
        REQUIRE(chars);

        chars = static_cast<unsigned char*>(arena.allocate(sizeof(unsigned char), DEFAULT_ALIGNMENT));
        REQUIRE_FALSE(chars);
    } // here arena_savepoint should go out of scope and set current and previous offsets to what they were before the savepoint

    unsigned char* chars = static_cast<unsigned char*>(arena.allocate(sizeof(unsigned char) * 248, DEFAULT_ALIGNMENT));
    REQUIRE(chars);

    chars = static_cast<unsigned char*>(arena.allocate(sizeof(unsigned char), DEFAULT_ALIGNMENT));
    REQUIRE_FALSE(chars);

    arena.deallocate(nullptr);
}
