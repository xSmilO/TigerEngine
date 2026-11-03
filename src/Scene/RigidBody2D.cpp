#include "Scene/RigidBody2D.h"
#include "Core/Application.h"
#include "Graphics/Renderer2D.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

namespace TigerEngine {

void RigidBody2D::Move(Vector2 &velocity, float deltaTime) {
    if(!Owner || !Owner->CurrentScene)
        return;

    BoxCollider2D *myCollider = Owner->GetComponent<BoxCollider2D>();
    if(!myCollider) {

        Owner->Position.x += velocity.x * deltaTime;
        Owner->Position.y += velocity.y * deltaTime;
        return;
    }

    Vector2 moveAmount = velocity * deltaTime;
    IsGrounded = false;

    float myHalfW = (Owner->Scale.x * myCollider->Size.x) / 2.0f;
    float myHalfH = (Owner->Scale.y * myCollider->Size.y) / 2.0f;

    Owner->Position.x += moveAmount.x;
    for(GameObject *obj : Owner->CurrentScene->GetObjects()) {
        if(obj == Owner)
            continue;

        BoxCollider2D *otherCollider = obj->GetComponent<BoxCollider2D>();
        if(!otherCollider)
            continue;

        if(myCollider->CheckAABB(otherCollider, Owner->Position)) {
            float otherHalfW = (obj->Scale.x * otherCollider->Size.x) / 2.0f;

            if(moveAmount.x > 0) {
                float otherLeft = (obj->Position.x + otherCollider->Offset.x) - otherHalfW;
                Owner->Position.x = otherLeft - myHalfW - myCollider->Offset.x;
                velocity.x = 0.0f;
            } else if(moveAmount.x < 0) {
                float otherRight = (obj->Position.x + otherCollider->Offset.x) + otherHalfW;
                Owner->Position.x = otherRight + myHalfW - myCollider->Offset.x;
                velocity.x = 0.0f;
            }
        }
    }

    Owner->Position.y += moveAmount.y;
    for(GameObject *obj : Owner->CurrentScene->GetObjects()) {
        if(obj == Owner)
            continue;

        BoxCollider2D *otherCollider = obj->GetComponent<BoxCollider2D>();
        if(!otherCollider)
            continue;

        if(myCollider->CheckAABB(otherCollider, Owner->Position)) {
            float otherHalfH = (obj->Scale.y * otherCollider->Size.y) / 2.0f;

            if(moveAmount.y < 0) {
                float otherTop = (obj->Position.y + otherCollider->Offset.y) + otherHalfH;
                Owner->Position.y = otherTop + myHalfH - myCollider->Offset.y;
                IsGrounded = true;
                velocity.y = 0.0f;
            } else if(moveAmount.y > 0) {
                float otherBottom = (obj->Position.y + otherCollider->Offset.y) - otherHalfH;
                Owner->Position.y = otherBottom - myHalfH - myCollider->Offset.y;
                velocity.y = 0.0f;
            }

            for(Component *comp : Owner->GetComponents()) {
                comp->OnCollisionEnter(obj);
            }
        }
    }
}

}
