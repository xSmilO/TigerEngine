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

add_executable(${PROJECT_NAME} 
    src/main.cpp
    
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

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    vendor
    vendor/imgui
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm::glm
    OpenGL::GL
)

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Generating TigerEngine documentation"
        VERBATIM
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:TigerEngine>/assets"
)
