#pragma once
#include "Core/Math.h"

namespace TigerEngine {
class Texture;

class GameObject {
  public:
    GameObject();
    virtual ~GameObject() = default;

    virtual void Start() {
    }
    virtual void Update(float deltaTime) {
    }
    virtual void Draw();
    virtual void OnMousePressed(int button, float mouseX, float mouseY) {
    }
    virtual void OnMouseReleased(int button, float mouseX, float mouseY) {
    }

    // Replaced raw floats with Vectors
    Vector2 Position;
    Vector2 Scale;
    Vector2 Velocity; // Great for physics/movement

    Texture *Sprite;
};
} // namespace TigerEngine