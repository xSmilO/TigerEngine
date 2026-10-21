#pragma once
#include "Core/Math.h"

namespace TigerEngine {
class Texture; // Forward declaration

class Material {
  public:
    Vector4 TintColor;
    Texture *MainTexture;

    // Constructor for a pure color material
    Material(const Vector4 &color) : TintColor(color), MainTexture(nullptr) {
    }

    // Constructor for a textured material
    Material(Texture *texture, const Vector4 &tintColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f))
        : TintColor(tintColor), MainTexture(texture) {
    }
};
} // namespace TigerEngine