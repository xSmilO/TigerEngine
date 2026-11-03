#pragma once

namespace TigerEngine {
class GameObject;

class Component {
  public:
    virtual ~Component() = default;

    virtual void Start() {
    }

    virtual void Update(float deltaTime) {
    }

    virtual void Draw() {
    }

    virtual void OnGUI() {
    }

    virtual void OnCollisionEnter(GameObject *other) {
    }

    GameObject *Owner = nullptr;
};
}
