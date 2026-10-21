#include "Scene/RigidBody2D.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

namespace TigerEngine {
bool RigidBody2D::CheckCollision(const GameObject *other) const {
    if(!other || !HasCollision)
        return false;

    RigidBody2D *otherRb = const_cast<GameObject *>(other)->GetComponent<RigidBody2D>();
    if(!otherRb || !otherRb->HasCollision)
        return false;

    // CHANGED: Removed the '=' so perfectly touching edges don't trigger collisions
    bool collisionX = Owner->Position.x + (Owner->Scale.x / 2.0f) > other->Position.x - (other->Scale.x / 2.0f) &&
                      other->Position.x + (other->Scale.x / 2.0f) > Owner->Position.x - (Owner->Scale.x / 2.0f);

    bool collisionY = Owner->Position.y + (Owner->Scale.y / 2.0f) > other->Position.y - (other->Scale.y / 2.0f) &&
                      other->Position.y + (other->Scale.y / 2.0f) > Owner->Position.y - (Owner->Scale.y / 2.0f);

    return collisionX && collisionY;
}

void RigidBody2D::Move(const Vector2 &velocity, float deltaTime) {
    if(!Owner || !Owner->CurrentScene)
        return;

    Vector2 moveAmount = velocity * deltaTime;
    IsGrounded = false;

    // --- X-AXIS RESOLUTION ---
    Owner->Position.x += moveAmount.x;
    for(GameObject *obj : Owner->CurrentScene->GetObjects()) {
        if(obj == Owner)
            continue;

        if(CheckCollision(obj)) {
            if(moveAmount.x > 0) { // Moving right
                Owner->Position.x = obj->Position.x - (obj->Scale.x / 2.0f) - (Owner->Scale.x / 2.0f);
            } else if(moveAmount.x < 0) { // Moving left
                Owner->Position.x = obj->Position.x + (obj->Scale.x / 2.0f) + (Owner->Scale.x / 2.0f);
            }
        }
    }

    // --- Y-AXIS RESOLUTION ---
    Owner->Position.y += moveAmount.y;
    for(GameObject *obj : Owner->CurrentScene->GetObjects()) {
        if(obj == Owner)
            continue;

        if(CheckCollision(obj)) {
            if(moveAmount.y < 0) { // Falling down
                Owner->Position.y = obj->Position.y + (obj->Scale.y / 2.0f) + (Owner->Scale.y / 2.0f);
                IsGrounded = true;
            } else if(moveAmount.y > 0) { // Jumping up
                Owner->Position.y = obj->Position.y - (obj->Scale.y / 2.0f) - (Owner->Scale.y / 2.0f);
            }
        }
    }
}
} // namespace TigerEngine