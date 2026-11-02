#pragma once
#include "UI/UIElement.h"
namespace TigerEngine {
class UIPanel : public UIElement {
public:
    glm::vec4 Color{0.10f, 0.12f, 0.18f, 0.94f};
    void Draw(const Font* font) const override;
};
} // namespace TigerEngine
