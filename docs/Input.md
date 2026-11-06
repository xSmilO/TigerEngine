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

## Named keys

Include `Core/KeyCodes.h`, or `Core/Input.h` which includes it.

| Keys | Names |
|---|---|
| Letters | `Key::A` through `Key::Z` |
| Number row | `Key::D0` through `Key::D9` |
| Function keys | `Key::F1` through `Key::F25` |
| Editing | `Enter`, `Tab`, `Backspace`, `Insert`, `Delete`, `Space`, `Escape` |
| Navigation | `Left`, `Right`, `Up`, `Down`, `Home`, `End`, `PageUp`, `PageDown` |
| Modifiers | `LeftShift`, `RightShift`, `LeftControl`, `RightControl`, `LeftAlt`, `RightAlt`, `LeftSuper`, `RightSuper` |
| Locks/system | `CapsLock`, `ScrollLock`, `NumLock`, `PrintScreen`, `Pause`, `Menu` |
| Punctuation | `Apostrophe`, `Comma`, `Minus`, `Period`, `Slash`, `Semicolon`, `Equal`, `LeftBracket`, `RightBracket`, `Backslash`, `GraveAccent` |
| Numeric keypad | `KP0` through `KP9`, `KPDecimal`, `KPDivide`, `KPMultiply`, `KPSubtract`, `KPAdd`, `KPEnter`, `KPEqual` |
| Extra layout keys | `World1`, `World2` |
| Mouse | `Mouse::Button1` through `Mouse::Button8`; `ButtonLeft`, `ButtonRight`, `ButtonMiddle` remain aliases |

Every keyboard name in the table uses the `Key::` namespace. `Key::Unknown` is
the unknown-key sentinel; polling it returns false. Names and numeric values
match the bundled GLFW key definitions. A physical keyboard may not have every
listed key. Super is usually the Windows key or Command key.

```cpp
using namespace TigerEngine;

if (Input::IsKeyPressed(Key::LeftShift))
    speed = 3.0f;

if (Input::IsKeyPressed(Key::LeftControl) && Input::IsKeyPressed(Key::S))
    Save();

if (Input::IsMouseButtonPressed(Mouse::Button4))
    GoBack();
```

These are held-state checks, so the example actions repeat while keys stay down.
They read key state, not typed text: Shift+A still uses `Key::A`; there is no
separate uppercase keycode or character-input API.
