#pragma once
#include "Graphics/OrthographicCamera.h"
#include "Scene/BoxCollider2D.h"
#include "Scene/GameObject.h"
#include <vector>

namespace TigerEngine {
class Scene {
  public:
    Scene();
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    // Transfers ownership; returned pointer is borrowed until Clear/destruction.
    GameObject* AddObject(std::unique_ptr<GameObject> object);
    // Call outside Update/Draw. Releases owned objects and resets the camera.
    void Clear();

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

    // Snapshot of borrowed pointers; callers must not delete scene objects.
    std::vector<GameObject *> GetObjects() const {
        std::vector<GameObject*> result;
        for (const auto& obj : m_Objects) result.push_back(obj.get());
        return result;
    }

    void OnWindowResized(int width, int height);

    void OnGUI();

    std::vector<class BoxCollider2D *> GetAllColliders() {
        std::vector<BoxCollider2D *> colliders;
        for(const auto& obj : m_Objects) {
            auto *col = obj->GetComponent<BoxCollider2D>();
            if(col)
                colliders.push_back(col);
        }
        return colliders;
    }

  private:
    std::vector<std::unique_ptr<GameObject>> m_Objects;

    OrthographicCamera m_DefaultCamera{-1.6f, 1.6f, -0.9f, 0.9f};

    // The camera the engine will actually use to draw
    OrthographicCamera *m_ActiveCamera;
};
} // namespace TigerEngine
