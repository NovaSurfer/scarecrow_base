set(TEST_SOURCES
    unit.cpp
    doctest/doctest.h
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
    ../vec.h
    )

add_executable(sc_base_test ${TEST_SOURCES})
target_include_directories(sc_base_test PUBLIC ${CMAKE_SOURCE_DIR})
