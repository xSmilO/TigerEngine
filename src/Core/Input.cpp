#include "Core/Input.h"
#include "Graphics/OrthographicCamera.h"
#include <cmath>
#include "imgui.h"
#include <GLFW/glfw3.h>

namespace TigerEngine {
GLFWwindow *Input::s_Window = nullptr;

void Input::Init(GLFWwindow *window) {
    s_Window = window;
}

bool Input::IsKeyPressed(KeyCode key) {
    if(!s_Window || key < GLFW_KEY_SPACE || key > GLFW_KEY_LAST)
        return false;
    if(ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureKeyboard)
        return false;
    int state = glfwGetKey(s_Window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(KeyCode button) {
    if(!s_Window || button < GLFW_MOUSE_BUTTON_1 || button > GLFW_MOUSE_BUTTON_LAST)
        return false;
    if(ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureMouse)
        return false;
    int state = glfwGetMouseButton(s_Window, button);
    return state == GLFW_PRESS;
}

Point2D Input::GetMousePosition() {
    if (!s_Window) return {0.0f, 0.0f};
    double x, y;
    glfwGetCursorPos(s_Window, &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

Point2D Input::GetMousePositionNDC() {
    if (!s_Window) return {0.0f, 0.0f};
    int width, height;
    glfwGetWindowSize(s_Window, &width, &height);
    if (width <= 0 || height <= 0) return {0.0f, 0.0f};
    auto point = GetMousePosition();
    return {2.0f * point.x / width - 1.0f, 1.0f - 2.0f * point.y / height};
}

Point2D Input::ScreenToWorld(Point2D point, Point2D windowSize,
                             const OrthographicCamera& camera) {
    if (!std::isfinite(windowSize.x) || !std::isfinite(windowSize.y) ||
        windowSize.x <= 0.0f || windowSize.y <= 0.0f) return {0.0f, 0.0f};
    const glm::vec4 ndc{2.0f * point.x / windowSize.x - 1.0f,
                        1.0f - 2.0f * point.y / windowSize.y, 0.0f, 1.0f};
    const auto world = glm::inverse(camera.GetViewProjectionMatrix()) * ndc;
    return {world.x / world.w, world.y / world.w};
}

Point2D Input::GetMouseWorldPosition(const OrthographicCamera& camera) {
    if (!s_Window) return {0.0f, 0.0f};
    int width, height;
    glfwGetWindowSize(s_Window, &width, &height);
    return ScreenToWorld(GetMousePosition(), {float(width), float(height)}, camera);
}

float Input::GetMouseX() { return GetMousePosition().x; }
float Input::GetMouseY() { return GetMousePosition().y; }
float Input::GetMouseX_NDC() { return GetMousePositionNDC().x; }
float Input::GetMouseY_NDC() { return GetMousePositionNDC().y; }
} // namespace TigerEngine
