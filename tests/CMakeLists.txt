cmake_minimum_required(VERSION 3.10)


project(sc_base_test)

set(TEST_SOURCES
        unit.cpp
        doctest/doctest.h
        ../compiler.h
        ../memory.h
        ../memory.cpp
        ../allocator.h
        ../heap_alloc.h
        ../heap_alloc.cpp
        ../linear_alloc.h
        ../linear_alloc.cpp
        ../temp_alloc.h
        ../temp_alloc.cpp
        ../pool_alloc.h
        ../pool_alloc.cpp
        ../arr.h
        ../vec.h
        ../queue.h
        ../MurmurHash3.h
        ../MurmurHash3.cpp
        ../hash.h
        ../hashmap.h
        ../atomic_utils.h
        ../futex.h
        ../mutex.h
        ../mutex.cpp
        ../typeutils.h
        ../smart_ptr.h
        ../unique.h
        ../shared.h
        ../event_queue.h
        ../event_queue.inl
        vec_test.cpp
        hashmap_test.cpp
        arr_test.cpp
        queue_test.cpp
        unique_test.cpp
        shared_test.cpp
        event_queue_tests.cpp
        )

add_executable(sc_base_test ${TEST_SOURCES})
target_include_directories(sc_base_test PUBLIC ${CMAKE_SOURCE_DIR})
if (WINSDKSYNC_FOUND)
    target_link_libraries(sc_base_test ${winsdk_sync_lib})
endif ()

