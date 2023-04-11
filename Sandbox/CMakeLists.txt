cmake_minimum_required(VERSION 3.22)

project(Sandbox)

add_executable(${PROJECT_NAME} src/sandbox.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/Phyzics/src/include
    ${CMAKE_SOURCE_DIR}/Engine/src/include
    PRIVATE
    ${CMAKE_SOURCE_DIR}/Engine/dependecies/zerializer
)

target_link_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_BINARY_DIR}/Phyzics
    ${CMAKE_BINARY_DIR}/Engine
    PRIVATE
    ${CMAKE_BINARY_DIR}/Engine/dependecies/zerializer
)

target_link_libraries(${PROJECT_NAME} Engine Phyzics Zerializer)
