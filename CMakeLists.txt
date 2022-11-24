cmake_minimum_required(VERSION 3.22)

project(Zeus)

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Compiling with GNU C/C++ Compiler")
    add_compile_options(-m64)           # Compiler for x64 architecture
    add_compile_options(-Wall)          # Show all warnings (level 4+)
    add_compile_options(-Wpedantic)     # Generate errors if code violates strict ISO C/C++ features
    add_compile_options(-g)             # To include debug information when compiling

    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
else()
	message(STATUS "GNU C++ compiler is not there")
endif()

# add_compile_definitions(_DEBUG)

add_subdirectory(Engine)
add_subdirectory(Phyzics)

option(BUILD_SIMPLEGAME "SimpleGame will be built" OFF)
option(BUILD_SANDBOX "Sandbox will be built" OFF)

if (BUILD_SIMPLEGAME)
    message(STATUS "Building Simple Game....")
    add_subdirectory(SimpleGame)
endif()

if (BUILD_SANDBOX)
    message(STATUS "Building Sandbox...")
    add_subdirectory(Sandbox)
endif()
