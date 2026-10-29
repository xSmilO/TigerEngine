#include "Scene/RigidBody2D.h"
#include "Core/Application.h"
#include "Graphics/Renderer2D.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

namespace TigerEngine {

void RigidBody2D::Move(Vector2 &velocity, float deltaTime) {
    if(!Owner || !Owner->CurrentScene)
        return;

    // 1. Get our own BoxCollider2D
    BoxCollider2D *myCollider = Owner->GetComponent<BoxCollider2D>();
    if(!myCollider) {
        // If no collider, just move freely and ignore physics
        Owner->Position.x += velocity.x * deltaTime;
        Owner->Position.y += velocity.y * deltaTime;
        return;
    }

    Vector2 moveAmount = velocity * deltaTime;
    IsGrounded = false;

    // Pre-calculate our custom hitbox dimensions
    float myHalfW = (Owner->Scale.x * myCollider->Size.x) / 2.0f;
    float myHalfH = (Owner->Scale.y * myCollider->Size.y) / 2.0f;

    // --- X-AXIS RESOLUTION ---
    Owner->Position.x += moveAmount.x;
    for(GameObject *obj : Owner->CurrentScene->GetObjects()) {
        if(obj == Owner)
            continue;

        BoxCollider2D *otherCollider = obj->GetComponent<BoxCollider2D>();
        if(!otherCollider)
            continue;

        // Check intersection using your BoxCollider2D AABB math
        if(myCollider->CheckAABB(otherCollider, Owner->Position)) {
            float otherHalfW = (obj->Scale.x * otherCollider->Size.x) / 2.0f;

            if(moveAmount.x > 0) { // Moving right, snap to their left edge
                float otherLeft = (obj->Position.x + otherCollider->Offset.x) - otherHalfW;
                Owner->Position.x = otherLeft - myHalfW - myCollider->Offset.x;
                velocity.x = 0.0f;        // Kill sideways momentum
            } else if(moveAmount.x < 0) { // Moving left, snap to their right edge
                float otherRight = (obj->Position.x + otherCollider->Offset.x) + otherHalfW;
                Owner->Position.x = otherRight + myHalfW - myCollider->Offset.x;
                velocity.x = 0.0f; // Kill sideways momentum
            }
        }
    }

    // --- Y-AXIS RESOLUTION ---
    Owner->Position.y += moveAmount.y;
    for(GameObject *obj : Owner->CurrentScene->GetObjects()) {
        if(obj == Owner)
            continue;

        BoxCollider2D *otherCollider = obj->GetComponent<BoxCollider2D>();
        if(!otherCollider)
            continue;

        if(myCollider->CheckAABB(otherCollider, Owner->Position)) {
            float otherHalfH = (obj->Scale.y * otherCollider->Size.y) / 2.0f;

            if(moveAmount.y < 0) { // Falling down, snap to their top edge
                float otherTop = (obj->Position.y + otherCollider->Offset.y) + otherHalfH;
                Owner->Position.y = otherTop + myHalfH - myCollider->Offset.y;
                IsGrounded = true;
                velocity.y = 0.0f;        // Kill falling momentum so gravity stops accumulating!
            } else if(moveAmount.y > 0) { // Jumping up, snap to their bottom edge
                float otherBottom = (obj->Position.y + otherCollider->Offset.y) - otherHalfH;
                Owner->Position.y = otherBottom - myHalfH - myCollider->Offset.y;
                velocity.y = 0.0f; // Kill upward momentum if we hit a ceiling
            }
        }
    }
}

} // namespace TigerEngine