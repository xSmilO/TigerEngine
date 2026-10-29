#pragma once

namespace TigerEngine {
class GameObject; // Forward declaration

class Component {
  public:
    virtual ~Component() = default;

    // Called when the component is added to the object
    virtual void Start() {
    }

    // Called every frame
    virtual void Update(float deltaTime) {
    }

    virtual void Draw() {
    }

    virtual void OnGUI() {
    }

    // A pointer to the GameObject this component is attached to
    GameObject *Owner = nullptr;
};
} // namespace TigerEngine