cmake_minimum_required(VERSION 3.15)
project(TigerEngine VERSION 1.0 LANGUAGES CXX C)

# Set C++17 as the standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------
# Vendor Dependencies
# ---------------------------------------------------------

# Disable GLFW tests, examples, and docs before adding it
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add the vendor folders. 
add_subdirectory(vendor/glfw EXCLUDE_FROM_ALL)
add_subdirectory(vendor/glad EXCLUDE_FROM_ALL)

# GLM is a header-only math library, but adding its subdirectory 
# allows us to use the standard glm::glm CMake target.
add_subdirectory(vendor/glm EXCLUDE_FROM_ALL)

# Find System OpenGL
find_package(OpenGL REQUIRED)


add_library(${PROJECT_NAME} STATIC
    src/Core/Application.cpp
    src/Core/Input.cpp
    src/Core/AudioSystem.cpp
    src/Graphics/Renderer2D.cpp
    src/Graphics/Texture.cpp
    src/Graphics/stb_image.cpp
    src/Graphics/OrthographicCamera.cpp
    src/Graphics/Shader.cpp
    src/Scene/GameObject.cpp
    src/Scene/Scene.cpp
    src/Scene/RigidBody2D.cpp
    src/Scene/SpriteRenderer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC    
    include
    vendor
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    glad
    glm::glm
    OpenGL::GL
)

add_executable(SnakeGame 
    src/main.cpp
    src/Snake.cpp
    include/Game/Snake.h
)

target_link_libraries(SnakeGame PRIVATE ${PROJECT_NAME})

add_custom_command(TARGET SnakeGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:SnakeGame>/assets"
)