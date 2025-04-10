
include(FetchContent)
FetchContent_Declare(doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG dev)
FetchContent_MakeAvailable(doctest)

if(NOT doctest_SOURCE_DIR)
    message(FATAL "No doctest was found, check FetchContent declaration")
else()
    message("Doctest sources location: ${doctest_SOURCE_DIR}")
endif()

