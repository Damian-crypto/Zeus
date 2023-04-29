project(Phyzics)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_library(${PROJECT_NAME}
    src/phyzics.cpp
    src/bodies/body.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ../Engine/vendor/glm/include/
)
