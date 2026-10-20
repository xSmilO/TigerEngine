#include "Scene/GameObject.h"
#include "Graphics/Renderer2D.h"

namespace TigerEngine {
GameObject::GameObject() : Position(0.0f, 0.0f), Scale(0.5f, 0.5f), Velocity(0.0f, 0.0f), Sprite(nullptr) {
}

void GameObject::Draw() {
    if(Sprite) {
        Renderer2D::DrawQuad(Position.x, Position.y, Scale.x, Scale.y, Sprite);
    } else {
        Renderer2D::DrawQuad(Position.x, Position.y, Scale.x, Scale.y, nullptr);
    }
}
} // namespace TigerEngine