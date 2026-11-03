#include "Core/Application.h"
#include "Core/AssetManager.h"
#include "Core/AssetPath.h"
#include "Core/AudioSystem.h"
#include "Core/Input.h"
#include "Core/UI.h"
#include "Graphics/Material.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/OrthographicCamera.h"
#include "Scene/BoxCollider2D.h"
#include "Scene/Camera/SmoothCamera.h"
#include "Scene/RigidBody2D.h"
#include "Scene/SpriteAnimator.h"
#include "Scene/SpriteRenderer.h"
#include <iostream>

class PlayerController : public TigerEngine::Component {
  public:
    float Speed = 3.0f;
    float JumpForce = 4.0f;
    float Gravity = -9.8f;

    TigerEngine::Vector2 Velocity = TigerEngine::Vector2(0.0f, 0.0f);

    void Update(float deltaTime) override {
        TigerEngine::RigidBody2D *rb = Owner->GetComponent<TigerEngine::RigidBody2D>();
        if(!rb)
            return;

        float moveX = 0.0f;
        if(TigerEngine::Input::IsKeyPressed(GLFW_KEY_D))
            moveX += 1.0f;
        if(TigerEngine::Input::IsKeyPressed(GLFW_KEY_A))
            moveX -= 1.0f;
        Velocity.x = moveX * Speed;

        if(rb->IsGrounded) {
            if(Velocity.y < 0.0f)
                Velocity.y = 0.0f;

            if(TigerEngine::Input::IsKeyPressed(GLFW_KEY_W)) {
                Velocity.y = JumpForce;
                TigerEngine::AudioSystem::PlaySound2D("assets/bomb.wav");
            }
        } else {
            Velocity.y += Gravity * deltaTime;
        }

        rb->Move(Velocity, deltaTime);
    }

    void OnGUI() override {
        TigerEngine::UI::BeginWindow("Engine Debug & Settings");

        TigerEngine::UI::Checkbox("Show Hitboxes", &TigerEngine::Application::Get().ShowHitboxes);

        TigerEngine::BoxCollider2D *col = Owner->GetComponent<TigerEngine::BoxCollider2D>();
        if(col) {
            TigerEngine::UI::Text("Hitbox Settings");
            TigerEngine::UI::SliderFloat("Hitbox Width", &col->Size.x, 0.1f, 2.0f);
            TigerEngine::UI::SliderFloat("Hitbox Height", &col->Size.y, 0.1f, 2.0f);
            TigerEngine::UI::SliderFloat("Hitbox Offset Y", &col->Offset.y, -1.0f, 1.0f);
        }

        TigerEngine::UI::EndWindow();
    }
};

class ShapeGuide : public TigerEngine::Component {
    void Draw() override {
        TigerEngine::Renderer2D::DrawLine({0.15f, 0.45f}, {1.2f, 0.45f}, {0.2f, 0.9f, 1.0f, 1.0f});
    }
};

class ShapeDemoControls : public TigerEngine::Component {
    void Update(float) override {
        using namespace TigerEngine;
        if (!Owner || !Owner->CurrentScene) return;
        auto* camera = Owner->CurrentScene->GetActiveCamera();
        if (camera && Input::IsMouseButtonPressed(Mouse::ButtonLeft)) {
            const Point2D point = Input::GetMouseWorldPosition(*camera);
            Owner->Position = point;
        }
    }
    void OnGUI() override {
        using namespace TigerEngine;
        UI::BeginWindow("Project Demo - Shapes");
        UI::Text("A/D: move player | W: jump | Escape: close");
        UI::Text("Hold left mouse outside panels to move the hexagon.");
        const auto mouse = Input::GetMousePosition();
        UI::Text("Mouse pixels: " + std::to_string(int(mouse.x)) + ", " + std::to_string(int(mouse.y)));
        UI::Text("Hexagon world: " + std::to_string(Owner->Position.x) + ", " + std::to_string(Owner->Position.y));
        UI::Separator();
        UI::Text("Geometric transformations");
        UI::SliderFloat("Rotation (degrees)", &Owner->Rotation, -180.0f, 180.0f);
        UI::SliderFloat("Width", &Owner->Scale.x, 0.05f, 1.0f);
        UI::SliderFloat("Height", &Owner->Scale.y, 0.05f, 1.0f);
        auto* renderer = Owner->GetComponent<SpriteRenderer>();
        if (renderer && renderer->SharedMaterial) {
            auto& color = renderer->SharedMaterial->TintColor;
            UI::Text("Fill color (RGBA)");
            UI::SliderFloat("Red", &color.r, 0.0f, 1.0f);
            UI::SliderFloat("Green", &color.g, 0.0f, 1.0f);
            UI::SliderFloat("Blue", &color.b, 0.0f, 1.0f);
            UI::SliderFloat("Opacity", &color.a, 0.0f, 1.0f);
            if (UI::Button("Reset hexagon")) {
                Owner->Position = {0.95f, 0.65f};
                Owner->Scale = {0.3f, 0.3f};
                Owner->Rotation = 0.0f;
                color = {1.0f, 0.65f, 0.1f, 1.0f};
            }
        }
        UI::EndWindow();
    }
};

class AnimationDemoControls : public TigerEngine::Component {
    void OnGUI() override {
        using namespace TigerEngine;
        auto* animator = Owner->GetComponent<SpriteAnimator>();
        if (!animator) return;
        UI::BeginWindow("Project Demo - Bitmap Animation");
        UI::Text("Sprite sheet: 4 columns, 1 row; 0.12 seconds per frame");
        UI::Text("Frame index (0-3): " + std::to_string(animator->GetFrameIndex()));
        if (UI::Button("Stop animation")) animator->Stop();
        if (UI::Button("Restart animation")) animator->Play("Idle", true);
        UI::EndWindow();
    }
};

