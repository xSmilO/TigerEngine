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
    Material *SharedMaterial; // NEW: Replaces Color and Sprite
    ShapeType Shape;
    int PolygonSides;

    // Constructor now just takes a Material and an optional shape
    SpriteRenderer(Material *material, ShapeType shape = ShapeType::Quad)
        : SharedMaterial(material), Shape(shape), PolygonSides(6) {
    }

    void Draw() override;
};
} // namespace TigerEngine