cmake_minimum_required(VERSION 3.22)

project(SimpleGame)

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo) # with debug info

add_executable(${PROJECT_NAME}
    src/game_layer.cpp
    src/game_level.cpp
    src/main.cpp
    src/character/character.cpp
    src/character/enemy.cpp
    src/character/enemy_registry.cpp
    src/character/player.cpp
    src/weapon/axe.cpp
    src/weapon/gun.cpp
    src/weapon/sword.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/Phyzics/src/include
    ${CMAKE_SOURCE_DIR}/Engine/src/include
    ${CMAKE_SOURCE_DIR}/Engine/dependencies/zerializer/include
    src
)

target_link_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_BINARY_DIR}/Engine
    ${CMAKE_BINARY_DIR}/Phyzics
    ${CMAKE_BINARY_DIR}/Engine/dependencies/zerializer
)

target_link_libraries(${PROJECT_NAME} Engine Phyzics Zerializer)