int RunDemo() {
    TigerEngine::Application app(1280, 720, "TigerEngine");
    TigerEngine::AssetManager assets;
    std::cout << "Hold left mouse outside the debug panel to move the orange hexagon.\n";

    auto playerTexture = assets.LoadTexture("assets/czadomen.jpg");
    auto sameTexture = assets.LoadTexture("./czadomen.jpg");
    std::cout << "Cached textures: " << assets.TextureCount() << "\n";
    std::cout << "Same resource ?" << (playerTexture == sameTexture) << "\n";

    std::cout << "Asset root: " << TigerEngine::AssetPath::GetRoot() << '\n';

    try { assets.LoadTexture("missing-demo-texture.png"); }
    catch (const std::exception& error) {
        std::cout << "Expected missing-asset check: " << error.what() << '\n';
    }

    auto playerMaterial = std::make_shared<TigerEngine::Material>(TigerEngine::Vector4(1.0f), playerTexture);
    auto player = std::make_unique<TigerEngine::GameObject>();
    player->AddComponent<TigerEngine::SpriteRenderer>(playerMaterial);
    player->Scale = TigerEngine::Vector2(0.5f, 0.5f);
    player->AddComponent<TigerEngine::RigidBody2D>();
    player->AddComponent<TigerEngine::BoxCollider2D>(TigerEngine::Vector2(0.6f, 0.9f),
                                                     TigerEngine::Vector2(0.0f, -0.05f));
    player->AddComponent<PlayerController>();
    player->AddComponent<TigerEngine::SmoothCamera>();

    auto font32 = assets.LoadFont("assets/fonts/main.ttf", 32.0f);
    auto again = assets.LoadFont("assets/fonts/main.ttf", 32.0f);
    std::cout << "Cached fonts: " << assets.FontCount() << '\n';

    TigerEngine::AudioSystem::PlayMusic("assets/main_music.mp3", true);
    TigerEngine::AudioSystem::SetMusicVolume(0.1);

    auto matFloor = std::make_shared<TigerEngine::Material>(TigerEngine::Vector4(0.0f, 1.0f, 0.0f, 1.0f));

    auto floor = std::make_unique<TigerEngine::GameObject>();
    floor->Position = TigerEngine::Vector2(0.0f, -0.8f);
    floor->Scale = TigerEngine::Vector2(2.0f, 0.2f);
    floor->AddComponent<TigerEngine::SpriteRenderer>(matFloor);
    floor->AddComponent<TigerEngine::RigidBody2D>();
    floor->AddComponent<TigerEngine::BoxCollider2D>();

    auto animationAtlas = assets.LoadTexture("assets/demo/animated_sprite.png");
    auto animationMaterial = std::make_shared<TigerEngine::Material>(TigerEngine::Vector4(1.0f), animationAtlas);
    auto animatedObject = std::make_unique<TigerEngine::GameObject>();
    animatedObject->Position = {0.8f, 0.25f};
    animatedObject->Scale = {0.5f, 0.5f};
    animatedObject->AddComponent<TigerEngine::SpriteRenderer>(animationMaterial);
    auto *animator = animatedObject->AddComponent<TigerEngine::SpriteAnimator>();
    animatedObject->AddComponent<AnimationDemoControls>();
    animator->AddAnimation("Idle", TigerEngine::SpriteAnimator::Grid(4, 1, 0, 4, 0.12f));
    animator->Play("Idle");
    app.GetScene().AddObject(std::move(animatedObject));

    auto triangle = std::make_unique<TigerEngine::GameObject>();
    triangle->Position = {0.35f, 0.65f};
    triangle->Scale = {0.3f, 0.3f};
    triangle->AddComponent<TigerEngine::SpriteRenderer>(
        std::make_shared<TigerEngine::Material>(TigerEngine::Vector4(1.0f, 0.25f, 0.2f, 1.0f)),
        TigerEngine::ShapeType::Triangle);
    app.GetScene().AddObject(std::move(triangle));
    auto polygon = std::make_unique<TigerEngine::GameObject>();
    polygon->Position = {0.95f, 0.65f};
    polygon->Scale = {0.3f, 0.3f};
    polygon->AddComponent<TigerEngine::SpriteRenderer>(
        std::make_shared<TigerEngine::Material>(TigerEngine::Vector4(1.0f, 0.65f, 0.1f, 1.0f)),
        TigerEngine::ShapeType::Polygon);
    polygon->AddComponent<ShapeGuide>();
    polygon->AddComponent<ShapeDemoControls>();
    app.GetScene().AddObject(std::move(polygon));

    app.GetScene().AddObject(std::move(player));
    app.GetScene().AddObject(std::move(floor));

    try {
        std::cout << "Starting Engine Loop..." << std::endl;
        app.Run();
    } catch(const std::exception &e) {

        std::cout << "\nFATAL ENGINE CRASH: " << e.what() << "\n" << std::endl;
    } catch(...) {
        std::cout << "\nUNKNOWN FATAL CRASH OCCURRED!\n" << std::endl;
    }
    return 0;
}
int main() {
    try {
        return RunDemo();
    } catch (const std::exception& error) {
        std::cerr << "Engine startup failed: " << error.what() << '\n';
        return 1;
    }
}
