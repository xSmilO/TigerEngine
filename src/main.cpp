#include "Core/Application.h"
#include "Core/Input.h"
#include "Scene/GameObject.h"
#include <iostream>

class Player : public TigerEngine::GameObject {
  public:
    float Speed = 1.5f;

    void Start() override {
        X = 0.0f;
        Y = 0.0f;
        Width = 0.3f;
        Height = 0.3f;
    }

    void Update(float deltaTime) override {
        // Continuous polling for movement
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::D)) {
            X += Speed * deltaTime;
        }
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::A)) {
            X -= Speed * deltaTime;
        }
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::W)) {
            Y += Speed * deltaTime;
        }
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::S)) {
            Y -= Speed * deltaTime;
        }
    }
};

int main() {
    TigerEngine::Application app(1280, 720, "TigerEngine");

    app.GetScene().AddObject(new Player());

    app.Run();
    return 0;
}