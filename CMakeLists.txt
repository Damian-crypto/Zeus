cmake_minimum_required(VERSION 3.22)

project(Zeus)

set(CMAKE_CXX_STANDARD 17)

if (__GCC__)
    message(STATUS "Compiling with GNU GCC Compiler")
    add_compile_options(-m64)
endif()

# add_compile_definitions(_DEBUG)

add_subdirectory(Engine)
add_subdirectory(Phyzics)

option(BUILD_SIMPLEGAME "Build Simple Game" ON)

if (BUILD_SIMPLEGAME)
    message(STATUS "Building Simple Game....")
    add_subdirectory(SimpleGame)
endif()
