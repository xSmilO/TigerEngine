#pragma once
#include "Core/Math.h"
#include "Scene/Component.h"

namespace TigerEngine {
class RigidBody2D : public Component {
  public:
    Vector2 Velocity{0.0f, 0.0f};
    bool IsGrounded = false;
    bool HasCollision = true;

    void Move(const Vector2 &velocity, float deltaTime);

  private:
    bool CheckCollision(const GameObject *other) const;
};
} // namespace TigerEngine