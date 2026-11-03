#pragma once
#include <glm/glm.hpp>
#include <string>
namespace TigerEngine {
class Font;
class UIElement {
public:
    virtual ~UIElement() = default;
    glm::vec2 Position{0.0f}, Size{0.0f};
    bool Visible = true;
    enum class Anchor { TopLeft, TopRight, BottomLeft, BottomRight, Center };
    Anchor Alignment = Anchor::TopLeft;
    UIElement* Parent = nullptr;

    glm::vec2 GetScreenPosition(glm::vec2 screenSize) const;
    void SetParent(UIElement* parent);
    void SetAnchor(Anchor anchor) { Alignment = anchor; }
    bool IsVisibleInHierarchy() const;
    virtual void Draw(const Font* font) const = 0;
    virtual bool Contains(glm::vec2 point) const {
        return Visible && point.x >= Position.x && point.y >= Position.y &&
               point.x < Position.x + Size.x && point.y < Position.y + Size.y;
    }
};
}
