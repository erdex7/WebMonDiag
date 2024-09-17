cmake_minimum_required (VERSION 3.16)

project (WebMonDiag VERSION 1.0 LANGUAGES CXX)

option(PROJ_TESTING "Enable unit tests" OFF)

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
        -Wshadow
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4)
endif()

add_subdirectory(src)

if (PROJ_TESTING)
    add_subdirectory(test)
else()
    message(STATUS "Testing project is turned off")
endif()
