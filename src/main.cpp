#include "Core/Application.h"
#include "Core/AudioSystem.h"
#include "Core/Input.h"
#include "Graphics/Material.h"
#include "Graphics/OrthographicCamera.h"
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
        // 1. Get the RigidBody attached to this GameObject
        TigerEngine::RigidBody2D *rb = Owner->GetComponent<TigerEngine::RigidBody2D>();
        if(!rb)
            return;

        // 2. Horizontal Movement (A/D keys)
        float moveX = 0.0f;
        if(TigerEngine::Input::IsKeyPressed(GLFW_KEY_D))
            moveX += 1.0f;
        if(TigerEngine::Input::IsKeyPressed(GLFW_KEY_A))
            moveX -= 1.0f;
        Velocity.x = moveX * Speed;

        // 3. Gravity and Jumping (W key)
        if(rb->IsGrounded) {
            // Stop applying downward force if we are standing on the floor
            if(Velocity.y < 0.0f)
                Velocity.y = 0.0f;

            // Jump!
            if(TigerEngine::Input::IsKeyPressed(GLFW_KEY_W)) {
                Velocity.y = JumpForce;
                TigerEngine::AudioSystem::PlaySound2D("assets/bomb.wav");
                double random = ((double)rand() / (RAND_MAX));

                std::cout << "new volue " << random << "\n";
                TigerEngine::AudioSystem::SetMusicVolume(random);
            }
        } else {
            // Apply gravity while in the air
            Velocity.y += Gravity * deltaTime;
        }

        // 4. Let the physics engine move the object and handle collisions!
        rb->Move(Velocity, deltaTime);
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
    player->AddComponent<TigerEngine::SmoothCamera>();
    player->AddComponent<PlayerController>();

    // 3. CREATE FLOOR
    TigerEngine::GameObject *floor = new TigerEngine::GameObject();
    floor->Position = TigerEngine::Vector2(0.0f, -0.8f);
    floor->Scale = TigerEngine::Vector2(2.0f, 0.2f);
    floor->AddComponent<TigerEngine::SpriteRenderer>(matFloor); // Pass the material!
    floor->AddComponent<TigerEngine::RigidBody2D>();

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