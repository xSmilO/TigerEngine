#include "Graphics/UI.h"
#include "imgui.h"

namespace TigerEngine {

void UI::BeginWindow(const std::string &title) {
    ImGui::Begin(title.c_str());
}

void UI::EndWindow() {
    ImGui::End();
}

void UI::Text(const std::string &text) {
    ImGui::Text("%s", text.c_str());
}

bool UI::Button(const std::string &label, const glm::vec2 &size) {
    // We convert your engine's glm::vec2 into ImGui's ImVec2 behind the scenes!
    return ImGui::Button(label.c_str(), ImVec2(size.x, size.y));
}

void UI::Separator() {
    ImGui::Separator();
}

} // namespace TigerEngine