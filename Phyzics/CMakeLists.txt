project(Phyzics)

add_library(${PROJECT_NAME}
    src/phyzics.cpp
    src/bodies/body.cpp
)

set(MYPROJECT_DIR "E:/DATA/Running On/Warehouse/Test/Zeus")

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${MYPROJECT_DIR}/Engine/vendor/glm/include/
)
