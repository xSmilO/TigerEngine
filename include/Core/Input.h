#pragma once
#include "Core/KeyCodes.h"

// Forward declaration
class GLFWwindow;

namespace TigerEngine {
    class Input {
    public:
        // Setup the internal window pointer
        static void Init(GLFWwindow* window);
        
        // The function the user will actually call
        static bool IsKeyPressed(KeyCode key);

    private:
        static GLFWwindow* s_Window;
    };
}