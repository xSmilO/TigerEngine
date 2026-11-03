#pragma once
#include "Core/Math.h"
#include <memory>
#include <utility>

namespace TigerEngine {

class Texture;

class Material {
  public:
    Vector4 TintColor;
    std::shared_ptr<Texture> MainTexture;

    Material(const Vector4 &color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), std::shared_ptr<Texture> texture = nullptr)
        : TintColor(color), MainTexture(std::move(texture)) {
    }

    ~Material() = default;
};

}
