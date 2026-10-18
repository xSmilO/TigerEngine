#include "Core/Application.h"
#include "Scene/GameObject.h"
#include <iostream>

class Player : public TigerEngine::GameObject {
  public:
    void Start() override {
        X = 0.0f;
        Y = 0.0f;
        Width = 0.5f;
        Height = 0.5f;
        std::cout << "Player Spawned in TigerEngine!" << std::endl;
    }

    void Update(float deltaTime) override {
        // Move to the right based on time
        X += 0.5f * deltaTime;
    }
};

int main() {
    TigerEngine::Application app(1280, 720, "TigerEngine");

    Player *myPlayer = new Player();
    app.GetScene().AddObject(myPlayer);

    app.Run();

    return 0;
}