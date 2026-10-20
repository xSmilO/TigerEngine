#include "Core/Application.h"
#include "Core/Input.h"
#include "Graphics/Renderer2D.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h" // Needed so we can call methods on CurrentScene
#include <iostream>

class CameraController : public TigerEngine::GameObject {
  public:
    float CameraSpeed = 3.0f; // Make it a bit faster than the player

    void Start() override {
        Position = TigerEngine::Vector2(0.0f, 0.0f);
        HasCollision = false;
    }

    void Update(float deltaTime) override {
        TigerEngine::Vector2 camVelocity = TigerEngine::Vector2(0.0f, 0.0f);

        // Read the Arrow Keys
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::Right))
            camVelocity.x += 1.0f;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::Left))
            camVelocity.x -= 1.0f;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::Up))
            camVelocity.y += 1.0f;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::Down))
            camVelocity.y -= 1.0f;

        // Normalize so diagonal panning isn't super fast
        if(glm::length(camVelocity) > 0.0f) {
            camVelocity = glm::normalize(camVelocity);
        }

        // Move the invisible object
        Position += camVelocity * CameraSpeed * deltaTime;

        // Apply the new position to the Scene's camera
        if(CurrentScene && CurrentScene->GetActiveCamera()) {
            TigerEngine::Vector3 cameraPos = {Position.x, Position.y, 0.0f};
            CurrentScene->GetActiveCamera()->SetPosition(cameraPos);
        }
    }

    // We do NOT override Draw(), so this object remains completely invisible!
    void Draw() override {
    }
};

class Enemy : public TigerEngine::GameObject {
  public:
    void Start() override {
        Position = TigerEngine::Vector2(1.0f, 0.0f);
        Scale = TigerEngine::Vector2(0.4f, 0.4f);
        Color = TigerEngine::Vector4(0.0f, 0.0f, 1.0f, 1.0f); // Blue enemy
    }
};

class Player : public TigerEngine::GameObject {
  public:
    float Speed = 1.5f;

    void Start() override {
        Position = TigerEngine::Vector2(0.0f, 0.0f);
        Scale = TigerEngine::Vector2(0.3f, 0.3f);
    }

    void Update(float deltaTime) override {
        Velocity = TigerEngine::Vector2(0.0f, 0.0f);

        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::D))
            Velocity.x += 1.0f;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::A))
            Velocity.x -= 1.0f;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::W))
            Velocity.y += 1.0f;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::S))
            Velocity.y -= 1.0f;

        if(glm::length(Velocity) > 0.0f) {
            Velocity = glm::normalize(Velocity);
        }

        // Reset color to white every frame
        Color = TigerEngine::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

        // Check for collisions!
        if(CurrentScene) {
            for(TigerEngine::GameObject *obj : CurrentScene->GetObjects()) {
                // Don't collide with ourselves
                if(obj == this || !obj->HasCollision)
                    continue;

                // If we bump into anything, turn red
                if(CheckCollision(obj)) {
                    Color = TigerEngine::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
                    std::cout << "Collision detected!\n";
                }
            }
        }

        Position += Velocity * Speed * deltaTime;
    }

    void Draw() override {
        // 1. Draw the Player's normal shape first
        TigerEngine::GameObject::Draw();

        // 2. Get the raw screen coordinates (-1.0 to 1.0)
        float ndcX = TigerEngine::Input::GetMouseX_NDC();
        float ndcY = TigerEngine::Input::GetMouseY_NDC();

        // 3. Convert Screen Space to World Space
        if(CurrentScene && CurrentScene->GetActiveCamera()) {
            // Get the camera's matrix and flip it backwards
            glm::mat4 viewProj = CurrentScene->GetActiveCamera()->GetViewProjectionMatrix();
            glm::mat4 inverseViewProj = glm::inverse(viewProj);

            glm::vec4 screenPos = glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
            glm::vec4 worldPos = inverseViewProj * screenPos;

            // Convert back to our standard Vector2
            TigerEngine::Vector2 finalMouseWorldPos(worldPos.x, worldPos.y);

            // 4. Draw the laser accurately!
            TigerEngine::Renderer2D::DrawLine(Position, finalMouseWorldPos,
                                              TigerEngine::Vector4(1.0f, 0.0f, 0.0f, 1.0f) // Red laser
            );
        }
    }
};

int main() {
    TigerEngine::Application app(1280, 720, "TigerEngine");

    app.GetScene().AddObject(new Player());
    app.GetScene().AddObject(new Enemy());
    app.GetScene().AddObject(new CameraController());

    app.Run();
    return 0;
}