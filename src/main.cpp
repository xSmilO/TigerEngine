#include "Core/Application.h"
#include "Core/AudioSystem.h"
#include "Core/Input.h"
#include "Core/UI.h"
#include "Graphics/Material.h"
#include "Graphics/OrthographicCamera.h"
#include "Scene/BoxCollider2D.h"
#include "Scene/Camera/SmoothCamera.h"
#include "Scene/RigidBody2D.h"
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

int main() {
    TigerEngine::Application app(1280, 720, "TigerEngine");

    // 1. CREATE MATERIALS (Do this once)
    TigerEngine::Texture *playerTex = new TigerEngine::Texture("assets/czadomen.jpg");

    TigerEngine::AudioSystem::PlayMusic("assets/main_music.mp3", true);
    TigerEngine::AudioSystem::SetMusicVolume(0.1);

    TigerEngine::Material *matPlayer =
        new TigerEngine::Material(TigerEngine::Vector4(1.0f, 1.0f, 1.0f, 1.0f), playerTex); // Cyan
    TigerEngine::Material *matFloor = new TigerEngine::Material(TigerEngine::Vector4(0.0f, 1.0f, 0.0f, 1.0f));

    // 2. CREATE PLAYER
    TigerEngine::GameObject *player = new TigerEngine::GameObject();
    player->Scale = TigerEngine::Vector2(0.5f, 0.5f);
    player->AddComponent<TigerEngine::SpriteRenderer>(matPlayer); // Pass the material!
    player->AddComponent<TigerEngine::RigidBody2D>();
    player->AddComponent<TigerEngine::BoxCollider2D>(TigerEngine::Vector2(0.6f, 0.9f),
                                                     TigerEngine::Vector2(0.0f, -0.05f));
    player->AddComponent<PlayerController>();
    player->AddComponent<TigerEngine::SmoothCamera>();

    // 3. CREATE FLOOR
    TigerEngine::GameObject *floor = new TigerEngine::GameObject();
    floor->Position = TigerEngine::Vector2(0.0f, -0.8f);
    floor->Scale = TigerEngine::Vector2(2.0f, 0.2f);
    floor->AddComponent<TigerEngine::SpriteRenderer>(matFloor); // Pass the material!
    floor->AddComponent<TigerEngine::RigidBody2D>();
    floor->AddComponent<TigerEngine::BoxCollider2D>();

    // Add to scene...
    app.GetScene().AddObject(player);
    app.GetScene().AddObject(floor);

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