cmake_minimum_required(VERSION 3.22)

project(Zeus)

set(CMAKE_CXX_STANDARD 17)

# add_compile_definitions(_DEBUG)

add_subdirectory(Engine)
add_subdirectory(Phyzics)

option(BUILD_SIMPLEGAME "Build Simple Game" ON)

if (BUILD_SIMPLEGAME)
    message(STATUS "Building Simple Game....")
    add_subdirectory(SimpleGame)
endif()
