# TigerEngine guide

TigerEngine is a small C++17 engine for drawing and controlling 2D objects.
You write the application in C++; GLFW manages the window and input, OpenGL
draws the scene, and the engine provides objects, components, assets and UI.

## Build and run

The current Windows build uses MinGW:

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build -j 4
.\build\TigerEngine.exe
```

Use a fresh build directory when changing compiler or generator. CMake copies
the assets beside the executable. The existing example is in `src/main.cpp`.
Put your own game code there, or in separate component files registered in CMake.
`Application.cpp` contains the engine loop and system setup.

## How the parts fit together

```text
Application
  Scene
    GameObject
      SpriteRenderer -> Material -> Texture
      SpriteAnimator
      Your custom components
  UIManager
    UILabel / UIButton / UIPanel
```

This is ownership/composition. A Scene contains GameObjects; a GameObject contains
Components. A SpriteRenderer holds a shared Material, and that Material holds a
shared Texture. A separate class inheritance hierarchy gives components their
common interface.

| Area | Main classes | Purpose |
|---|---|---|
| Core | Application, Input, AssetManager, AssetPath, AudioSystem | Window, loop, input and resource services |
| Scene | Scene, GameObject, Component | Objects and their behavior |
| Graphics | Renderer2D, Material, Texture, Shader, OrthographicCamera | Drawing, appearance and camera view |
| Animation | SpriteAnimator, SpriteAnimation, SpriteFrame | Texture-atlas animation |
| Collision | RigidBody2D, BoxCollider2D | Basic movement and axis-aligned collision |
| Debug UI | UI | ImGui windows, sliders and buttons |
| Game UI | UIManager, UIElement, UILabel, UIButton, UIPanel | Screen-space labels, panels and buttons |
| Fonts | Font, FontRenderer | Font atlas, measurement and drawing |

## A complete small application

This can replace the demo in `src/main.cpp`. It loads a bitmap and lets you move
it with A/D. It uses the existing sample asset.

```cpp
#include "Core/Application.h"
#include "Core/AssetManager.h"
#include "Core/Input.h"
#include "Graphics/Material.h"
#include "Scene/SpriteRenderer.h"
#include <exception>
#include <iostream>
#include <memory>

using namespace TigerEngine;

class MoveWithKeys : public Component {
public:
    float Speed = 1.0f;

    void Update(float deltaTime) override {
        if (Input::IsKeyPressed(Key::D)) Owner->Position.x += Speed * deltaTime;
        if (Input::IsKeyPressed(Key::A)) Owner->Position.x -= Speed * deltaTime;
    }
};

