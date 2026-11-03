#include "Scene/GameObject.h"
#include "Graphics/Renderer2D.h"
#include <algorithm>
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace TigerEngine {
GameObject::GameObject()
    : Position(0.0f, 0.0f), Scale(0.5f, 0.5f), Rotation(0.0f), CurrentScene(nullptr), Parent(nullptr) {
}

GameObject::~GameObject() {
    if (Parent) {
        auto& siblings = Parent->Children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
    }
    for (auto* child : Children) child->Parent = nullptr;
}

void GameObject::Start() {

}

void GameObject::Update(float deltaTime) {

    for(const auto& comp : m_Components) {
        comp->Update(deltaTime);
    }
}

void GameObject::Draw() {
    for(const auto& comp : m_Components)
        comp->Draw();
}

void GameObject::AddChild(GameObject *child) {
    if(!child)
        return;
    for (auto* ancestor = this; ancestor; ancestor = ancestor->Parent)
        if (ancestor == child) throw std::invalid_argument("Parenting would create a cycle");
    if (child->Parent == this) return;
    if (child->Parent) {
        auto& siblings = child->Parent->Children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), child), siblings.end());
    }
    child->Parent = this;
    Children.push_back(child);
}

glm::mat4 GameObject::GetWorldTransform() const {

    glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, 0.0f)) *
                               glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                               glm::scale(glm::mat4(1.0f), glm::vec3(Scale.x, Scale.y, 1.0f));

    if(Parent) {
        return Parent->GetWorldTransform() * localTransform;
    }

    return localTransform;
}
}
