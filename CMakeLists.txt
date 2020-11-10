cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(sc_memory)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
	add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
	add_compile_options("/W4" "$<$<CONFIG:DEBUG>:/O0>")
else()
	add_compile_options("-Wall" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-pedantic" "-Wpadded" "$<$<CONFIG:RELEASE>:-O2>")
	add_compile_options("-Wall" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-pedantic" "-Wpadded" "-Winvalid-pch" "-faligned-new" "$<$<CONFIG:DEBUG>:-O0>")
endif()

add_definitions("-DSC_ALLOC_WITH_HEADER")


set(SC_MEM_SRC
    memory.h
    memory.cpp
    test.cpp)

add_executable(sc_memory_test ${SC_MEM_SRC})
target_include_directories(sc_memory_test PUBLIC ${CMAKE_SOURCE_DIR})
