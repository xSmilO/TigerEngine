#pragma once
#include "Graphics/OrthographicCamera.h"
#include "Scene/GameObject.h"
#include <vector>

namespace TigerEngine {
class Scene {
  public:
    Scene();
    ~Scene();

    void AddObject(GameObject *object);

    void Start();
    void Update(float deltaTime);
    void Draw();

    void OnMousePressed(int button, float mouseX, float mouseY);
    void OnMouseReleased(int button, float mouseX, float mouseY);

    void SetActiveCamera(OrthographicCamera *camera) {
        m_ActiveCamera = camera;
    }
    OrthographicCamera *GetActiveCamera() {
        return m_ActiveCamera;
    }

    const std::vector<GameObject *> &GetObjects() const {
        return m_Objects;
    }

    void OnWindowResized(int width, int height);

  private:
    std::vector<GameObject *> m_Objects;

    OrthographicCamera m_DefaultCamera{-1.6f, 1.6f, -0.9f, 0.9f};

    // The camera the engine will actually use to draw
    OrthographicCamera *m_ActiveCamera;
};
} // namespace TigerEngine