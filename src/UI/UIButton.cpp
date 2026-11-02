#include "UI/UIButton.h"
#include "UI/UIManager.h"
#include "Graphics/FontRenderer.h"
#include <algorithm>

namespace TigerEngine {
void UIButton::Draw(const Font* font) const {
    if (UIManager::s_Current)
        UIManager::s_Current->DrawRect(Position, Size,
            Pressed ? PressBackground : Hovered ? HoverBackground : Background);

    if (!font || Text.empty()) return;

    // DrawText positions glyphs relative to a nominal top-left / baseline.
    // Center the actual glyph rectangle rather than the font's nominal height.
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
} // namespace TigerEngine
