#include "Core/Input.h"
#include <GLFW/glfw3.h>

namespace TigerEngine {
// Define the static variable
GLFWwindow *Input::s_Window = nullptr;

void Input::Init(GLFWwindow *window) {
    s_Window = window;
}

bool Input::IsKeyPressed(KeyCode key) {
    if(!s_Window)
        return false;

    // Ask GLFW for the key state
    int state = glfwGetKey(s_Window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
} // namespace TigerEngine