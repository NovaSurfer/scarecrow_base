cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(sc_base)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (MSVC)
    add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O3>")
    add_compile_options("/W4" "$<$<CONFIG:DEBUG>:/O0>")
else ()
    add_compile_options("-Wall" "-Werror" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-pedantic" "-Wpadded" "$<$<CONFIG:RELEASE>:-O3>")
    add_compile_options("-Wall" "-Werror" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-pedantic" "-Wpadded" "-Winvalid-pch" "-faligned-new" "$<$<CONFIG:DEBUG>:-O0>")
endif ()

add_definitions("-DSC_ALLOC_WITH_HEADER")

set(SC_MEM_SRC
        compiler.h
        memory.h
        memory.cpp
        allocator.h
        heap_alloc.h
        heap_alloc.cpp
        linear_alloc.h
        linear_alloc.cpp
        temp_alloc.h
        temp_alloc.cpp
        pool_alloc.h
        pool_alloc.cpp #TODO: fix SEGFAULT
        arr.h
        vec.h
        queue.h
        murmur.h
        murmur.cpp
        hash.h
        hashmap.h
        atomic_utils.h
        futex.h
        mutex.h
        mutex.cpp
#        thread.h TODO: implement
#        thread.cpp TODO: implement
        typeutils.h
        smart_ptr.h
        unique.h #TODO: add unit tests
        shared.h #TODO: implement, add unit tests
        event_queue.h
        event_queue.inl
        types.h)
        #test.cpp)

add_library(${PROJECT_NAME} ${SC_MEM_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
if (MSVC)
    target_link_libraries(${PROJECT_NAME} Synchronization)
endif ()

add_subdirectory(tests)
