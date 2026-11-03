#include "UI/UIButton.h"
#include "UI/UIManager.h"
#include "Graphics/FontRenderer.h"
#include <algorithm>

namespace TigerEngine {
void UIButton::Draw(const Font* font) const {
    if (UIManager::s_Current) {
        const Texture* image = NormalTexture.get();
        if (Pressed && PressTexture) image = PressTexture.get();
        else if (Hovered && HoverTexture) image = HoverTexture.get();
        if (image) {
            if (UseNineSlice)
                UIManager::s_Current->DrawNineSlice(Position, Size, image, NineSliceBorders, TextureTint);
            else
                UIManager::s_Current->DrawTexture(Position, Size, image, TextureTint);
        }
        else UIManager::s_Current->DrawRect(Position, Size,
            Pressed ? PressBackground : Hovered ? HoverBackground : Background);
    }

    if (!font || Text.empty()) return;

    const auto bounds = font->MeasureVisibleBounds(Text);
    const glm::vec2 textSize = bounds.Size();
    glm::vec2 targetTop = Position;

    switch (TextHorizontalAlignment) {
    case HorizontalAlignment::Left:
        targetTop.x += TextPadding.x;
        break;
    case HorizontalAlignment::Center:
        targetTop.x += (Size.x - textSize.x) * 0.5f;
        break;
    case HorizontalAlignment::Right:
        targetTop.x += Size.x - TextPadding.x - textSize.x;
        break;
    }

    switch (TextVerticalAlignment) {
    case VerticalAlignment::Top:
        targetTop.y += TextPadding.y;
        break;
    case VerticalAlignment::Center:
        targetTop.y += (Size.y - textSize.y) * 0.5f;
        break;
    case VerticalAlignment::Bottom:
        targetTop.y += Size.y - TextPadding.y - textSize.y;
        break;
    }

    const glm::vec2 textPos = targetTop - bounds.Min;
    FontRenderer::DrawText(*font, Text, textPos, TextColor);
}
}
