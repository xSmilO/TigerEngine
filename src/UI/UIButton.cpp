#include "UI/UIButton.h"
#include "UI/UIManager.h"
#include "Graphics/FontRenderer.h"
#include <algorithm>
namespace TigerEngine {
void UIButton::Draw(const Font* font) const {
    if (UIManager::s_Current) UIManager::s_Current->DrawRect(Position, Size, Pressed ? PressBackground : Hovered ? HoverBackground : Background);
    if (font) FontRenderer::DrawText(*font, Text, Position + glm::vec2(12.0f, std::max(0.0f, (Size.y - font->GetPixelHeight()) * 0.5f)), TextColor);
}
} // namespace TigerEngine
