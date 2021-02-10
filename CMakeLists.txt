cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(sc_memory)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(MSVC)
	find_package(WindowsSDK)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(win_lib_suffixes lib/x64 lib)
	else()
		set(win_lib_suffixes lib/x86 lib)
	endif()

	if(WINDOWSSDK_FOUND)
		set(winsdk_libs_dirs)
		foreach(_dir ${WINDOWSSDK_DIRS})
				get_windowssdk_library_dirs(${_dir} _lib_dirs)
				if(_lib_dirs)
					list(APPEND winsdk_hints ${_lib_dirs})
				endif()	
		endforeach()
	endif()


find_library(
	winsdk_sync_lib
	NAMES
	synchronization.lib
	PATHS
	winsdk_libs_dirs)

	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(winsdksync winsdk_sync_lib)
	
	add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
	add_compile_options("/W4" "$<$<CONFIG:DEBUG>:/O0>")
else()
	add_compile_options("-Wall" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-pedantic" "-Wpadded" "$<$<CONFIG:RELEASE>:-O2>")
	add_compile_options("-Wall" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-pedantic" "-Wpadded" "-Winvalid-pch" "-faligned-new" "$<$<CONFIG:DEBUG>:-O0>")
endif()

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
    pool_alloc.cpp
    vec.h
    MurmurHash3.h
    MurmurHash3.cpp
    hash.h
    hashmap.h
    futex.h
    mutex.h
    mutex.cpp
    test.cpp)

add_executable(sc_memory_test ${SC_MEM_SRC})
target_include_directories(sc_memory_test PUBLIC ${CMAKE_SOURCE_DIR})
if(WINSDKSYNC_FOUND)
	target_link_libraries(${PROJECT_NAME} ${winsdk_sync_lib})
endif()

add_subdirectory(tests)
