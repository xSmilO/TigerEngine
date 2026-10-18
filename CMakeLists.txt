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

# ---------------------------------------------------------
# Engine Executable
# ---------------------------------------------------------

add_executable(${PROJECT_NAME} 
    src/main.cpp
)

# Link everything together
# Notice we link 'glad' instead of adding glad.c to our executable
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm::glm
    OpenGL::GL
)