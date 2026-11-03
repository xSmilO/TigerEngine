#pragma once
#include "UI/UIElement.h"
namespace TigerEngine {
class UILabel : public UIElement {
public:
    std::string Text;
    glm::vec4 Color{1.0f};
    void Draw(const Font* font) const override;
};
}
