cmake_minimum_required(VERSION 3.0)

project(RGalaga)

set(CMAKE_CXX_STANDARD 11)

# IrrKlang
set(IRRKLANG_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/IrrKlang/bin/linux-gcc-64/libIrrKlang.so")

include_directories(
        ${PROJECT_SOURCE_DIR}/dependencies/glfw-3.3/include
        ${PROJECT_SOURCE_DIR}/dependencies/glew-2.1.0/include
        ${PROJECT_SOURCE_DIR}/src/headers
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/vendor
        ${PROJECT_SOURCE_DIR}/src/vendor/IrrKlang/include
        ${PROJECT_SOURCE_DIR}/src/vendor/IrrKlang/
)

file(COPY ${PROJECT_SOURCE_DIR}/src/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

# Disable building GLFW tests, examples and documentation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(RGalaga
        ${PROJECT_SOURCE_DIR}/src/Application.cpp
        ${PROJECT_SOURCE_DIR}/src/IndexBuffer.cpp
        ${PROJECT_SOURCE_DIR}/src/Renderer.cpp
        ${PROJECT_SOURCE_DIR}/src/Shader.cpp
        ${PROJECT_SOURCE_DIR}/src/Texture.cpp
        ${PROJECT_SOURCE_DIR}/src/VertexArray.cpp
        ${PROJECT_SOURCE_DIR}/src/VertexBuffer.cpp
        ${PROJECT_SOURCE_DIR}/src/GameObject.cpp
        ${PROJECT_SOURCE_DIR}/src/Player.cpp
        ${PROJECT_SOURCE_DIR}/src/Enemy.cpp
        ${PROJECT_SOURCE_DIR}/src/Game.cpp
        ${PROJECT_SOURCE_DIR}/src/GameLevel.cpp
        ${PROJECT_SOURCE_DIR}/src/Missile.cpp
        ${PROJECT_SOURCE_DIR}/src/vendor/stb_image/stb_image.cpp
        )

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/glfw-3.3)

# GLEW
add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/glew-2.1.0/build/cmake)

# GLM
add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/glm-0.9.9.5)

target_link_libraries(RGalaga ${IRRKLANG_LIBRARY} glfw glew glm)
