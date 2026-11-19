cmake_minimum_required(VERSION 3.21)
project(TigerEngine VERSION 1.0 LANGUAGES CXX C)

set(TIGERENGINE_STANDALONE OFF)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(TIGERENGINE_STANDALONE ON)
endif()
option(TIGERENGINE_BUILD_SHOWCASE "Build the engine feature showcase" ${TIGERENGINE_STANDALONE})
option(TIGERENGINE_BUILD_DEMO "Build the Snake demo" ${TIGERENGINE_STANDALONE})
option(TIGERENGINE_BUILD_DOCS "Enable the Doxygen target" ${TIGERENGINE_STANDALONE})

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
add_library(TigerEngine::Core ALIAS TigerEngineCore)
target_compile_features(TigerEngineCore PUBLIC cxx_std_17)

target_link_libraries(TigerEngineCore PUBLIC
    glfw
    glad
    glm::glm
    OpenGL::GL
)

function(tiger_copy_assets target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/assets"
            "$<TARGET_FILE_DIR:${target}>/assets"
        VERBATIM
    )
endfunction()

if(TIGERENGINE_BUILD_SHOWCASE OR TIGERENGINE_BUILD_DEMO)
    add_custom_target(TigerAssets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "${CMAKE_CURRENT_BINARY_DIR}/assets"
        VERBATIM
    )
endif()

if(TIGERENGINE_BUILD_SHOWCASE)
    add_executable(TigerEngine src/main.cpp)
    target_link_libraries(TigerEngine PRIVATE TigerEngine::Core)
    set_target_properties(TigerEngine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_CURRENT_BINARY_DIR}>"
    )
    add_dependencies(TigerEngine TigerAssets)
endif()

if(TIGERENGINE_BUILD_DEMO)
    add_subdirectory(demo)
endif()

if(TIGERENGINE_BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        add_custom_target(docs
            COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT "Generating TigerEngine documentation"
            VERBATIM
        )
    endif()
endif()
