# Coordinates and input

Include `Core/Input.h` to read input and `Core/Point2D.h` to use points.
`Point2D` stores floating-point `x` and `y` values. It aliases `glm::vec2`;
the existing `Vector2` name remains compatible.

```cpp
TigerEngine::Point2D position{1.0f, 2.0f};
position.x += 0.5f;
```

The same point type represents different coordinate spaces:

| API | Coordinates |
|---|---|
| `Input::GetMousePosition()` | Logical window pixels; top-left is `(0,0)`, Y increases downwards |
| `Input::GetMousePositionNDC()` | Center is `(0,0)`, top-left is `(-1,1)`, bottom-right is `(1,-1)` |
| `Input::GetMouseWorldPosition(camera)` | Scene coordinates under the cursor, including camera position and projection |

For explicit conversion, `Input::ScreenToWorld(point, windowSize, camera)` takes
logical window dimensions, rather than framebuffer dimensions. This keeps
conversion consistent on high DPI displays. Nonpositive/nonfinite dimensions
return `(0,0)`. Cursor values outside the window are not clamped.

In a component's `Update` method:

```cpp
using namespace TigerEngine;
if (Input::IsKeyPressed(Key::D))
    Owner->Position.x += 1.0f * deltaTime;

auto* camera = Owner->CurrentScene->GetActiveCamera();
if (camera && Input::IsMouseButtonPressed(Mouse::ButtonLeft))
    Owner->Position = Input::GetMouseWorldPosition(*camera);
```

The example assumes the owner belongs to a scene and has no parent. For a child,
`Position` is local to its parent, while the mouse-world result is in world space.

Button/key queries return true while held. ImGui's keyboard/mouse capture flags
block the corresponding queries so interacting with the debug panel does not
move the demo shape. Cursor position queries remain available. Without an
initialized window, queries return false or `(0,0)`.

In the demo, use A/D to move the player, W to jump, and hold left mouse outside
the debug panel to move the orange hexagon. Resize the window and move the
camera by walking to check that the hexagon still follows the cursor.
