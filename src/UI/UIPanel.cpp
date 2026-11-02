#include "UI/UIPanel.h"
#include "UI/UIManager.h"
namespace TigerEngine {
void UIPanel::Draw(const Font*) const {
    if (UIManager::s_Current) UIManager::s_Current->DrawRect(Position, Size, Color);
}
} // namespace TigerEngine
