#include "Scene/GameObject.h"
#include "Graphics/Renderer2D.h"

namespace TigerEngine {
GameObject::GameObject()
    : Position(0.0f, 0.0f), Scale(0.5f, 0.5f), Velocity(0.0f, 0.0f), Rotation(0.0f), // Initialize to zero
      Color(1.0f, 1.0f, 1.0f, 1.0f), Sprite(nullptr), CurrentScene(nullptr) {
}

void GameObject::Draw() {
    if(Shape == ShapeType::Triangle) {
        Renderer2D::DrawTriangle(Position, Scale, Color, Rotation);
    } else if(Shape == ShapeType::Polygon) {
        Renderer2D::DrawPolygon(Position, Scale, PolygonSides, Color, Rotation);
    } else {
        Renderer2D::DrawQuad(Position, Scale, Sprite, Color, Rotation);
    }
}

bool GameObject::CheckCollision(const GameObject *other) const {
    if(!other)
        return false;

    // Check if there is overlap on the X axis
    bool collisionX = Position.x + (Scale.x / 2.0f) >= other->Position.x - (other->Scale.x / 2.0f) &&
                      other->Position.x + (other->Scale.x / 2.0f) >= Position.x - (Scale.x / 2.0f);

    // Check if there is overlap on the Y axis
    bool collisionY = Position.y + (Scale.y / 2.0f) >= other->Position.y - (other->Scale.y / 2.0f) &&
                      other->Position.y + (other->Scale.y / 2.0f) >= Position.y - (Scale.y / 2.0f);

    // Collision only happens if overlapping on BOTH axes
    return collisionX && collisionY;
}
} // namespace TigerEngine