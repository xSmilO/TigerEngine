#include "Scene/GameObject.h"
#include "Graphics/Renderer2D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // <-- ADD THIS LINE
#include <glm/gtc/type_ptr.hpp>

namespace TigerEngine {
GameObject::GameObject()
    : Position(0.0f, 0.0f), Scale(0.5f, 0.5f), Rotation(0.0f), CurrentScene(nullptr), Parent(nullptr) {
}

GameObject::~GameObject() {
    for(Component *comp : m_Components) {
        delete comp;
    }
    m_Components.clear();
}

void GameObject::Start() {
    // Start is now handled when AddComponent is called
}

void GameObject::Update(float deltaTime) {
    // Automatically update all attached components!
    for(Component *comp : m_Components) {
        comp->Update(deltaTime);
    }
}

void GameObject::Draw() {
    for(Component *comp : m_Components)
        comp->Draw();
}

void GameObject::AddChild(GameObject *child) {
    if(!child)
        return;
    child->Parent = this;
    Children.push_back(child);
}

glm::mat4 GameObject::GetWorldTransform() const {
    // 1. Calculate this object's LOCAL matrix
    glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, 0.0f)) *
                               glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                               glm::scale(glm::mat4(1.0f), glm::vec3(Scale.x, Scale.y, 1.0f));

    // 2. If it has a parent, multiply local by the parent's world matrix
    if(Parent) {
        return Parent->GetWorldTransform() * localTransform;
    }

    // 3. If no parent, the local matrix IS the world matrix
    return localTransform;
}
} // namespace TigerEngine