#pragma once
#include <glm/glm.hpp>
#include <string>

namespace TigerEngine {

// A static class to handle all UI drawing in your engine
class UI {
  public:
    static void BeginWindow(const std::string &title);
    static void EndWindow();

    static void Text(const std::string &text);

    // Returns true if the button was clicked this frame
    static bool Button(const std::string &label, const glm::vec2 &size = glm::vec2(0.0f, 0.0f));

    static void Separator();
};

} // namespace TigerEngine