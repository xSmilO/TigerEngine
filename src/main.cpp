#include "Core/Application.h"
#include "Core/Input.h"
#include "Scene/GameObject.h"
#include <iostream>

class Player : public TigerEngine::GameObject {
  public:
    void Start() override {
        X = 0.0f;
        Y = 0.0f;
        Width = 0.3f;
        Height = 0.3f;
    }

    void Update(float deltaTime) override {
        // We can still use polling for continuous things like movement
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::D))
            X += 1.5f * deltaTime;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::A))
            X -= 1.5f * deltaTime;
        if(TigerEngine::Input::IsKeyPressed(TigerEngine::Key::R)) {
            Width = 0.3f;
            Height = 0.3f;
        }
    }

    void OnMousePressed(int button, float mouseX, float mouseY) override {
        if(button == TigerEngine::Mouse::ButtonLeft) {
            // Get the converted coordinates
            float ndcX = TigerEngine::Input::GetMouseX_NDC();
            float ndcY = TigerEngine::Input::GetMouseY_NDC();

            // Calculate the boundaries of this GameObject
            float left = X - (Width / 2.0f);
            float right = X + (Width / 2.0f);
            float bottom = Y - (Height / 2.0f);
            float top = Y + (Height / 2.0f);

            // Check if the mouse is inside the bounding box
            if(ndcX >= left && ndcX <= right && ndcY >= bottom && ndcY <= top) {
                std::cout << "Player Clicked! Direct hit at " << ndcX << ", " << ndcY << "\n";

                Width += 0.1f;
                Height += 0.1f;
            }
        }
    }
};

int main() {
    TigerEngine::Application app(1280, 720, "TigerEngine");

    app.GetScene().AddObject(new Player());

    app.Run();
    return 0;
}