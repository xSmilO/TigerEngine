cmake_minimum_required(VERSION 3.15)
project(TigerEngine VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glfw EXCLUDE_FROM_ALL)
add_subdirectory(vendor/glad EXCLUDE_FROM_ALL)

add_subdirectory(vendor/glm EXCLUDE_FROM_ALL)

find_package(OpenGL REQUIRED)

add_library(TigerEngineCore STATIC
    src/Core/Application.cpp
    src/Core/Input.cpp
    src/Core/AudioSystem.cpp
    src/Core/UI.cpp
    src/Core/AssetManager.cpp
    src/Core/AssetPath.cpp

    src/UI/UIManager.cpp
    src/UI/UIElement.cpp
    src/UI/UILabel.cpp
    src/UI/UIButton.cpp
    src/UI/UIPanel.cpp

    src/Graphics/FontRenderer.cpp
    src/Graphics/Renderer2D.cpp
    src/Graphics/Texture.cpp
    src/Graphics/stb_image.cpp
    src/Graphics/OrthographicCamera.cpp
    src/Graphics/Shader.cpp
    
    src/Scene/GameObject.cpp
    src/Scene/Scene.cpp
    src/Scene/RigidBody2D.cpp
    src/Scene/SpriteRenderer.cpp
    src/Scene/BoxCollider2D.cpp
    src/Scene/SpriteAnimator.cpp

    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(TigerEngineCore PUBLIC
    include
    vendor
    vendor/imgui
)

target_link_libraries(TigerEngineCore PUBLIC
    glfw
    glad
    glm::glm
    OpenGL::GL
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE TigerEngineCore)

add_custom_target(TigerAssets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets"
)

function(tiger_copy_assets target)
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}>"
    )
    add_dependencies(${target} TigerAssets)
endfunction()

tiger_copy_assets(${PROJECT_NAME})
add_subdirectory(demo)

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Generating TigerEngine documentation"
        VERBATIM
    )
endif()