int main() {
    try {
        Application app(1280, 720, "My 2D project");
        AssetManager assets;
        auto texture = assets.LoadTexture("czadomen.jpg");
        auto material = std::make_shared<Material>(Vector4(1.0f), texture);
        auto object = std::make_unique<GameObject>();
        object->Name = "Player";
        object->Position = Point2D{0.0f, 0.0f};
        object->Scale = {0.5f, 0.5f};
        object->AddComponent<SpriteRenderer>(material);
        object->AddComponent<MoveWithKeys>();
        app.GetScene().AddObject(std::move(object));
        app.Run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
```

Create Application first so OpenGL exists when textures and fonts load. Variables
declared after it are destroyed before it. The Scene retains resources still
used by its renderers and releases them before the window is destroyed.

## Application and the frame loop

Application creates the window, loads OpenGL functions, initializes renderers,
UI, audio and input, then owns their shutdown. Only one Application may be active.
`Application::Get()` accesses it from a component. Startup failures throw an exception.

`Run()` starts the scene and repeats this sequence:

1. Measure elapsed time and apply TimeScale.
2. Update scene objects and their components.
3. Clear the screen and draw the scene through its active camera.
4. Update/draw game UI and call component OnGUI methods for debug UI.
5. Present the frame, poll window events and apply the optional FPS limit.

Update receives seconds, so movement should use `speed * deltaTime`.
There is one update per rendered frame; this is not a fixed-step simulation.
TimeScale affects scene updates and animation; it does not pause UI interaction.

```cpp
app.SetClearColor({0.08f, 0.1f, 0.15f, 1.0f});
app.SetTargetFPS(60);
app.TimeScale = 0.5f;
app.ShowHitboxes = true;
```

SetTargetFPS with a positive value disables VSync. SetVSync(true) enables it again.
Zero means no explicit FPS cap. Close the window or press Escape to end the demo.
Run currently catches and prints exceptions from the frame loop, so a zero process
exit code alone does not prove that no runtime error was logged.

## Scene, GameObject and Component

Use `app.GetScene()` to access the application's single scene. Add an object with
`AddObject(std::move(object))`; the Scene becomes its owner. The returned pointer
is borrowed and remains valid until the scene destroys the object.

GameObject stores Name, Position, Scale and Rotation. Its default position is
zero, default scale is `(0.5, 0.5)` and default rotation is zero.

AddComponent constructs and owns a component. GetComponent returns the first
matching component or nullptr. The component's Owner points to its GameObject.

| Hook | When to use it |
|---|---|
| Start() | Initial setup; called immediately by AddComponent |
| Update(deltaTime) | Input, movement and other behavior every frame |
| Draw() | Custom scene drawing |
| OnGUI() | Debug UI controls |
| OnCollisionEnter(other) | Current collision callback; see collision limitations below |

Start runs before AddObject if you attach components before inserting the object.
At that point Owner is valid, but Owner->CurrentScene may still be nullptr. Add
dependencies before components that need them. If you subclass GameObject and
override Update/Draw, call the base implementation to retain component dispatch.

Create objects before Run. There is no deferred-add/remove system, so changing
the scene's object list while it is being iterated is unsafe. Scene::Clear destroys
all objects and resets the active camera; call it outside scene iteration.

## Point2D, transforms and cameras

Point2D and Vector2 are both aliases for `glm::vec2`, with floating-point x and y.
Vector3, Vector4 and Matrix4 wrap other GLM types.

```cpp
Point2D position{0.5f, 0.25f};
object->Position = position;
object->Scale = {0.4f, 0.2f};
object->Rotation = 45.0f;
```

World X increases right; world Y increases up. Shape geometry is centered on the
object origin. A quad with scale `(1,1)` is one world unit wide and high. Rotation
uses degrees about the origin. The transform matrix is translation * rotation * scale.

`parent->AddChild(child)` makes the child's transform relative to the parent.
Both must still be owned by the scene. Parenting does not transfer ownership.
The child's world matrix is parent-world * child-local; reparenting preserves its
local values rather than its previous world position. Use AddChild instead of
editing Parent/Children directly; it rejects cycles and maintains both links.

OrthographicCamera controls the visible world rectangle. Its constructor takes
left, right, bottom and top bounds. Move it with SetPosition. For example, a camera
at `(2,0,0)` views the region around world X=2.

```cpp
auto* camera = app.GetScene().GetActiveCamera();
camera->SetPosition({0.5f, 0.0f, 0.0f});
camera->SetProjection(-1.6f, 1.6f, -0.9f, 0.9f);
```

Scene has a default camera. SetActiveCamera accepts a borrowed pointer to a custom
camera that must outlive its use by the scene. Do not set it to nullptr. Window
resize currently resets both default and active camera projection to fit aspect
ratio with vertical half-height 0.9, so custom zoom is not preserved on resize.
ClassicCamera follows an owner's position directly; SmoothCamera approaches it
gradually. Attach one to the object you want the camera to follow.

## Keyboard and mouse

```cpp
Input::IsKeyPressed(Key::A);
Input::IsMouseButtonPressed(Mouse::ButtonLeft);
Point2D pixels = Input::GetMousePosition();
Point2D normalized = Input::GetMousePositionNDC();
Point2D world = Input::GetMouseWorldPosition(*camera);
```

Key/button queries return true while held, including every frame of a hold.
They do not mean "just pressed". ImGui capture blocks the corresponding gameplay
queries. This does not provide general event consumption between game UI and
gameplay. Scene's OnMousePressed/Released functions are currently empty.

Pixels use logical window dimensions, top-left origin and downward Y. NDC uses
center origin, with top-left `(-1,1)` and bottom-right `(1,-1)`. World conversion
uses the inverse camera matrix. Use world coordinates to position a root object;
a child's Position is local to its parent. See [Input.md](Input.md) for details.

## Drawing primitives and choosing a material

SpriteRenderer draws a GameObject using its world transform and a shared Material.
The supported shapes are Quad, Triangle and Polygon. PolygonSides defaults to six;
the renderer clamps it to 3-98. Quads can be textured; triangles and polygons
currently use color only.

```cpp
auto color = std::make_shared<Material>(Vector4(1.0f, 0.4f, 0.1f, 1.0f));
auto* renderer = object->AddComponent<SpriteRenderer>(color, ShapeType::Polygon);
renderer->PolygonSides = 5;
color->TintColor = {0.2f, 0.8f, 1.0f, 0.7f};
```

Colors are red, green, blue, alpha in the 0-1 range. Alpha zero is transparent;
one is opaque. A textured material's color tints its image; use white to keep the
original colors. Editing a shared Material changes every renderer using it.
Give objects separate Material instances when they need independent colors.

For custom drawing, call Renderer2D in a component's Draw method. Scene already
calls BeginScene/EndScene, so you do not need to call them inside the component.

```cpp
void Draw() override {
    Renderer2D::DrawLine({-0.5f, 0.0f}, {0.5f, 0.0f}, {1, 1, 1, 1});
    Renderer2D::DrawTriangle(Owner->GetWorldTransform(), {1, 0, 0, 1});
}
```

Lines take world-space endpoints. Matrix overloads apply full parent transforms.
Objects are drawn in scene insertion order; later draws normally cover earlier
ones. There is no separate sprite layer sorting or batching system.

Shader wraps a GLSL program and uniform setters. The default vertex shader
combines view/projection and object transforms and maps UV rectangles; its
fragment shader produces a solid color or tinted image. Most applications can
use SpriteRenderer without editing shaders. Compile/link/file errors throw.
Renderer2D uses its own default shader; Material does not select a custom shader.

## Assets, textures and fonts

AssetPath resolves relative paths inside the assets folder beside the executable.
`czadomen.jpg` and `assets/./czadomen.jpg` resolve to the same file. Absolute paths
work directly. For another asset root, call AssetPath::SetRoot before constructing
Application, since startup loads shaders and a default font.

```cpp
auto texture = assets.LoadTexture("czadomen.jpg");
auto font = assets.LoadFont("fonts/main.ttf", 32.0f);
```

AssetManager caches textures by normalized path and fonts by normalized path plus
pixel size. Returns are shared_ptr handles. UnloadTexture, UnloadFont and Clear
remove cache references, not resources held elsewhere. Reloading after eviction
can create a second resource while an earlier handle still exists.

Texture reads images with stb_image and uploads RGBA data to OpenGL. It uses
nearest-neighbor filtering, which suits pixel art. Share handles rather than
copying Texture values. Missing/invalid images throw and are not cached.

Font bakes an atlas for printable ASCII characters. Unicode/Polish characters
are not supported by this implementation; unsupported bytes render as `?`.
MeasureText gives text dimensions; MeasureVisibleBounds measures visible glyph
bounds. FontRenderer::DrawText uses framebuffer-pixel positions with top-left
origin. Game UI labels usually avoid the need to call it yourself.

Keep GPU resources on the render thread and within the Application lifetime.
Avoid global/static owning texture/font handles that survive window shutdown.
UIManager::SetFont borrows the font pointer: retain its owning handle while UI
can draw. Application already provides a default font.

## Bitmap animation

An atlas stores several frames in one image. SpriteAnimator changes the
SpriteRenderer UV rectangle rather than creating another texture each frame.

```cpp
auto atlas = assets.LoadTexture("demo/animated_sprite.png");
object->AddComponent<SpriteRenderer>(std::make_shared<Material>(Vector4(1), atlas));
auto* animator = object->AddComponent<SpriteAnimator>();
animator->AddAnimation("Idle", SpriteAnimator::Grid(4, 1, 0, 4, 0.12f));
animator->Play("Idle");
```

Grid takes columns, rows, first frame, frame count, seconds per frame and optional
loop flag. Frame numbers start at zero, left to right from the top row. The example
uses four frames and loops. Use false as the final argument for a nonlooping clip.
It stops at the last frame. For an irregular atlas, build SpriteAnimation::Frames
from SpriteFrame UVMin/UVMax pairs with normalized 0-1 coordinates.

Stop freezes the current frame. Play(name, true) restarts at frame zero. In the
current implementation, Play with the same name and restart=false does not resume
a stopped animation. Use restart=true. GetFrameIndex is relative to the clip, not
the entire atlas. Different sprites can share a texture/material and animate
independently because each SpriteRenderer has its own UV rectangle.

## Basic collision and movement

These systems support the existing platform demo; they are optional for the
required drawing features. Add BoxCollider2D to anything that should block movement.
Add RigidBody2D to moving objects and call Move from your controller.

```cpp
auto* body = object->AddComponent<RigidBody2D>();
object->AddComponent<BoxCollider2D>(Vector2{1.0f, 1.0f});
```

During Update, keep velocity in units per second, apply any gravity in your
controller, then call `body->Move(velocity, deltaTime)`. Move changes the supplied
velocity by reference when collision stops an axis. RigidBody2D does not apply
gravity or integrate its Velocity field automatically.

Box size is multiplied by the owner's Scale; Offset is added to Position.
Collision uses axis-aligned boxes and resolves horizontal then vertical movement.
It does not account for rotation or parent world transforms. Fast movement can
pass through thin obstacles. The public HasCollision field currently does not
disable collision because Move does not read it.

OnCollisionEnter is currently called for detected overlaps during vertical
resolution, potentially every update. It is not a tracked enter-only event and
there are no stay/exit events. ShowHitboxes draws translucent red collision boxes.

## Debug UI and game UI

UI wraps ImGui for developer controls. Use it in OnGUI, not Update:

```cpp
void OnGUI() override {
    UI::BeginWindow("Settings");
    UI::SliderFloat("Speed", &Speed, 0.0f, 5.0f);
    UI::Text("A/D to move");
    UI::EndWindow();
}
```

Game UI is separate and is created once before Run. Application updates and draws
it each frame. UIManager owns created elements; returned pointers are borrowed.
Its coordinates are framebuffer pixels, top-left origin, so they differ from
world units and can differ from logical mouse pixels on high DPI displays.

```cpp
auto& ui = app.GetUIManager();
auto* panel = ui.CreatePanel({20, 20}, {260, 140});
auto* label = ui.CreateLabel("Hello", {12, 12});
label->SetParent(panel);
auto* button = ui.CreateButton("Press me", {12, 60}, {220, 48});
button->SetParent(panel);
button->SetOnClick([] { std::cout << "Clicked\n"; });
```

Position is an offset from the selected anchor within the parent or screen.
SetAnchor supports TopLeft, TopRight, BottomLeft, BottomRight and Center. Setting
Visible=false on a parent hides its descendants. Parent relationships do not
transfer ownership: UIManager owns all elements. Buttons fire when pressed and
released over the same button. Labels do not intercept pointer hits; panels block
hits to UI elements behind them. Child clipping and keyboard navigation are not implemented.

Buttons support colors or textures for normal, hover and pressed states:

```cpp
button->SetTextures(assets.LoadTexture("ui/button_normal.png"),
                    assets.LoadTexture("ui/button_hover.png"),
                    assets.LoadTexture("ui/button_pressed.png"));
button->SetNineSlice(8, 8, 8, 8);
```

Nine-slice preserves image corners while stretching edges and center. Border
arguments are source-texture pixels in left, right, top, bottom order; choose them
for the actual image artwork. UI panels currently draw solid colors.

## Audio

Application initializes AudioSystem. Use it after constructing Application:

```cpp
AudioSystem::PlaySound2D("bomb.wav");
AudioSystem::PlayMusic("main_music.mp3", true);
AudioSystem::SetMusicVolume(0.1f);
AudioSystem::SetGlobalVolume(0.5f);
AudioSystem::StopMusic();
```

Sound effects are fire-and-forget. Only one music stream is retained; starting
another replaces it. Global volume affects the engine mix, including music;
music volume adjusts the music stream separately. Audio uses AssetPath but is
not cached by AssetManager. Failed initialization/file playback is logged.
Calling sound playback every Update while a key is held starts it repeatedly.

## Ownership and class inheritance

unique_ptr means one owner: Scene owns objects, and objects own components.
Use make_unique and move ownership into AddObject. Do not delete pointers
returned by AddObject, AddComponent, GetComponent or UIManager creation methods.
shared_ptr means several owners may retain an asset or material; the last owner
releases it. Cache eviction does not invalidate a retained shared handle.

Component is a polymorphic base class. SpriteRenderer, SpriteAnimator,
RigidBody2D, BoxCollider2D and your controllers inherit it and override its hooks.
GameObject is a separate polymorphic class that contains components; it does not
inherit Component. Likewise, UILabel, UIButton and UIPanel inherit UIElement.
Transform parenting is a relationship between objects, not C++ inheritance.

## Doxygen reference

```powershell
cmake -S . -B build
cmake --build build --target docs
```

Alternatively, run `doxygen Doxyfile` from the repository root. Open
`docs/generated/html/index.html`. It contains this guide, class/member lists and
source browsing. The reference explanations live in `docs/api.dox`, so source
files do not need long documentation comments. Generated output is ignored by Git.

## Files to edit most often

| Change | Location |
|---|---|
| Demo/application setup | src/main.cpp |
| Your reusable component | New header/source under include/ and src/ |
| Register a new source | CMakeLists.txt |
| Textures, fonts, sounds, shaders | assets/ |
| Engine API explanations | docs/api.dox |
| This guide | docs/EngineGuide.md |

To add a feature, start with a component or demo change. Change an engine system
only when the needed capability belongs in the engine itself.
