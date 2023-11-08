cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(sc_base)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_definitions("-DSC_ALLOC_WITH_HEADER")

set(SC_MEM_SRC
        compiler.h
        memory.h
        memory.cpp
        allocator.h
        heap_alloc.h
        heap_alloc.cpp
        arr.h
        vec.h
        queue.h
        murmur.h
        murmur.cpp
        hash.h
        hash.cpp
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
        sc_types.h
        dbg_asserts.h
        logout.h
        logout.cpp
        arena_alloc.cpp
        arena_alloc.h)
#test.cpp)
#set_source_files_properties(foo.cpp PROPERTIES COMPILE_FLAGS "-Wall" "-Werror" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-pedantic" "-Wpadded")
add_library(${PROJECT_NAME} ${SC_MEM_SRC})


if (MSVC)
    set(MSVC_COMPILE_OPTIONS "/Wall")
    set(MSVC_COMPILE_DEBUG_OPTIONS ${MSVC_COMPILE_OPTIONS} "/ZI" "/Od")
    set(MSVC_COMPILE_RELEASE_OPTIONS ${MSVC_COMPILE_OPTIONS} "/O2")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${MSVC_COMPILE_DEBUG_OPTIONS}>")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${MSVC_COMPILE_RELEASE_OPTIONS}>")
else ()
    set(UNIX_COMPILE_OPTIONS "-Wall" "-Werror" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-pedantic" "-Wpadded" "-Winvalid-pch" "-faligned-new")
    set(UNIX_COMPILE_OPTIONS_DEBUG "${UNIX_COMPILE_OPTIONS};-O0")
    set(UNIX_COMPILE_OPTIONS_RELEASE "${UNIX_COMPILE_OPTIONS};-O3")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${UNIX_COMPILE_OPTIONS_DEBUG}>")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${UNIX_COMPILE_OPTIONS_RELEASE}>")
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
if (MSVC)
    target_link_libraries(${PROJECT_NAME} Synchronization)
endif ()

add_subdirectory(tests)
