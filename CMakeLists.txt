cmake_minimum_required(VERSION 4.2.1)

project(App LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Glad
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/externals/glad/src/glad.c")

# GLFW
find_package(glfw3 CONFIG REQUIRED)

#GLM
find_package(glm REQUIRED)

#ASSIMP
find_package(assimp REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# ImGUI
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/imgui)

# Your executable
add_executable(App 
    src/main.cpp
    ${GLAD_SRC} 
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp 
    externals/stb/stb.cpp
    src/Texture.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/Window.cpp
    src/TestApp.cpp
)

add_library(imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_compile_options(imgui PRIVATE -I${IMGUI_DIR})


target_include_directories(App PUBLIC
    "${CMAKE_SOURCE_DIR}/externals/glad/include"
    ${CMAKE_SOURCE_DIR}/externals/stb
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)



# This copies your shaders into the same folder as the .App file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# Link libraries
target_link_libraries(App PRIVATE
    glfw
    ${CMAKE_DL_LIBS}
    ${glm_INCLUDE_DIRS}
    assimp
    imgui
    OpenGL::GL
)

