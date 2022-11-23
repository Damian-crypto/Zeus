cmake_minimum_required(VERSION 3.22)

project(SimpleGame)

set(MYPROJECT_DIR "/run/media/damianchamel/Data/DATA/Running On/Warehouse/Test/Zeus")

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

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${MYPROJECT_DIR}/Phyzics/src/include
    ${MYPROJECT_DIR}/Engine/src/include
    src
)

target_link_directories(${PROJECT_NAME}
    PUBLIC
    ${MYPROJECT_DIR}/Engine
    ${MYPROJECT_DIR}/Phyzics
)

target_link_libraries(${PROJECT_NAME} Engine Phyzics)
