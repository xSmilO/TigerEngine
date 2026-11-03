#pragma once
#include "Core/Component.h"
#include "Graphics/Material.h"

namespace TigerEngine {
enum class ShapeType {
    Quad,
    Triangle,
    Polygon
};

class SpriteRenderer : public Component {
  public:
    std::shared_ptr<Material> SharedMaterial;
    ShapeType Shape;
    int PolygonSides;

    SpriteRenderer(std::shared_ptr<Material> material, ShapeType shape = ShapeType::Quad)
        : SharedMaterial(std::move(material)), Shape(shape), PolygonSides(6) {
    }

    void SetUVRect(const Vector2& uvMin, const Vector2& uvMax) {
        UVMin = uvMin;
        UVMax = uvMax;
    }
    Vector2 UVMin{0.0f, 0.0f};
    Vector2 UVMax{1.0f, 1.0f};

    void Draw() override;
};
}
