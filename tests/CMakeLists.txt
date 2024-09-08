cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(sc_base_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add_definitions("-DSC_ALLOC_WITH_HEADER")

set(TEST_SOURCES
        unit.cpp
        ../compiler.h
        ../memory.h
        ../memory.cpp
        ../allocator.h
        ../heap_alloc.h
        ../heap_alloc.cpp
        ../arena_alloc.cpp
        ../arena_alloc.h
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

        ../ecs/component.h
        ../ecs/ecs.cpp
        ../ecs/ecs.h
        ../ecs/entity.cpp
        ../ecs/entity.h
        ../ecs/system.cpp
        ../ecs/system.h

#        non_pod_struct.h
#        vec_test.cpp
#       hashmap_test.cpp
#        arr_test.cpp
#        queue_test.cpp
#        unique_test.cpp
#        shared_test.cpp
#        event_queue_tests.cpp
#        arena_alloc_test.cpp
        ecs_tests.cpp
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
    set(UNIX_COMPILE_OPTIONS_DEBUG "-O0;-Wall;-g;-fsanitize=address")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${UNIX_COMPILE_OPTIONS}>")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${UNIX_COMPILE_OPTIONS_DEBUG}>")
    # Setting COMPILE_OPTIONS specifically for TEST_SOURCES to check for warnings library's code and test cases only
    #    set_source_files_properties(${TEST_SOURCES} PROPERTIES COMPILE_FLAGS ${UNIX_COMPILE_OPTIONS})
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_link_options(${PROJECT_NAME} PRIVATE -g -fsanitize=address)
    endif()
endif ()


include(doctest.cmake)
target_include_directories(sc_base_test PUBLIC ${CMAKE_SOURCE_DIR} ${doctest_SOURCE_DIR})
if (MSVC)
    target_link_libraries(sc_base_test Synchronization)
endif ()

