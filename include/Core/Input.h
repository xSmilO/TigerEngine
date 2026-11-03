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

    // Window coordinates: logical pixels, origin at top-left, Y points down.
    static Point2D GetMousePosition();
    // Normalized device coordinates: center (0,0), top-right (1,1).
    static Point2D GetMousePositionNDC();
    // World coordinates under the cursor, accounting for camera position/zoom.
    static Point2D GetMouseWorldPosition(const OrthographicCamera& camera);
    // Uses logical window dimensions, not framebuffer dimensions (high DPI).
    // Returns (0,0) when either dimension is nonpositive or nonfinite.
    static Point2D ScreenToWorld(Point2D point, Point2D windowSize,
                                const OrthographicCamera& camera);

  private:
    static GLFWwindow *s_Window;
};
} // namespace TigerEngine
