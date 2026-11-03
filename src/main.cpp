#include "Core/Application.h"
#include "Core/AssetManager.h"
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

        // Horizontal Movement
        float moveX = 0.0f;
        if(TigerEngine::Input::IsKeyPressed(GLFW_KEY_D))
            moveX += 1.0f;
        if(TigerEngine::Input::IsKeyPressed(GLFW_KEY_A))
            moveX -= 1.0f;
        Velocity.x = moveX * Speed;

        // Jumping Logic with God Mode
        if(rb->IsGrounded) { // <-- God mode lets us bypass the ground check!
            if(Velocity.y < 0.0f)
                Velocity.y = 0.0f;

            if(TigerEngine::Input::IsKeyPressed(GLFW_KEY_W)) {
                Velocity.y = JumpForce;
                TigerEngine::AudioSystem::PlaySound2D("assets/jump.wav");
            }
        } else {
            Velocity.y += Gravity * deltaTime;
        }

        rb->Move(Velocity, deltaTime);
    }

    void OnGUI() override {
        TigerEngine::UI::BeginWindow("Engine Debug & Settings");

        TigerEngine::UI::Checkbox("Show Hitboxes", &TigerEngine::Application::Get().ShowHitboxes);

        // Grab the collider and make sliders for it!
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

// Demonstration code belongs here, outside the Application loop.
class ShapeGuide : public TigerEngine::Component {
    void Draw() override {
        TigerEngine::Renderer2D::DrawLine({0.15f, 0.45f}, {1.2f, 0.45f}, {0.2f, 0.9f, 1.0f, 1.0f});
    }
};

int RunDemo() {
    TigerEngine::Application app(1280, 720, "TigerEngine");
    TigerEngine::AssetManager assets;

    auto playerTexture = assets.LoadTexture("assets/czadomen.jpg");
    auto sameTexture = assets.LoadTexture("assets/czadomen.jpg");
    std::cout << "Cached textures: " << assets.TextureCount() << "\n";
    std::cout << "Same resource ?" << (playerTexture == sameTexture) << "\n";

    auto playerMaterial = std::make_shared<TigerEngine::Material>(TigerEngine::Vector4(1.0f), playerTexture);
    auto player = std::make_unique<TigerEngine::GameObject>();
    player->AddComponent<TigerEngine::SpriteRenderer>(playerMaterial);
    player->Scale = TigerEngine::Vector2(0.5f, 0.5f);
    player->AddComponent<TigerEngine::RigidBody2D>();
    player->AddComponent<TigerEngine::BoxCollider2D>(TigerEngine::Vector2(0.6f, 0.9f),
                                                     TigerEngine::Vector2(0.0f, -0.05f));
    player->AddComponent<PlayerController>();
    player->AddComponent<TigerEngine::SmoothCamera>();

    // Fonts are also cached by path + pixel height.
    auto font32 = assets.LoadFont("assets/fonts/main.ttf", 32.0f);
    auto again = assets.LoadFont("assets/fonts/main.ttf", 32.0f);
    std::cout << "Cached fonts: " << assets.FontCount() << '\n'; // 1

    TigerEngine::AudioSystem::PlayMusic("assets/main_music.mp3", true);
    TigerEngine::AudioSystem::SetMusicVolume(0.1);

    auto matFloor = std::make_shared<TigerEngine::Material>(TigerEngine::Vector4(0.0f, 1.0f, 0.0f, 1.0f));

    // 3. CREATE FLOOR
    auto floor = std::make_unique<TigerEngine::GameObject>();
    floor->Position = TigerEngine::Vector2(0.0f, -0.8f);
    floor->Scale = TigerEngine::Vector2(2.0f, 0.2f);
    floor->AddComponent<TigerEngine::SpriteRenderer>(matFloor); // Pass the material!
    floor->AddComponent<TigerEngine::RigidBody2D>();
    floor->AddComponent<TigerEngine::BoxCollider2D>();

    // Sprite animation DEMO: 4 frames in one row, looping every 0.12 seconds.
    // Demo assets live at assets/demo/animated_sprite.png.
    auto animationAtlas = assets.LoadTexture("assets/demo/animated_sprite.png");
    auto animationMaterial = std::make_shared<TigerEngine::Material>(TigerEngine::Vector4(1.0f), animationAtlas);
    auto animatedObject = std::make_unique<TigerEngine::GameObject>();
    animatedObject->Position = {0.8f, 0.25f};
    animatedObject->Scale = {0.5f, 0.5f};
    animatedObject->AddComponent<TigerEngine::SpriteRenderer>(animationMaterial);
    auto *animator = animatedObject->AddComponent<TigerEngine::SpriteAnimator>();
    animator->AddAnimation("Idle", TigerEngine::SpriteAnimator::Grid(4, 1, 0, 4, 0.12f));
    animator->Play("Idle");
    app.GetScene().AddObject(std::move(animatedObject));

    // Visible shape checks: red triangle, orange hexagon and cyan line.
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
    app.GetScene().AddObject(std::move(polygon));

    // Add to scene...
    app.GetScene().AddObject(std::move(player));
    app.GetScene().AddObject(std::move(floor));

    try {
        std::cout << "Starting Engine Loop..." << std::endl;
        app.Run();
    } catch(const std::exception &e) {
        // If C++ throws a fatal error, we catch it here and print it!
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
