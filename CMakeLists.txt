cmake_minimum_required(VERSION 4.2.1)

project(App LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Glad
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/externals/glad/src/glad.c")

# GLFW
find_package(glfw3 CONFIG REQUIRED)

#GLM
find_package(glm REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# Your executable
add_executable(App 
    src/main.cpp
    ${GLAD_SRC} 
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp 
    src/stb.cpp
    src/Texture.cpp
)

target_include_directories(App PRIVATE
    "${CMAKE_SOURCE_DIR}/externals/glad/include"
    ${CMAKE_SOURCE_DIR}/externals/stb
)

# This copies your shaders into the same folder as the .App file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# Link libraries
target_link_libraries(App PRIVATE
    glfw
    ${CMAKE_DL_LIBS}
    ${glm_INCLUDE_DIRS}
    OpenGL::GL
)

