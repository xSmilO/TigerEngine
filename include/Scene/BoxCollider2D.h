#pragma once
#include "Core/Component.h"
#include "Core/Math.h"
#include "Graphics/Renderer2D.h"
#include <glm/gtc/matrix_transform.hpp>

namespace TigerEngine {

class BoxCollider2D : public Component {
  public:
    Vector2 Size = Vector2(1.0f, 1.0f);
    Vector2 Offset = Vector2(0.0f, 0.0f);

    BoxCollider2D() = default;
    BoxCollider2D(Vector2 size, Vector2 offset = Vector2(0.0f, 0.0f)) : Size(size), Offset(offset) {
    }

    // Only DECLARE the functions here
    void Draw() override;
    bool CheckAABB(BoxCollider2D *other, Vector2 myNextPosition);
};

} // namespace TigerEngine