#pragma once
#include "Core/Component.h"
#include "Core/Math.h"

namespace TigerEngine {
class RigidBody2D : public Component {
  public:
    Vector2 Velocity{0.0f, 0.0f};
    bool IsGrounded = false;
    bool HasCollision = true;

    void Move(Vector2 &velocity, float deltaTime);

  private:
};
} // namespace TigerEngine