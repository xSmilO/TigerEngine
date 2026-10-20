#include "Core/Input.h"
#include <GLFW/glfw3.h>

namespace TigerEngine {
GLFWwindow *Input::s_Window = nullptr;

void Input::Init(GLFWwindow *window) {
    s_Window = window;
}

bool Input::IsKeyPressed(KeyCode key) {
    if(!s_Window)
        return false;
    int state = glfwGetKey(s_Window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(KeyCode button) {
    if(!s_Window)
        return false;
    int state = glfwGetMouseButton(s_Window, button);
    return state == GLFW_PRESS;
}

float Input::GetMouseX() {
    if(!s_Window)
        return 0.0f;
    double xpos, ypos;
    glfwGetCursorPos(s_Window, &xpos, &ypos);
    return (float)xpos;
}

float Input::GetMouseY() {
    if(!s_Window)
        return 0.0f;
    double xpos, ypos;
    glfwGetCursorPos(s_Window, &xpos, &ypos);
    return (float)ypos;
}

float Input::GetMouseX_NDC() {
    if(!s_Window)
        return 0.0f;
    int width, height;
    glfwGetWindowSize(s_Window, &width, &height);

    return (GetMouseX() / (float)width) * 2.0f - 1.0f;
}

float Input::GetMouseY_NDC() {
    if(!s_Window)
        return 0.0f;
    int width, height;
    glfwGetWindowSize(s_Window, &width, &height);

    return 1.0f - (GetMouseY() / (float)height) * 2.0f;
}
} // namespace TigerEngine