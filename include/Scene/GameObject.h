#pragma once
#include "Core/Component.h"
#include "Core/Math.h"
#include "Graphics/Texture.h"
#include <memory>
#include <vector>
#include <utility>

namespace TigerEngine {
class Scene;

class GameObject {
  public:
    GameObject();
    virtual ~GameObject();
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void Draw();

    // Owns added components; returned pointers are borrowed until object destruction.
    template <typename T, typename... Args> T *AddComponent(Args &&...args) {
        auto owned = std::make_unique<T>(std::forward<Args>(args)...);
        T *component = owned.get();
        component->Owner = this;
        m_Components.push_back(std::move(owned));
        component->Start();
        return component;
    }

    template <typename T> T *GetComponent() {
        for(const auto& comp : m_Components) {
            // Check if the component matches the requested type
            if(T *typedComp = dynamic_cast<T *>(comp.get())) {
                return typedComp;
            }
        }
        return nullptr;
    }

    // Snapshot of borrowed pointers; callers must not delete components.
    std::vector<Component *> GetComponents() {
        std::vector<Component*> result;
        for (const auto& comp : m_Components) result.push_back(comp.get());
        return result;
    }

    Vector2 Position;
    Vector2 Scale;
    float Rotation;
    std::string Name = "GameObject";

    Scene *CurrentScene;

    // Borrowed hierarchy links. Use AddChild to reparent and prevent cycles.
    GameObject *Parent = nullptr;
    std::vector<GameObject *> Children;

    void AddChild(GameObject *child);
    glm::mat4 GetWorldTransform() const;

  private:
    std::vector<std::unique_ptr<Component>> m_Components;
};
} // namespace TigerEngine
