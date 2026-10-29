#include "Scene/BoxCollider2D.h"
#include "Core/Application.h"
#include "Graphics/Renderer2D.h"
#include "Scene/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace TigerEngine {

void BoxCollider2D::Draw() {
    if(Application::Get().ShowHitboxes) {
        // Now the compiler knows exactly what Owner is!
        float worldX = Owner->Position.x + Offset.x;
        float worldY = Owner->Position.y + Offset.y;

        float worldScaleX = Owner->Scale.x * Size.x;
        float worldScaleY = Owner->Scale.y * Size.y;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(worldX, worldY, 0.0f)) *
                              glm::scale(glm::mat4(1.0f), glm::vec3(worldScaleX, worldScaleY, 1.0f));

        Renderer2D::DrawQuad(transform, Vector4(1.0f, 0.0f, 0.0f, 0.5f));
    }
}

bool BoxCollider2D::CheckAABB(BoxCollider2D *other, Vector2 myNextPosition) {
    float myHalfW = (Owner->Scale.x * Size.x) / 2.0f;
    float myHalfH = (Owner->Scale.y * Size.y) / 2.0f;
    float myLeft = (myNextPosition.x + Offset.x) - myHalfW;
    float myRight = (myNextPosition.x + Offset.x) + myHalfW;
    float myBottom = (myNextPosition.y + Offset.y) - myHalfH;
    float myTop = (myNextPosition.y + Offset.y) + myHalfH;

    float otherHalfW = (other->Owner->Scale.x * other->Size.x) / 2.0f;
    float otherHalfH = (other->Owner->Scale.y * other->Size.y) / 2.0f;
    float otherLeft = (other->Owner->Position.x + other->Offset.x) - otherHalfW;
    float otherRight = (other->Owner->Position.x + other->Offset.x) + otherHalfW;
    float otherBottom = (other->Owner->Position.y + other->Offset.y) - otherHalfH;
    float otherTop = (other->Owner->Position.y + other->Offset.y) + otherHalfH;

    return (myRight > otherLeft && myLeft < otherRight && myTop > otherBottom && myBottom < otherTop);
}

} // namespace TigerEngine