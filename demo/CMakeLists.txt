add_executable(TigerSnake main.cpp SnakeGame.cpp SnakeModel.cpp)
target_link_libraries(TigerSnake PRIVATE TigerEngineCore)
tiger_copy_assets(TigerSnake)