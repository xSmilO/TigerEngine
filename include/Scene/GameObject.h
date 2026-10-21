#pragma once
#include "Core/Math.h"
#include "Graphics/Texture.h"
#include "Scene/Component.h"
#include <memory>
#include <vector>

namespace TigerEngine {
class Scene;

class GameObject {
  public:
    GameObject();
    virtual ~GameObject();

    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void Draw();

    // --- COMPONENT SYSTEM ---
    template <typename T, typename... Args> T *AddComponent(Args &&...args) {
        T *component = new T(std::forward<Args>(args)...);
        component->Owner = this;
        m_Components.push_back(component);
        component->Start();
        return component;
    }

    template <typename T> T *GetComponent() {
        for(Component *comp : m_Components) {
            // Check if the component matches the requested type
            if(T *typedComp = dynamic_cast<T *>(comp)) {
                return typedComp;
            }
        }
        return nullptr;
    }

    Vector2 Position;
    Vector2 Scale;
    float Rotation;

    Scene *CurrentScene;

    GameObject *Parent = nullptr;
    std::vector<GameObject *> Children;

    void AddChild(GameObject *child);
    glm::mat4 GetWorldTransform() const;

  private:
    std::vector<Component *> m_Components;
};
} // namespace TigerEngine