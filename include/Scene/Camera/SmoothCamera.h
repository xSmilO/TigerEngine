#include "Core/Application.h"
#include "Scene/Component.h"

namespace TigerEngine {
class SmoothCamera : public TigerEngine::Component {
  public:
    float FollowSpeed = 5.0f;

    void Update(float deltaTime) override {
        auto *cam = TigerEngine::Application::Get().GetScene().GetActiveCamera();
        if(cam) {
            glm::vec3 camPos = cam->GetPosition();

            float newX = camPos.x + (Owner->Position.x - camPos.x) * FollowSpeed * deltaTime;
            float newY = camPos.y + (Owner->Position.y - camPos.y) * FollowSpeed * deltaTime;

            cam->SetPosition(glm::vec3(newX, newY, 0.0f));
        }
    }
};
} // namespace TigerEngine