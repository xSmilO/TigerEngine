#pragma once
#include "Core/Math.h"

namespace TigerEngine {

class Texture; // Forward declaration so we don't need to include Texture.h here

class Material {
  public:
    Vector4 TintColor;
    Texture *MainTexture;

    // Constructor with default arguments so you can create a material with just a color,
    // just a texture, or both.
    Material(const Vector4 &color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), Texture *texture = nullptr)
        : TintColor(color), MainTexture(texture) {
    }

    ~Material() = default;
};

} // namespace TigerEngine