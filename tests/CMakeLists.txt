cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(sc_base_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TEST_SOURCES
        unit.cpp
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
        ../murmur.h
        ../murmur.cpp
        ../hash.h
        ../hash.cpp
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
        ../dbg_asserts.h
        ../logout.h
        ../logout.cpp
        ../sc_types.h
        non_pod_struct.h
        vec_test.cpp
        hashmap_test.cpp
        arr_test.cpp
        queue_test.cpp
        unique_test.cpp
        shared_test.cpp
        event_queue_tests.cpp
        )

add_executable(sc_base_test ${TEST_SOURCES})

if (MSVC)
    set(MSVC_COMPILE_OPTIONS "/Wall")
    set(MSVC_COMPILE_DEBUG_OPTIONS ${MSVC_COMPILE_OPTIONS} "/ZI" "/Od")
    set(MSVC_COMPILE_RELEASE_OPTIONS ${MSVC_COMPILE_OPTIONS} "/O2")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${MSVC_COMPILE_DEBUG_OPTIONS}>")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${MSVC_COMPILE_RELEASE_OPTIONS}>")
#    set_source_files_properties(TEST_SOURCES PROPERTIES COMPILE_FLAGS ${MSVC_COMPILE_OPTIONS})
else ()
    set(UNIX_COMPILE_OPTIONS "-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-pedantic;-Wpadded;-Winvalid-pch;-faligned-new;-O3;-g")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${UNIX_COMPILE_OPTIONS}>")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:-O0>")
    # Setting COMPILE_OPTIONS specifically for TEST_SOURCES to check for warnings library's code and test cases only
#    set_source_files_properties(${TEST_SOURCES} PROPERTIES COMPILE_FLAGS ${UNIX_COMPILE_OPTIONS})
endif ()


include(doctest.cmake)
target_include_directories(sc_base_test PUBLIC ${CMAKE_SOURCE_DIR} ${doctest_SOURCE_DIR})
if (MSVC)
    target_link_libraries(sc_base_test Synchronization)
endif ()

