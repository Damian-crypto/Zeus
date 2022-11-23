cmake_minimum_required(VERSION 3.22)

project(Zeus)

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)

if (__GCC__)
    message(STATUS "Compiling with GNU GCC Compiler")
    add_compile_options(-m64)
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
