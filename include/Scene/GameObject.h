#pragma once
#include "Core/Math.h"

namespace TigerEngine {
class Texture;
class Scene; // Forward declaration

enum class ShapeType {
    Quad,
    Triangle,
    Polygon
};

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

    bool CheckCollision(const GameObject *other) const;

    Vector2 Position;
    Vector2 Scale;
    Vector2 Velocity;
    Texture *Sprite;
    Vector4 Color;
    ShapeType Shape = ShapeType::Quad;
    float Rotation;
    bool HasCollision = true;
    int PolygonSides = 6;

    // A pointer to the scene this object lives in
    Scene *CurrentScene;
};
} // namespace TigerEngine