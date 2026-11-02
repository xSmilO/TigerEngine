#include "UI/UIElement.h"
#include <stdexcept>
namespace TigerEngine {
glm::vec2 UIElement::GetScreenPosition(glm::vec2 screenSize) const {
    const glm::vec2 bounds = Parent ? Parent->Size : screenSize;
    glm::vec2 origin(0.0f);
    switch (Alignment) {
    case Anchor::TopLeft: break;
    case Anchor::TopRight: origin.x = bounds.x; break;
    case Anchor::BottomLeft: origin.y = bounds.y; break;
    case Anchor::BottomRight: origin = bounds; break;
    case Anchor::Center: origin = bounds * 0.5f; break;
    }
    // Right/bottom/center align the element itself to the corresponding anchor.
    if (Alignment == Anchor::TopRight || Alignment == Anchor::BottomRight) origin.x -= Size.x;
    if (Alignment == Anchor::BottomLeft || Alignment == Anchor::BottomRight) origin.y -= Size.y;
    if (Alignment == Anchor::Center) origin -= Size * 0.5f;
    return (Parent ? Parent->GetScreenPosition(screenSize) : glm::vec2(0.0f)) + origin + Position;
}
void UIElement::SetParent(UIElement* parent) {
    if (parent == this) throw std::invalid_argument("UI element cannot parent itself");
    for (auto* p = parent; p; p = p->Parent)
        if (p == this) throw std::invalid_argument("UI hierarchy cycle detected");
    Parent = parent;
}
bool UIElement::IsVisibleInHierarchy() const {
    for (auto* p = this; p; p = p->Parent) if (!p->Visible) return false;
    return true;
}
} // namespace TigerEngine
