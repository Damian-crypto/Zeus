project(Engine)

add_compile_definitions(ZUES_ENGINE WINDOWING_SYSTEM_GLFW)

add_library(${PROJECT_NAME}
    corepch.cpp
    src/application.cpp
    src/layer.cpp
    src/layer_queue.cpp
    src/event/event.cpp
    src/event/key_event.cpp
    src/event/mouse_event.cpp
    src/renderer/framebuffer.cpp
    src/renderer/renderer.cpp
    src/renderer/render_command.cpp
    src/renderer/shader.cpp
    src/renderer/opengl/debugger.cpp
    src/renderer/opengl/opengl_framebuffer.cpp
    src/renderer/opengl/opengl_renderer.cpp
    src/renderer/opengl/opengl_shader.cpp
    src/renderer/opengl/opengl_texture.cpp
    src/util/camera.cpp
    src/util/imgui_ui.cpp
    src/util/level_manager.cpp
    src/util/logger.cpp
    src/util/random.cpp
    src/util/texture.cpp
    src/util/texture_manager.cpp
    src/window/input.cpp
    src/window/window.cpp
    src/window/windows_window.cpp
    vendor/glad/src/gl.c
    vendor/glm/include/glm/detail/glm.cpp
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    vendor/imgui/backends/imgui_impl_win32.cpp
    vendor/imgui/misc/cpp/imgui_stdlib.cpp
    vendor/imgui/misc/fonts/binary_to_compressed_c.cpp
    vendor/stb_image/include/stb/stb_image.cpp
)

add_subdirectory(dependency/glfw)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    src
    dependency/glfw/include
    vendor/glm/include
    vendor/glad/include
    vendor/stb_image/include
    vendor/imgui/
)

target_link_directories(${PROJECT_NAME}
    PRIVATE
    dependency/glfw/src
)

target_link_libraries(${PROJECT_NAME} glfw)
