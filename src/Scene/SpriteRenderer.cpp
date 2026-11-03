#include "Scene/SpriteRenderer.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/Texture.h"
#include "Scene/GameObject.h"
#include <iostream>

namespace TigerEngine {
void SpriteRenderer::Draw() {
    if(!Owner || !SharedMaterial)
        return;

    glm::mat4 worldMatrix = Owner->GetWorldTransform();

    if(Shape == ShapeType::Quad) {
        if(SharedMaterial->MainTexture) {
            Renderer2D::DrawQuad(worldMatrix, SharedMaterial->MainTexture.get(), SharedMaterial->TintColor, UVMin, UVMax);
        } else {
            Renderer2D::DrawQuad(worldMatrix, SharedMaterial->TintColor);
        }
    } else if(Shape == ShapeType::Triangle) {
        Renderer2D::DrawTriangle(worldMatrix, SharedMaterial->TintColor);
    } else if(Shape == ShapeType::Polygon) {
        Renderer2D::DrawPolygon(worldMatrix, PolygonSides, SharedMaterial->TintColor);
    }
}
} // namespace TigerEngine