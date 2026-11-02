#pragma once
#include "UI/UIElement.h"
#include <functional>
#include <utility>
namespace TigerEngine {
class UIButton : public UIElement {
public:
    std::string Text;
    glm::vec4 Background{0.16f, 0.36f, 0.62f, 1.0f};
    glm::vec4 HoverBackground{0.25f, 0.48f, 0.75f, 1.0f};
    glm::vec4 PressBackground{0.11f, 0.27f, 0.46f, 1.0f};
    glm::vec4 TextColor{1.0f};
    std::function<void()> OnClick;
    bool Hovered = false, Pressed = false;
    void SetOnClick(std::function<void()> callback) { OnClick = std::move(callback); }
    void Draw(const Font* font) const override;
};
} // namespace TigerEngine
