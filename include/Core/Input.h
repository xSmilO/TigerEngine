#pragma once
#include "Core/KeyCodes.h"

struct GLFWwindow;

namespace TigerEngine {
class Input {
  public:
    static void Init(GLFWwindow *window);

    static bool IsKeyPressed(KeyCode key);
    static bool IsMouseButtonPressed(KeyCode button);

    static float GetMouseX();
    static float GetMouseY();

    static float GetMouseX_NDC();
    static float GetMouseY_NDC();

  private:
    static GLFWwindow *s_Window;
};
} // namespace TigerEngine