#pragma once
#include "Core/Component.h"
#include "Graphics/Material.h" // Include the new Material system

namespace TigerEngine {
enum class ShapeType {
    Quad,
    Triangle,
    Polygon
};

class SpriteRenderer : public Component {
  public:
    std::shared_ptr<Material> SharedMaterial; // Retains shared material and texture.
    ShapeType Shape;
    int PolygonSides;

    // Constructor now just takes a Material and an optional shape
    SpriteRenderer(std::shared_ptr<Material> material, ShapeType shape = ShapeType::Quad)
        : SharedMaterial(std::move(material)), Shape(shape), PolygonSides(6) {
    }

    // UV range defaults to the full texture. Each SpriteRenderer owns its own UVs,
    // so multiple objects can share one material/texture but show different frames.
    void SetUVRect(const Vector2& uvMin, const Vector2& uvMax) {
        UVMin = uvMin;
        UVMax = uvMax;
    }
    Vector2 UVMin{0.0f, 0.0f};
    Vector2 UVMax{1.0f, 1.0f};

    void Draw() override;
};
} // namespace TigerEngine