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

    GameObject* AddObject(std::unique_ptr<GameObject> object);

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

    OrthographicCamera *m_ActiveCamera;
};
}
