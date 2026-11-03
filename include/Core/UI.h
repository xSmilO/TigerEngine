#pragma once
#include <glm/glm.hpp>
#include <string>

namespace TigerEngine {

class UI {
  public:
    static void BeginWindow(const std::string &title);
    static void EndWindow();

    static void Text(const std::string &text);

    static bool Button(const std::string &label, const glm::vec2 &size = glm::vec2(0.0f, 0.0f));

    static void Separator();

    static bool SliderFloat(const std::string &label, float *value, float min, float max);
    static bool Checkbox(const std::string &label, bool *value);
};

}
