#pragma once
#include "Core/KeyCodes.h"
#include "Core/Point2D.h"

class GLFWwindow;

namespace TigerEngine {
class OrthographicCamera;
class Input {
  public:
    static void Init(GLFWwindow *window);

    static bool IsKeyPressed(KeyCode key);
    static bool IsMouseButtonPressed(KeyCode button);

    static float GetMouseX();
    static float GetMouseY();

    static float GetMouseX_NDC();
    static float GetMouseY_NDC();

    static Point2D GetMousePosition();

    static Point2D GetMousePositionNDC();

    static Point2D GetMouseWorldPosition(const OrthographicCamera& camera);

    static Point2D ScreenToWorld(Point2D point, Point2D windowSize,
                                const OrthographicCamera& camera);

  private:
    static GLFWwindow *s_Window;
};
}
