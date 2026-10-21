#include "Core/Application.h"
#include "Scene/Component.h"

namespace TigerEngine {
class ClassicCamera : public TigerEngine::Component {
    void Update(float deltaTime) override {
        auto *cam = TigerEngine::Application::Get().GetScene().GetActiveCamera();
        if(cam) {
            cam->SetPosition(glm::vec3(Owner->Position.x, Owner->Position.y, 0.0f));
        }
    }
};
} // namespace TigerEngine