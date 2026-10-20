#include "Core/Application.h"
#include "Core/Input.h"
#include "Scene/GameObject.h"
#include <iostream>

class Player : public TigerEngine::GameObject {
  public:
    float Speed = 1.5f;

    void Start() override {
        // Look how much cleaner this is!
        Position = TigerEngine::Vector2(0.0f, 0.0f);
        Scale = TigerEngine::Vector2(0.3f, 0.3f);
    }

    void Update(float deltaTime) override {
        // 1. Reset velocity every frame
        Velocity = TigerEngine::Vector2(0.0f, 0.0f);

        // 2. Add directions based on input
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::D))
            Velocity.x += 1.0f;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::A))
            Velocity.x -= 1.0f;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::W))
            Velocity.y += 1.0f;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::S))
            Velocity.y -= 1.0f;

        // 3. Normalize vector so diagonal movement isn't faster!
        if(glm::length(Velocity) > 0.0f) {
            Velocity = glm::normalize(Velocity);
        }

        // 4. Apply vector math to move the object
        Position += Velocity * Speed * deltaTime;
    }
};

int main() {
    TigerEngine::Application app(1280, 720, "TigerEngine");

    // User decides they want a super zoomed-out camera (-10 to 10)
    TigerEngine::OrthographicCamera myCustomCamera(-10.0f, 10.0f, -5.625f, 5.625f);

    // User plugs it into the engine
    app.GetScene().SetActiveCamera(&myCustomCamera);

    // Add objects and run normally...
    app.GetScene().AddObject(new Player());
    app.Run();

    return 0;
}