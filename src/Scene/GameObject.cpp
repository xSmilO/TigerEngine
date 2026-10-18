#include "Scene/GameObject.h"
#include "Graphics/Renderer2D.h"

namespace TigerEngine {
GameObject::GameObject() : X(0.0f), Y(0.0f), Width(0.5f), Height(0.5f), Sprite(nullptr) {
}

void GameObject::Draw() {
    if(Sprite) {
        Renderer2D::DrawQuad(X, Y, Width, Height, Sprite);
    } else {
        Renderer2D::DrawQuad(X, Y, Width, Height, nullptr);
    }
}
} // namespace TigerEngine