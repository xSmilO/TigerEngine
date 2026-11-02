#include "UI/UILabel.h"
#include "Graphics/FontRenderer.h"
namespace TigerEngine {
void UILabel::Draw(const Font* font) const {
    if (font) FontRenderer::DrawText(*font, Text, Position, Color);
}
} // namespace TigerEngine
