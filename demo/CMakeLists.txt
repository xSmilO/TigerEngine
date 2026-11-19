add_executable(TigerSnake main.cpp SnakeGame.cpp SnakeModel.cpp)
target_link_libraries(TigerSnake PRIVATE TigerEngine::Core)
set_target_properties(TigerSnake PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_CURRENT_BINARY_DIR}/..>"
)
add_dependencies(TigerSnake TigerAssets)
