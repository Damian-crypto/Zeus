cmake_minimum_required(VERSION 3.22)

project(Sandbox)

set(MYPROJECT_DIR "/run/media/damianchamel/Data/DATA/Running On/Warehouse/Test/Zeus")

add_executable(${PROJECT_NAME} src/sandbox.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${MYPROJECT_DIR}/Phyzics/src/include
    ${MYPROJECT_DIR}/Engine/src/include
    PRIVATE
    ${MYPROJECT_DIR}/Engine/dependecies/zerializer
)

target_link_directories(${PROJECT_NAME}
    PUBLIC
    ${MYPROJECT_DIR}/Phyzics
    ${MYPROJECT_DIR}/Engine
    PRIVATE
    ${MYPROJECT_DIR}/Engine/dependecies/zerializer
)

target_link_libraries(${PROJECT_NAME} Engine Phyzics Zerializer)
